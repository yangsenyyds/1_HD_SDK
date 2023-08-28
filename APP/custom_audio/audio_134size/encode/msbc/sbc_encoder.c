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
 *  contains code for encoder flow and initalization of encoder
 *
 ******************************************************************************/
#include <string.h>
#include "sbc_encoder.h"
#include "sbc_enc_func_declare.h"

#if (SBC_JOINT_STE_INCLUDED == TRUE)
SINT32 s32LRDiff[SBC_MAX_NUM_OF_BLOCKS] = {0};
SINT32 s32LRSum[SBC_MAX_NUM_OF_BLOCKS] = {0};
#endif

SINT16 EncMaxShiftCounter;

void SBC_Encoder(SBC_ENC_PARAMS *pstrEncParams)
{
    SINT32 s32Sb;                               /* counter for sub-band*/
    UINT32 u32Count, maxBit = 0;                          /* loop count*/
    SINT32 s32MaxValue;                         /* temp variable to store max value */

    SINT16 *ps16ScfL;
    SINT32 *SbBuffer;
    SINT32 s32Blk;                              /* counter for block*/

    pstrEncParams->pu8NextPacket = pstrEncParams->pu8Packet;

#if (SBC_NO_PCM_CPY_OPTION == TRUE)
    pstrEncParams->ps16NextPcmBuffer = pstrEncParams->ps16PcmBuffer;
#else
    pstrEncParams->ps16NextPcmBuffer  = pstrEncParams->as16PcmBuffer;
#endif
    do
    {
        SbcAnalysisFilter8(pstrEncParams);

        ps16ScfL = pstrEncParams->as16ScaleFactor;
		pstrEncParams->ps16NextPcmBuffer += 120;

        for (s32Sb=0; s32Sb<8; s32Sb++)
        {
            SbBuffer=pstrEncParams->s32SbBuffer+s32Sb;
            s32MaxValue=0;
            for (s32Blk=15; s32Blk>0; s32Blk--)
            {
                if (s32MaxValue<abs32(*SbBuffer))
                    s32MaxValue=abs32(*SbBuffer);
                SbBuffer+=8;
            }

            u32Count = (s32MaxValue > 0x800000) ? 9 : 0;

            for ( ; u32Count < 15; u32Count++)
            {
                if (s32MaxValue <= (SINT32)(0x8000 << u32Count))
                    break;
            }
            *ps16ScfL++ = (SINT16)u32Count;

            if (u32Count > maxBit)
                maxBit = u32Count;
        }

        pstrEncParams->s16MaxBitNeed = (SINT16)maxBit;
        sbc_enc_bit_alloc_mono(pstrEncParams);
        EncPacking(pstrEncParams);
    }
    while(--(pstrEncParams->u8NumPacketToEncode));

    pstrEncParams->u8NumPacketToEncode = 1;

}

void SBC_Encoder_Init(void)
{
    EncMaxShiftCounter = ((ENC_VX_BUFFER_SIZE-(8*10))>>3)<<3;
    SbcAnalysisInit();
}
