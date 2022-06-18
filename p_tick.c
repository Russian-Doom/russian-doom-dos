//
// Copyright (C) 1993-1996 Id Software, Inc.
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
//	Archiving: SaveGame I/O.
//	Thinker, Ticker.
//

#include <stdlib.h>
#include "z_zone.h"
#include "p_local.h"
#include "doomstat.h"
#include "jn.h"


int	leveltime;


//
// THINKERS
// All thinkers should be allocated by Z_Malloc
// so they can be operated on uniformly.
// The actual structures will vary in size,
// but the first element must be thinker_t.
//

// Both the head and tail of the thinker list.
thinker_t	thinkercap;


//
// P_InitThinkers
//
void P_InitThinkers (void)
{
    thinkercap.prev = thinkercap.next  = &thinkercap;
}


//
// P_AddThinker
// Adds a new thinker at the end of the list.
//
void P_AddThinker (thinker_t *thinker)
{
    thinkercap.prev->next = thinker;
    thinker->next = &thinkercap;
    thinker->prev = thinkercap.prev;
    thinkercap.prev = thinker;
}


//
// P_RemoveThinker
// Deallocation is lazy -- it will not actually be freed
// until its thinking turn comes up.
//
void P_RemoveThinker (thinker_t *thinker)
{
    // FIXME: NOP.
    thinker->function.acv = (actionf_v)(-1);
}


//
// P_RunThinkers
//

int bmap_flick = 0;
int bmap_glow = 0;
static int bmap_count_flick = 0;
static int bmap_count_glow = 0;

void P_RunThinkers (void)
{
    thinker_t *currentthinker;

    currentthinker = thinkercap.next;

    // [JN] Run brightmap timers.
    bmap_count_flick++;
    bmap_count_glow++;

    while (currentthinker != &thinkercap)
    {
		// [JN] Random brightmap flickering effect.
        if (bmap_count_flick < 2)
        {
            if (currentthinker->function.acp1 == (actionf_p1)P_MobjThinker)
            {
                mobj_t *mo = (mobj_t *)currentthinker;

                if (mo->sprite == SPR_CAND  // Candestick
                ||  mo->sprite == SPR_CBRA  // Candelabra
                ||  mo->sprite == SPR_FCAN  // Flaming Barrel
                ||  mo->sprite == SPR_TBLU  // Tall Blue Torch
                ||  mo->sprite == SPR_TGRN  // Tall Green Torch
                ||  mo->sprite == SPR_TRED  // Tall Red Torch
                ||  mo->sprite == SPR_SMBT  // Short Blue Torch
                ||  mo->sprite == SPR_SMGT  // Short Green Torch
                ||  mo->sprite == SPR_SMRT  // Short Red Torch
                ||  mo->sprite == SPR_POL3) // Pile of Skulls and Candles
                {
                    if (brightmaps && !vanilla)
                    {
                        mo->bmap_flick = rand() % 16;
                    }
                    else
                    {
                        mo->bmap_flick =  0;
                    }
                }
            }
        }

        if (currentthinker->function.acv == (actionf_v)(-1))
        {
            // time to remove it
            currentthinker->next->prev = currentthinker->prev;
            currentthinker->prev->next = currentthinker->next;
            Z_Free (currentthinker);
        }
        else
        {
            if (currentthinker->function.acp1)
            currentthinker->function.acp1 (currentthinker);
        }
        currentthinker = currentthinker->next;
    }

    // [JN] Brightmap glowing effect.
    if (brightmaps && !vanilla)
    {
        if (bmap_count_glow < 7)
        {
            bmap_glow++;
        }
        else if (bmap_count_glow < 13)
        {
            bmap_glow--;
        }
    }
    else
    {
        bmap_glow = 0;
        bmap_count_glow = 0;
    }

    // [JN] Reset brightmap timers.
    if (bmap_count_flick >= 4)
    {
        bmap_count_flick = 0;
    }
    if (bmap_count_glow >= 13)
    {
        bmap_count_glow = 0;
    }
}


//
// P_Ticker
//
void P_Ticker (void)
{
    int i;

    // run the tic
    if (paused)
    {
        return;
    }

    // pause if in menu and at least one tic has been run
    if (!netgame && menuactive && !demoplayback && players[consoleplayer].viewz != 1)
    {
        return;
    }


    for (i=0 ; i<MAXPLAYERS ; i++)
        if (playeringame[i])
        P_PlayerThink (&players[i]);

    P_RunThinkers ();
    P_UpdateSpecials ();
    P_RespawnSpecials ();

    // for par times
    leveltime++;	
}
