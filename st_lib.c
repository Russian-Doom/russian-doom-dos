//
// Copyright (C) 1993-1996 Id Software, Inc.
// Copyright (C) 2016-2017 Alexey Khokholov (Nuke.YKT)
// Copyright (C) 2017 Alexandre-Xavier Labonte-Lamoureux
// Copyright (C) 2017-2021 Julian Nechaevsky
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
//	The status bar widget code.
//
#include <ctype.h>

#include "doomdef.h"

#include "z_zone.h"
#include "v_video.h"

#include "i_system.h"

#include "w_wad.h"

#include "st_stuff.h"
#include "st_lib.h"
#include "r_local.h"
#include "m_menu.h"
#include "v_trans.h"
#include "jn.h"


// in AM_map.c
extern boolean		automapactive; 




//
// Hack display negative frags.
//  Loads and store the stminus lump.
//
patch_t*		sttminus;

void STlib_init(void)
{
    sttminus = (patch_t *) W_CacheLumpName("STTMINUS", PU_STATIC);
}


// ?
void
STlib_initNum
( st_number_t*		n,
  int			x,
  int			y,
  patch_t**		pl,
  int*			num,
  boolean*		on,
  int			width )
{
    n->x	= x;
    n->y	= y;
    n->oldnum	= 0;
    n->width	= width;
    n->num	= num;
    n->on	= on;
    n->p	= pl;
}


// 
// A fairly efficient way to draw a number
//  based on differences from the old number.
// Note: worth the trouble?
//
void
STlib_drawNum
( st_number_t*	n,
  boolean	refresh )
{

    int		numdigits = n->width;
    int		num = *n->num;
    
    int		w = SHORT(n->p[0]->width);
    int		h = SHORT(n->p[0]->height);
    int		x = n->x;
    
    int		neg;

    n->oldnum = *n->num;

    neg = num < 0;

    if (neg)
    {
	if (numdigits == 2 && num < -9)
	    num = -9;
	else if (numdigits == 3 && num < -99)
	    num = -99;
	
	num = -num;
    }

    // clear the area
    x = n->x - numdigits*w;

    if (n->y - ST_Y < 0)
	I_Error("drawNum: n->y - ST_Y < 0");

    // [JN] Copy background only on standard HUD
    if (screenblocks < 11 || automapactive)
    V_CopyRect(x, n->y - ST_Y, BG, w*numdigits, h, x, n->y, FG);

    // if non-number, do not draw it
    if (num == 1994)
	return;

    x = n->x;

    // in the special case of 0, you draw 0
    if (!num)
    {
        if (screenblocks < 11 || automapactive)
        V_DrawPatch(x - w, n->y, FG, n->p[ 0 ]);
        else
        V_DrawPatchDirect(x - w, n->y, 0, n->p[ 0 ]);
    }

    // draw the new number
    while (num && numdigits--)
    {
	x -= w;
    
    if (screenblocks < 11 || automapactive)
	V_DrawPatch(x, n->y, FG, n->p[ num % 10 ]);
    else
    V_DrawPatchDirect(x, n->y, 0, n->p[ num % 10 ]);

	num /= 10;
    }

    // draw a minus sign if necessary
    if (neg)
    {
        if (screenblocks < 11 || automapactive)
        V_DrawPatch(x - 8, n->y, FG, sttminus);
        else
        V_DrawPatchDirect(x - 8, n->y, 0, sttminus);
    }
}


//
void
STlib_updateNum
( st_number_t*		n,
  boolean		refresh )
{
    if (*n->on) STlib_drawNum(n, refresh);
}


//
void
STlib_initPercent
( st_percent_t*		p,
  int			x,
  int			y,
  patch_t**		pl,
  int*			num,
  boolean*		on,
  patch_t*		percent )
{
    STlib_initNum(&p->n, x, y, pl, num, on, 3);
    p->p = percent;
}




void
STlib_updatePercent
( st_percent_t*		per,
  int			refresh )
{
    STlib_updateNum(&per->n, refresh); // [crispy] moved here

    if (colored_hud && !vanilla)
        dp_translation = cr[CR_GRAY];

    if (refresh && *per->n.on)
    {
        if (screenblocks < 11 || automapactive)
        V_DrawPatch(per->n.x, per->n.y, FG, per->p);
        else
        V_DrawPatchDirect(per->n.x, per->n.y, 0, per->p);
    }
    
    dp_translation = NULL;
}



void
STlib_initMultIcon
( st_multicon_t*	i,
  int			x,
  int			y,
  patch_t**		il,
  int*			inum,
  boolean*		on )
{
    i->x	= x;
    i->y	= y;
    i->oldinum 	= -1;
    i->inum	= inum;
    i->on	= on;
    i->p	= il;
}



void
STlib_updateMultIcon
( st_multicon_t*	mi,
  boolean		refresh )
{
    int			w;
    int			h;
    int			x;
    int			y;

    if (*mi->on
	&& (mi->oldinum != *mi->inum || refresh)
	&& (*mi->inum!=-1))
    {
	if (mi->oldinum != -1)
	{
	    x = mi->x - SHORT(mi->p[mi->oldinum]->leftoffset);
	    y = mi->y - SHORT(mi->p[mi->oldinum]->topoffset);
	    w = SHORT(mi->p[mi->oldinum]->width);
	    h = SHORT(mi->p[mi->oldinum]->height);

	    if (y - ST_Y < 0)
		I_Error("updateMultIcon: y - ST_Y < 0");

        // [JN] Copy background only in standard HUD
        if (screenblocks < 11 || automapactive)
	    V_CopyRect(x, y-ST_Y, BG, w, h, x, y, FG);
	}
    
    if (screenblocks < 11 || automapactive)
	V_DrawPatch(mi->x, mi->y, FG, mi->p[*mi->inum]);
    else
    V_DrawPatchDirect(mi->x, mi->y, 0, mi->p[*mi->inum]);

	mi->oldinum = *mi->inum;
    }
}



void
STlib_initBinIcon
( st_binicon_t*		b,
  int			x,
  int			y,
  patch_t*		i,
  boolean*		val,
  boolean*		on )
{
    b->x	= x;
    b->y	= y;
    b->oldval	= 0;
    b->val	= val;
    b->on	= on;
    b->p	= i;
}



void
STlib_updateBinIcon
( st_binicon_t*		bi,
  boolean		refresh )
{
    int			x;
    int			y;
    int			w;
    int			h;

    if (*bi->on
	&& (bi->oldval != *bi->val || refresh))
    {
	x = bi->x - SHORT(bi->p->leftoffset);
	y = bi->y - SHORT(bi->p->topoffset);
	w = SHORT(bi->p->width);
	h = SHORT(bi->p->height);

	if (y - ST_Y < 0)
	    I_Error("updateBinIcon: y - ST_Y < 0");

	if (*bi->val)
    {
        if (screenblocks < 11 || automapactive)
	    V_DrawPatch(bi->x, bi->y, FG, bi->p);
        else
        V_DrawPatchDirect(bi->x, bi->y, 0, bi->p);
    }
    // [JN] Update backfround only in standard HUD
	else if (screenblocks < 11 || automapactive)
	    V_CopyRect(x, y-ST_Y, BG, w, h, x, y, FG);

	bi->oldval = *bi->val;
    }

}

