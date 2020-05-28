//
// Copyright (C) 1993-1996 Id Software, Inc.
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
//	Handles WAD file header, directory, lump I/O.
//


#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <alloca.h>

#include "doomtype.h"
#include "doomstat.h"
#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"
#include "jn.h"


//
// GLOBALS
//

// Location of each lump on disk.
lumpinfo_t   *lumpinfo;		
int           numlumps;
void        **lumpcache;


void ExtractFileBase (char *path, char *dest)
{
    char  *src = path + strlen(path) - 1;
    int    length;

    // back up until a \ or the start
    while (src != path && *(src-1) != '\\' && *(src-1) != '/')
    {
        src--;
    }

    // copy up to eight characters
    memset (dest,0,8);
    length = 0;

    while (*src && *src != '.')
    {
        if (++length == 9)
        {
            I_Error(english_language ?
                    "Filename base of %s >8 chars" :
                    "����� ����� 䠩�� %s >8 ᨬ�����", path);
        }

        *dest++ = toupper((int)*src++);
    }
}


//
// LUMP BASED ROUTINES.
//

//
// W_AddFile
// All files are optional, but at least one file must be
//  found (PWAD, if all required lumps are present).
// Files with a .wad extension are wadlink files
//  with multiple lumps.
// Other files are single lumps with the base filename
//  for the lump name.
//
// If filename starts with a tilde, the file is handled
//  specially to allow map reloads.
// But: the reload feature is a fragile hack...

int     reloadlump;
char   *reloadname;

void W_AddFile (char *filename)
{
    int          handle;
    int          length;
    int          startlump;
    int          storehandle;
    unsigned     i;
    filelump_t  *fileinfo;
    filelump_t   singleinfo;    
    wadinfo_t    header;
    lumpinfo_t  *lump_p;

    // open the file and add to directory

    // handle reload indicator.
    if (filename[0] == '~')
    {
        filename++;
        reloadname = filename;
        reloadlump = numlumps;
    }

    if ((handle = open (filename,O_RDONLY | O_BINARY)) == -1)
    {
        printf (english_language ?
                "\tcouldn't open %s\n" :
                "\t���������� ������ %s\n", filename);
        return;
    }

    printf (english_language ?
            "\tadding %s\n" :
            "\t���������� %s\n", filename);

    startlump = numlumps;

    if (strcmpi (filename+strlen(filename)-3 , "wad" ) )
    {
        // single lump file
        fileinfo = &singleinfo;
        singleinfo.filepos = 0;
        singleinfo.size = LONG(filelength(handle));
        ExtractFileBase (filename, singleinfo.name);
        numlumps++;
    }
    else 
    {
        // WAD file
        read (handle, &header, sizeof(header));
        if (strncmp(header.identification,"IWAD",4))
        {
            // Homebrew levels?
            if (strncmp(header.identification,"PWAD",4))
            {
            I_Error (english_language ?
                    "Wad file %s doesn't have IWAD or PWAD id\n" :
                    "Wad-䠩� %s �� ᮤ�ন� ���ଠ樨 IWAD ��� PWAD\n", filename);
            }

            modifiedgame = true;		
        }

        header.numlumps = LONG(header.numlumps);
        header.infotableofs = LONG(header.infotableofs);
        length = header.numlumps*sizeof(filelump_t);

        if (header.numlumps > 4046)
        {
            // alloca would saturate the stack segment
            I_Error (english_language ?
                    "There isn't sufficient stack space available for %s (%d lumps)\n" :
                    "������⢮ ������ � 䠩�� %s (%d ������) �ॢ�ᨫ� �����\n",
                    filename, header.numlumps);
        }

        fileinfo = alloca (length);
        lseek (handle, header.infotableofs, SEEK_SET);
        read (handle, fileinfo, length);
        numlumps += header.numlumps;
    }

    // Fill in lumpinfo
    lumpinfo = realloc (lumpinfo, numlumps*sizeof(lumpinfo_t));

    if (!lumpinfo)
    {
        I_Error (english_language ?
                "Couldn't realloc lumpinfo" :
                "���������� ��।����� ���ଠ�� � ������");
    }

    lump_p = &lumpinfo[startlump];

    storehandle = reloadname ? -1 : handle;

    for (i=startlump ; i<numlumps ; i++,lump_p++, fileinfo++)
    {
        lump_p->handle = storehandle;
        lump_p->position = LONG(fileinfo->filepos);
        lump_p->size = LONG(fileinfo->size);
        strncpy (lump_p->name, fileinfo->name, 8);
    }

    if (reloadname)
    {
        close (handle);
    }
}


// jff 1/23/98 Create routines to reorder the master directory
// putting all flats into one marked block, and all sprites into another.
// This will allow loading of sprites and flats from a PWAD with no
// other changes to code, particularly fast hashes of the lumps.
//
// killough 1/24/98 modified routines to be a little faster and smaller

static int IsMarker(const char *marker, const char *name)
{
    return !strncasecmp(name, marker, 8) ||
    (*name == *marker && !strncasecmp(name+1, marker, 7));
}

// killough 4/17/98: add namespace tags

static void W_CoalesceMarkedResource(const char *start_marker, 
                                     const char *end_marker, int namespace)
{
    lumpinfo_t *marked = malloc(sizeof(*marked) * numlumps);
    size_t i, num_marked = 0, num_unmarked = 0;
    int is_marked = 0, mark_end = 0;
    lumpinfo_t *lump = lumpinfo;

    for (i=numlumps; i--; lump++)
    if (IsMarker(start_marker, lump->name))       // start marker found
    {
        // If this is the first start marker, add start marker to marked lumps
        if (!num_marked)
        {
            strncpy(marked->name, start_marker, 8);
            marked->size = 0;                   // killough 3/20/98: force size to be 0
            marked->namespace = ns_global;      // killough 4/17/98
            num_marked = 1;
        }

        is_marked = 1;                          // start marking lumps
    }
    else if (IsMarker(end_marker, lump->name))  // end marker found
    {
        mark_end = 1;                           // add end marker below
        is_marked = 0;                          // stop marking lumps
    } 
    else if (is_marked)                         // if we are marking lumps,
    {                                           // move lump to marked list
        marked[num_marked] = *lump;
        marked[num_marked++].namespace = namespace;  // killough 4/17/98
    }
    else
    {
        lumpinfo[num_unmarked++] = *lump;       // else move down THIS list
    }

    // Append marked list to end of unmarked list
    memcpy(lumpinfo + num_unmarked, marked, num_marked * sizeof(*marked));

    free(marked);                               // free marked list

    numlumps = num_unmarked + num_marked;       // new total number of lumps

    if (mark_end)                               // add end marker
    {
        lumpinfo[numlumps].size = 0;            // killough 3/20/98: force size to be 0
        lumpinfo[numlumps].namespace = ns_global;   // killough 4/17/98
        strncpy(lumpinfo[numlumps++].name, end_marker, 8);
    }
}


//
// W_Reload
// Flushes any of the reloadable lumps in memory
//  and reloads the directory.
//
void W_Reload (void)
{
    int          lumpcount;
    int          handle;
    int          length;
    unsigned     i;
    wadinfo_t    header;
    lumpinfo_t  *lump_p;
    filelump_t  *fileinfo;

    if (!reloadname)
    {
        return;
    }

    if ( (handle = open (reloadname,O_RDONLY | O_BINARY)) == -1)
    {
        I_Error(english_language ?
                "W_Reload: couldn't open %s" :
                "W_Reload: ���������� ������ %s", reloadname);
    }

    read (handle, &header, sizeof(header));
    lumpcount = LONG(header.numlumps);
    header.infotableofs = LONG(header.infotableofs);
    length = lumpcount*sizeof(filelump_t);
    fileinfo = alloca (length);
    lseek (handle, header.infotableofs, SEEK_SET);
    read (handle, fileinfo, length);

    // Fill in lumpinfo
    lump_p = &lumpinfo[reloadlump];

    for (i=reloadlump ; i<reloadlump+lumpcount ; i++,lump_p++, fileinfo++)
    {
        if (lumpcache[i])
        {
            Z_Free(lumpcache[i]);
        }

        lump_p->position = LONG(fileinfo->filepos);
        lump_p->size = LONG(fileinfo->size);
    }

    close (handle);
}


//
// W_InitMultipleFiles
// Pass a null terminated list of files to use.
// All files are optional, but at least one file
//  must be found.
// Files with a .wad extension are idlink files
//  with multiple lumps.
// Other files are single lumps with the base filename
//  for the lump name.
// Lump names can appear multiple times.
// The name searcher looks backwards, so a later file
//  does override all earlier ones.
//
void W_InitMultipleFiles (char **filenames)
{	
    int size;

    // open all the files, load headers, and count lumps
    numlumps = 0;

    // will be realloced as lumps are added
    lumpinfo = malloc(1);	

    for ( ; *filenames ; filenames++)
    {
        W_AddFile (*filenames);
    }

    if (!numlumps)
    {
        I_Error (english_language ?
                 "W_InitFiles: no files found" :
                 "W_InitFiles: ����� �� �������");
    }

    //jff 1/23/98
    // get all the sprites and flats into one marked block each
    // killough 1/24/98: change interface to use M_START/M_END explicitly
    // killough 4/17/98: Add namespace tags to each entry

    W_CoalesceMarkedResource("S_START", "S_END", ns_sprites);
    W_CoalesceMarkedResource("F_START", "F_END", ns_flats);

    // killough 4/4/98: add colormap markers
    W_CoalesceMarkedResource("C_START", "C_END", ns_colormaps);

    // set up caching
    size = numlumps * sizeof(*lumpcache);
    lumpcache = malloc (size);

    if (!lumpcache)
    {
	    I_Error (english_language ?
                 "Couldn't allocate lumpcache" :
                 "���������� �����㦨�� ��� ������");
    }

    memset (lumpcache,0, size);
}


//
// W_InitFile
// Just initialize from a single file.
//
void W_InitFile (char *filename)
{
    char *names[2];

    names[0] = filename;
    names[1] = NULL;
    W_InitMultipleFiles (names);
}


//
// W_NumLumps
//
int W_NumLumps (void)
{
    return numlumps;
}


//
// W_CheckNumForName
// Returns -1 if name not found.
//

int W_CheckNumForName (char *name)
{
    int          v1;
    int          v2;
    lumpinfo_t  *lump_p;

    union 
    {
        char  s[9];
        int   x[2];
    } name8;

    // make the name into two integers for easy compares
    strncpy (name8.s,name,8);

    // in case the name was a fill 8 chars
    name8.s[8] = 0;

    // case insensitive
    strupr (name8.s);		

    v1 = name8.x[0];
    v2 = name8.x[1];

    // scan backwards so patch lump files take precedence
    lump_p = lumpinfo + numlumps;

    while (lump_p-- != lumpinfo)
    {
        if ( *(int *)lump_p->name == v1 && *(int *)&lump_p->name[4] == v2)
        {
            return lump_p - lumpinfo;
        }
    }

    // TFB. Not found.
    return -1;
}


//
// W_GetNumForName
// Calls W_CheckNumForName, but bombs out if not found.
//
int W_GetNumForName (char *name)
{
    int	i = W_CheckNumForName (name);

    if (i == -1)
    {
      I_Error (english_language ?
               "W_GetNumForName: %s not found!" :
               "W_GetNumForName: %s �� ������!", name);
    }

    return i;
}


//
// W_LumpLength
// Returns the buffer size needed to load the given lump.
//
int W_LumpLength (int lump)
{
    if (lump >= numlumps)
    {
        I_Error ("W_LumpLength: %i >= numlumps", lump);
    }

    return lumpinfo[lump].size;
}


//
// W_ReadLump
// Loads the lump into the given buffer,
//  which must be >= W_LumpLength().
//
void W_ReadLump (int lump, void *dest)
{
    int		     c;
    int		     handle;
    lumpinfo_t  *l;

    if (lump >= numlumps)
    {
        I_Error ("W_ReadLump: %i >= numlumps", lump);
    }

    l = lumpinfo+lump;

    I_BeginRead ();

    if (l->handle == -1)
    {
        // reloadable file, so use open / read / close
        if ((handle = open (reloadname,O_RDONLY | O_BINARY)) == -1)
        {
            I_Error (english_language ?
                    "W_ReadLump: couldn't open %s" :
                    "W_ReadLump: ���������� ������ %s", reloadname);
        }
    }
    else
    {
        handle = l->handle;
    }

    lseek (handle, l->position, SEEK_SET);
    c = read (handle, dest, l->size);

    if (c < l->size)
    {
	    I_Error (english_language ?
                 "W_ReadLump: only read %i of %i on lump %i" :
                 "W_ReadLump: ���⠭� �ᥣ� %i �� %i � ����� %i",
                 c,l->size,lump);
    }

    if (l->handle == -1)
    {
        close (handle);
    }

    I_EndRead ();
}


//
// W_CacheLumpNum
//
void *W_CacheLumpNum (int lump, int tag)
{
    byte *ptr;

    if ((unsigned)lump >= numlumps)
    {
        I_Error ("W_CacheLumpNum: %i >= numlumps",lump);
    }

    if (!lumpcache[lump])
    {
        // read the lump in
        ptr = Z_Malloc (W_LumpLength (lump), tag, &lumpcache[lump]);
        W_ReadLump (lump, lumpcache[lump]);
    }
    else
    {
        Z_ChangeTag (lumpcache[lump],tag);
    }

    return lumpcache[lump];
}


//
// W_CacheLumpName
//
void *W_CacheLumpName (char *name, int tag)
{
    return W_CacheLumpNum (W_GetNumForName(name), tag);
}


//
// W_Profile
//
int info[2500][10];
int profilecount;

void W_Profile (void)
{
    int          i;
    int          j;
    char         ch;
    char         name[9];
    void        *ptr;
    FILE        *f;
    memblock_t  *block;

    for (i=0 ; i<numlumps ; i++)
    {	
        ptr = lumpcache[i];

        if (!ptr)
        {
            ch = ' ';
            continue;
        }
        else
        {
            block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));

            if (block->tag < PU_PURGELEVEL)
            {
                ch = 'S';
            }
            else
            {
                ch = 'P';
            }
        }

        info[i][profilecount] = ch;
    }

    profilecount++;
	
    f = fopen ("waddump.txt","w");
    name[8] = 0;

    for (i=0 ; i<numlumps ; i++)
    {
        memcpy (name,lumpinfo[i].name,8);

        for (j=0 ; j<8 ; j++)
            if (!name[j])
            break;

        for ( ; j<8 ; j++)
	    name[j] = ' ';

        fprintf (f,"%s ",name);

        for (j=0 ; j<profilecount ; j++)
        fprintf (f,"    %c",info[i][j]);

        fprintf (f,"\n");
    }

    fclose (f);
}

