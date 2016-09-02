/* 
 * The MIT License (MIT)
 * 
 * ESP8266 Non-OS Firmware
 * Copyright (c) 2015 Michael Jacobsen (github.com/mikejac)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "hexdump.h"
#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>
#include <osapi.h>

/******************************************************************************************************************
 * public functions
 *
 */

/**
 * 
 * @param pAddressIn
 * @param lSize
 */
void ICACHE_FLASH_ATTR hexdump(const void *pAddressIn, long lSize)
{
    char szBuf[100];
    long lIndent = 1;
    long lOutLen, lIndex, lIndex2, lOutLen2;
    long lRelPos;
    struct { char *pData; unsigned long lSize; } buf;
    unsigned char *pTmp,ucTmp;
    unsigned char *pAddress = (unsigned char *)pAddressIn;

    buf.pData   = (char *)pAddress;
    buf.lSize   = lSize;

    while (buf.lSize > 0) {
        pTmp     = (unsigned char *)buf.pData;
        lOutLen  = (int)buf.lSize;

        if (lOutLen > 16)
            lOutLen = 16;

        // create a 64-character formatted output line:
        os_sprintf(szBuf,  " >                            "
                           "                      "
                         "    %08X", pTmp-pAddress);

        lOutLen2 = lOutLen;

        for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
            lOutLen2;
            lOutLen2--, lIndex += 2, lIndex2++
        )

        {
            ucTmp = *pTmp++;

            os_sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
            
            if(ucTmp < 0x20 || ucTmp > 0x7E) {
                ucTmp = '.'; // nonprintable char
            }
            
            szBuf[lIndex2] = ucTmp;

            if (!(++lRelPos & 3))     // extra blank after 4 bytes
                {  lIndex++; szBuf[lIndex+2] = ' '; }
        }

        if (!(lRelPos & 3)) lIndex--;

        szBuf[lIndex  ]   = '<';
        szBuf[lIndex+1]   = ' ';

        os_printf("%s\n", szBuf);

        buf.pData   += lOutLen;
        buf.lSize   -= lOutLen;
    }
}
