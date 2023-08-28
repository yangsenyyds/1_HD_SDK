/*
 * @Author: Liujiapeng 
 * @Date: 2023-08-14 17:03:17 
 * @Last Modified by: Liujiapeng
 * @Last Modified time: 2023-08-14 17:45:04
 */
#include <stdlib.h>
#include <stdbool.h>
#include "app_sleep.h"
#include "remote_control.h"
#include "att.h"

extern bool g_factorymode;
unsigned short get_VoiceHandle(void);
void factory_WriteDataParse(unsigned short handle, unsigned char *data, unsigned char len);
void factory_KeyProcess(unsigned char keynum);