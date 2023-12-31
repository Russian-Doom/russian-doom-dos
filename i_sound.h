//
// Copyright (C) 1993-1996 id Software, Inc.
// Copyright (C) 1993-2008 Raven Software
// Copyright (C) 2016-2017 Alexey Khokholov (Nuke.YKT)
// Copyright (C) 2017 Alexandre-Xavier Labonte-Lamoureux
// Copyright (C) 2017-2022 Julian Nechaevsky
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
//  System interface for sound.
//


#pragma once


#define SND_TICRATE     140     // tic rate for updating sound
#define SND_MAXSONGS    40      // max number of songs in game

typedef enum
{
    snd_none,
    snd_PC,
    snd_Adlib,
    snd_SB,
    snd_PAS,
    snd_GUS,
    snd_MPU,
    snd_MPU2,
    snd_MPU3,
    snd_AWE,
    snd_ENSONIQ,
    snd_CODEC,
    NUM_SCARDS
} cardenum_t;
