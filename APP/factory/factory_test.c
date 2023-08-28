#define LOG_TAG "FACTORY"
#include "log.h"
#include "ls_dbg.h"
#include "factory_svc.h"
#include "ls_ble.h"
#include "prf_fotas.h"
#include "ota_settings.h"
#include "platform.h"
#include "cpu.h"
#include "app_sleep.h"
#include "voice_svc.h"
#include "led.h"

#include "remote_control.h"

#define FACTORY_SERVER_MAX_MTU      519
#define FACTORY_SVC_RX_MAX_LEN      FACTORY_SERVER_MAX_MTU
#define FACTORY_SVC_TX_MAX_LEN      FACTORY_SERVER_MAX_MTU
#define FACTORY_SVC_BUFFER_SIZE     FACTORY_SERVER_MAX_MTU

#define FACTORY_SYNC_BYTE       0xAA
#define FACTORY_SYNC_BYTE_LEN   1
#define FACTORY_CMD_ID_LEN      1
#define FACTORY_DATA_BYTE_LEN   2
#define FACTORY_CRC_BYTE_LEN    2
#define FACTORY_FP_LEN          (FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN + FACTORY_DATA_BYTE_LEN)

#define CON_IDX_INVALID_VAL     0xFF

enum factory_svc_cmd_id {
    PEER_MAC_ADDR,
    FOTA,
    FACTORY_WRITE,
    FACTORY_READ,
    CMD_NUM_MAX
};

enum factory_svc_att_db_handles {
    FACTORY_SVC_IDX_RX_CHAR,
    FACTORY_SVC_IDX_RX_VAL,
    FACTORY_SVC_IDX_TX_CHAR,
    FACTORY_SVC_IDX_TX_VAL,
    FACTORY_SVC_IDX_TX_NTF_CFG,
    FACTORY_SVC_ATT_NUM
};

static const uint8_t factory_svc_uuid_128[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x01,0x00,0x40,0x6e};
static const uint8_t factory_rx_char_uuid_128[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x00,0x40,0x6e};
static const uint8_t factory_tx_char_uuid_128[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x00,0x40,0x6e};

static const struct att_decl factory_server_att_decl[FACTORY_SVC_ATT_NUM] =
{
    [FACTORY_SVC_IDX_RX_CHAR] = {
        .uuid = att_decl_char_array,
        .s.max_len = 0,
        .s.uuid_len = UUID_LEN_16BIT,
        .s.read_indication = 1,   
        .char_prop.rd_en = 1,
    },
    [FACTORY_SVC_IDX_RX_VAL] = {
        .uuid = factory_rx_char_uuid_128,
        .s.max_len = FACTORY_SVC_RX_MAX_LEN,
        .s.uuid_len = UUID_LEN_128BIT,
        .s.read_indication = 1,
        .char_prop.wr_cmd = 1,
        .char_prop.wr_req = 1,
    },
    [FACTORY_SVC_IDX_TX_CHAR] = {
        .uuid = att_decl_char_array,
        .s.max_len = 0,
        .s.uuid_len = UUID_LEN_16BIT,
        .s.read_indication = 1,
        .char_prop.rd_en = 1, 
    },
    [FACTORY_SVC_IDX_TX_VAL] = {
        .uuid = factory_tx_char_uuid_128,
        .s.max_len = FACTORY_SVC_TX_MAX_LEN,
        .s.uuid_len = UUID_LEN_128BIT,
        .s.read_indication = 1,
        .char_prop.ntf_en = 1,
    },
    [FACTORY_SVC_IDX_TX_NTF_CFG] = {
        .uuid = att_desc_client_char_cfg_array,
        .s.max_len = 0,
        .s.uuid_len = UUID_LEN_16BIT,
        .s.read_indication = 1,
        .char_prop.rd_en = 1,
        .char_prop.wr_req = 1,
    },
};

static const struct svc_decl factory_server_svc =
{
    .uuid = factory_svc_uuid_128,
    .att = (struct att_decl*)factory_server_att_decl,
    .nb_att = FACTORY_SVC_ATT_NUM,
    .uuid_len = UUID_LEN_128BIT,
};

EM_BUF_ATTR static uint8_t factory_server_recv_buf[FACTORY_SVC_BUFFER_SIZE];
EM_BUF_ATTR static uint8_t factory_server_send_buf[FACTORY_SVC_BUFFER_SIZE];
static uint16_t factory_server_send_data_length;
static bool factory_server_ntf_done;
static bool factory_server_recv_handle_done;
static uint8_t con_idx;
static uint16_t cccd_config;
static struct gatt_svc_env factory_server_svc_env;

static uint16_t CRC16(uint8_t *buf, uint16_t len)
{
    uint16_t i,j = 0;
    uint16_t crc = 0xffff;

    while (len > 0)
    {
        crc ^= buf[j++];
        for (i = 0; i < 8; ++i)
        {
            if ((crc & 0x01) != 0) {
                crc >>= 1;
                crc ^= 0xa001;
            }
            else {
                crc >>= 1;
            }
        }

        len--;
    }

    return crc;
}

static void factory_cmd_done(uint8_t cmd_id, uint16_t len)
{
    LOG_I("FACTORY CMD REPLY");
    factory_server_send_buf[0] = FACTORY_SYNC_BYTE;
    factory_server_send_buf[FACTORY_SYNC_BYTE_LEN] = cmd_id;
    factory_server_send_buf[FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN] = (uint8_t)(len & 0xFF);
    factory_server_send_buf[FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN + 1] = (uint8_t)((len >> 8) & 0xFF);
    factory_server_send_data_length = FACTORY_FP_LEN + len;

    if (con_idx != CON_IDX_INVALID_VAL && factory_server_send_data_length != 0 && factory_server_ntf_done)
    {
        factory_server_ntf_done = false;
        uint16_t handle = gatt_manager_get_svc_att_handle(&factory_server_svc_env, FACTORY_SVC_IDX_TX_VAL);
        gatt_manager_server_send_notification(con_idx, handle, factory_server_send_buf, factory_server_send_data_length, NULL);         
    }
}

// static const uint8_t auth[3] = {'H', 'D', '4'};
// static const uint8_t auth_rsp[3] = {'H', 'D', '5'};
// static const uint8_t factory_server_conn[7] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x0d};
// static const uint8_t start_mic[7] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x0d};
// static const uint8_t stop_mic[7] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0x0d};
// static const uint8_t quit_factory_mode[7] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0d};
// static void factory_server_recv_handle(uint8_t *value, uint16_t len)
// {
//     if (!memcmp(auth, value, 2))
//     {
//         uint16_t handle = gatt_manager_get_svc_att_handle(&factory_server_svc_env, FACTORY_SVC_IDX_TX_VAL);
//         gatt_manager_server_send_notification(con_idx, handle, (uint8_t *)auth_rsp, 3, NULL);
// 
//         app_sleep_stop();
//     }
//     else if (!memcmp(factory_server_conn, value, sizeof(factory_server_conn))) {
//         set_led_blink(0, 0, RED_LED);
//     }
//     else if (!memcmp(start_mic, value, sizeof(start_mic))) {
//         start_mic_pdm();
//     }
//     else if (!memcmp(stop_mic, value, sizeof(stop_mic))) {
//         stop_mic_pdm();
//     }
//     else if (!memcmp(quit_factory_mode, value, sizeof(quit_factory_mode))) {
//         LOG_I("quit factory mode !!!");
//         set_led_blink(500, 100, BLUE_LED);
//     }
// }

static void factory_server_recv_handle(void)
{
    LOG_I("FACTORY CMD HANDLE");
    factory_server_recv_handle_done = false;

    if (factory_server_recv_buf[0] == FACTORY_SYNC_BYTE)
    {
        uint8_t cmd_id = factory_server_recv_buf[FACTORY_SYNC_BYTE_LEN];
        uint16_t len = (factory_server_recv_buf[FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN + 1] << 8)
                        + factory_server_recv_buf[FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN];
        uint16_t crc = CRC16(factory_server_recv_buf, 
                             len + FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN + FACTORY_DATA_BYTE_LEN);

        if (crc == (factory_server_recv_buf[FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN + FACTORY_DATA_BYTE_LEN + len + 1] << 8)
                     + factory_server_recv_buf[FACTORY_SYNC_BYTE_LEN + FACTORY_CMD_ID_LEN + FACTORY_DATA_BYTE_LEN + len])
        {
            switch (cmd_id)
            {
            case FOTA:
            {
                struct fota_copy_info copy;
                memset(&copy, 0, sizeof(copy));
                ota_copy_info_set(&copy);
                ota_boot_addr_set(0x18070000);
                platform_reset(0xFF);
            }
                break;
            case FACTORY_WRITE:

                factory_cmd_done(cmd_id, 0);
                break;

            default:
                break;
            }
        }
        else {
            LOG_I("FACTORY CMD CRC ERROR");
        }
    }
    else {
        factory_server_recv_handle_done = true;
    }
}

void factory_server_write_req_ind(uint8_t att_idx, uint16_t length, uint8_t const *value)
{
    if (att_idx == FACTORY_SVC_IDX_RX_VAL)
    {
        LS_ASSERT(length <= FACTORY_SERVER_MAX_MTU);
        if (factory_server_recv_handle_done == true)
        {
            memcpy((void*)&factory_server_recv_buf[0], value, length);

            uint32_t cpu_stat = enter_critical();
            factory_server_recv_handle();
            exit_critical(cpu_stat);
        }
    }
    else if (att_idx == FACTORY_SVC_IDX_TX_NTF_CFG)
    {
        LS_ASSERT(length == 2);
        memcpy(&cccd_config, value, length);
    }
}

void factory_server_read_req_ind(uint8_t att_idx, uint8_t con_idx)
{
    uint16_t handle = 0;

    if (att_idx == FACTORY_SVC_IDX_TX_NTF_CFG) {
        handle = gatt_manager_get_svc_att_handle(&factory_server_svc_env, att_idx);
        gatt_manager_server_read_req_reply(con_idx, handle, 0, (void*)&cccd_config, 2);
    }
}
struct gatt_svc_env *factory_server_svc_env_get(void)
{
    return &factory_server_svc_env;
}

bool get_factory_server_ntf_state(void)
{
    return factory_server_ntf_done;
}

void set_factory_server_ntf_state(bool state)
{
    factory_server_ntf_done = state;
}

void factory_server_con_idx_set(uint8_t con_id)
{
    con_idx = con_id;
}

void factory_server_reinit(void)
{
    con_idx = CON_IDX_INVALID_VAL;
    factory_server_recv_handle_done = true;
    factory_server_ntf_done = true;
    factory_server_send_data_length = 0;   
}

void factory_server_reg(uint16_t start_hdl)
{
    gatt_manager_svc_register(start_hdl, FACTORY_SVC_ATT_NUM, &factory_server_svc_env);
}

void factory_server_init(void)
{
    dev_manager_add_service((struct svc_decl *)&factory_server_svc);

    con_idx = CON_IDX_INVALID_VAL;
    factory_server_recv_handle_done = true;
    factory_server_ntf_done = true;
    factory_server_send_data_length = 0;   
} 
