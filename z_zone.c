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
//	Zone Memory Allocation. Neat.
//

#include "doomdef.h"
#include "i_system.h"
#include "z_zone.h"
#include "jn.h"


//
// ZONE MEMORY ALLOCATION
//
// There is never any space between memblocks,
//  and there will never be two contiguous free memblocks.
// The rover can be left pointing at a non-empty block.
//
// It is of no value to free a cachable block,
//  because it will get overwritten automatically if needed.
// 
 
#define ZONEID	0x1d4a11

boolean MallocFailureOk;


typedef struct
{
    int          size;      // total bytes malloced, including header
    memblock_t   blocklist; // start / end cap for linked list
    memblock_t  *rover;
} memzone_t;

memzone_t*	mainzone;


//
// Z_ClearZone
//
void Z_ClearZone (memzone_t *zone)
{
    memblock_t *block;

    // set the entire zone to one free block
    zone->blocklist.next =
    zone->blocklist.prev =
    block = (memblock_t *)( (byte *)zone + sizeof(memzone_t) );

    zone->blocklist.user = (void *)zone;
    zone->blocklist.tag = PU_STATIC;
    zone->rover = block;

    block->prev = block->next = &zone->blocklist;

    // NULL indicates a free block.
    block->user = NULL;	

    block->size = zone->size - sizeof(memzone_t);
}


//
// Z_Init
//
void Z_Init (void)
{
    int          size;
    memblock_t  *block;

    MallocFailureOk = false;

    mainzone = (memzone_t *)I_ZoneBase (&size);
    mainzone->size = size;

    // set the entire zone to one free block
    mainzone->blocklist.next =
    mainzone->blocklist.prev =
    block = (memblock_t *)((byte *)mainzone + sizeof(memzone_t));

    mainzone->blocklist.user = (void *)mainzone;
    mainzone->blocklist.tag = PU_STATIC;
    mainzone->rover = block;

    block->prev = block->next = &mainzone->blocklist;

    // NULL indicates a free block.
    block->user = NULL;

    block->size = mainzone->size - sizeof(memzone_t);
}


//
// Z_Free
//
void Z_Free (void *ptr)
{
    memblock_t *block;
    memblock_t *other;

    block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));

    if (block->id != ZONEID)
    {
        I_Error (english_language ?
        "Z_Free: freed a pointer without ZONEID" :
        "Z_Free: ��᢮�������� 㪠��⥫� ��� ZONEID");
    }

    if (block->user > (void **)0x100)
    {
        // smaller values are not pointers
        // Note: OS-dependend?

        // clear the user's mark
        *block->user = 0;
    }

    // mark as free
    block->user = NULL;	
    block->tag = 0;
    block->id = 0;

    other = block->prev;

    if (!other->user)
    {
        // merge with previous free block
        other->size += block->size;
        other->next = block->next;
        other->next->prev = other;

        if (block == mainzone->rover)
        {
            mainzone->rover = other;
        }

        block = other;
    }

    other = block->next;

    if (!other->user)
    {
        // merge the next free block onto the end
        block->size += other->size;
        block->next = other->next;
        block->next->prev = block;

        if (other == mainzone->rover)
        {
            mainzone->rover = block;
        }
    }
}


//
// Z_Malloc
// You can pass a NULL user if the tag is < PU_PURGELEVEL.
//

#define MINFRAGMENT		64

void *Z_Malloc (int size, int tag, void *user)
{
    int          extra;
    memblock_t  *start;
    memblock_t  *rover;
    memblock_t  *newblock;
    memblock_t  *base;

    size = (size + 3) & ~3;

    // scan through the block list,
    // looking for the first free block
    // of sufficient size,
    // throwing out any purgable blocks along the way.

    // account for size of block header
    size += sizeof(memblock_t);

    // if there is a free block behind the rover,
    //  back up over them
    base = mainzone->rover;

    if (!base->prev->user)
    {
        base = base->prev;
    }

    rover = base;
    start = base->prev;

    do
    {
        if (rover == start)
        {
            // scanned all the way around the list
            if (MallocFailureOk == true)
            {
                return NULL;
            }
            I_Error (english_language ?
                     "Z_Malloc: failed on allocation of %i bytes" :
                     "Z_Malloc: �訡�� �����㦥��� %i ���� �����", size);
        }

        if (rover->user)
        {
            if (rover->tag < PU_PURGELEVEL)
            {
                // hit a block that can't be purged,
                //  so move base past it
                base = rover = rover->next;
            }
            else
            {
                // free the rover block (adding the size to base)

                // the rover can be the base block
                base = base->prev;
                Z_Free ((byte *)rover+sizeof(memblock_t));
                base = base->next;
                rover = base->next;
            }
        }
        else
        {
            rover = rover->next;
        }
    } while (base->user || base->size < size);

    // found a block big enough
    extra = base->size - size;

    if (extra >  MINFRAGMENT)
    {
        // there will be a free fragment after the allocated block
        newblock = (memblock_t *) ((byte *)base + size );
        newblock->size = extra;

        // NULL indicates free block.
        newblock->user = NULL;	
        newblock->tag = 0;
        newblock->prev = base;
        newblock->next = base->next;
        newblock->next->prev = newblock;

        base->next = newblock;
        base->size = size;
    }

    if (user)
    {
        // mark as an in use block
        base->user = user;			
        *(void **)user = (void *) ((byte *)base + sizeof(memblock_t));
    }
    else
    {
        if (tag >= PU_PURGELEVEL)
        {
            I_Error (english_language ?
                    "Z_Malloc: an owner is required for purgable blocks" :
                    "Z_Malloc: ��� ��頥��� ������ ����� �ॡ���� ���������⨢�� ��ꥪ�");
        }

        // mark as in use, but unowned	
        base->user = (void *)2;		
    }

    base->tag = tag;

    // next allocation will start looking here
    mainzone->rover = base->next;	

    base->id = ZONEID;

    return (void *) ((byte *)base + sizeof(memblock_t));
}


//
// Z_FreeTags
//
void Z_FreeTags (int lowtag, int hightag)
{
    memblock_t *block;
    memblock_t *next;

    for (block = mainzone->blocklist.next ; block != &mainzone->blocklist ; block = next)
    {
        // get link before freeing
        next = block->next;

        // free block?
        if (!block->user)
        {
            continue;
        }

        if (block->tag >= lowtag && block->tag <= hightag)
        {
            Z_Free ( (byte *)block+sizeof(memblock_t));
        }
    }
}


//
// Z_DumpHeap
// Note: TFileDumpHeap( stdout ) ?
//
void Z_DumpHeap (int lowtag, int hightag)
{
    memblock_t *block;

    printf (english_language ?
            "zone size: %i  location: %p\n" :
            "ࠧ��� ����: %i  �ᯮ�������: %p\n",
            mainzone->size,mainzone);

    printf (english_language ?
            "tag range: %i to %i\n" :
            "�������� ���: %i � %i\n",
            lowtag, hightag);

    for (block = mainzone->blocklist.next ; ; block = block->next)
    {
        if (block->tag >= lowtag && block->tag <= hightag)
        {
            printf (english_language ?
            "block:%p    size:%7i    user:%p    tag:%3i\n" :
            "����:%p    ࠧ���:%7i    ���짮��⥫�:%p    ��:%3i\n",
		    block, block->size, block->user, block->tag);
        }

        if (block->next == &mainzone->blocklist)
        {
            // all blocks have been hit
            break;
        }

        if ((byte *)block + block->size != (byte *)block->next)
        {
            printf (english_language ?
                    "ERROR: block size does not touch the next block\n" :
                    "������: ࠧ��� ����� �� ���⨣��� ᫥���饣� �����\n");
        }

        if (block->next->prev != block)
        {
            printf (english_language ?
                    "ERROR: next block doesn't have proper back link\n" :
                    "������: ᫥���騩 ���� �� ����� ���४⭮� ���⭮� �裡\n");
        }

        if (!block->user && !block->next->user)
        {
            printf (english_language ?
                    "ERROR: two consecutive free blocks\n" :
                    "������: �᢮������� ��� ����� �����\n");
        }
    }
}


//
// Z_FileDumpHeap
//
void Z_FileDumpHeap (FILE *f)
{
    memblock_t *block;

    fprintf (f, english_language ?
             "zone size: %i  location: %p\n" :
             "ࠧ��� ����: %i  �ᯮ�������: %p\n", mainzone->size,mainzone);

    for (block = mainzone->blocklist.next ; ; block = block->next)
    {
        fprintf (f, english_language ?
                 "block:%p    size:%7i    user:%p    tag:%3i\n" :
                 "����:%p    ࠧ���:%7i    ���짮��⥫�:%p    ��:%3i\n",
                 block, block->size, block->user, block->tag);

        if (block->next == &mainzone->blocklist)
        {
            // all blocks have been hit
            break;
        }

        if ((byte *)block + block->size != (byte *)block->next)
        {
            printf (english_language ?
                    "ERROR: block size does not touch the next block\n" :
                    "������: ࠧ��� ����� �� ���⨣��� ᫥���饣� �����\n");
        }

        if (block->next->prev != block)
        {
            printf (english_language ?
                    "ERROR: next block doesn't have proper back link\n" :
                    "������: ᫥���騩 ���� �� ����� ���४⭮� ���⭮� �裡\n");
        }

        if (!block->user && !block->next->user)
        {
            printf (english_language ?
                    "ERROR: two consecutive free blocks\n" :
                    "������: �᢮������� ��� ����� �����\n");
        }
    }
}


//
// Z_CheckHeap
//
void Z_CheckHeap (void)
{
    memblock_t *block;

    for (block = mainzone->blocklist.next ; ; block = block->next)
    {
        if (block->next == &mainzone->blocklist)
        {
            // all blocks have been hit
            break;
        }

        if ((byte *)block + block->size != (byte *)block->next)
        {
            I_Error (english_language ?
                     "Z_CheckHeap: block size does not touch the next block\n" :
                     "Z_CheckHeap: ࠧ��� ����� �� ���ࠣ����� ᫥���騩 ����\n");
        }

        if (block->next->prev != block)
        {
            I_Error (english_language ?
                     "Z_CheckHeap: next block doesn't have proper back link\n" :
                     "Z_CheckHeap: ᫥���騩 ���� �� ����� ���४⭮� ���⭮� �裡\n");
        }

        if (!block->user && !block->next->user)
        {
            I_Error (english_language ?
                     "Z_CheckHeap: two consecutive free blocks\n" :
                     "Z_CheckHeap: �����㦥�� ��� ��᫥����⥫��� ᢮������ �����\n");
        }
    }
}


//
// Z_ChangeTag
//
void Z_ChangeTag2 (void *ptr, int tag)
{
    memblock_t *block;

    block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));

    if (block->id != ZONEID)
    {
        I_Error (english_language ?
                 "Z_ChangeTag: freed a pointer without ZONEID" :
                 "Z_ChangeTag: ��᢮�������� 㪠��⥫� ��� ZONEID");
    }

    if (tag >= PU_PURGELEVEL && (unsigned)block->user < 0x100)
    {
        I_Error (english_language ?
                 "Z_ChangeTag: an owner is required for purgable blocks" :
                 "Z_ChangeTag: ��� ��頥��� ������ ����� �ॡ���� ���������⨢�� ��ꥪ�");
    }

    block->tag = tag;
}


//
// Z_FreeMemory
//
int Z_FreeMemory (void)
{
    int          free;
    memblock_t  *block;

    free = 0;

    for (block = mainzone->blocklist.next ; block != &mainzone->blocklist; block = block->next)
    {
        if (!block->user || block->tag >= PU_PURGELEVEL)
        {
            free += block->size;
        }
    }
    return free;
}

