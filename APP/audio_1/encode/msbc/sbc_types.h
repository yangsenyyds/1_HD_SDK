/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *  Data type declarations.
 *
 ******************************************************************************/
#ifndef SBC_TYPES_H
#define SBC_TYPES_H

#ifdef BUILDCFG
#include "bt_target.h"
#endif

/* BK4BTSTACK_CHANGE START */
typedef signed char         SINT8;
typedef signed short        SINT16;
typedef signed int          SINT32;
typedef signed long long    SINT64;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned long long  UINT64;
/* BK4BTSTACK_CHANGE STOP */

#define SBC_API
#define abs32(x) ( (x >= 0) ? x : (-x) )

#endif
