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
//	Refresh of things, i.e. objects represented by sprites.
//


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "doomdef.h"
#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"
#include "r_local.h"
#include "doomstat.h"
#include "v_trans.h"
#include "jn.h"


#define MINZ				(FRACUNIT*4)
#define BASEYCENTER			100
#define R_SpriteNameHash(s) ((unsigned)((s)[0]-((s)[1]*3-(s)[3]*2-(s)[2])*2))


typedef struct
{
    int x1;
    int x2;
    int column;
    int topclip;
    int bottomclip;

} maskdraw_t;


// constant arrays used for psprite clipping and initializing clipping
int negonearray[SCREENWIDTH];       // [crispy] 32-bit integer math
int screenheightarray[SCREENWIDTH]; // [crispy] 32-bit integer math

// variables used to look up and range check thing_t sprites patches
int               numsprites;
int               maxframe;
char             *spritename;
spritedef_t      *sprites;
spriteframe_t     sprtemp[29];

// Game functions
int               newvissprite;
vissprite_t       vissprites[MAXVISSPRITES];
vissprite_t      *vissprite_p;
vissprite_t       overflowsprite;
vissprite_t       vsprsortedhead;

// Sprite rotation 0 is facing the viewer,
//  rotation 1 is one angle turn CLOCKWISE around the axis.
// This is not the same as the angle,
//  which increases counter clockwise (protractor).
// There was a lot of stuff grabbed wrong, so I changed it...
int              *mfloorclip;   // [crispy] 32-bit integer math
int              *mceilingclip; // [crispy] 32-bit integer math
int64_t           sprtopscreen; // [crispy] WiggleFix
fixed_t           spryscale;
fixed_t           pspritescale;
fixed_t           pspriteiscale;


lighttable_t    **spritelights;

// [JN] Brightmaps
lighttable_t    **fullbrights_notgray;
lighttable_t    **fullbrights_notgrayorbrown;
lighttable_t    **fullbrights_redonly;
lighttable_t    **fullbrights_greenonly1;
lighttable_t    **fullbrights_greenonly2;
lighttable_t    **fullbrights_greenonly3;
lighttable_t    **fullbrights_orangeyellow;
lighttable_t    **fullbrights_dimmeditems;
lighttable_t    **fullbrights_explosivebarrel;
lighttable_t    **fullbrights_alllights;
lighttable_t    **fullbrights_candles;
lighttable_t    **fullbrights_pileofskulls;




extern boolean chainsaw_attack_swing;


//
// R_InstallSpriteLump
// Local function for R_InitSprites.
//
// [JN] Modified for proper sprite loading, code is taken from MBF.
//
void R_InstallSpriteLump (int lump, unsigned frame, unsigned rotation, boolean flipped)
{
    if (frame >= 29 || rotation > 8)
    {
        I_Error(english_language ?
                "R_InstallSpriteLump: Bad frame characters in lump %i" :
                "R_InstallSpriteLump: �����४�� �३� � ����� %i", lump);
    }

    if ((int)frame > maxframe)
    {
        maxframe = frame;
    }

    if (rotation == 0)
    {
        // the lump should be used for all rotations
        int r;

        for (r=0 ; r<8 ; r++)
            if (sprtemp[frame].lump[r]==-1)
            {
                sprtemp[frame].lump[r] = lump - firstspritelump;
                sprtemp[frame].flip[r] = (byte) flipped;
                sprtemp[frame].rotate = false; //jff 4/24/98 if any subbed, rotless
            }
        return;
    }

    // the lump is only used for one rotation
    if (sprtemp[frame].lump[--rotation] == -1)
    {
        sprtemp[frame].lump[rotation] = lump - firstspritelump;
        sprtemp[frame].flip[rotation] = (byte) flipped;
        sprtemp[frame].rotate = true; //jff 4/24/98 only change if rot used
    }
}


//
// R_InitSpriteDefs
// Pass a null terminated list of sprite names
//  (4 chars exactly) to be used.
// Builds the sprite rotation matrixes to account
//  for horizontally flipped sprites.
// Will report an error if the lumps are inconsistant. 
// Only called at startup.
//
// Sprite lump names are 4 characters for the actor,
//  a letter for the frame, and a number for the rotation.
// A sprite that is flippable will have an additional
//  letter/number appended.
// The rotation character can be 0 to signify no rotations.
//
// 1/25/98, 1/31/98 killough : Rewritten for performance
//
// Empirically verified to have excellent hash
// properties across standard Doom sprites:



void R_InitSpriteDefs (char **namelist) 
{
    int i;
    size_t numentries = lastspritelump-firstspritelump+1;
    struct { int index, next; } *hash;

    if (!numentries || !*namelist)
    {
        return;
    }

    // count the number of sprite names
    for (i=0; namelist[i]; i++);

    numsprites = i;

    sprites = Z_Malloc(numsprites *sizeof(*sprites), PU_STATIC, NULL);

    // Create hash table based on just the first four letters of each sprite
    // killough 1/31/98

    hash = malloc(sizeof(*hash)*numentries); // allocate hash table

    for (i=0; i<numentries; i++)             // initialize hash table as empty
    hash[i].index = -1;

    for (i=0; i<numentries; i++)             // Prepend each sprite to hash chain
    {                                        // prepend so that later ones win
        int j = R_SpriteNameHash(lumpinfo[i+firstspritelump].name) % numentries;
        hash[i].next = hash[j].index;
        hash[j].index = i;
    }

    // scan all the lump names for each of the names,
    //  noting the highest frame letter.

    for (i=0 ; i<numsprites ; i++)
    {
        const char *spritename = namelist[i];
        int j = hash[R_SpriteNameHash(spritename) % numentries].index;

        if (j >= 0)
        {
            memset(sprtemp, -1, sizeof(sprtemp));
            maxframe = -1;
            do
            {
                register lumpinfo_t *lump = lumpinfo + j + firstspritelump;

                // Fast portable comparison -- killough
                // (using int pointer cast is nonportable):

                if (!((lump->name[0] ^ spritename[0]) |
                      (lump->name[1] ^ spritename[1]) |
                      (lump->name[2] ^ spritename[2]) |
                      (lump->name[3] ^ spritename[3])))
                {
                    R_InstallSpriteLump(j+firstspritelump,
                                      lump->name[4] - 'A',
                                      lump->name[5] - '0',
                                      false);
                    if (lump->name[6])
                    R_InstallSpriteLump(j+firstspritelump,
                                      lump->name[6] - 'A',
                                      lump->name[7] - '0',
                                      true);
                }
            }
            while ((j = hash[j].next) >= 0);

            // check the frames that were found for completeness
            if ((sprites[i].numframes = ++maxframe))  // killough 1/31/98
            {
                int frame;

                for (frame = 0; frame < maxframe; frame++)
                switch ((int) sprtemp[frame].rotate)
                {
                    case -1:
                    // no rotations were found for that frame at all
                    // [crispy] make non-fatal
                    fprintf (stderr, english_language ?
                    "R_InitSprites: No patches found for %s frame %c" :
                    "R_InitSprites: ���������� ���� � �ࠩ� %.8s �३�� %c",
                    namelist[i], frame+'A');
                    break;

                    case 0:
                    // only the first rotation is needed
                    break;

                    case 1:
                    // must have all 8 frames
                    {
                        int rotation;
                        for (rotation=0 ; rotation<8 ; rotation++)
                            if (sprtemp[frame].lump[rotation] == -1)
                                I_Error (english_language ?
                                "R_InitSprites: Sprite %.8s frame %c is missing rotations" :
                                "R_InitSprites: � �ࠩ� %.8s ���������� ���� ����� � �३�� %c",
                                namelist[i], frame+'A');
                        break;
                    }
                }

                // allocate space for the frames present and copy sprtemp to it
                sprites[i].spriteframes = Z_Malloc (maxframe * sizeof(spriteframe_t), PU_STATIC, NULL);
                memcpy (sprites[i].spriteframes, sprtemp, maxframe*sizeof(spriteframe_t));
            }
        }
    }

    free(hash); // free hash table
}


//
// R_InitSprites
// Called at program start.
//
void R_InitSprites (char **namelist)
{
    int i;

    for (i=0 ; i<SCREENWIDTH ; i++)
    {
        negonearray[i] = -1;
    }

    R_InitSpriteDefs (namelist);
}


//
// R_ClearSprites
// Called at frame start.
//
void R_ClearSprites (void)
{
    vissprite_p = vissprites;
}


//
// R_NewVisSprite
//
vissprite_t* R_NewVisSprite (void)
{
    if (vissprite_p == &vissprites[MAXVISSPRITES])
    {
        return &overflowsprite;
    }

    vissprite_p++;

    return vissprite_p-1;
}


//
// R_DrawMaskedColumn
// Used for sprites and masked mid textures.
// Masked means: partly transparent, i.e. stored
//  in posts/runs of opaque pixels.
//
void R_DrawMaskedColumn (column_t* column)
{
    int64_t	topscreen;      // [crispy] WiggleFix
    int64_t	bottomscreen;   // [crispy] WiggleFix
    fixed_t	basetexturemid;

    basetexturemid = dc_texturemid;
    dc_texheight = 0;

    for ( ; column->topdelta != 0xff ; ) 
    {
        // calculate unclipped screen coordinates
        //  for post
        topscreen = sprtopscreen + spryscale*column->topdelta;
        bottomscreen = topscreen + spryscale*column->length;

        dc_yl = (int)((topscreen+FRACUNIT-1)>>FRACBITS); // [crispy] WiggleFix
        dc_yh = (int)((bottomscreen-1)>>FRACBITS); // [crispy] WiggleFix

        if (dc_yh >= mfloorclip[dc_x])
            dc_yh = mfloorclip[dc_x]-1;
        if (dc_yl <= mceilingclip[dc_x])
            dc_yl = mceilingclip[dc_x]+1;

        if (dc_yl <= dc_yh)
        {
            dc_source = (byte *)column + 3;
            dc_texturemid = basetexturemid - (column->topdelta<<FRACBITS);

            // Drawn by either R_DrawColumn or (SHADOW) R_DrawFuzzColumn.
            colfunc ();	
        }

        column = (column_t *)(  (byte *)column + column->length + 4);
    }

    dc_texturemid = basetexturemid;
}


//
// R_DrawVisSprite
//  mfloorclip and mceilingclip should also be set.
//
void R_DrawVisSprite (vissprite_t *vis, int x1, int x2)
{
    int        texturecolumn;
    fixed_t    frac;
    patch_t   *patch;
    column_t  *column;

    patch = W_CacheLumpNum (vis->patch+firstspritelump, PU_CACHE);
    dc_colormap = vis->colormap;

    if (!dc_colormap)
    {
        // NULL colormap = shadow draw
        colfunc = fuzzcolfunc;
    }
    else if (vis->mobjflags & MF_TRANSLATION)
    {
        colfunc = transcolfunc;
        dc_translation = translationtables - 256 +
            ((vis->mobjflags & MF_TRANSLATION) >> (MF_TRANSSHIFT-8));
    }
    else if (vis->translation)
    {
        colfunc = transcolfunc;
        dc_translation = vis->translation;
    }

    // [crispy] translucent sprites
    if (translucency && vis->mobjflags & MF_TRANSLUCENT && !vanilla)
    {
        colfunc = tlcolfunc;
    }

    dc_iscale = abs(vis->xiscale)>>detailshift;
    dc_texturemid = vis->texturemid;
    frac = vis->startfrac;
    spryscale = vis->scale;
    sprtopscreen = centeryfrac - FixedMul(dc_texturemid,spryscale);

    for (dc_x=vis->x1 ; dc_x<=vis->x2 ; dc_x++, frac += vis->xiscale)
    {
        static boolean error = 0;
        texturecolumn = frac>>FRACBITS;
#ifdef RANGECHECK
        if (texturecolumn < 0 || texturecolumn >= SHORT(patch->width))
        {
            // [crispy] make non-fatal
            if (!error)
            {
            fprintf (stderr, english_language ?
                    "R_DrawSpriteRange: bad texturecolumn" :
                    "R_DrawSpriteRange: �����४⭠� ���ଠ�� texturecolumn\n");
            error++;
            }
            continue;
        }
#endif
        column = (column_t *) ((byte *)patch + LONG(patch->columnofs[texturecolumn]));
        R_DrawMaskedColumn (column);
    }

    colfunc = basecolfunc;
}


//
// R_ProjectSprite
// Generates a vissprite for a thing
//  if it might be visible.
//
void R_ProjectSprite (mobj_t *thing)
{
    int             index;
    int             lump;
    int             x1;
    int             x2;
    unsigned        rot;
    boolean         flip;
    fixed_t         tr_x;
    fixed_t         tr_y;
    fixed_t         gxt;
    fixed_t         gyt;
    fixed_t         gzt;    // [JN] killough 3/27/98
    fixed_t         tx;
    fixed_t         tz;
    fixed_t         xscale;
    fixed_t         iscale;
    angle_t         ang;
    vissprite_t    *vis;
    spritedef_t    *sprdef;
    spriteframe_t  *sprframe;

    // transform the origin point
    tr_x = thing->x - viewx;
    tr_y = thing->y - viewy;

    gxt = FixedMul(tr_x,viewcos); 
    gyt = -FixedMul(tr_y,viewsin);

    tz = gxt-gyt; 

    // thing is behind view plane?
    if (tz < MINZ)
    {
        return;
    }

    xscale = FixedDiv(projection, tz);

    gxt = -FixedMul(tr_x,viewsin); 
    gyt = FixedMul(tr_y,viewcos); 
    tx = -(gyt+gxt); 

    // too far off the side?
    if (abs(tx)>(tz<<2))
    {
        return;
    }

    // decide which patch to use for sprite relative to player
#ifdef RANGECHECK
    if ((unsigned)thing->sprite >= numsprites)
    {
        I_Error (english_language ?
                "R_ProjectSprite: invalid sprite number %i " :
                "R_ProjectSprite: �����४�� ����� �ࠩ� %i ", thing->sprite);
    }
#endif
    sprdef = &sprites[thing->sprite];
#ifdef RANGECHECK
    if ((thing->frame&FF_FRAMEMASK) >= sprdef->numframes)
    {
    I_Error (english_language ?
            "R_ProjectSprite: invalid sprite frame %i : %i " :
            "R_ProjectSprite: �����४�� �३� �ࠩ� %i : %i ",
            thing->sprite, thing->frame);
    }
#endif
    sprframe = &sprdef->spriteframes[ thing->frame & FF_FRAMEMASK];

    if (sprframe->rotate)
    {
        // choose a different rotation based on player view
        ang = R_PointToAngle (thing->x, thing->y);
        rot = (ang-thing->angle+(unsigned)(ANG45/2)*9)>>29;
        lump = sprframe->lump[rot];
        flip = (boolean)sprframe->flip[rot];
    }
    else
    {
        // use single rotation for all views
        lump = sprframe->lump[0];
        flip = (boolean)sprframe->flip[0];
    }

    // [crispy] flip death sprites and corpses randomly
    if ((thing->flags & MF_CORPSE
    &&   thing->type != MT_CYBORG
    &&   thing->type != MT_BARREL
    &&   thing->type != MT_BOSSBRAIN)
    ||   thing->info->spawnstate == S_PLAY_DIE7
    ||   thing->info->spawnstate == S_PLAY_XDIE9)
    {
        if ((thing->health & 1) && randomly_flipcorpses && !vanilla)
        {
            flip = true;
        }
    }

    // calculate edges of the shape
    // [crispy] fix sprite offsets for mirrored sprites
    tx -= flip ? spritewidth[lump] - spriteoffset[lump] : spriteoffset[lump];
    x1 = (centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS;

    // off the right side?
    if (x1 > viewwidth)
    {
        return;
    }

    tx +=  spritewidth[lump];
    x2 = ((centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS) - 1;

    // off the left side
    if (x2 < 0)
    {
        return;
    }

    // [JN] killough 4/9/98: clip things which are out of view due to height
    gzt = thing->z + spritetopoffset[lump];

    if (thing->z > viewz + FixedDiv(viewheight << FRACBITS, xscale)
    ||  gzt      < viewz - FixedDiv((viewheight << FRACBITS)-viewheight, xscale))
    {
        return;
    }

    // [JN] quickly reject sprites with bad x ranges
    if (x1 >= x2)
    return;

    // store information in a vissprite
    vis = R_NewVisSprite ();
    vis->translation = NULL;
    vis->mobjflags = thing->flags;
    vis->scale = xscale<<detailshift;
    vis->gx = thing->x;
    vis->gy = thing->y;
    vis->gz = thing->z;
    vis->gzt = gzt;     // [JN] killough 3/27/98
    vis->texturemid = gzt - viewz;
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= viewwidth ? viewwidth-1 : x2;	
    iscale = FixedDiv (FRACUNIT, xscale);

    if (flip)
    {
        vis->startfrac = spritewidth[lump]-1;
        vis->xiscale = -iscale;
    }
    else
    {
        vis->startfrac = 0;
        vis->xiscale = iscale;
    }

    if (vis->x1 > x1)
    {
        vis->startfrac += vis->xiscale*(vis->x1-x1);
    }

    vis->patch = lump;

    // get light level
    if (thing->flags & MF_SHADOW)
    {
        // shadow draw
        vis->colormap = NULL;
    }
    else if (fixedcolormap)
    {
        // fixed map
        vis->colormap = fixedcolormap;
    }
    else if (thing->frame & FF_FULLBRIGHT)
    {
        // full bright
        vis->colormap = colormaps;
    }
    else
    {
        // diminished light
        index = xscale>>(LIGHTSCALESHIFT-detailshift);

        if (index >= MAXLIGHTSCALE)
        {
            index = MAXLIGHTSCALE-1;
        }

        vis->colormap = spritelights[index];

        // [JN] Applying brightmaps to sprites...
        if (brightmaps && !vanilla)
        {
            // Armor Bonus
            if (thing->type == MT_MISC3)
            vis->colormap = fullbrights_dimmeditems[index];

            // Cell Charge
            else if (thing->type == MT_MISC20)
            vis->colormap = fullbrights_dimmeditems[index];

            // Cell Charge Pack
            else if (thing->type == MT_MISC21)
            vis->colormap = fullbrights_dimmeditems[index];

            // BFG9000
            else if (thing->type == MT_MISC25)
            vis->colormap = fullbrights_redonly[index];

            // Plasmagun
            else if (thing->type == MT_MISC28)
            vis->colormap = fullbrights_redonly[index];

            // Explosive barrel
            else if (thing->type == MT_BARREL)
            vis->colormap = fullbrights_explosivebarrel[index];

            // Pile of skulls and candles (29)
            else if (thing->type == MT_MISC73)
            vis->colormap = fullbrights_pileofskulls[index];

            // Candlestick (34)
            else if (thing->type == MT_MISC49)
            vis->colormap = fullbrights_candles[index];

            // Candelabra (35)
            else if (thing->type == MT_MISC50)
            vis->colormap = fullbrights_candles[index];

            // Tall blue torch (44)
            else if (thing->type == MT_MISC41)
            vis->colormap = fullbrights_alllights[index];

            // Tall green torch (45)
            else if (thing->type == MT_MISC42)
            vis->colormap = fullbrights_alllights[index];

            // Tall red torch (46)
            else if (thing->type == MT_MISC43)
            vis->colormap = fullbrights_alllights[index];

            // Short blue torch (55)
            else if (thing->type == MT_MISC44)
            vis->colormap = fullbrights_alllights[index];

            // Short green torch (56)
            else if (thing->type == MT_MISC45)
            vis->colormap = fullbrights_alllights[index];

            // Short red torch (57)
            else if (thing->type == MT_MISC46)
            vis->colormap = fullbrights_alllights[index];

            // Burning barrel (70)
            else if (thing->type == MT_MISC77)
            vis->colormap = fullbrights_alllights[index];

            // Tall tech lamp (85)
            else if (thing->type == MT_MISC29)
            vis->colormap = fullbrights_alllights[index];

            // Short tech lamp (86)
            else if (thing->type == MT_MISC30)
            vis->colormap = fullbrights_alllights[index];

            // Floor lamp (2028)
            else if (thing->type == MT_MISC31)
            vis->colormap = fullbrights_alllights[index];
        }
        // [JN] Fallback. If we are not using brightmaps, apply full brightness
        // to the objects, thats no longer lit in info.c.
        else
        {
            if (thing->type == MT_MISC49    // Candlestick
            ||  thing->type == MT_MISC50    // Candelabra
            ||  thing->type == MT_MISC41    // Tall blue torch
            ||  thing->type == MT_MISC42    // Tall green torch
            ||  thing->type == MT_MISC43    // Tall red torch
            ||  thing->type == MT_MISC44    // Short blue torch
            ||  thing->type == MT_MISC45    // Short green torch
            ||  thing->type == MT_MISC46    // Short red torch
            ||  thing->type == MT_MISC73    // Pile of skulls and candles
            ||  thing->type == MT_MISC77    // Burning barrel
            ||  thing->type == MT_MISC29    // Tall tech lamp
            ||  thing->type == MT_MISC30    // Short tech lamp
            ||  thing->type == MT_MISC31)   // Floor lamp
            {
                vis->colormap = colormaps;
            }
        }
    }	

    // [crispy] colored blood 
    if (colored_blood && !vanilla 
    && (thing->type == MT_BLOOD || thing->sprite == SPR_POL5) && thing->target)
    {
        // [crispy] Cacodemons bleed blue blood
        if (thing->target->type == MT_HEAD)
        {
            vis->translation = cr[CR_BLUE2];
        }

        // [crispy] Barons of Hell and Hell Knights bleed green blood
        else
        if (thing->target->type == MT_BRUISER
        ||  thing->target->type == MT_KNIGHT)
        {
            vis->translation = cr[CR_GREEN];
        }
    }
}


//
// R_AddSprites
// During BSP traversal, this adds sprites by sector.
//
void R_AddSprites (sector_t *sec)
{
    int      lightnum;
    mobj_t  *thing;

    // BSP is traversed by subsector.
    // A sector might have been split into several
    //  subsectors during BSP building.
    // Thus we check whether its already added.
    if (sec->validcount == validcount)
    {
        return;
    }

    // Well, now it will be done.
    sec->validcount = validcount;

    lightnum = ((sec->lightlevel + level_brightness)
               >> LIGHTSEGSHIFT) + extralight;

    if (lightnum < 0)		
    {
        spritelights = scalelight[0];

        // [JN] Calculating sprite brightmaps
        fullbrights_dimmeditems = fullbright_dimmeditems[0];
        fullbrights_redonly = fullbright_redonly[0];
        fullbrights_explosivebarrel = fullbright_explosivebarrel[0];
        fullbrights_alllights = fullbright_alllights[0];
        fullbrights_candles = fullbright_candles[0];
        fullbrights_pileofskulls = fullbright_pileofskulls[0];
    }
    else if (lightnum >= LIGHTLEVELS)
    {
        spritelights = scalelight[LIGHTLEVELS-1];

        // [JN] Calculating sprite brightmaps
        fullbrights_dimmeditems = fullbright_dimmeditems[LIGHTLEVELS-1];
        fullbrights_redonly = fullbright_redonly[LIGHTLEVELS-1];
        fullbrights_explosivebarrel = fullbright_explosivebarrel[LIGHTLEVELS-1];
        fullbrights_alllights = fullbright_alllights[LIGHTLEVELS-1];
        fullbrights_candles = fullbright_candles[LIGHTLEVELS-1];
        fullbrights_pileofskulls = fullbright_pileofskulls[LIGHTLEVELS-1];
    }
    else
    {
        spritelights = scalelight[lightnum];

        // [JN] Calculating sprite brightmaps
        fullbrights_dimmeditems = fullbright_dimmeditems[lightnum];
        fullbrights_redonly = fullbright_redonly[lightnum];
        fullbrights_explosivebarrel = fullbright_explosivebarrel[lightnum];
        fullbrights_alllights = fullbright_alllights[lightnum];
        fullbrights_candles = fullbright_candles[lightnum];
        fullbrights_pileofskulls = fullbright_pileofskulls[lightnum];
    }

    // Handle all things in sector.
    for (thing = sec->thinglist ; thing ; thing = thing->snext)
    {
        R_ProjectSprite (thing);
    }
}


// [crispy] apply bobbing (or centering) to the player's weapon sprite
static inline void R_ApplyWeaponBob (fixed_t *sx, boolean bobx, fixed_t *sy, boolean boby)
{
	const angle_t angle = (128 * leveltime) & FINEMASK;

	if (sx)
	{
		*sx = FRACUNIT;

		if (bobx)
		{
			 *sx += FixedMul(viewplayer->bob, finecosine[angle]);
		}
	}

	if (sy)
	{
		*sy = 32 * FRACUNIT; // [crispy] WEAPONTOP

		if (boby)
		{
			*sy += FixedMul(viewplayer->bob, finesine[angle & (FINEANGLES / 2 - 1)]);
		}
	}
}

// [JN] Smooth bobbing for raise and lowering weapons
static inline void R_ApplyRaiseLowerBob (fixed_t *sx, boolean bobx)
{
	const angle_t angle = (128 * leveltime) & FINEMASK;

	if (sx)
	{
		*sx = FRACUNIT;

		if (bobx)
		{
			 *sx += FixedMul(viewplayer->bob, finecosine[angle]);
		}
	}
}

// [crispy] & [JN] Halfed amplitude and special SAW'ing amplitude
static inline void R_ApplyWeaponFiringBob (fixed_t *sx, boolean bobx, fixed_t *sy, boolean boby)
{
	const angle_t angle = (128 * leveltime) & FINEMASK;

	if (sx)
	{
		*sx = FRACUNIT;

		if (bobx)
		{
            if (chainsaw_attack_swing)
            *sx += FixedMul(viewplayer->bob, finecosine[angle] / 16);

            else
			*sx += FixedMul(viewplayer->bob, finecosine[angle] / 2);
		}
	}

	if (sy)
	{
		*sy = 32 * FRACUNIT; // [crispy] WEAPONTOP

		if (boby)
		{
            if (chainsaw_attack_swing)
            *sy += FixedMul(viewplayer->bob, finesine[angle*16 & (FINEANGLES / 2 - 1)] / 12);

            else
			*sy += FixedMul(viewplayer->bob, finesine[angle & (FINEANGLES / 2 - 1)] / 2);
		}
	}
}

//
// R_DrawPSprite
//
void R_DrawPSprite (pspdef_t *psp)
{
    int             lump;
    int             x1;
    int             x2;
    boolean         flip;
    fixed_t         tx;
    fixed_t         psp_sx = psp->sx;
    fixed_t         psp_sy = psp->sy;
    spritedef_t    *sprdef;
    spriteframe_t  *sprframe;
    vissprite_t    *vis;
    vissprite_t     avis;
    // [JN] For smoothen Chainsaw idle animation (from Crispy Doom).
    const int state = viewplayer->psprites[ps_weapon].state - states;

    // decide which patch to use
#ifdef RANGECHECK
    if ((unsigned)psp->state->sprite >= numsprites)
    {
        I_Error (english_language ?
                "R_ProjectSprite: invalid sprite number %i " :
                "R_ProjectSprite: �����४�� ����� �ࠩ� %i ",
                psp->state->sprite);
    }
#endif
    sprdef = &sprites[psp->state->sprite];
#ifdef RANGECHECK
    if ((psp->state->frame & FF_FRAMEMASK)  >= sprdef->numframes)
    {
        I_Error (english_language ?
                "R_ProjectSprite: invalid sprite frame %i : %i " :
                "R_ProjectSprite: �����४�� �३� �ࠩ� %i : %i ",
                psp->state->sprite, psp->state->frame);
    }
#endif
    sprframe = &sprdef->spriteframes[ psp->state->frame & FF_FRAMEMASK ];

    lump = sprframe->lump[0];
    flip = (boolean)sprframe->flip[0] ^ flip_weapons;

    // [crispy] Smoothen Chainsaw idle animation
    // [JN] ...and also apply standard bobbing for some frames of weapons
    if (weapon_bobbing && !vanilla && (
    /* Chainsaw  */ state == S_SAW      || state == S_SAWB     ||
    /* Shotgun   */ state == S_SGUN8    || state == S_SGUN9    ||
    /* SSG       */ state == S_DSGUN9   || state == S_DSGUN10  ||
    /* Plasmagun */ state == S_PLASMA2  ||
    /* BFG9000   */ state == S_BFG3     || state == S_BFG4 ))
    {
        R_ApplyWeaponBob(&psp_sx, true, &psp_sy, true);
    }

    // [JN] Smoothen bobbing while weapon changing (raising and lowering).
    if (weapon_bobbing && !vanilla && (
    /* Fist       */ state == S_PUNCHDOWN   || state == S_PUNCHUP   ||
    /* Chainsaw   */ state == S_SAWDOWN     || state == S_SAWUP     ||
    /* Pistol     */ state == S_PISTOLDOWN  || state == S_PISTOLUP  ||
    /* Shotgun    */ state == S_SGUNDOWN    || state == S_SGUNUP    ||
    /* SSG        */ state == S_DSGUNDOWN   || state == S_DSGUNUP   ||
    /* Chaingun   */ state == S_CHAINDOWN   || state == S_CHAINUP   ||
    /* RLauncher  */ state == S_MISSILEDOWN || state == S_MISSILEUP ||
    /* Plasmagun  */ state == S_PLASMADOWN  || state == S_PLASMAUP  ||
    /* BFG9000    */ state == S_BFGDOWN     || state == S_BFGUP ))
    {
        R_ApplyRaiseLowerBob(&psp_sx, true);
    }

    // [JN] Halfed amplitude for bobbing while moving and shooting
    if (weapon_bobbing && !vanilla && (
    /* Fist      */ state == S_PUNCH1   || state == S_PUNCH2   || state == S_PUNCH3   ||
                    state == S_PUNCH4   || state == S_PUNCH5   ||
    /* Chainsaw  */ state == S_SAW1     || state == S_SAW2     ||
    /* Pistol    */ state == S_PISTOL1  || state == S_PISTOL2  || state == S_PISTOL3  ||
                    state == S_PISTOL4  ||
    /* Shotgun   */ state == S_SGUN1    || state == S_SGUN2    || state == S_SGUN3    ||
                    state == S_SGUN4    || state == S_SGUN5    || state == S_SGUN6    ||
                    state == S_SGUN7    ||
    /* SSG       */ state == S_DSGUN1   || state == S_DSGUN2   || state == S_DSGUN3   ||
                    state == S_DSGUN4   || state == S_DSGUN5   || state == S_DSGUN6   ||
                    state == S_DSGUN7   ||
    /* Chaingun  */ state == S_CHAIN1   || state == S_CHAIN2   || state == S_CHAIN3   ||
    /* RLauncher */ state == S_MISSILE1 || state == S_MISSILE2 || state == S_MISSILE3 ||
    /* Plasmagun */ state == S_PLASMA1  ||
    /* BFG9000   */ state == S_BFG1     || state == S_BFG2 ))
    {
        R_ApplyWeaponFiringBob(&psp_sx, true, &psp_sy, true);
    }

    // [crispy] squat down weapon sprite a bit after hitting the ground
    if (weapon_bobbing && !vanilla)
    {
        psp_sy += abs(viewplayer->psp_dy);
    }

    // calculate edges of the shape
    tx = psp_sx-160*FRACUNIT;

    // [crispy] fix sprite offsets for mirrored sprites
    tx -= flip ? 2 * tx - spriteoffset[lump] + spritewidth[lump] : spriteoffset[lump];
    x1 = (centerxfrac + FixedMul (tx,pspritescale) ) >>FRACBITS;

    // off the right side
    if (x1 > viewwidth)
    {
        return;
    }

    tx +=  spritewidth[lump];
    x2 = ((centerxfrac + FixedMul (tx, pspritescale) ) >>FRACBITS) - 1;

    // off the left side
    if (x2 < 0)
    {
        return;
    }
    
    // store information in a vissprite
    vis = &avis;
    vis->translation = NULL;
    vis->mobjflags = 0;
    vis->texturemid = (BASEYCENTER<<FRACBITS)+FRACUNIT/2-(psp_sy-spritetopoffset[lump]);
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= viewwidth ? viewwidth-1 : x2;	
    vis->scale = pspritescale<<detailshift;

    if (flip)
    {
        vis->xiscale = -pspriteiscale;
        vis->startfrac = spritewidth[lump]-1;
    }
    else
    {
        vis->xiscale = pspriteiscale;
        vis->startfrac = 0;
    }

    // [JN] Mouselook: also move HUD weapons while mouse look. LOW detail friendly.
    vis->texturemid += FixedMul(((centery - viewheight / 2) << FRACBITS),
                       pspriteiscale>>detailshift);

    if (vis->x1 > x1)
    {
        vis->startfrac += vis->xiscale*(vis->x1-x1);
    }

    vis->patch = lump;

    if (viewplayer->powers[pw_invisibility] > 4*32
    || viewplayer->powers[pw_invisibility] & 8)
    {
        // shadow draw
        vis->colormap = NULL;
    }
    else if (fixedcolormap)
    {
        // fixed color
        vis->colormap = fixedcolormap;
    }
    else if (psp->state->frame & FF_FULLBRIGHT)
    {
        // full bright
        vis->colormap = colormaps;
    }
    else
    {
        // local light
        vis->colormap = spritelights[MAXLIGHTSCALE-1];
    }

    R_DrawVisSprite (vis, vis->x1, vis->x2);
}


//
// R_DrawPlayerSprites
//
void R_DrawPlayerSprites (void)
{
    int        i;
    int        lightnum;
    pspdef_t  *psp;
    // [JN] Define what "state" actually is:
    const int  state = viewplayer->psprites[ps_weapon].state - states;

    // get light level
    lightnum = ((viewplayer->mo->subsector->sector->lightlevel + level_brightness)
               >> LIGHTSEGSHIFT) + extralight;

    if (lightnum < 0)		
    {
        spritelights = scalelight[0];
    }
    else if (lightnum >= LIGHTLEVELS)
    {
        spritelights = scalelight[LIGHTLEVELS-1];
    }
    else
    {
        // [JN] Standard formula first
        spritelights = scalelight[lightnum];

        // [JN] Applying brightmaps to HUD weapons...
        if (!vanilla)
        {
            // BFG9000
            if (state == S_BFG1 || state == S_BFG2 || state == S_BFG3 || state == S_BFG4)
            spritelights = fullbright_redonly[lightnum];
        }
    }

    // clip to screen bounds
    mfloorclip = screenheightarray;
    mceilingclip = negonearray;

    // add all active psprites
    for (i=0, psp=viewplayer->psprites ; i < NUMPSPRITES ; i++, psp++)
    {
        if (psp->state)
        {
            R_DrawPSprite (psp);
        }
    }
}


//
// R_SortVisSprites
//
void R_SortVisSprites (void)
{
    int           i;
    int           count;
    fixed_t       bestscale;
    vissprite_t  *ds;
    vissprite_t  *best;
    vissprite_t   unsorted;

    count = vissprite_p - vissprites;
    unsorted.next = unsorted.prev = &unsorted;

    if (!count)
    {
        return;
    }

    for (ds=vissprites ; ds<vissprite_p ; ds++)
    {
        ds->next = ds+1;
        ds->prev = ds-1;
    }

    vissprites[0].prev = &unsorted;
    unsorted.next = &vissprites[0];
    (vissprite_p-1)->next = &unsorted;
    unsorted.prev = vissprite_p-1;

    // pull the vissprites out by scale
    //best = 0;		// shut up the compiler warning
    vsprsortedhead.next = vsprsortedhead.prev = &vsprsortedhead;
    for (i=0 ; i<count ; i++)
    {
        bestscale = MAXINT;

        for (ds=unsorted.next ; ds!= &unsorted ; ds=ds->next)
        {
            if (ds->scale < bestscale)
            {
                bestscale = ds->scale;
                best = ds;
            }
        }

        best->next->prev = best->prev;
        best->prev->next = best->next;
        best->next = &vsprsortedhead;
        best->prev = vsprsortedhead.prev;
        vsprsortedhead.prev->next = best;
        vsprsortedhead.prev = best;
    }
}


//
// R_DrawSprite
//
void R_DrawSprite (vissprite_t* spr)
{
    int         clipbot[SCREENWIDTH]; // [crispy] 32-bit integer math
    int         cliptop[SCREENWIDTH]; // [crispy] 32-bit integer math
    int         x;
    int         r1;
    int         r2;
    fixed_t     scale;
    fixed_t     lowscale;
    drawseg_t  *ds;

    for (x = spr->x1 ; x<=spr->x2 ; x++)
    {
        clipbot[x] = cliptop[x] = -2;
    }

    // Scan drawsegs from end to start for obscuring segs.
    // The first drawseg that has a greater scale is the clip seg.

    // Modified by Lee Killough:
    // (pointer check was originally nonportable
    // and buggy, by going past LEFT end of array):
    for (ds=ds_p ; ds-- > drawsegs ; )  // new -- killough
    {
        // determine if the drawseg obscures the sprite
        if (ds->x1 > spr->x2 || ds->x2 < spr->x1
        || (!ds->silhouette && !ds->maskedtexturecol))
        {
            continue;   // does not cover sprite
        }

        r1 = ds->x1 < spr->x1 ? spr->x1 : ds->x1;
        r2 = ds->x2 > spr->x2 ? spr->x2 : ds->x2;

        if (ds->scale1 > ds->scale2)
        {
            lowscale = ds->scale2;
            scale = ds->scale1;
        }
        else
        {
            lowscale = ds->scale1;
            scale = ds->scale2;
        }

        if (scale < spr->scale || (lowscale < spr->scale
        && !R_PointOnSegSide (spr->gx, spr->gy, ds->curline)))
        {
            if (ds->maskedtexturecol)   // masked mid texture?
            {
                R_RenderMaskedSegRange(ds, r1, r2);
            }
            continue;                   // seg is behind sprite
        }

        // clip this piece of the sprite
        // killough 3/27/98: optimized and made much shorter

        if (ds->silhouette&SIL_BOTTOM && spr->gz < ds->bsilheight) //bottom sil
            for (x=r1 ; x<=r2 ; x++)
                if (clipbot[x] == -2)
                    clipbot[x] = ds->sprbottomclip[x];

        if (ds->silhouette&SIL_TOP && spr->gzt > ds->tsilheight)   // top sil
            for (x=r1 ; x<=r2 ; x++)
                if (cliptop[x] == -2)
                    cliptop[x] = ds->sprtopclip[x];
    }

    // all clipping has been performed, so draw the sprite

    // check for unclipped columns
    for (x = spr->x1 ; x<=spr->x2 ; x++)
    {
        if (clipbot[x] == -2)
        {
            clipbot[x] = viewheight;
        }

        if (cliptop[x] == -2)
        {
            cliptop[x] = -1;
        }
    }

    mfloorclip = clipbot;
    mceilingclip = cliptop;
    R_DrawVisSprite (spr, spr->x1, spr->x2);
}


//
// R_DrawMasked
//
void R_DrawMasked (void)
{
    drawseg_t    *ds;
    vissprite_t  *spr;

    R_SortVisSprites ();

    if (vissprite_p > vissprites)
    {
        // draw all vissprites back to front
        for (spr = vsprsortedhead.next ; spr != &vsprsortedhead ; spr=spr->next)
        {
            R_DrawSprite (spr);
        }
    }

    // render any remaining masked mid textures

    // Modified by Lee Killough:
    // (pointer check was originally nonportable
    // and buggy, by going past LEFT end of array):
    for (ds=ds_p ; ds-- > drawsegs ; )  // new -- killough
        if (ds->maskedtexturecol)
            R_RenderMaskedSegRange (ds, ds->x1, ds->x2);

    // draw the psprites on top of everything
    //  but does not draw on side views
    if (!viewangleoffset)
    {
        R_DrawPlayerSprites ();
    }
}

