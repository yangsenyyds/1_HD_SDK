#ifndef FACTORY_SVC_H
#define FACTORY_SVC_H
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "ls_ble.h"

extern void factory_server_write_req_ind(uint8_t att_idx, uint16_t length, uint8_t const *value);
extern void factory_server_read_req_ind(uint8_t att_idx, uint8_t con_idx);
extern struct gatt_svc_env *factory_server_svc_env_get(void);
extern bool get_factory_server_ntf_state(void);
extern void set_factory_server_ntf_state(bool state);
extern void factory_server_con_idx_set(uint8_t con_id);
extern void factory_server_reinit(void);
extern void factory_server_reg(uint16_t start_hdl);
extern void factory_server_init(void);

#endif
