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
//	Refresh/render internal state variables (global).
//


#pragma once

#include "d_player.h"
#include "r_data.h"


//
// Refresh internal data structures,
//  for rendering.
//

extern int		viewwidth;
extern int		scaledviewwidth;
extern int		viewheight;

// needed for texture pegging
extern fixed_t  *textureheight;

// needed for pre rendering (fracs)
extern fixed_t  *spritewidth;
extern fixed_t  *spriteoffset;
extern fixed_t  *spritetopoffset;

extern lighttable_t *colormaps;

// [JN] Black and white colormap
extern lighttable_t *colormaps_bw;

// for global animation
extern int   firstflat;
extern int  *flattranslation;	
extern int  *texturetranslation;	

// Sprite....
extern int   firstspritelump;
extern int   lastspritelump;
extern int   numspritelumps;


//
// Lookup tables for map data.
//
extern int          numsprites;
extern spritedef_t *sprites;

extern int          numvertexes;
extern vertex_t    *vertexes;

extern int          numsegs;
extern seg_t       *segs;

extern int          numsectors;
extern sector_t    *sectors;

extern int          numsubsectors;
extern subsector_t *subsectors;

extern int          numnodes;
extern node_t      *nodes;

extern int          numlines;
extern line_t      *lines;

extern int          numsides;
extern side_t      *sides;


//
// POV data.
//
extern fixed_t      viewx;
extern fixed_t      viewy;
extern fixed_t      viewz;

extern angle_t      viewangle;
extern player_t    *viewplayer;

extern angle_t      clipangle;

extern int          viewangletox[FINEANGLES/2];
extern angle_t     *xtoviewangle;
extern angle_t     *linearskyangle;

// angle to line origin
extern int          rw_angle1;

// Segs count?
extern int          sscount;

extern visplane_t  *floorplane;
extern visplane_t  *ceilingplane;
