//
// Copyright (C) 1993-1996 Id Software, Inc.
// Copyright (C) 1993-2008 Raven Software
// Copyright (C) 2016-2017 Alexey Khokholov (Nuke.YKT)
// Copyright (C) 2017 Alexandre-Xavier Labonte-Lamoureux
// Copyright (C) 2017-2020 Julian Nechaevsky
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//  CyberMan
//

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i_system.h"
#include "jn.h"

//
// Doom control structure
//
// The keybaord and joystick will add to the values set by the cyberman,
// to a maximum of 0x19000 for forwardmove and sidemove.  Angleturn is
// not bounded at all.
//
// parm                    normal          fast
// -----           ------          ----
// forwardmove             0xc800          0x19000
// sidemove                0xc000          0x14000
// angleturn               0x2800000       0x5000000
//
// The keyboard and joystick have a 1/3 second slow turn of 0x1400000 under
// normal speed to help aiming.
//

#define BT_ATTACK       1
#define BT_USE          2
#define BT_CHANGE       4           // if true, the next 3 bits hold weapon num
#define BT_WEAPONMASK   (8+16+32)
#define BT_WEAPONSHIFT  3



//
// CyberMan detection and usage info
//
#define DPMI_INT        0x31
#define MOUSE_INT       0x33

#define DOSMEMSIZE      64          // enough for any SWIFT structure

typedef struct {
    short   x;
    short   y;
    short   z;
    short   pitch;
    short   roll;
    short   yaw;
    short   buttons;
} SWIFT_3DStatus;

// DPMI real mode interrupt structure
static struct rminfo {
    long EDI;
    long ESI;
    long EBP;
    long reserved_by_system;
    long EBX;
    long EDX;
    long ECX;
    long EAX;
    short flags;
    short ES,DS,FS,GS,IP,CS,SP,SS;
} RMI;

typedef struct {
    unsigned char   deviceType;
    unsigned char   majorVersion;
    unsigned char   minorVersion;
    unsigned char   absRelFlags;
    unsigned char   centeringFlags;
    unsigned char   reserved[5];
} StaticDeviceData;

// values for deviceType:
#define DEVTYPE_CYBERMAN        1

short selector;
unsigned short segment; // segment of DOS memory block
SWIFT_3DStatus *cyberstat;
int isCyberPresent;         // is CyberMan present?


static union REGS regs;
static struct SREGS sregs;


extern int mousepresent;

//
// I_StartupCyberMan
// If a cyberman is present, init it and set isCyberPresent to 1
//
void I_StartupCyberMan(void)
{
    StaticDeviceData *pbuf;
    // int success = 0;

    isCyberPresent = 0;

    cyberstat = (SWIFT_3DStatus *)I_AllocLow(DOSMEMSIZE);
    segment = (int)cyberstat >> 4;

    pbuf = (StaticDeviceData *)cyberstat;
    memset(pbuf, 0, sizeof(StaticDeviceData));

    // Use DPMI call 300h to issue mouse interrupt
    memset(&RMI, 0, sizeof(RMI));
    RMI.EAX = 0x53C1;            // SWIFT: Get Static Device Data
    RMI.ES = segment;
    RMI.EDX = 0;
    memset(&sregs, 0, sizeof(sregs));
    regs.w.ax = 0x0300;          // DPMI: simulate interrupt
    regs.w.bx = MOUSE_INT;
    regs.w.cx = 0;
    regs.x.edi = FP_OFF(&RMI);
    sregs.es = FP_SEG(&RMI);
    int386x(DPMI_INT, &regs, &regs, &sregs);

    if ((short)RMI.EAX != 1)
    {
        // SWIFT functions not present
        printf(english_language ?
               "CyberMan: no SWIFT device connected.\n" :
               "CyberMan: �ࠩ��� ��� ��� �����প� SWIFT (AX=%04x).\n",
                (unsigned)(short)RMI.EAX);
    }
    else
    {
        if (pbuf->deviceType != DEVTYPE_CYBERMAN)
        {
            // no SWIFT device, or not CyberMan
            if (pbuf->deviceType == 0)
            {
                printf(english_language ?
                       "CyberMan: no SWIFT device connected.\n" :
                       "CyberMan: ���ன�⢮ � �����প�� SWIFT �� �����㦥��.\n");
            }
            else
            {
                printf(english_language ?
                       "CyberMan: SWIFT device is not a CyberMan! (type=%d)\n" :
                       "CyberMan: ���ன�⢮ SWIFT �� �� �ந�����⥫� CyberMan! (⨯=%d)\n",
                       pbuf->deviceType);
            }
        }
        else
        {
            printf(english_language ?
                   "CyberMan: CyberMan %d.%02d connected.\n" :
                   "CyberMan: ���ன�⢮ CyberMan %d.%02d ������祭�.\n",
                   pbuf->majorVersion, pbuf->minorVersion);
            isCyberPresent = 1;
            mousepresent = 0;
        }
    }
}



//
// I_ReadCyberCmds
//

int oldpos;

void I_ReadCyberCmd(ticcmd_t *cmd)
{
    int delta;

    // Use DPMI call 300h to issue mouse interrupt
    memset(&RMI, 0, sizeof(RMI));
    RMI.EAX = 0x5301;            // SWIFT: Get Position and Buttons
    RMI.ES = segment;
    RMI.EDX = 0;
    memset(&sregs, 0, sizeof(sregs));
    regs.w.ax = 0x0300;          // DPMI: simulate interrupt
    regs.w.bx = MOUSE_INT;
    regs.w.cx = 0;
    regs.x.edi = FP_OFF(&RMI);
    sregs.es = FP_SEG(&RMI);
    int386x(DPMI_INT, &regs, &regs, &sregs);

    if (cyberstat->y < -7900)
    {
        cmd->forwardmove = 0xc800 / 2048;
    }
    else if (cyberstat->y > 7900)
    {
        cmd->forwardmove = -0xc800 / 2048;
    }

    if (cyberstat->buttons & 4)
    {
        cmd->buttons |= BT_ATTACK;
    }
    if (cyberstat->buttons & 2)
    {
        cmd->buttons |= BT_USE;
    }

    delta = cyberstat->x - oldpos;
    oldpos = cyberstat->x;

    if (cyberstat->buttons & 1) // strafe
    {
        if (cyberstat->x < -7900)
        {
            cmd->sidemove = -0xc800 / 2048;
        }
        else if (cyberstat->x > 7900)
        {
            cmd->sidemove = 0xc800 / 2048;
        }
        else
        {
            cmd->sidemove = delta * 40 / 2048;
        }
    }
    else
    {
        if (cyberstat->x < -7900)
        {
            cmd->angleturn = 0x280;
        }
        else if (cyberstat->x > 7900)
        {
            cmd->angleturn = -0x280;
        }
        else
        {
            cmd->angleturn = -delta * 0xa / 16;
        }
    }
}


void I_Tactile(int on, int off, int total)
{
    if (!isCyberPresent)
    {
        return;
    }

    on /= 5;
    off /= 5;
    total /= 40;
    if (on > 255)
    {
        on = 255;
    }
    if (off > 255)
    {
        off = 255;
    }
    if (total > 255)
    {
        total = 255;
    }

    memset(&RMI, 0, sizeof(RMI));
    RMI.EAX = 0x5330;            // SWIFT: Get Position and Buttons
    RMI.EBX = on * 256 + off;
    RMI.ECX = total;
    memset(&sregs, 0, sizeof(sregs));
    regs.w.ax = 0x0300;          // DPMI: simulate interrupt
    regs.w.bx = MOUSE_INT;
    regs.w.cx = 0;
    regs.x.edi = FP_OFF(&RMI);
    sregs.es = FP_SEG(&RMI);
    int386x(DPMI_INT, &regs, &regs, &sregs);
}
