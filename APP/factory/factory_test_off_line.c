/* @Author: Liujiapeng
 * @Date: 2023-08-14 14:20:26
 * @Last Modified by: Liujiapeng
 * @Last Modified time: 2023-08-15 17:22:17
 */

#include "factory_test_off_line.h"
#include "yc_debug.h"
#include "audio.h"

const char cmd_test[] = "start factory test";
const char cmd_mic_close[] = "mic close";
bool g_factorymode = false;

unsigned short get_VoiceHandle(void)
{
    if (g_factorymode)
        return 65532;
    else
        return AUDIO_CMD_HANDLE + 2;

}

void factory_WriteDataParse(unsigned short handle, unsigned char *data, unsigned char len)
{
    if (65532 == handle)
    {
        if (0 == memcmp(cmd_test, data, sizeof(cmd_test) - 1))
        {
            // enter factory mode
            DEBUG_LOG_STRING("memcmp\r\n");
            g_factorymode = true;
            ATT_sendNotify(65532, (void *)&product_key_s, 3);
            app_sleep_timer_set(PAIR_DONE_DELAY);
        }
        else if (0 == memcmp(cmd_mic_close, data, sizeof(cmd_mic_close) - 1))
        {
            mic_close();
        }
    }
}

void factory_KeyProcess(unsigned char keynum)
{
    if (g_factorymode)
    {
        if (0xff == keynum)
        {
            // voice key
            mic_open();
        }
        ATT_sendNotify(65532, (void *)&keynum, 1);
        DEBUG_LOG_STRING("factory att send  [%d]  \r\n", keynum);
    }
}