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
//	DOOM selection menu, options, episode etc.
//	Sliders and icons. Kinda widget stuff.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>


#include "doomdef.h"
#include "doomstat.h"
#include "dstrings.h"
#include "d_main.h"
#include "i_system.h"
#include "z_zone.h"
#include "v_video.h"
#include "w_wad.h"
#include "r_local.h"
#include "hu_stuff.h"
#include "g_game.h"
#include "m_misc.h"
#include "s_sound.h"
#include "doomstat.h"
#include "sounds.h"
#include "m_menu.h"
#include "v_trans.h"
#include "r_main.h"
#include "s_sound.h"
#include "st_stuff.h"

#include "jn.h"


#define SKULLXOFF		-32
#define LINEHEIGHT		16
#define LINEHEIGHT_SML  10  // [JN] Line height for small font
#define SAVESTRINGSIZE  24

void    (*messageRoutine)(int response);

int     mouseSensitivity;       // has default
int     showMessages;           // Show messages has default, 0 = off, 1 = on
int     sfxVolume;
int     musicVolume;
int     detailLevel;            // Blocky mode, has default, 0 = high, 1 = normal
int     screenblocks;           // has default
int     screenSize;             // temp for screenblocks (0-9)
int     quickSaveSlot;          // -1 = no quicksave slot picked!
int     messageToPrint;         // 1 = message to be printed
int     messx;                  // message x
int     messy;                  // message y
int     messageLastMenuActive;
int     saveStringEnter;        // we are going to be entering a savegame string
int     saveSlot;               // which slot to save in
int     saveCharIndex;          // which char we're editing
int     epi;

char    saveOldString[SAVESTRINGSIZE];  // old save description before edit
char    savegamestrings[10][SAVESTRINGSIZE];
char    tempstring[80];
char    endstring[160];
char    skullName[2][9] = {"M_SKULL1","M_SKULL2"};  // graphic name of skulls
char    detailNames[2][9] = {"M_GDHIGH","M_GDLOW"};
char    msgNames[2][9] = {"M_MSGOFF","M_MSGON"};

char   *messageString;          // ...and here is the message string!

short   itemOn;                 // menu item skull is on
short   skullAnimCounter;       // skull animation counter
short   whichSkull;             // which skull to draw

boolean inhelpscreens;
boolean menuactive;
boolean messageNeedsInput;      // timed message = no input from user
boolean QuickSaveTitle;         // [JN] Additional title "������� ����������"


extern int      st_palette;

extern boolean  chat_on;		// in heads-up code
extern boolean  sendpause;
extern boolean  message_dontfuckwithme;

extern patch_t *hu_font[HU_FONTSIZE];
extern patch_t *hu_font_small_eng[HU_FONTSIZE];
extern patch_t *hu_font_small_rus[HU_FONTSIZE];
extern patch_t *hu_font_big_eng[HU_FONTSIZE2];
extern patch_t *hu_font_big_rus[HU_FONTSIZE2];



//
// MENU TYPEDEFS
//
typedef struct
{
    // 0 = no cursor here, 1 = ok, 2 = arrows ok
    short	status;
    
    // [JN] Extended from 10 to 128, so long text string may appear
    char	name[128];
    
    // choice = menu item #.
    // if status = 2,
    //   choice=0:leftarrow,1:rightarrow
    void	(*routine)(int choice);
    
    // hotkey in menu
    char	alphaKey;			
} menuitem_t;


typedef struct menu_s
{
    short           numitems;	// # of menu items
    struct menu_s  *prevMenu;	// previous menu
    menuitem_t     *menuitems;	// menu items
    void          (*routine)();	// draw routine
    short           x;          // x of menu
    short           y;          // y of menu
    short           lastOn;		// last item user was on in menu
} menu_t;

menu_t*	currentMenu; // current menudef


char gammamsg[18][41] =
{
    GAMMA_IMPROVED_OFF,
    GAMMA_IMPROVED_05,
    GAMMA_IMPROVED_1,
    GAMMA_IMPROVED_15,
    GAMMA_IMPROVED_2,
    GAMMA_IMPROVED_25,
    GAMMA_IMPROVED_3,
    GAMMA_IMPROVED_35,
    GAMMA_IMPROVED_4,
    GAMMA_ORIGINAL_OFF,
    GAMMA_ORIGINAL_05,
    GAMMA_ORIGINAL_1,
    GAMMA_ORIGINAL_15,
    GAMMA_ORIGINAL_2,
    GAMMA_ORIGINAL_25,
    GAMMA_ORIGINAL_3,
    GAMMA_ORIGINAL_35,
    GAMMA_ORIGINAL_4
};

char gammamsg_rus[18][41] =
{
    GAMMA_IMPROVED_OFF_RUS,
    GAMMA_IMPROVED_05_RUS,
    GAMMA_IMPROVED_1_RUS,
    GAMMA_IMPROVED_15_RUS,
    GAMMA_IMPROVED_2_RUS,
    GAMMA_IMPROVED_25_RUS,
    GAMMA_IMPROVED_3_RUS,
    GAMMA_IMPROVED_35_RUS,
    GAMMA_IMPROVED_4_RUS,
    GAMMA_ORIGINAL_OFF_RUS,
    GAMMA_ORIGINAL_05_RUS,
    GAMMA_ORIGINAL_1_RUS,
    GAMMA_ORIGINAL_15_RUS,
    GAMMA_ORIGINAL_2_RUS,
    GAMMA_ORIGINAL_25_RUS,
    GAMMA_ORIGINAL_3_RUS,
    GAMMA_ORIGINAL_35_RUS,
    GAMMA_ORIGINAL_4_RUS
};

char endmsg1[NUM_QUITMESSAGES][80] =
{
    // DOOM1
    "are you sure you want to\nquit this great game?",
    "please don't leave, there's more\ndemons to toast!",
    "let's beat it -- this is turning\ninto a bloodbath!",
    "i wouldn't leave if i were you.\ndos is much worse.",
    "you're trying to say you like dos\nbetter than me, right?",
    "don't leave yet -- there's a\ndemon around that corner!",
    "ya know, next time you come in here\ni'm gonna toast ya.",
    "go ahead and leave. see if i care."
};

char endmsg2[NUM_QUITMESSAGES][80] =
{
    // QuitDOOM II messages
    QUITMSG,
    "you want to quit?\nthen, thou hast lost an eighth!",
    "don't go now, there's a \ndimensional shambler waiting\nat the dos prompt!",
    "get outta here and go back\nto your boring programs.",
    "if i were your boss, i'd \n deathmatch ya in a minute!",
    "look, bud. you leave now\nand you forfeit your body count!",
    "just leave. when you come\nback, i'll be waiting with a bat.",
    "you're lucky i don't smack\nyou for thinking about leaving."
};

char endmsg1_rus[NUM_QUITMESSAGES][80] =
{
    // DOOM1
    "are you sure you want to\nquit this great game?",
	// ��������, �� �室��! \n ����� ��⠫��� ��� ����� �������!
	"gj;fkeqcnf< yt e[jlbnt! \n pltcm jcnfkjcm tot vyjuj ltvjyjd!",
	// ������ ��।������, �� �㤥� \n ��� ��ࢠ� � ��᫥���� ����⪠!
	"lfdfqnt jghtltkbvcz< 'nj ,eltn \n dfif gthdfz b gjcktlyzz gjgsnrf!",
	// �� ��襬 ���� � �� �室��. \n ������� ������� ��筥�.
	"yf dfitv vtcnt z ,s yt e[jlbk>\nhf,jnfnm yfvyjuj crexytt>",
	// �� ��� ᪠����, �� ����樮���� \n ��⥬� ���� 祬 �, ��?
	"ds [jnbnt crfpfnm< xnj jgthfwbjyyfz \n cbcntvf kexit xtv z< lf?",
	// �� �室��, � ���쭥� 㣫� \n ��⠨��� �� ���� ������!
	"yt e[jlbnt< d lfkmytv euke \n ghbnfbkcz tot jlby vjycnh!",
	// �����, ����� �� �������, \n ��� ���� ����� ����訥 �����⭮��.
	"pyftnt< rjulf ds dthytntcm< \n dfc ,elen ;lfnm ,jkmibt ytghbznyjcnb>",
	// ������, �室��. \n ��� ��᮫�⭮ ��� ࠢ��.
	"lfdfqnt< e[jlbnt> \n vyt f,cjk.nyj dct hfdyj>"
};

char endmsg2_rus[NUM_QUITMESSAGES][80] =
{
    // QuitDOOM II messages
    // �� ����⢨⥫쭮 ��� ��� \n �� �⮩ �����⥫쭮� ����?
    "ds ltqcndbntkmyj [jnbnt dsqnb\nbp 'njq pfvtxfntkmyjq buhs?",
    // ���쥧 ���㬠�� ���? \n �������, �� �� �祭�-� ࠧ㬭�!
    "dcthmtp pflevfkb dsqnb?\ngjkfuf.< 'nj yt jxtym-nj hfpevyj!", 
    // �� �室��, � ����樮���� ��⥬� \n ��� ���� 㦠�� ������!
    "yt e[jlbnt< d jgthfwbjyyjq cbcntvt\ndfc ;len e;fcyst ltvjys!",
    // �⫨筮, ��� � 㡨ࠩ��� ��� � \n ᢮�� ���� �ணࠬ���!
    "jnkbxyj< djn b e,bhfqntcm jnc.lf\nr cdjbv crexysv ghjuhfvvfv!", 
    // ��� ��।��񭭮 ᫥��� \n ������� �� ᢥ����筮� �६�!
    "dfv jghtltktyyj cktletn\njcnfnmcz yf cdth[ehjxyjt dhtvz!",
    // ���蠩�, ���� ��⠭����, ���� \n ����� ������ ��-� ���譮�.
    "ckeifqnt< kexit jcnfymntcm< byfxt\nvj;tn ckexbnmcz xnj-nj cnhfiyjt>",
    // ���� �室��. ����� �������, \n ��� ��㯭� �� �����஢����.
    "ghjcnj e[jlbnt> rjulf dthytntcm<\ndfv rhegyj yt gjpljhjdbncz>", 
    // ��� �祭� �������, �� �� �� \n � ���� ��祣� �� ������!
    "dfv jxtym gjdtpkj< xnj pf 'nj \n c dfvb ybxtuj yt ckexbncz!"
};


// -----------------------------------------------------------------------------
// [JN] Custom RD menu: font writing prototypes
// -----------------------------------------------------------------------------

void M_WriteText(int x, int y, char *string);
void M_WriteTextSmall_ENG(int x, int y, char *string);
void M_WriteTextSmall_RUS(int x, int y, char *string);
void M_WriteTextBig_ENG(int x, int y, char *string);
void M_WriteTextBig_RUS(int x, int y, char *string);
void M_WriteTextBigCentered_ENG(int y, char *string);
void M_WriteTextBigCentered_RUS(int y, char *string);

//
// PROTOTYPES
//
void M_NewGame(int choice);
void M_Episode(int choice);
void M_ChooseSkill(int choice);
void M_LoadGame(int choice);
void M_SaveGame(int choice);
void M_Options(int choice);
void M_EndGame(int choice);
void M_ReadThis(int choice);
void M_ReadThis2(int choice);
void M_QuitDOOM(int choice);

void M_SfxVol(int choice);
void M_MusicVol(int choice);
void M_StartGame(int choice);
void M_Sound(int choice);

void M_FinishReadThis(int choice);
void M_LoadSelect(int choice);
void M_SaveSelect(int choice);
void M_ReadSaveStrings(void);
void M_QuickSave(void);
void M_QuickLoad(void);

void M_DrawMainMenu(void);
void M_DrawReadThis1(void);
void M_DrawReadThis2(void);
void M_DrawReadThisRetail(void);
void M_DrawNewGame(void);
void M_DrawEpisode(void);
void M_DrawOptions(void);
void M_DrawSound(void);
void M_DrawLoad(void);
void M_DrawSave(void);

void M_DrawSaveLoadBorder(int x,int y);
void M_SetupNextMenu(menu_t *menudef);
void M_DrawThermo(int x,int y,int thermWidth,int thermDot);
void M_DrawThermo_Small (int x, int y, int thermWidth, int thermDot);
void M_DrawEmptyCell(menu_t *menu,int item);
void M_DrawSelCell(menu_t *menu,int item);
int  M_StringWidth(char *string);
int  M_StringHeight(char *string);
void M_StartControlPanel(void);
void M_StartMessage(char *string,void *routine,boolean input);
void M_StopMessage(void);
void M_ClearMenus (void);


// -----------------------------------------------------------------------------
// [JN] Custom RD menu prototypes
// -----------------------------------------------------------------------------

// Main Options menu
void M_RD_Draw_Options(void);

// Rendering
void M_RD_Choose_Rendering(int choice);
void M_RD_Draw_Rendering(void);
void M_RD_Change_DiskIcon(int choice);
void M_RD_Change_NoFlats(int choice);
void M_RD_Change_Wiping(int choice);

// Display
void M_RD_Choose_Display(int choice);
void M_RD_Draw_Display(void);
void M_RD_Change_ScreenSize(int choice);
void M_RD_Change_Gamma(int choice);
void M_RD_Change_Detail(int choice);
void M_RD_Change_Messages(int choice);

// Automap
void M_RD_Choose_AutomapSettings(int choice);
void M_RD_Draw_AutomapSettings(void);
void M_RD_Change_AutomapColor(int choice);
void M_RD_Change_AutomapAntialias(int choice);
void M_RD_Change_AutomapStats(int choice);
// void M_RD_Change_AutomapOverlay(int choice);
void M_RD_Change_AutomapRotate(int choice);
void M_RD_Change_AutomapFollow(int choice);
void M_RD_Change_AutomapGrid(int choice);

// Sound
void M_RD_Choose_Audio(int choice);
void M_RD_Draw_Audio(void);
void M_RD_Change_SfxVol(int choice);
void M_RD_Change_MusicVol(int choice);
void M_RD_Change_SfxChannels(int choice);
void M_RD_Change_SndMode(int choice);
void M_RD_Change_PitchShifting(int choice);

// Controls
void M_RD_Choose_Controls(int choice);
void M_RD_Draw_Controls(void);
void M_RD_Change_AlwaysRun();
void M_RD_Change_MouseLook(int choice);
void M_RD_Change_Sensitivity(int choice);
void M_RD_Change_InvertY(int choice);
void M_RD_Change_Novert(int choice);

// Gameplay
void M_RD_Choose_Gameplay_1(int choice);
void M_RD_Choose_Gameplay_2(int choice);
void M_RD_Choose_Gameplay_3(int choice);
void M_RD_Choose_Gameplay_4(int choice);
void M_RD_Draw_Gameplay_1(void);
void M_RD_Draw_Gameplay_2(void);
void M_RD_Draw_Gameplay_3(void);
void M_RD_Draw_Gameplay_4(void);

void M_RD_Change_Brightmaps(int choice);
void M_RD_Change_FakeContrast(int choice);
void M_RD_Change_Transparency(int choice);
void M_RD_Change_ColoredHUD(int choice);
void M_RD_Change_ColoredBlood(int choice);
void M_RD_Change_SwirlingLiquids(int choice);
void M_RD_Change_InvulSky(int choice);
void M_RD_Change_ShadowedText(int choice);

void M_RD_Change_ExitSfx(int choice);
void M_RD_Change_CrushingSfx(int choice);
void M_RD_Change_BlazingSfx(int choice);
void M_RD_Change_AlertSfx(int choice);
void M_RD_Change_SecretNotify(int choice);
void M_RD_Change_NegativeHealth(int choice);

void M_RD_Change_WalkOverUnder(int choice);
void M_RD_Change_Torque(int choice);
void M_RD_Change_Bobbing(int choice);
void M_RD_Change_SSGBlast(int choice);
void M_RD_Change_FlipCorpses(int choice);
void M_RD_Change_FloatPowerups(int choice);

void M_RD_Change_CrosshairDraw(int choice);
void M_RD_Change_CrosshairHealth(int choice);

void M_RD_Change_ExtraPlayerFaces(int choice);
void M_RD_Change_LostSoulsQty(int choice);
void M_RD_Change_FastQSaveLoad(int choice);
void M_RD_Change_NoInternalDemos(int choice);

// Back to Defaults
void M_RD_BackToDefaultsResponse(int key);
void M_RD_BackToDefaults(int choice);

// Language hot-swapping
void M_RD_ChangeLanguage(int choice);


// -----------------------------------------------------------------------------
// M_WriteText
//
// Write a string using the hu_font
// -----------------------------------------------------------------------------
void M_WriteText (int x, int y, char *string)
{
    int    w, c, cx, cy;
    char  *ch;

    ch = string;
    cx = x;
    cy = y;

    while(1)
    {
        c = *ch++;
        if (!c)
            break;
        if (c == '\n')
        {
            cx = x;
            cy += 12;
            continue;
        }

        c = toupper(c) - HU_FONTSTART;
        if (c < 0 || c>= HU_FONTSIZE)
        {
            cx += 4;
            continue;
        }

        w = SHORT (hu_font[c]->width);
        if (cx+w > SCREENWIDTH)
            break;

        V_DrawShadowDirect(cx+1, cy+1, 0, hu_font[c]);
        V_DrawPatchDirect(cx, cy, 0, hu_font[c]);

        cx+=w;
    }
}

// -----------------------------------------------------------------------------
// M_WriteTextSmall_ENG
//
// [JN] Write a string using a small STCFS font
// -----------------------------------------------------------------------------

void M_WriteTextSmall_ENG (int x, int y, char *string)
{
    int     w, c;
    int     cx = x;
    int     cy = y;
    char   *ch = string;

    while(1)
    {
        c = *ch++;
        if (!c)
            break;
        if (c == '\n')
        {
            cx = x;
            cy += 12;
            continue;
        }

        c = toupper(c) - HU_FONTSTART;
        if (c < 0 || c>= HU_FONTSIZE)
        {
            cx += 4;
            continue;
        }

        w = SHORT (hu_font_small_eng[c]->width);
        if (cx+w > SCREENWIDTH)
            break;

        V_DrawShadowDirect(cx+1, cy+1, 0, hu_font_small_eng[c]);
        V_DrawPatchDirect(cx, cy, 0, hu_font_small_eng[c]);

        cx+=w;
    }
}

// -----------------------------------------------------------------------------
// M_WriteTextSmall_ENG
//
// [JN] Write a string using a small STCFS font
// -----------------------------------------------------------------------------

void M_WriteTextSmall_RUS (int x, int y, char *string)
{
    int     w, c;
    int     cx = x;
    int     cy = y;
    char   *ch = string;

    while(1)
    {
        c = *ch++;
        if (!c)
            break;
        if (c == '\n')
        {
            cx = x;
            cy += 12;
            continue;
        }

        c = toupper(c) - HU_FONTSTART;
        if (c < 0 || c>= HU_FONTSIZE)
        {
            cx += 4;
            continue;
        }

        w = SHORT (hu_font_small_rus[c]->width);
        if (cx+w > SCREENWIDTH)
            break;

        V_DrawShadowDirect(cx+1, cy+1, 0, hu_font_small_rus[c]);
        V_DrawPatchDirect(cx, cy, 0, hu_font_small_rus[c]);

        cx+=w;
    }
}

// -----------------------------------------------------------------------------
// M_WriteTextBig
//
// [JN] Write a string using a big STCFB font
// -----------------------------------------------------------------------------

void M_WriteTextBig_ENG (int x, int y, char *string)
{
    int    w, c, cx, cy;
    char  *ch;

    ch = string;
    cx = x;
    cy = y;

    while(1)
    {
        c = *ch++;
        if (!c)
        break;

        if (c == '\n')
        {
            cx = x;
            cy += 12;
            continue;
        }

        c = c - HU_FONTSTART2;
        if (c < 0 || c>= HU_FONTSIZE2)
        {
            cx += 7;
            continue;
        }

        w = SHORT (hu_font_big_eng[c]->width);
        if (cx+w > SCREENWIDTH)
        break;

        V_DrawShadowDirect(cx+1, cy+1, 0, hu_font_big_eng[c]);
        V_DrawPatchDirect(cx, cy, 0, hu_font_big_eng[c]);

        // Place one char to another with one pixel
        cx += w-1;
    }
}

// -----------------------------------------------------------------------------
// M_WriteTextBig
//
// [JN] Write a string using a big STCFB font
// -----------------------------------------------------------------------------

void M_WriteTextBig_RUS (int x, int y, char *string)
{
    int    w, c, cx, cy;
    char  *ch;

    ch = string;
    cx = x;
    cy = y;

    while(1)
    {
        c = *ch++;
        if (!c)
        break;

        if (c == '\n')
        {
            cx = x;
            cy += 12;
            continue;
        }

        c = c - HU_FONTSTART2;
        if (c < 0 || c>= HU_FONTSIZE2)
        {
            cx += 7;
            continue;
        }

        w = SHORT (hu_font_big_rus[c]->width);
        if (cx+w > SCREENWIDTH)
        break;

        V_DrawShadowDirect(cx+1, cy+1, 0, hu_font_big_rus[c]);
        V_DrawPatchDirect(cx, cy, 0, hu_font_big_rus[c]);

        // Place one char to another with one pixel
        cx += w-1;
    }
}

// -----------------------------------------------------------------------------
// HU_WriteTextBigCentered
//
// [JN] Write a centered string using the BIG hu_font_big. Only Y coord is set.
// -----------------------------------------------------------------------------

void M_WriteTextBigCentered_ENG (int y, char *string)
{
    int    c, cx, cy, w, width;
    char  *ch;

    // find width
    ch = string;
    width = 0;
    cy = y;

    while (ch)
    {
        c = *ch++;

        if (!c)
        break;

        c = c - HU_FONTSTART2;

        if (c < 0 || c> HU_FONTSIZE2)
        {
            width += 10;
            continue;
        }

        w = SHORT (hu_font_big_eng[c]->width);
        width += w;
    }

    // draw it
    cx = SCREENWIDTH/2-width/2;
    ch = string;
    while (ch)
    {
        c = *ch++;

        if (!c)
        break;

        c = c - HU_FONTSTART2;

        if (c < 0 || c> HU_FONTSIZE2)
        {
            cx += 10;
            continue;
        }

        w = SHORT (hu_font_big_eng[c]->width);

        V_DrawShadowDirect(cx+1, cy+1, 0, hu_font_big_eng[c]);
        V_DrawPatchDirect(cx, cy, 0, hu_font_big_eng[c]);

        cx+=w;
    }
}

// -----------------------------------------------------------------------------
// HU_WriteTextBigCentered
//
// [JN] Write a centered string using the BIG hu_font_big. Only Y coord is set.
// -----------------------------------------------------------------------------

void M_WriteTextBigCentered_RUS (int y, char *string)
{
    int    c, cx, cy, w, width;
    char  *ch;

    // find width
    ch = string;
    width = 0;
    cy = y;

    while (ch)
    {
        c = *ch++;

        if (!c)
        break;

        c = c - HU_FONTSTART2;

        if (c < 0 || c> HU_FONTSIZE2)
        {
            width += 10;
            continue;
        }

        w = SHORT (hu_font_big_rus[c]->width);
        width += w;
    }

    // draw it
    cx = SCREENWIDTH/2-width/2;
    ch = string;
    while (ch)
    {
        c = *ch++;

        if (!c)
        break;

        c = c - HU_FONTSTART2;

        if (c < 0 || c> HU_FONTSIZE2)
        {
            cx += 10;
            continue;
        }

        w = SHORT (hu_font_big_rus[c]->width);

        V_DrawShadowDirect(cx+1, cy+1, 0, hu_font_big_rus[c]);
        V_DrawPatchDirect(cx, cy, 0, hu_font_big_rus[c]);

        cx+=w;
    }
}


//
// DOOM MENU
//
enum
{
    newgame = 0,
    options,
    loadgame,
    savegame,
    readthis,
    quitdoom,
    main_end
} main_e;

// ------------
// English menu
// ------------

menuitem_t MainMenu[]=
{
    {1,"M_NGAME",M_NewGame,'n'},
    {1,"M_OPTION",M_Options,'o'},
    {1,"M_LOADG",M_LoadGame,'l'},
    {1,"M_SAVEG",M_SaveGame,'s'},
    // Another hickup with Special edition.
    {1,"M_RDTHIS",M_ReadThis,'r'},
    {1,"M_QUITG",M_QuitDOOM,'q'}
};

menu_t  MainDef =
{
    main_end,
    NULL,
    MainMenu,
    M_DrawMainMenu,
    97,64,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t MainMenu_Rus[]=
{
    {1, "RD_NGAME", M_NewGame,  'y'}, // ����� ���
    {1, "RD_OPTN",  M_Options,  'y'}, // ����ன��
    {1, "RD_LOADG", M_LoadGame, 'p'}, // ����㧪�
    {1, "RD_SAVEG", M_SaveGame, 'c'}, // ���࠭����
    {1, "RD_INFO",  M_ReadThis, 'b'}, // ���ଠ��!
    {1, "RD_QUITG", M_QuitDOOM, 'd'}  // ��室
};

menu_t  MainDef_Rus =
{
    main_end,
    NULL,
    MainMenu_Rus,
    M_DrawMainMenu,
    97,64,
    0
};

//
// EPISODE SELECT
//
enum
{
    ep1,
    ep2,
    ep3,
    ep4,
    ep_end
} episodes_e;

// ------------
// English menu
// ------------

menuitem_t EpisodeMenu[]=
{
    {1,"M_EPI1", M_Episode, 'k'},
    {1,"M_EPI2", M_Episode, 't'},
    {1,"M_EPI3", M_Episode, 'i'},
    {1,"M_EPI4", M_Episode, 't'}
};

menu_t  EpiDef =
{
    ep_end,
    &MainDef,
    EpisodeMenu,
    M_DrawEpisode,
    48,63,
    ep1
};

// ------------
// Russian menu
// ------------

menuitem_t EpisodeMenu_Rus[]=
{
    {1, "RD_EPI1", M_Episode, 'g'}, // �� ������ � ��㯠�
    {1, "RD_EPI2", M_Episode, 'g'}, // �ਡ०� ���
    {1, "RD_EPI3", M_Episode, 'b'}, // ���୮
    {1, "RD_EPI4", M_Episode, 'n'}  // ���� ����� ���饭�
};

menu_t  EpiDef_Rus =
{
    ep_end,
    &MainDef_Rus,
    EpisodeMenu_Rus,
    M_DrawEpisode,
    48,63,
    ep1
};

//
// NEW GAME
//
enum
{
    killthings,
    toorough,
    hurtme,
    violence,
    nightmare,
    ultra_nm,
    newg_end
} newgame_e;

// ------------
// English menu
// ------------

menuitem_t NewGameMenu[]=
{
    {1,"M_JKILL",	M_ChooseSkill, 'i'},
    {1,"M_ROUGH",	M_ChooseSkill, 'h'},
    {1,"M_HURT",	M_ChooseSkill, 'h'},
    {1,"M_ULTRA",	M_ChooseSkill, 'u'},
    {1,"M_NMARE",	M_ChooseSkill, 'n'},
    {1,"M_UNMARE",  M_ChooseSkill, 'z'}
};

menu_t  NewDef =
{
    newg_end,
    &EpiDef,
    NewGameMenu,
    M_DrawNewGame,
    48,63,
    hurtme
};

// ------------
// Russian menu
// ------------

menuitem_t NewGameMenu_Rus[]=
{
    {1, "RD_JKILL", M_ChooseSkill, 'v'}, // ��� ࠭� 㬨���.
    {1, "RD_ROUGH", M_ChooseSkill, 'y'}, // ��, �� ⠪ ��㡮.
    {1, "RD_HURT",  M_ChooseSkill, 'c'}, // ������ ��� ���쭮.
    {1, "RD_ULTRA", M_ChooseSkill, 'e'}, // ����࠭�ᨫ��
    {1, "RD_NMARE", M_ChooseSkill, 'r'}, // ��謠�.
    {1, "RD_UNMAR", M_ChooseSkill, 'e'}  // ����� ��謠�!
};

menu_t  NewDef_Rus =
{
    newg_end,
    &EpiDef_Rus,
    NewGameMenu_Rus,
    M_DrawNewGame,
    48,63,
    hurtme
};

// =============================================================================
// [JN] NEW OPTIONS MENU: STRUCTURE
// =============================================================================


//
// OPTIONS MENU
//
enum
{
    rd_rendering,
    rd_display,
    rd_sound,
    rd_controls,
    rd_gameplay,
    rd_endgame,
    rd_defaults,
    rd_language,
    rd_end
} options_e;

// ------------
// English menu
// ------------

menuitem_t RD_Options_Menu[]=
{
    {1,"Rendering",         M_RD_Choose_Rendering,  'r'},
    {1,"Display",           M_RD_Choose_Display,    'd'},
    {1,"Sound",             M_RD_Choose_Audio,      's'},
    {1,"Controls",          M_RD_Choose_Controls,   'c'},
    {1,"Gameplay",          M_RD_Choose_Gameplay_1, 'g'},
    {1,"End Game",          M_EndGame,              'e'},
    {1,"Reset settings",    M_RD_BackToDefaults,    'r'},
    {2, "Language:english", M_RD_ChangeLanguage,    'l'},
    {-1,"",0,'\0'}
};

menu_t  RD_Options_Def =
{
    rd_end,
    &MainDef,
    RD_Options_Menu,
    M_RD_Draw_Options,
    60,37,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t RD_Options_Menu_Rus[]=
{
    {1,"Dbltj",          M_RD_Choose_Rendering,  'd'},  // �����
    {1,"\"rhfy",         M_RD_Choose_Display,    '\''}, // ��࠭
    {1,"Felbj",          M_RD_Choose_Audio,      'f'},  // �㤨�
    {1,"Eghfdktybt",     M_RD_Choose_Controls,   'e'},  // ��ࠢ�����
    {1,"Utqvgktq",       M_RD_Choose_Gameplay_1, 'u'},  // ��������
    {1,"Pfrjyxbnm buhe", M_EndGame,              'p'},  // �������� ����
    {1,"C,hjc yfcnhjtr", M_RD_BackToDefaults,    'c'},  // ���� ����஥�
    {2, "Zpsr#heccrbq",  M_RD_ChangeLanguage,    'z'},  // ���: ���᪨�
    {-1,"",0,'\0'}
};

menu_t  RD_Options_Def_Rus =
{
    rd_end,
    &MainDef_Rus,
    RD_Options_Menu_Rus,
    M_RD_Draw_Options,
    60,37,
    0
};

// -----------------------------------------------------------------------------
// Video and Rendering
// -----------------------------------------------------------------------------

enum
{
    rd_rendering_wiping,
    rd_rendering_empty1,
    rd_rendering_diskicon,
    rd_rendering_noflats,
    rd_rendering_end
} rd_rendering_e;

// ------------
// English menu
// ------------

menuitem_t RD_Rendering_Menu[]=
{
    {2,"Floor and ceiling textures:", M_RD_Change_NoFlats,  'f'},
    {-1,"",0,'\0'},
    {2,"Show disk icon:",             M_RD_Change_DiskIcon, 's'},
    {2,"Screen wiping effect:",       M_RD_Change_Wiping,   's'},
    {-1,"",0,'\0'}
};

menu_t  RD_Rendering_Def =
{
    rd_rendering_end,
    &RD_Options_Def,
    RD_Rendering_Menu,
    M_RD_Draw_Rendering,
    35,45,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t RD_Rendering_Menu_Rus[]=
{
    {2,"Ntrcnehs gjkf b gjnjkrf:",   M_RD_Change_NoFlats,  'n'}, // �������� ���� � ��⮫��
    {-1,"",0,'\0'},
    {2,"Jnj,hf;fnm pyfxjr lbcrtns:", M_RD_Change_DiskIcon, 'j'}, // �⮡ࠦ��� ���箪 ��᪥��
    {2,"Gkfdyfz cvtyf \'rhfyjd:",    M_RD_Change_Wiping,   'g'}, // ������� ᬥ�� �࠭��
    {-1,"",0,'\0'}
};

menu_t  RD_Rendering_Def_Rus =
{
    rd_rendering_end,
    &RD_Options_Def_Rus,
    RD_Rendering_Menu_Rus,
    M_RD_Draw_Rendering,
    35,45,
    0
};

// -----------------------------------------------------------------------------
// Display settings
// -----------------------------------------------------------------------------

enum
{
    rd_display_screensize,
    rd_display_empty1,
    rd_display_gamma,
    rd_display_empty2,
    rd_display_detail,
    rd_display_messages,
    rd_display_empty3,
    rd_display_automap_settings,
    rd_display_end
} rd_display_e;

// ------------
// English menu
// ------------

menuitem_t RD_Display_Menu[]=
{
    {2,"screen size",       M_RD_Change_ScreenSize, 's'},
    {-1,"",0,'\0'},
    {2,"gamma-correction",  M_RD_Change_Gamma,      'g'},
    {-1,"",0,'\0'},
    {2,"detail level:",     M_RD_Change_Detail,     'e'},
    {2,"messages enabled:", M_RD_Change_Messages,   'j'},
    {-1,"",0,'\0'},
    {1, "automap settings", M_RD_Choose_AutomapSettings, 'a'},
    {-1,"",0,'\0'}
};

menu_t  RD_Display_Def =
{
    rd_display_end,
    &RD_Options_Def,
    RD_Display_Menu,
    M_RD_Draw_Display,
    35,45,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t RD_Display_Menu_Rus[]=
{
    {2,"hfpvth buhjdjuj \'rhfyf", M_RD_Change_ScreenSize, 'h'}, // ������ ��஢��� �࠭�
    {-1,"",0,'\0'},                                             //
    {2,"ehjdtym ufvvf-rjhhtrwbb", M_RD_Change_Gamma,      'e'}, // �஢��� �����-���४樨
    {-1,"",0,'\0'},                                             //
    {2,"ehjdtym ltnfkbpfwbb:",    M_RD_Change_Detail,     'e'}, // �஢��� ��⠫���樨:
    {2,"jnj,hf;tybt cjj,otybq:",  M_RD_Change_Messages,   'j'}, // �⮡ࠦ���� ᮮ�饭��:
    {-1,"",0,'\0'},
    {1, "yfcnhjqrb rfhns",        M_RD_Choose_AutomapSettings, 'y'}, // ����ன�� �����
    {-1,"",0,'\0'}
};

menu_t  RD_Display_Def_Rus =
{
    rd_display_end,
    &RD_Options_Def_Rus,
    RD_Display_Menu_Rus,
    M_RD_Draw_Display,
    35,45,
    0
};

// -----------------------------------------------------------------------------
// Automap settings
// -----------------------------------------------------------------------------

enum
{
    rd_automap_colors,
    rd_automap_antialias,
    rd_automap_stats,
//  rd_automap_overlay,
    rd_automap_rotate,
    rd_automap_follow,
    rd_automap_grid,
    rd_automap_end
} rd_automap_e;

// ------------
// English menu
// ------------

menuitem_t RD_Automap_Menu[]=
{
    {2, "color scheme:",      M_RD_Change_AutomapColor,     'c'},
    {2, "line antialiasing:", M_RD_Change_AutomapAntialias, 'l'},
    {2, "level stats:",       M_RD_Change_AutomapStats,     'l'},
//  {2, "overlay mode:",      M_RD_Change_AutomapOverlay,   'o'},
    {2, "rotate mode:",       M_RD_Change_AutomapRotate,    'r'},
    {2, "follow mode:",       M_RD_Change_AutomapFollow,    'f'},
    {2, "grid: o",            M_RD_Change_AutomapGrid,      'g'},
    {-1,"",0,'\0'}
};

menu_t  RD_Automap_Def =
{
    rd_automap_end,
    &RD_Display_Def,
    RD_Automap_Menu,
    M_RD_Draw_AutomapSettings,
    70,35,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t RD_Automap_Menu_Rus[]=
{
    {2, "wdtnjdfz c[tvf:",    M_RD_Change_AutomapColor,     'w'}, // ���⮢�� �奬�:
    {2, "cukf;bdfybt kbybq:", M_RD_Change_AutomapAntialias, 'c'}, // ����������� �����:
    {2, "cnfnbcnbrf ehjdyz:", M_RD_Change_AutomapStats,     'c'}, // ����⨪� �஢��:
//  {2, "ht;bv yfkj;tybz:",   M_RD_Change_AutomapOverlay,   'h'}, // ����� ���������:
    {2, "ht;bv dhfotybz:",    M_RD_Change_AutomapRotate,    'h'}, // ����� ��饭��:
    {2, "ht;bv cktljdfybz:",  M_RD_Change_AutomapFollow,    'h'}, // ����� ᫥�������:
    {2, "ctnrf:",             M_RD_Change_AutomapGrid,      'c'}, // ��⪠:
    {-1,"",0,'\0'}
};

menu_t  RD_Automap_Def_Rus =
{
    rd_automap_end,
    &RD_Display_Def_Rus,
    RD_Automap_Menu_Rus,
    M_RD_Draw_AutomapSettings,
    70,35,
    0
};

// -----------------------------------------------------------------------------
// Sound and Music
// -----------------------------------------------------------------------------

enum
{
    rd_audio_sfxvolume,
    rd_audio_empty1,
    rd_audio_musvolume,
    rd_audio_empty2,
    rd_audio_empty3,
    rd_audio_sfxchannels,
    rd_audio_empty4,
    rd_audio_sndmode,
    rd_audio_sndpitch,
    rd_audio_end
} rd_audio_e;

// ------------
// English menu
// ------------

menuitem_t RD_Audio_Menu[]=
{
    {2,"sfx volume",            M_RD_Change_SfxVol,        's'},
    {-1,"",0,'\0'},
    {2,"music volume",          M_RD_Change_MusicVol,      'm'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {2,"sound channels",        M_RD_Change_SfxChannels,   's'},
    {-1,"",0,'\0'},
    {2,"sound effects mode:",   M_RD_Change_SndMode,       's'},
    {2,"pitch-shifted sounds:", M_RD_Change_PitchShifting, 'g'},
    {-1,"",0,'\0'}
};

menu_t RD_Audio_Def =
{
    rd_audio_end,
    &RD_Options_Def,
    RD_Audio_Menu,
    M_RD_Draw_Audio,
    35,45,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t RD_Audio_Menu_Rus[]=
{
    {2,"pder",                       M_RD_Change_SfxVol,        'p'}, // ���
    {-1,"",0,'\0'},                                                   //
    {2,"vepsrf",                     M_RD_Change_MusicVol,      'v'}, // ��몠
    {-1,"",0,'\0'},                                                   //
    {-1,"",0,'\0'},                                                   //
    {2,"Pderjdst rfyfks",            M_RD_Change_SfxChannels,   'p'}, // ��㪮�� ������
    {-1,"",0,'\0'},                                                   //
    {2,"Ht;bv pderf:",               M_RD_Change_SndMode,       'h'}, // ����� ��㪠
    {2,"ghjbpdjkmysq gbnx-ibanbyu:", M_RD_Change_PitchShifting, 'g'}, // �ந������ ����-��⨭�
    {-1,"",0,'\0'}
};

menu_t RD_Audio_Def_Rus =
{
    rd_audio_end,
    &RD_Options_Def_Rus,
    RD_Audio_Menu_Rus,
    M_RD_Draw_Audio,
    35,45,
    0
};

// -----------------------------------------------------------------------------
// Keyboard and Mouse
// -----------------------------------------------------------------------------

enum
{
    rd_controls_alwaysrun,
    rd_controls_empty1,
    rd_controls_sensitivity,
    rd_controls_empty2,
    rd_controls_mouselook,
    rd_controls_inverty,
    rd_controls_novert,
    rd_controls_end
} rd_controls_e;

// ------------
// English menu
// ------------

menuitem_t RD_Controls_Menu[]=
{
    {2,"always run:",     M_RD_Change_AlwaysRun,   'a'},
    {-1,"",0,'\0'},
    {2,"mouse sensivity", M_RD_Change_Sensitivity, 'm'},
    {-1,"",0,'\0'},
    {2,"mouse look:",     M_RD_Change_MouseLook,   'm'},
    {2, "invert y axis:",     M_RD_Change_InvertY, 'i'},
    {2, "vertical movement:", M_RD_Change_Novert,  'v'},
    {-1,"",0,'\0'}
};

menu_t  RD_Controls_Def =
{
    rd_controls_end,
    &RD_Options_Def,
    RD_Controls_Menu,
    M_RD_Draw_Controls,
    35,45,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t RD_Controls_Menu_Rus[]=
{
    {2,"ht;bv gjcnjzyyjuj ,tuf:", M_RD_Change_AlwaysRun,   'g'}, // ����� ����ﭭ��� ����
    {-1,"",0,'\0'},                                              //
    {2,"Crjhjcnm vsib",           M_RD_Change_Sensitivity, 'c'}, // ������� ���
    {-1,"",0,'\0'},                                              //
    {2,"J,pjh vsim.:",            M_RD_Change_MouseLook,   'j'}, // ����� �����
    {2, "dthnbrfkmyfz bydthcbz:",    M_RD_Change_InvertY,  'd'}, // ���⨪��쭠� �������
    {2, "dthnbrfkmyjt gthtvtotybt:", M_RD_Change_Novert,   'd'}, // ���⨪��쭮� ��६�饭��
    {-1,"",0,'\0'}
};

menu_t  RD_Controls_Def_Rus =
{
    rd_controls_end,
    &RD_Options_Def_Rus,
    RD_Controls_Menu_Rus,
    M_RD_Draw_Controls,
    35,45,
    0
};

// -----------------------------------------------------------------------------
// Gameplay enhancements
// -----------------------------------------------------------------------------

enum
{
    rd_gameplay_1_brightmaps,
    rd_gameplay_1_fake_contrast,
    rd_gameplay_1_colored_hud,
    rd_gameplay_1_colored_blood,
    rd_gameplay_1_swirling_liquids,
    rd_gameplay_1_invul_sky,
    rd_gameplay_1_draw_shadowed_text,
    rd_gameplay_1_empty1,
    rd_gameplay_1_empty2,
    rd_gameplay_1_empty3,
    rd_gameplay_1_next_page,
    rd_gameplay_1_last_page,
    rd_gameplay_1_end
} rd_gameplay_1_e;

enum
{
    rd_gameplay_2_play_exit_sfx,
    rd_gameplay_2_crushed_corpses_sfx,
    rd_gameplay_2_blazing_door_fix_sfx,
    rd_gameplay_2_noise_alert_sfx,
    rd_gameplay_2_empty1,
    rd_gameplay_2_secret_notification,
    rd_gameplay_2_negative_health,
    rd_gameplay_2_empty2,
    rd_gameplay_2_empty3,
    rd_gameplay_2_next_page,
    rd_gameplay_2_prev_page,
    rd_gameplay_2_end
} rd_gameplay_2_e;

enum
{
    rd_gameplay_3_over_under,
    rd_gameplay_3_torque,
    rd_gameplay_3_weapon_bobbing,
    rd_gameplay_3_ssg_blast_enemies,
    rd_gameplay_3_randomly_flipcorpses,
    rd_gameplay_3_floating_powerups,
    rd_gameplay_3_empty1,
    rd_gameplay_3_crosshair_draw,
    rd_gameplay_3_crosshair_health,
    rd_gameplay_3_next_page,
    rd_gameplay_3_prev_page,
    rd_gameplay_3_end
} rd_gameplay_3_e;

enum
{
    rd_gameplay_4_extra_player_faces,
    rd_gameplay_4_unlimited_lost_souls,
    rd_gameplay_4_fast_quickload,
    rd_gameplay_4_no_internal_demos,
    rd_gameplay_4_empty1,
    rd_gameplay_4_empty2,
    rd_gameplay_4_empty3,
    rd_gameplay_4_empty4,
    rd_gameplay_4_first_page,
    rd_gameplay_4_prev_page,
    rd_gameplay_4_end
} rd_gameplay_4_e;

// ------------
// English menu
// ------------

menuitem_t RD_Gameplay_Menu_1[]=
{
    {2,"Brightmaps:",                  M_RD_Change_Brightmaps,      'b'},
    {2,"Fake contrast:",               M_RD_Change_FakeContrast,    'f'},
    {2,"Colored HUD elements:",        M_RD_Change_ColoredHUD,      'c'},
    {2,"Colored blood and corpses:",   M_RD_Change_ColoredBlood,    'c'},
    {2,"Swirling liquids:",            M_RD_Change_SwirlingLiquids, 's'},
    {2,"Invulnerability affects sky:", M_RD_Change_InvulSky,        'i'},
    {2,"Text casts shadows:",          M_RD_Change_ShadowedText,    't'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {1,"", /* Next Page > */           M_RD_Choose_Gameplay_2,      'n'},
    {1,"", /* < Last Page */           M_RD_Choose_Gameplay_4,      'l'},
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_1 =
{
    rd_gameplay_1_end,
    &RD_Options_Def,
    RD_Gameplay_Menu_1,
    M_RD_Draw_Gameplay_1,
    35,45,
    0
};

menuitem_t RD_Gameplay_Menu_2[]=
{
    {2,"Play exit sounds:",               M_RD_Change_ExitSfx,        'p'},
    {2,"Sound of crushing corpses:",      M_RD_Change_CrushingSfx,    's'},
    {2,"Single sound of blazing door:",   M_RD_Change_BlazingSfx,     's'},
    {2,"Monster alert waking up others:", M_RD_Change_AlertSfx,       'm'},
    {-1,"",0,'\0'},
    {2,"Notify of revealed secrets:",     M_RD_Change_SecretNotify,   'n'},
    {2,"Show negative health:",           M_RD_Change_NegativeHealth, 's'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {1,"", /* Next page >   */            M_RD_Choose_Gameplay_3,     'n'},
    {1,"", /* < Prev page > */            M_RD_Choose_Gameplay_1,     'p'},
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_2 =
{
    rd_gameplay_2_end,
    &RD_Options_Def,
    RD_Gameplay_Menu_2,
    M_RD_Draw_Gameplay_2,
    35,45,
    0
};

menuitem_t RD_Gameplay_Menu_3[]=
{
    {2,"Walk over and under monsters:",       M_RD_Change_WalkOverUnder,   'w'},
    {2,"Corpses sliding from the ledges:",    M_RD_Change_Torque,          'c'},
    {2,"Weapon bobbing while firing:",        M_RD_Change_Bobbing,         'w'},
    {2,"Lethal pellet of a point-blank SSG:", M_RD_Change_SSGBlast,        'l'},
    {2,"Randomly mirrored corpses:",          M_RD_Change_FlipCorpses,     'r'},
    {2,"Floating powerups:",                  M_RD_Change_FloatPowerups,   'f'},
    {-1,"",0,'\0'},
    {2,"Draw crosshair:",                     M_RD_Change_CrosshairDraw,   'd'},
    {2,"Health indication:",                  M_RD_Change_CrosshairHealth, 'h'},
    {1,"", /* Next page >   */                M_RD_Choose_Gameplay_4,      'n'},
    {1,"", /* < Prev page > */                M_RD_Choose_Gameplay_2,      'p'},
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_3 =
{
    rd_gameplay_3_end,
    &RD_Options_Def,
    RD_Gameplay_Menu_3,
    M_RD_Draw_Gameplay_3,
    35,45,
    0
};

menuitem_t RD_Gameplay_Menu_4[]=
{
    {2,"Extra player faces on the HUD:",      M_RD_Change_ExtraPlayerFaces, 'e'},
    {2,"Pain Elemental without Souls limit:", M_RD_Change_LostSoulsQty,     'p'},
    {2,"Don't prompt for q. saving/loading:", M_RD_Change_FastQSaveLoad,    'd'},
    {2,"Play internal demos:",                M_RD_Change_NoInternalDemos,  'p'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {1,"", /* First page >  */                M_RD_Choose_Gameplay_1,       'f'},
    {1,"", /* < Prev page > */                M_RD_Choose_Gameplay_3,       'p'},
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_4 =
{
    rd_gameplay_4_end,
    &RD_Options_Def,
    RD_Gameplay_Menu_4,
    M_RD_Draw_Gameplay_4,
    35,45,
    0
};

// ------------
// Russian menu
// ------------

menuitem_t RD_Gameplay_Menu_1_Rus[]=
{
    {2,",hfqnvfggbyu:",                  M_RD_Change_Brightmaps,      ','}, // �ࠩ⬠�����
    {2,"Bvbnfwbz rjynhfcnyjcnb:",        M_RD_Change_FakeContrast,    'b'}, // ������ ������⭮��
    {2,"Hfpyjwdtnyst 'ktvtyns $:",       M_RD_Change_ColoredHUD,      'h'}, // �����梥�� ������ HUD
    {2,"Hfpyjwdtnyfz rhjdm b nhegs:",    M_RD_Change_ColoredBlood,    'h'}, // �����梥⭠� �஢� � ����
    {2,"ekexityyfz fybvfwbz ;blrjcntq:", M_RD_Change_SwirlingLiquids, 'e'}, // ����襭��� ������� ������⥩
    {2,"ytezpdbvjcnm jrhfibdftn yt,j:",  M_RD_Change_InvulSky,        'y'}, // ���梨������ ���訢��� ����
    {2,"ntrcns jn,hfcsdf.n ntym:",       M_RD_Change_ShadowedText,    'n'}, // ������ ����뢠�� ⥭�
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {1,"",                               M_RD_Choose_Gameplay_2,      'l'}, // ����� >
    {1,"",                               M_RD_Choose_Gameplay_4,      'y'}, // < �����
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_1_Rus =
{
    rd_gameplay_1_end,
    &RD_Options_Def_Rus,
    RD_Gameplay_Menu_1_Rus,
    M_RD_Draw_Gameplay_1,
    35,45,
    0
};

menuitem_t RD_Gameplay_Menu_2_Rus[]=
{
    {2,"Pderb ghb ds[jlt bp buhs:",     M_RD_Change_ExitSfx,        'p'}, // ��� �� ��室� �� ����
    {2,"Pder hfplfdkbdfybz nhegjd:",    M_RD_Change_CrushingSfx,    'p'}, // ��� ࠧ���������� ��㯮�
    {2,"Jlbyjxysq pder ,scnhjq ldthb:", M_RD_Change_BlazingSfx,     'j'}, // ������� ��� ����ன ����
    {2,"J,ofz nhtdjuf e vjycnhjd:",     M_RD_Change_AlertSfx,       'j'}, // ���� �ॢ��� � �����஢
    {-1,"",0,'\0'},
    {2,"Cjj,ofnm j yfqltyyjv nfqybrt:", M_RD_Change_SecretNotify,   'c'}, // ������� � ��������� ⠩����
    {2,"jnhbwfntkmyjt pljhjdmt d $:",   M_RD_Change_NegativeHealth, 'j'}, // ����⥫쭮� ���஢� � HUD
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {1,"",                              M_RD_Choose_Gameplay_3,     'l'}, // ����� >
    {1,"",                              M_RD_Choose_Gameplay_1,     'y'}, // < �����
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_2_Rus =
{
    rd_gameplay_2_end,
    &RD_Options_Def_Rus,
    RD_Gameplay_Menu_2_Rus,
    M_RD_Draw_Gameplay_2,
    35,45,
    0
};

menuitem_t RD_Gameplay_Menu_3_Rus[]=
{
    {2,"Gthtvtotybt gjl/yfl vjycnhfvb:", M_RD_Change_WalkOverUnder,   'g'}, // ��६�饭�� ���/��� �����ࠬ�
    {2,"Nhegs cgjkpf.n c djpdsitybq:",   M_RD_Change_Torque,          'n'}, // ���� ᯮ����� � �����襭��
    {2,"Ekexityyjt gjrfxbdfybt jhe;bz:", M_RD_Change_Bobbing,         'e'}, // ����襭��� ����稢���� ��㦨�
    {2,"ldecndjkrf hfphsdftn dhfujd:",   M_RD_Change_SSGBlast,        'l'}, // ����⢮��� ࠧ�뢠�� �ࠣ��
    {2,"pthrfkbhjdfybt nhegjd:",         M_RD_Change_FlipCorpses,     'p'}, // ��ઠ��஢���� ��㯮�
    {2,"Ktdbnbhe.obt caths-fhntafrns:",  M_RD_Change_FloatPowerups,   'k'}, // ��������騥 ����-���䠪��
    {-1,"",0,'\0'},
    {2,"Jnj,hf;fnm ghbwtk:",             M_RD_Change_CrosshairDraw,   'j'}, // �⮡ࠦ��� ��楫
    {2,"Bylbrfwbz pljhjdmz:",            M_RD_Change_CrosshairHealth, 'b'}, // �������� ���஢��
    {1,"",                               M_RD_Choose_Gameplay_4,      'l'}, // ����� >
    {1,"",                               M_RD_Choose_Gameplay_2,      'y'}, // < �����
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_3_Rus =
{
    rd_gameplay_3_end,
    &RD_Options_Def_Rus,
    RD_Gameplay_Menu_3_Rus,
    M_RD_Draw_Gameplay_3,
    35,45,
    0
};

menuitem_t RD_Gameplay_Menu_4_Rus[]=
{
    {2,"Ljgjkybntkmyst kbwf buhjrf:",     M_RD_Change_ExtraPlayerFaces, 'l'}, // �������⥫�� ��� ��ப�
    {2,"'ktvtynfkm ,tp juhfybxtybz lei:", M_RD_Change_LostSoulsQty,     '\''},// ������⠫� ��� ��࠭�祭�� ���
    {2,"jnrk.xbnm pfghjc ,> pfuheprb:",   M_RD_Change_FastQSaveLoad,    'j'}, // �⪫���� ����� �. ����㧪�
    {2,"Ghjbuhsdfnm ltvjpfgbcb:",         M_RD_Change_NoInternalDemos,  'g'}, // �ந��뢠�� ���������
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {-1,"",0,'\0'},
    {1,"",                                M_RD_Choose_Gameplay_1,       'l'}, // ����� >
    {1,"",                                M_RD_Choose_Gameplay_3,       'y'}, // < �����
    {-1,"",0,'\0'}
};

menu_t  RD_Gameplay_Def_4_Rus =
{
    rd_gameplay_4_end,
    &RD_Options_Def_Rus,
    RD_Gameplay_Menu_4_Rus,
    M_RD_Draw_Gameplay_4,
    35,45,
    0
};


// =============================================================================
// [JN] NEW OPTIONS MENU: DRAWING
// =============================================================================

// -----------------------------------------------------------------------------
// Main Options menu
// -----------------------------------------------------------------------------

void M_RD_Draw_Options(void)
{
    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "OPTIONS");
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB"); // ���������
    }
}

// -----------------------------------------------------------------------------
// Rendering
// -----------------------------------------------------------------------------

void M_RD_Choose_Rendering(int choice)
{
    M_SetupNextMenu(english_language ? 
                    &RD_Rendering_Def :
                    &RD_Rendering_Def_Rus);
}

void M_RD_Draw_Rendering(void)
{
    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "RENDERING OPTIONS");

        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "Rendering");
        dp_translation = NULL;

        M_WriteTextSmall_ENG(217, 45, noflats ? "off" : "on");

        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 55, "Extra");
        dp_translation = NULL;

        M_WriteTextSmall_ENG(241, 65, show_diskicon ? "on" : "off");
        M_WriteTextSmall_ENG(204, 75, screen_wiping ? "on" : "off");
    }
    else
    {
        // ��������� �����
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB DBLTJ");

        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "htylthbyu"); // �����ਭ�
        dp_translation = NULL;

        M_WriteTextSmall_RUS(217, 45, noflats ? "dsrk" : "drk");

        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 55, "ljgjkybntkmyj"); // �������⥫쭮
        dp_translation = NULL;

        M_WriteTextSmall_RUS(241, 65, show_diskicon ? "drk" : "dsrk");
        M_WriteTextSmall_RUS(204, 75, screen_wiping ? "drk" : "dsrk");
    }
}

void M_RD_Change_DiskIcon (int choice)
{
    show_diskicon ^= 1;
}

void M_RD_Change_NoFlats (int choice)
{
    noflats ^= 1;

    // Reinitialize drawing functions
    R_ExecuteSetViewSize();
}

void M_RD_Change_Wiping (int choice)
{
    screen_wiping ^= 1;
}

// -----------------------------------------------------------------------------
// Display settings
// -----------------------------------------------------------------------------

void M_RD_Choose_Display(int choice)
{
    M_SetupNextMenu(english_language ?
                    &RD_Display_Def :
                    &RD_Display_Def_Rus);
}

void M_RD_Draw_Display(void)
{
    char    num[4];

    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "DISPLAY OPTIONS");

        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "Screen");
        dp_translation = NULL;

        // Detail level
        M_WriteTextSmall_ENG(193, 85, detailLevel ? "ybprbq" : "dscjrbq");

        // Messages allowed
        M_WriteTextSmall_ENG(214, 95, showMessages ? "drk" : "dsrk");

        //
        // Interface
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 105, "Interface");  
        dp_translation = NULL;
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB \"RHFYF"); // ��������� ������

        //
        // ��࠭
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "\'rhfy");
        dp_translation = NULL;

        // Write "on" / "off" strings for features
        M_WriteTextSmall_RUS(193, 85, detailLevel ? "ybprbq" : "dscjrbq");
        M_WriteTextSmall_RUS(214, 95, showMessages ? "drk" : "dsrk");

        //
        // ����䥩�
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 105, "bynthatqc");  
        dp_translation = NULL;
    }

    // Draw screen size slider
    M_DrawThermo_Small(35, 55, 12, screenSize);
    
    // Draw numerical representation of slider position
    snprintf(num, 4, "%3d", screenblocks);
    M_WriteTextSmall_ENG(145, 55, num);

    // Draw gamma-correction slider
    M_DrawThermo_Small(35, 75, 18, usegamma);
}

void M_RD_Change_ScreenSize(int choice)
{
    switch(choice)
    {
        case 0:
        if (screenSize > 0)
        {
            screenblocks--;
            screenSize--;
        }
        break;

        case 1:
        if (screenSize < 11)
        {
            screenblocks++;
            screenSize++;
        }
        break;
    }

    R_SetViewSize (screenblocks, detailLevel);
}

void M_RD_Change_Gamma(int choice)
{
    switch(choice)
    {
        case 0:
        if (usegamma > 0)
        {
            usegamma--;
        }
        break;

        case 1:
        if (usegamma < 17)
        {
            usegamma++;
        }
        break;
    }
    I_SetPalette ((byte *)W_CacheLumpName(usegamma <= 8 ?
                                          "PALFIX" :
                                          "PLAYPAL",
                                          PU_CACHE) + 
                                          st_palette * 768);
    players[consoleplayer].message = english_language ?
                                     gammamsg[usegamma] :
                                     gammamsg_rus[usegamma];
}

void M_RD_Change_Detail(int choice)
{
    detailLevel ^= 1;
    R_SetViewSize (screenblocks, detailLevel);
    players[consoleplayer].message = detailLevel ? detaillo : detailhi;
}

void M_RD_Change_Messages (int choice)
{
    showMessages ^= 1;
    players[consoleplayer].message = showMessages ? msgon : msgoff;
    message_dontfuckwithme = true;
}

// -----------------------------------------------------------------------------
// Automap settings
// -----------------------------------------------------------------------------

void M_RD_Choose_AutomapSettings(int choice)
{
    M_SetupNextMenu(english_language ? 
                    &RD_Automap_Def :
                    &RD_Automap_Def_Rus);
}

void M_RD_Draw_AutomapSettings(void)
{
    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "AUTOMAP SETTINGS");

        // Automap colors (English only names, different placement)
        if (automap_color == 0)
        M_WriteTextSmall_ENG (170, 35, "doom");
        else if (automap_color == 1)
        M_WriteTextSmall_ENG (170, 35, "boom");
        else if (automap_color == 2)
        M_WriteTextSmall_ENG (170, 35, "jaguar");
        else if (automap_color == 3)
        M_WriteTextSmall_ENG (170, 35, "raven");
        else
        M_WriteTextSmall_ENG (170, 35, "strife");

        // Line antialiasing
        M_WriteTextSmall_ENG(193, 45, automap_antialias ? "on" : "off");

        // Level stats
        M_WriteTextSmall_ENG(159, 55, automap_stats ? "on" : "off");

        /*
        // Overlay mode
        M_WriteTextSmall_ENG(170, 65, automap_overlay ? "on" : "off");
        */

        // Rotate mode
        M_WriteTextSmall_ENG(163, 65, automap_rotate ? "on" : "off");


        // Follow mode
        M_WriteTextSmall_ENG(164, 75, automap_follow ? "on" : "off");

        // Grid
        M_WriteTextSmall_ENG(106, 85, automap_grid ? "on" : "off");
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB RFHNS"); // ��������� �����

        // Automap colors (English only names, different placement)
        if (automap_color == 0)
        M_WriteTextSmall_ENG (191, 35, "doom");
        else if (automap_color == 1)
        M_WriteTextSmall_ENG (191, 35, "boom");
        else if (automap_color == 2)
        M_WriteTextSmall_ENG (191, 35, "jaguar");
        else if (automap_color == 3)
        M_WriteTextSmall_ENG (191, 35, "raven");
        else
        M_WriteTextSmall_ENG (191, 35, "strife");

        // ����������� �����
        M_WriteTextSmall_RUS(214, 45, automap_antialias ? "drk" : "dsrk");

        // ����⨪� �஢��
        M_WriteTextSmall_RUS(210, 55, automap_stats ? "drk" : "dsrk");

        /*
        // ����� ���������
        M_WriteTextSmall_RUS(203, 65, automap_overlay ? "drk" : "dsrk");
        */

        // ����� ��饭��
        M_WriteTextSmall_RUS(194, 65, automap_rotate ? "drk" : "dsrk");

        // ����� ᫥�������
        M_WriteTextSmall_RUS(208, 75, automap_follow ? "drk" : "dsrk");

        // ��⪠
        M_WriteTextSmall_RUS(118, 85, automap_grid ? "drk" : "dsrk");
    }
}

void M_RD_Change_AutomapColor(int choice)
{
    switch(choice)
    {
        case 0: 
        automap_color--;
        if (automap_color < 0) 
            automap_color = 4;
        break;
    
        case 1:
        automap_color++;
        if (automap_color > 4)
            automap_color = 0;
        break;
    }
}

void M_RD_Change_AutomapAntialias(int choice)
{
    automap_antialias ^= 1;
}

/*
void M_RD_Change_AutomapOverlay(int choice)
{
    automap_overlay ^= 1;
}
*/

void M_RD_Change_AutomapStats(int choice)
{
    automap_stats ^= 1;
}

void M_RD_Change_AutomapRotate(int choice)
{
    automap_rotate ^= 1;
}

void M_RD_Change_AutomapFollow(int choice)
{
    automap_follow ^= 1;
}

void M_RD_Change_AutomapGrid(int choice)
{
    automap_grid ^= 1;
}

// -----------------------------------------------------------------------------
// Sound
// -----------------------------------------------------------------------------

void M_RD_Choose_Audio(int choice)
{
    M_SetupNextMenu(english_language ?
                    &RD_Audio_Def :
                    &RD_Audio_Def_Rus);
}

void M_RD_Draw_Audio(void)
{
    static char num[4];

    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "SOUND OPTIONS");

        //
        // Volume
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "volume");
        dp_translation = NULL;

        //
        // Extra
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 85, "extra");
        dp_translation = NULL;

        M_WriteTextSmall_ENG(178, 115, snd_monomode ? "mono" : "stereo");
        M_WriteTextSmall_ENG(186, 125, snd_pitchshift ? "on" : "off");
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB PDERF"); // ��������� �����

        //
        // �஬�����
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "uhjvrjcnm");
        dp_translation = NULL;

        //
        // �������⥫쭮
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 85, "ljgjkybntkmyj");
        dp_translation = NULL;

        M_WriteTextSmall_RUS(132, 115, snd_monomode ? "vjyj" : "cnthtj");
        M_WriteTextSmall_RUS(242, 125, snd_pitchshift ? "drk" : "dsrk");
    }

    // Draw SFX volume slider
    M_DrawThermo_Small(35, 55, 16, sfxVolume);
    // Draw numerical representation of SFX volume
    snprintf(num, 4, "%3d", sfxVolume);
    M_WriteTextSmall_ENG(177, 55, num);

    // Draw music volume slider
    M_DrawThermo_Small(35, 75, 16, musicVolume);
    // Draw numerical representation of music volume
    snprintf(num, 4, "%3d", musicVolume);
    M_WriteTextSmall_ENG(177, 75, num);

    // Draw SFX channels slider
    M_DrawThermo_Small(35, 105, 16, numChannels / 4 - 1);
    // Draw numerical representation of channels
    snprintf(num, 4, "%3d", numChannels);
    M_WriteTextSmall_ENG(177, 105, num);
}

void M_RD_Change_SfxVol (int choice)
{
    switch(choice)
    {
        case 0:
        if (sfxVolume)
        {
            sfxVolume--;
        }
        break;

        case 1:
        if (sfxVolume < 15)
        {
            sfxVolume++;
        }
        break;
    }

    S_SetSfxVolume(sfxVolume * 8);
}

void M_RD_Change_MusicVol (int choice)
{
    switch(choice)
    {
        case 0:
        if (musicVolume)
        {
            musicVolume--;
        }
        break;

        case 1:
        if (musicVolume < 15)
        {
            musicVolume++;
        }
        break;
    }

    S_SetMusicVolume(musicVolume * 8);
}

void M_RD_Change_SfxChannels (int choice)
{
    switch(choice)
    {
        case 0:
        if (numChannels > 4)
        {
            numChannels -= 4;
        }
        break;
    
        case 1:
        if (numChannels < 64)
        {
            numChannels += 4;
        }
        break;
    }

    // Reallocate sound channels
    S_ChannelsRealloc();
}

void M_RD_Change_SndMode (int choice)
{
    snd_monomode ^= 1;
}

void M_RD_Change_PitchShifting (int choice)
{
    snd_pitchshift ^= 1;
}


// -----------------------------------------------------------------------------
// Keyboard and Mouse
// -----------------------------------------------------------------------------

void M_RD_Choose_Controls(int choice)
{
    M_SetupNextMenu(english_language ?
                    &RD_Controls_Def :
                    &RD_Controls_Def_Rus);
}

void M_RD_Draw_Controls(void)
{
    static char num[4];

    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "CONTROL SETTINGS");

        //
        // Movement
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "movement");
        dp_translation = NULL;

        // Always run
        M_WriteTextSmall_ENG(119, 45, joybspeed >= 20 ? "on" : "off");

        //
        // Mouse
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 55, "mouse");
        dp_translation = NULL;

        // Mouse look
        M_WriteTextSmall_ENG(119, 85, mlook ? "on" : "off");

        // Invert Y axis
        if (!mlook)
        dp_translation = cr[CR_DARKRED];
        M_WriteTextSmall_ENG(130, 95, mouse_y_invert ? "on" : "off");
        dp_translation = NULL;

        // Vertical movement
        if (mlook)
        dp_translation = cr[CR_DARKRED];
        M_WriteTextSmall_ENG(171, 105, !novert ? "on" : "off");
        dp_translation = NULL;
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "EGHFDKTYBT"); // ����������

        //
        // ��।�������
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "gthtldb;tybt");
        dp_translation = NULL;

        // ����� ����ﭭ��� ����
        M_WriteTextSmall_RUS(216, 45, joybspeed >= 20 ? "drk" : "dsrk");

        //
        // ����
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 55, "vsim");
        dp_translation = NULL;

        // ����� �����
        M_WriteTextSmall_RUS(135, 85, mlook ? "drk" : "dsrk");

        // ���⨪��쭠� �������
        if (!mlook)
        dp_translation = cr[CR_DARKRED];
        M_WriteTextSmall_RUS(207, 95, mouse_y_invert ? "drk" : "dsrk");
        dp_translation = NULL;

        // ���⨪��쭮� ��६�饭��
        if (mlook)
        dp_translation = cr[CR_DARKRED];
        M_WriteTextSmall_RUS(235, 105, !novert ? "drk" : "dsrk");
        dp_translation = NULL;
    }

    // Draw mouse sensivity slider
    M_DrawThermo_Small(35, 74, 17, mouseSensitivity);
    // Draw numerical representation of mouse sensivity
    snprintf(num, 4, "%3d", mouseSensitivity);
    M_WriteTextSmall_ENG(189, 75, num);
}

void M_RD_Change_AlwaysRun(int choice)
{
    static int joybspeed_old = 2;

    if (joybspeed >= 20)
    {
        joybspeed = joybspeed_old;
    }
    else
    {
        joybspeed_old = joybspeed;
        joybspeed = 29;
    }
}

void M_RD_Change_MouseLook(int choice)
{
    mlook ^= 1;

    if (!mlook)
    {
        players[consoleplayer].centering = true;
    }
}

void M_RD_Change_Sensitivity(int choice)
{
    switch(choice)
    {
        case 0:
        if (mouseSensitivity)
            mouseSensitivity--;
        break;

        case 1:
        if (mouseSensitivity < 255) // [crispy] extended range
            mouseSensitivity++;
        break;
    }
}

void M_RD_Change_InvertY(int choice)
{
    mouse_y_invert ^= 1;
}

void M_RD_Change_Novert(int choice)
{
    novert ^= 1;
}


// -----------------------------------------------------------------------------
// Gameplay features
// -----------------------------------------------------------------------------

void M_RD_Choose_Gameplay_1(int choice)
{
    // [JN] Don't allow to enter in -vanilla mode
    if (vanilla)
    {
        return;
    }

    M_SetupNextMenu(english_language ?
                    &RD_Gameplay_Def_1 :
                    &RD_Gameplay_Def_1_Rus);
}

void M_RD_Choose_Gameplay_2(int choice)
{
    M_SetupNextMenu(english_language ?
                    &RD_Gameplay_Def_2 :
                    &RD_Gameplay_Def_2_Rus);
}

void M_RD_Choose_Gameplay_3(int choice)
{
    M_SetupNextMenu(english_language ?
                    &RD_Gameplay_Def_3 :
                    &RD_Gameplay_Def_3_Rus);
}

void M_RD_Choose_Gameplay_4(int choice)
{
    M_SetupNextMenu(english_language ? 
                    &RD_Gameplay_Def_4 :
                    &RD_Gameplay_Def_4_Rus);
}

void M_RD_Draw_Gameplay_1(void)
{
    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "GAMEPLAY FEATURES");

        //
        // Graphical
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "Graphical");
        dp_translation = NULL;

        // Brightmaps
        dp_translation = brightmaps ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(140, 45, brightmaps ? "on" : "off");
        dp_translation = NULL;

        // Fake contrast
        dp_translation = fake_contrast ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(140, 55, fake_contrast ? "on" : "off");
        dp_translation = NULL;

        // Colored HUD elements
        dp_translation = colored_hud ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(140, 65, colored_hud ? "on" : "off");
        dp_translation = NULL;

        // Colored blood and corpses
        dp_translation = colored_blood ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(140, 75, colored_blood ? "on" : "off");
        dp_translation = NULL;

        // Swirling liquids
        dp_translation = swirling_liquids ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(140, 85, swirling_liquids ? "on" : "off");
        dp_translation = NULL;

        // Invulnerability affects sky
        dp_translation = invul_sky ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(140, 95, invul_sky ? "on" : "off");
        dp_translation = NULL;

        // Test casts shadows
        dp_translation = draw_shadowed_text ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(140, 105, draw_shadowed_text ? "on" : "off");
        dp_translation = NULL;

        //
        // Footer
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 145, "next page >"); 
        M_WriteTextSmall_ENG(35, 155, "< last page"); 
        M_WriteTextSmall_ENG(231, 155, "page 1/4");
        dp_translation = NULL;
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB UTQVGKTZ"); // ��������� ��������

        //
        // ��䨪�
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "uhfabrf");
        dp_translation = NULL;

        // �ࠩ⬠�����
        dp_translation = brightmaps ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(140, 45, brightmaps ? "drk" : "dsrk");
        dp_translation = NULL;

        // ������ ������⭮��
        dp_translation = fake_contrast ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(140, 55, fake_contrast ? "drk" : "dsrk");
        dp_translation = NULL;

        // �����梥�� ������ HUD
        dp_translation = colored_hud ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(140, 65, colored_hud ? "drk" : "dsrk");
        dp_translation = NULL;

        // �����梥⭠� �஢� � ����
        dp_translation = colored_blood ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(140, 75, colored_blood ? "drk" : "dsrk");
        dp_translation = NULL;

        // ����襭��� ������� ������⥩
        dp_translation = swirling_liquids ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(140, 85, swirling_liquids ? "drk" : "dsrk");
        dp_translation = NULL;

        // ���梨������ ���訢��� ����
        dp_translation = invul_sky ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(140, 95, invul_sky ? "drk" : "dsrk");
        dp_translation = NULL;

        // ������ ����뢠�� ⥭�
        dp_translation = draw_shadowed_text ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(140, 105, draw_shadowed_text ? "drk" : "dsrk");
        dp_translation = NULL;

        //
        // Footer
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 145, "lfktt \\");      // ����� >
        M_WriteTextSmall_RUS(35, 155, "/ yfpfl");       // < �����
        M_WriteTextSmall_RUS(197, 155, "cnhfybwf 1*4"); // ��࠭�� 1/4
        dp_translation = NULL;
    }
}

void M_RD_Draw_Gameplay_2(void)
{
    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "GAMEPLAY FEATURES");

        //
        // Audible
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "Audible");
        dp_translation = NULL;

        // Play exit sounds
        dp_translation = play_exit_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(158, 45, play_exit_sfx ? "on" : "off");
        dp_translation = NULL;

        // Sound of crushing corpses
        dp_translation = crushed_corpses_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(223 , 55, crushed_corpses_sfx ? "on" : "off");
        dp_translation = NULL;

        // Single sound of closing blazing door
        dp_translation = blazing_door_fix_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(240, 65, blazing_door_fix_sfx ? "on" : "off");
        dp_translation = NULL;

        // Monster alert waking up other monsters
        dp_translation = noise_alert_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(263, 75, noise_alert_sfx ? "on" : "off");
        dp_translation = NULL;

        //
        // Tactical
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 85, "Tactical");
        dp_translation = NULL;

        // Show automap stats
        dp_translation = automap_stats ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(263, 95, automap_stats ? "on" : "off");
        dp_translation = NULL;

        // Notify of revealed secrets
        dp_translation = secret_notification ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(232, 105, secret_notification ? "on" : "off");
        dp_translation = NULL;

        // Show negative health
        dp_translation = negative_health ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(190, 105, negative_health ? "on" : "off");
        dp_translation = NULL;

        //
        // Footer
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 145, "next page >");
        M_WriteTextSmall_ENG(35, 155, "< prev page");
        M_WriteTextSmall_ENG(231, 155, "page 2/4");
        dp_translation = NULL;
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB UTQVGKTZ"); // ��������� ��������

        //
        // ���
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "Pder");
        dp_translation = NULL;

        // ��㪨 �� ��室� �� ����
        dp_translation = play_exit_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(225, 45, play_exit_sfx ? "drk" : "dsrk");
        dp_translation = NULL;

        // ��� ࠧ���������� ��㯮�
        dp_translation = crushed_corpses_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(236, 55, crushed_corpses_sfx ? "drk" : "dsrk");
        dp_translation = NULL;

        // ������� ��� ����ன ����
        dp_translation = blazing_door_fix_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(260, 65, blazing_door_fix_sfx ? "drk" : "dsrk");
        dp_translation = NULL;

        // ���� �ॢ��� � �����஢
        dp_translation = noise_alert_sfx ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(227, 75, noise_alert_sfx ? "drk" : "dsrk");
        dp_translation = NULL;

        //
        // ���⨪�
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 85, "Nfrnbrf");
        dp_translation = NULL;

        // ����⨪� �஢�� �� ����
        dp_translation = automap_stats ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(260, 95, automap_stats ? "drk" : "dsrk");
        dp_translation = NULL;

        // ������� � ��������� ⠩����
        dp_translation = secret_notification ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(260, 105, secret_notification ? "drk" : "dsrk");
        dp_translation = NULL;

        // ����⥫쭮� ���஢� � HUD
        dp_translation = negative_health ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(255, 105, negative_health ? "drk" : "dsrk");
        dp_translation = NULL;

        // Footer
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 145, "lfktt \\");      // ����� >
        M_WriteTextSmall_RUS(35, 155, "/ yfpfl");       // < �����
        M_WriteTextSmall_RUS(197, 155, "cnhfybwf 2*4"); // ��࠭�� 2/4
        dp_translation = NULL;
    }
}

void M_RD_Draw_Gameplay_3(void)
{
    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "GAMEPLAY FEATURES");

        //
        // Physical
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "Physical");
        dp_translation = NULL;

        // Walk over and under monsters
        dp_translation = over_under ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(250, 45, over_under ? "on" : "off");
        dp_translation = NULL;

        // Corpses sliding from the ledges
        dp_translation = torque ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(264, 55, torque ? "on" : "off");
        dp_translation = NULL;

        // Weapon bobbing while firing
        dp_translation = weapon_bobbing ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(233, 65, weapon_bobbing ? "on" : "off");
        dp_translation = NULL;

        // Lethal pellet of a point-blank SSG
        dp_translation = ssg_blast_enemies ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(287, 75, ssg_blast_enemies ? "on" : "off");
        dp_translation = NULL;

        // Randomly mirrored corpses
        dp_translation = randomly_flipcorpses ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(231, 85, randomly_flipcorpses ? "on" : "off");
        dp_translation = NULL;

        // Floating powerups
        dp_translation = floating_powerups ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(171, 95, floating_powerups ? "on" : "off");
        dp_translation = NULL;

        //
        // Crosshair
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 105, "Crosshair");
        dp_translation = NULL;

        // Draw crosshair
        dp_translation = crosshair_draw ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(146, 115, crosshair_draw ? "on" : "off");
        dp_translation = NULL;

        // Health indication
        dp_translation = crosshair_health ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(163, 125, crosshair_health ? "on" : "off");
        dp_translation = NULL;

        //
        // Footer
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 145, "next page >");
        M_WriteTextSmall_ENG(35, 155, "< prev page");
        M_WriteTextSmall_ENG(231, 155, "page 3/4");
        dp_translation = NULL;
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB UTQVGKTZ"); // ��������� ��������

        //
        // ������
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "Abpbrf");
        dp_translation = NULL;

        // ��६�饭�� ���/��� �����ࠬ�
        dp_translation = over_under ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(274, 45, over_under ? "drk" : "dsrk");
        dp_translation = NULL;

        // ���� ᯮ����� � �����襭��
        dp_translation = torque ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(256, 55, torque ? "drk" : "dsrk");
        dp_translation = NULL;

        // ����襭��� ����稢���� ��㦨�
        dp_translation = weapon_bobbing ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(271, 65, weapon_bobbing ? "drk" : "dsrk");
        dp_translation = NULL;

        // ����⢮��� ࠧ�뢠�� �ࠣ��
        dp_translation = ssg_blast_enemies ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(254, 75, ssg_blast_enemies ? "drk" : "dsrk");
        dp_translation = NULL;

        // ��ઠ��஢���� ��㯮�
        dp_translation = randomly_flipcorpses ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(207, 85, randomly_flipcorpses ? "drk" : "dsrk");
        dp_translation = NULL;

        // ��������騥 ����-���䠪��
        dp_translation = floating_powerups ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(275, 95, floating_powerups ? "drk" : "dsrk");
        dp_translation = NULL;

        //
        // ��楫
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 105, "Ghbwtk");
        dp_translation = NULL;

        // �⮡ࠦ��� ��楫
        dp_translation = crosshair_draw ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(180, 115, crosshair_draw ? "drk" : "dsrk");
        dp_translation = NULL;

        // �������� ���஢��
        dp_translation = crosshair_health ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(186, 125, crosshair_health ? "drk" : "dsrk");
        dp_translation = NULL;

        //
        // Footer
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 145, "lfktt \\");      // ����� >
        M_WriteTextSmall_RUS(35, 155, "/ yfpfl");       // < �����
        M_WriteTextSmall_RUS(197, 155, "cnhfybwf 3*4"); // ��࠭�� 3/4
        dp_translation = NULL;
    }
}

void M_RD_Draw_Gameplay_4(void)
{
    if (english_language)
    {
        M_WriteTextBigCentered_ENG(12, "GAMEPLAY FEATURES");

        //
        // Gameplay
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 35, "Gameplay");
        dp_translation = NULL;

        // Extra player faces on the HUD
        dp_translation = extra_player_faces ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(255, 45, extra_player_faces ? "on" : "off");
        dp_translation = NULL;

        // Pain Elemental without Souls limit
        dp_translation = unlimited_lost_souls ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(284, 55, unlimited_lost_souls ? "on" : "off");
        dp_translation = NULL;

        // Don't prompt for q. saving/loading
        dp_translation = fast_quickload ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_ENG(284, 65, fast_quickload ? "on" : "off");
        dp_translation = NULL;

        // Play internal demos
        dp_translation = no_internal_demos ? cr[CR_DARKRED] : cr[CR_GREEN];
        M_WriteTextSmall_ENG(183, 105, no_internal_demos ? "off" : "on");
        dp_translation = NULL;

        //
        // Footer
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_ENG(35, 145, "first page >");
        M_WriteTextSmall_ENG(35, 155, "< prev page");
        M_WriteTextSmall_ENG(231, 155, "page 4/4");
        dp_translation = NULL;
    }
    else
    {
        M_WriteTextBigCentered_RUS(12, "YFCNHJQRB UTQVGKTZ"); // ��������� ��������

        //
        // ��������
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 35, "Utqvgktq");
        dp_translation = NULL;

        // �������⥫�� ��� ��ப� � HUD
        dp_translation = extra_player_faces ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(247, 45, extra_player_faces ? "drk" : "dsrk");
        dp_translation = NULL;

        // ������⠫� ��� ��࠭�祭�� ���
        dp_translation = unlimited_lost_souls ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(274, 55, unlimited_lost_souls ? "drk" : "dsrk");
        dp_translation = NULL;

        // �⪫���� ����� �. ����㧪�
        dp_translation = fast_quickload ? cr[CR_GREEN] : cr[CR_DARKRED];
        M_WriteTextSmall_RUS(253, 65, fast_quickload ? "drk" : "dsrk");
        dp_translation = NULL;

        // �ந��뢠�� ���������
        dp_translation = no_internal_demos ? cr[CR_DARKRED] : cr[CR_GREEN];
        M_WriteTextSmall_RUS(219, 75, no_internal_demos ? "dsrk" : "drk");
        dp_translation = NULL;

        //
        // Footer
        //
        dp_translation = cr[CR_GOLD];
        M_WriteTextSmall_RUS(35, 145, "lfktt \\");      // ����� >
        M_WriteTextSmall_RUS(35, 155, "/ yfpfl");       // < �����
        M_WriteTextSmall_RUS(197, 155, "cnhfybwf 4*4"); // ��࠭�� 4/4
        dp_translation = NULL;
    }
}

void M_RD_Change_Brightmaps (int choice)
{
    brightmaps ^= 1;
}

void M_RD_Change_FakeContrast (int choice)
{
    fake_contrast ^= 1;
}

void M_RD_Change_ColoredHUD (int choice)
{
    colored_hud ^= 1;
    
    // Update background of classic HUD and player face 
    if (gamestate == GS_LEVEL)
    {
        ST_refreshBackground();
        ST_drawWidgets(true);
    }
}

void M_RD_Change_ColoredBlood (int choice)
{
    colored_blood ^= 1;
}

void M_RD_Change_SwirlingLiquids (int choice)
{
    swirling_liquids ^= 1;
}

void M_RD_Change_InvulSky (int choice)
{
    invul_sky ^= 1;
}

void M_RD_Change_ShadowedText (int choice)
{
    draw_shadowed_text ^= 1;
}

void M_RD_Change_ExitSfx (int choice)
{
    play_exit_sfx ^= 1;
}

void M_RD_Change_CrushingSfx(int choice)
{
    crushed_corpses_sfx ^= 1;
}

void M_RD_Change_BlazingSfx(int choice)
{
    blazing_door_fix_sfx ^= 1;
}

void M_RD_Change_AlertSfx (int choice)
{
    noise_alert_sfx ^= 1;
}

void M_RD_Change_SecretNotify (int choice)
{
    secret_notification ^= 1;
}

void M_RD_Change_NegativeHealth (int choice)
{
    negative_health ^= 1;
}

void M_RD_Change_WalkOverUnder (int choice)
{
    over_under ^= 1;
}

void M_RD_Change_Torque (int choice)
{
    torque ^= 1;
}

void M_RD_Change_Bobbing (int choice)
{
    weapon_bobbing ^= 1;
}

void M_RD_Change_SSGBlast (int choice)
{
    ssg_blast_enemies ^= 1;
}

void M_RD_Change_FlipCorpses (int choice)
{
    randomly_flipcorpses ^= 1;
}

void M_RD_Change_FloatPowerups (int choice)
{
    floating_powerups ^= 1;
}

void M_RD_Change_CrosshairDraw (int choice)
{
    crosshair_draw ^= 1;
}

void M_RD_Change_CrosshairHealth (int choice)
{
    crosshair_health ^= 1;
}

void M_RD_Change_ExtraPlayerFaces (int choice)
{
    extra_player_faces ^= 1;
}

void M_RD_Change_LostSoulsQty (int choice)
{
    unlimited_lost_souls ^= 1;
}

void M_RD_Change_FastQSaveLoad (int choice)
{
    fast_quickload ^= 1;
}

void M_RD_Change_NoInternalDemos (int choice)
{
    no_internal_demos ^= 1;
}

// -----------------------------------------------------------------------------
// Back to Defaults
// -----------------------------------------------------------------------------

void M_RD_BackToDefaultsResponse (int ch)
{
    static char resetmsg[24];

    if (ch != 'y')
    return;

    // Rendering
    show_diskicon = 1;
    noflats       = 0;
    screen_wiping = 1;

    // Display
    screenblocks = 10;
    screenSize = screenblocks - 3;
    usegamma     = 0;
    detailLevel  = 0;
    showMessages = 1;

    // Audio
    sfxVolume       = 8;  S_SetSfxVolume(sfxVolume * 8);
    musicVolume     = 8;  S_SetMusicVolume(musicVolume * 8);
    numChannels     = 32; S_ChannelsRealloc();
    snd_monomode    = 0;
    snd_pitchshift  = 0;

    // Controls
    joybspeed        = 29;
    mlook            = 0; players[consoleplayer].centering = true;
    mouseSensitivity = 5;

    // Gameplay
    brightmaps         = 1;
    fake_contrast      = 0;
    colored_hud        = 0;
    colored_blood      = 1;
    swirling_liquids   = 1;
    invul_sky          = 1;
    draw_shadowed_text = 1;

    play_exit_sfx        = 1;
    crushed_corpses_sfx  = 1;
    blazing_door_fix_sfx = 1;
    noise_alert_sfx      = 0;

    automap_stats       = 1;
    secret_notification = 1;
    negative_health     = 0;

    over_under           = 0;
    torque               = 1;
    weapon_bobbing       = 1;
    ssg_blast_enemies    = 1;
    randomly_flipcorpses = 1;
    floating_powerups    = 0;

    crosshair_draw   = 0;
    crosshair_health = 1;

    extra_player_faces   = 1;
    unlimited_lost_souls = 1;
    fast_quickload       = 1;
    no_internal_demos    = 0;

    // Set view size, execute drawing functions
    R_SetViewSize (screenblocks, detailLevel); 

    // Set palette
    I_SetPalette ((byte *)W_CacheLumpName(usegamma <= 8 ?
                                          "PALFIX" :
                                          "PLAYPAL",
                                          PU_CACHE) + 
                                          st_palette * 768);

    // Update background of classic HUD and player face 
    if (gamestate == GS_LEVEL)
    {
        ST_refreshBackground();
        ST_drawWidgets(true);
    }

    // Print informative message (����ன�� ��襭�)
    snprintf(resetmsg, sizeof(resetmsg), english_language ?
                                         "Settings reset" :
                                         "Yfcnhjqrb c,hjitys");
    players[consoleplayer].message = resetmsg;
}

void M_RD_BackToDefaults (int choice)
{
    choice = 0;
    M_StartMessage(RD_DEFAULTS, M_RD_BackToDefaultsResponse,true);
}


// -----------------------------------------------------------------------------
// Language hot-swapping
// -----------------------------------------------------------------------------

void M_RD_ChangeLanguage (int choice)
{
    extern void D_DoAdvanceDemo(void);
    extern void F_StartFinale(void);
    extern int  demosequence;

    english_language ^= 1;

    // Reset options menu
    currentMenu = english_language ? &RD_Options_Def : &RD_Options_Def_Rus;

    // Update messages
    RD_DefineLanguageStrings();

    // Update TITLEPIC/CREDIT screens in live mode
    if (gamestate == GS_DEMOSCREEN)
    {
        if (demosequence == 0
        ||  demosequence == 2
        ||  demosequence == 4)
        {
            demosequence--;
            D_DoAdvanceDemo();
        }
    }

    if (gamestate == GS_LEVEL)
    {
        // Update HUD system
        HU_Start();

        // Update status bar
        ST_Start();
    }

    // Restart finale text
    if (gamestate == GS_FINALE)
    {
        F_StartFinale();
    }
}

//
// Read This! MENU 1 & 2
//
enum
{
    rdthsempty1,
    read1_end
} read_e;

// ------------
// English menu
// ------------

menuitem_t ReadMenu1[] =
{
    {1,"",M_ReadThis2,0}
};

menu_t  ReadDef1 =
{
    read1_end,
    &MainDef,
    ReadMenu1,
    M_DrawReadThis1,
    280,181,
    0
};

// ------------
// Russian menu
// ------------

menu_t  ReadDef1_Rus =
{
    read1_end,
    &MainDef_Rus,
    ReadMenu1,
    M_DrawReadThis1,
    280,181,
    0
};

// ------------
// English menu
// ------------

enum
{
    rdthsempty2,
    read2_end
} read_e2;

menuitem_t ReadMenu2[]=
{
    {1,"",M_FinishReadThis,0}
};

menu_t  ReadDef2 =
{
    read2_end,
    NULL,
    ReadMenu2,
    M_DrawReadThisRetail,
    330,168,
    0
};

// ------------
// Russian menu
// ------------

menu_t  ReadDef2_Rus =
{
    read2_end,
    &ReadDef1_Rus,
    ReadMenu2,
    M_DrawReadThisRetail,
    330,168,
    0
};


//
// LOAD GAME MENU
//
enum
{
    load1,
    load2,
    load3,
    load4,
    load5,
    load6,
    load7,
    load8,
    load_end
} load_e;

// ------------
// English menu
// ------------

menuitem_t LoadMenu[]=
{
    {1,"", M_LoadSelect,'1'},
    {1,"", M_LoadSelect,'2'},
    {1,"", M_LoadSelect,'3'},
    {1,"", M_LoadSelect,'4'},
    {1,"", M_LoadSelect,'5'},
    {1,"", M_LoadSelect,'6'},
    {1,"", M_LoadSelect,'7'},
    {1,"", M_LoadSelect,'8'},
};

menu_t  LoadDef =
{
    load_end,
    &MainDef,
    LoadMenu,
    M_DrawLoad,
    67,38,
    0
};

// ------------
// Russian menu
// ------------

menu_t  LoadDef_Rus =
{
    load_end,
    &MainDef_Rus,
    LoadMenu,
    M_DrawLoad,
    67,38,
    0
};


//
// SAVE GAME MENU
//

// ------------
// English menu
// ------------

menuitem_t SaveMenu[]=
{
    {1,"", M_SaveSelect,'1'},
    {1,"", M_SaveSelect,'2'},
    {1,"", M_SaveSelect,'3'},
    {1,"", M_SaveSelect,'4'},
    {1,"", M_SaveSelect,'5'},
    {1,"", M_SaveSelect,'6'},
    {1,"", M_SaveSelect,'7'},
    {1,"", M_SaveSelect,'8'}
};

menu_t  SaveDef =
{
    load_end,
    &MainDef,
    SaveMenu,
    M_DrawSave,
    67,38,
    0
};

// ------------
// Russian menu
// ------------

menu_t  SaveDef_Rus =
{
    load_end,
    &MainDef_Rus,
    SaveMenu,
    M_DrawSave,
    67,38,
    0
};


//
// M_ReadSaveStrings
//  read the strings from the savegame files
//
void M_ReadSaveStrings (void)
{
    int   handle;
    int   count;
    int   i;
    char  name[256];

    for (i = 0;i < load_end;i++)
    {
        if (M_CheckParm("-cdrom"))
        {
            sprintf(name,"c:\\doomdata\\"SAVEGAMENAME"%d.dsg",i);
        }
        else
        {
            sprintf(name,SAVEGAMENAME"%d.dsg",i);
        }

        handle = open (name, O_RDONLY | 0, 0666);

        if (handle == -1)
        {
            strcpy(&savegamestrings[i][0],EMPTYSTRING);
            LoadMenu[i].status = 0;
            continue;
        }

        count = read (handle, &savegamestrings[i], SAVESTRINGSIZE);
        close (handle);
        LoadMenu[i].status = 1;
    }
}


//
// M_LoadGame & Cie.
//

void M_DrawLoad (void)
{
    int i;

    if (english_language)
    {
        V_DrawShadowDirect (73,14,0,W_CacheLumpName("M_LOADG",PU_CACHE));
        V_DrawPatchDirect (72,13,0,W_CacheLumpName("M_LOADG",PU_CACHE));
    }
    else
    {
        M_WriteTextBigCentered_RUS(13, "PFUHEPBNM BUHE"); // ��������� ����
    }

    for (i = 0;i < load_end; i++)
    {
        M_DrawSaveLoadBorder(LoadDef.x,LoadDef.y+LINEHEIGHT*i);

        // [crispy] shade empty savegame slots
        if (!LoadMenu[i].status && colored_hud && !vanilla)
        dp_translation = cr[CR_DARKRED];

        M_WriteText(LoadDef.x,LoadDef.y+LINEHEIGHT*i,savegamestrings[i]);

        dp_translation = NULL;
    }
}


//
// Draw border for the savegame description
//
void M_DrawSaveLoadBorder (int x,int y)
{
    int i;

    V_DrawShadowDirect (x-7,y+9,0,W_CacheLumpName("M_LSLEFT",PU_CACHE));
    V_DrawPatchDirect (x-8,y+8,0,W_CacheLumpName("M_LSLEFT",PU_CACHE));

    for (i = 0 ; i < 24 ; i++)
    {
        V_DrawShadowDirect (x+1,y+9,0,W_CacheLumpName("M_LSCNTR",PU_CACHE));
        V_DrawPatchDirect (x,y+8,0,W_CacheLumpName("M_LSCNTR",PU_CACHE));
        x += 8;
    }

    V_DrawShadowDirect (x+1,y+9,0,W_CacheLumpName("M_LSRGHT",PU_CACHE));
    V_DrawPatchDirect (x,y+8,0,W_CacheLumpName("M_LSRGHT",PU_CACHE));
}


//
// User wants to load this game
//
void M_LoadSelect (int choice)
{
    char name[256];

    if (M_CheckParm("-cdrom"))
    {
        sprintf(name,"c:\\doomdata\\"SAVEGAMENAME"%d.dsg",choice);
    }
    else
    {
        sprintf(name,SAVEGAMENAME"%d.dsg",choice);
    }

    G_LoadGame (name);
    M_ClearMenus ();
}

//
// Selected from DOOM menu
//
void M_LoadGame (int choice)
{
    if (netgame)
    {
        M_StartMessage(LOADNET,NULL,false);
        return;
    }

    M_SetupNextMenu(english_language ? &LoadDef : &LoadDef_Rus);
    M_ReadSaveStrings();
}


//
//  M_SaveGame & Cie.
//
void M_DrawSave (void)
{
    int i;

    if (english_language)
    {
        // [JN] Use standard title "M_SAVEG"
        V_DrawShadowDirect (73, 14, 0, W_CacheLumpName("M_SAVEG",PU_CACHE));
        V_DrawPatchDirect (72, 13, 0, W_CacheLumpName("M_SAVEG",PU_CACHE));
    }
    else
    {
        if (QuickSaveTitle) // ������� ����������
        M_WriteTextBigCentered_RUS(13, "<SCNHJT CJ{HFYTYBT");
        else                // ��������� ����
        M_WriteTextBigCentered_RUS(13, "CJ{HFYBNM BUHE");
    }

    for (i = 0;i < load_end; i++)
    {
        M_DrawSaveLoadBorder(LoadDef.x,LoadDef.y+LINEHEIGHT*i);
        M_WriteText(LoadDef.x,LoadDef.y+LINEHEIGHT*i,savegamestrings[i]);
    }

    if (saveStringEnter)
    {
        i = M_StringWidth(savegamestrings[saveSlot]);
        M_WriteText(LoadDef.x + i,LoadDef.y+LINEHEIGHT*saveSlot,"_");
    }
}


//
// M_Responder calls this when user is finished
//
void M_DoSave (int slot)
{
    G_SaveGame (slot,savegamestrings[slot]);
    M_ClearMenus ();

    // PICK QUICKSAVE SLOT YET?
    if (quickSaveSlot == -2)
    {
        quickSaveSlot = slot;
    }
}


//
// User wants to save. Start string input for M_Responder
//
void M_SaveSelect (int choice)
{
    // we are going to be intercepting all chars
    saveStringEnter = 1;

    saveSlot = choice;
    strcpy(saveOldString,savegamestrings[choice]);
    if (!strcmp(savegamestrings[choice],EMPTYSTRING))
    {
        savegamestrings[choice][0] = 0;
    }
    saveCharIndex = strlen(savegamestrings[choice]);
}


//
// Selected from DOOM menu
//
void M_SaveGame (int choice)
{
    if (!usergame)
    {
        M_StartMessage(SAVEDEAD,NULL,false);
        return;
    }

    if (gamestate != GS_LEVEL)
    {
        return;
    }

    M_SetupNextMenu(english_language ? &SaveDef : &SaveDef_Rus);
    M_ReadSaveStrings();
}


//
// M_QuickSave
//

void M_QuickSaveResponse (int ch)
{
    if (ch == 'y')
    {
        M_DoSave(quickSaveSlot);
        S_StartSound(NULL,sfx_swtchx);
    }
}


void M_QuickSave (void)
{
    if (!usergame)
    {
        S_StartSound(NULL,sfx_oof);
        return;
    }

    if (gamestate != GS_LEVEL)
    {
        return;
    }

    if (quickSaveSlot < 0)
    {
        M_StartControlPanel();
        M_ReadSaveStrings();
        M_SetupNextMenu(english_language ? &SaveDef : &SaveDef_Rus);
        quickSaveSlot = -2;	// means to pick a slot now
        return;
    }

    if (fast_quickload)
    {
        // [JN] Don't ask for overwriting, just save it.
        M_DoSave(quickSaveSlot);
    }
    else
    {
        sprintf(tempstring,QSPROMPT,savegamestrings[quickSaveSlot]);
        M_StartMessage(tempstring,M_QuickSaveResponse,true);
    }
}


//
// M_QuickLoad
//
void M_QuickLoadResponse (int ch)
{
    if (ch == 'y')
    {
        M_LoadSelect(quickSaveSlot);
        S_StartSound(NULL,sfx_swtchx);
    }
}


void M_QuickLoad (void)
{
    if (netgame)
    {
        M_StartMessage(QLOADNET,NULL,false);
        return;
    }

    if (quickSaveSlot < 0)
    {
        M_StartMessage(QSAVESPOT,NULL,false);
        return;
    }

    if (fast_quickload)
    {
        // [JN] Don't ask for loading, just load it.
        M_LoadSelect(quickSaveSlot);
    }
    else
    {
        sprintf(tempstring,QLPROMPT,savegamestrings[quickSaveSlot]);
        M_StartMessage(tempstring,M_QuickLoadResponse,true);        
    }
}


//
// Read This Menus
// Had a "quick hack to fix romero bug"
//
void M_DrawReadThis1 (void)
{
    inhelpscreens = true;
    V_DrawPatchDirect(0, 0, 0, W_CacheLumpName
                     (english_language ? "HELP2" : "HELP2R", PU_CACHE));
}


//
// Read This Menus - optional second page.
//
void M_DrawReadThisRetail (void)
{
    inhelpscreens = true;

    if (commercial)
    {
        V_DrawPatchDirect(0, 0, 0, W_CacheLumpName
                         (english_language ? "HELP" : "HELPR", PU_CACHE));
    }
    else
    {
        V_DrawPatchDirect(0, 0, 0, W_CacheLumpName
                         (english_language ? "HELP1" : "HELP1R", PU_CACHE));
    }
}


//
// M_DrawMainMenu
//
void M_DrawMainMenu (void)
{
    if (english_language)
    {
        // [JN] Always draw original "M_DOOM" in English language
        V_DrawPatchDirect (94, 2, 0, W_CacheLumpName("M_DOOM",PU_CACHE));
    }
    else
    {
        // [JN] Draw translated titles for Plutonia and TNT
        V_DrawPatchDirect (94, 2, 0, W_CacheLumpName 
                           (tnt ? "M_DOOMT" : 
                            plutonia ? "M_DOOMP" :
                                          "M_DOOM", PU_CACHE));
    }
}


//
// M_NewGame
//
void M_DrawNewGame (void)
{
    if (english_language)
    {
        V_DrawShadowDirect(97, 15, 0, W_CacheLumpName("M_NEWG", PU_CACHE));
        V_DrawPatchDirect (96, 14, 0, W_CacheLumpName("M_NEWG", PU_CACHE));

        V_DrawShadowDirect(55, 39, 0, W_CacheLumpName("M_SKILL", PU_CACHE));
        V_DrawPatchDirect (54, 38, 0, W_CacheLumpName("M_SKILL", PU_CACHE));
    }
    else
    {
        M_WriteTextBigCentered_RUS(14, "YJDFZ BUHF");         // ����� ����
        M_WriteTextBigCentered_RUS(38, "Ehjdtym ckj;yjcnb:"); // �஢��� ᫮�����:
    }
}

void M_NewGame (int choice)
{
    if (netgame && !demoplayback)
    {
        M_StartMessage(NEWGAME,NULL,false);
        return;
    }

    if (commercial)
    {
        M_SetupNextMenu(english_language ? &NewDef : &NewDef_Rus);
    }
    else
    {
        M_SetupNextMenu(english_language ? &EpiDef : &EpiDef_Rus);
    }
}


//
// M_Episode
//
void M_DrawEpisode (void)
{
    if (english_language)
    {
        V_DrawShadowDirect(97, 15, 0, W_CacheLumpName("M_NEWG", PU_CACHE));
        V_DrawPatchDirect(96, 14, 0, W_CacheLumpName("M_NEWG", PU_CACHE));

        V_DrawShadowDirect (55, 39, 0, W_CacheLumpName("M_EPISOD",PU_CACHE));
        V_DrawPatchDirect (54, 38, 0, W_CacheLumpName("M_EPISOD",PU_CACHE));
    }
    else
    {
        M_WriteTextBigCentered_RUS(14, "YJDFZ BUHF");       // ����� ����
        M_WriteTextBigCentered_RUS(38, "Rfrjq \'gbpjl?");   // ����� ����?
    }
}

void M_VerifyNightmare (int ch)
{
    if (ch != 'y')
    {
        return;
    }

    G_DeferedInitNew(nightmare, epi+1, 1);
    M_ClearMenus ();
}

void M_VerifyUltraNightmare (int ch)
{
    if (ch != 'y')
    {
        return;
    }

    G_DeferedInitNew(ultra_nm, epi+1, 1);
    M_ClearMenus();
}

void M_ChooseSkill (int choice)
{
    if (choice == nightmare)
    {
        M_StartMessage(NIGHTMARE, M_VerifyNightmare, true);
        return;
    }

    if (choice == ultra_nm)
    {
        M_StartMessage(ULTRANM, M_VerifyUltraNightmare, true);
        return;
    }

    G_DeferedInitNew(choice, epi+1, 1);
    M_ClearMenus();
}

void M_Episode (int choice)
{
    if (shareware && choice)
    {
        M_StartMessage(SWSTRING,NULL,false);
        M_SetupNextMenu(&ReadDef1);
        return;
    }

    epi = choice;
    M_SetupNextMenu(english_language ? &NewDef : &NewDef_Rus);
}


//
// M_Options
//

void M_DrawOptions (void)
{
    V_DrawShadowDirect (90,14,0,W_CacheLumpName("M_OPTTTL",PU_CACHE));
    V_DrawPatchDirect (89,13,0,W_CacheLumpName("M_OPTTTL",PU_CACHE));
}


void M_Options (int choice)
{
    M_SetupNextMenu(english_language ? &RD_Options_Def : &RD_Options_Def_Rus);
}


//
// M_EndGame
//
void M_EndGameResponse (int ch)
{
    if (ch != 'y')
    {
        return;
    }

    currentMenu->lastOn = itemOn;
    M_ClearMenus();
    D_StartTitle();
}

void M_EndGame (int choice)
{
    choice = 0;

    if (!usergame)
    {
        S_StartSound(NULL, sfx_oof);
        return;
    }

    if (netgame)
    {
        M_StartMessage(NETEND, NULL, false);
        return;
    }

    M_StartMessage(ENDGAME, M_EndGameResponse, true);
}


//
// M_ReadThis
//
void M_ReadThis (int choice)
{
    choice = 0;
    M_SetupNextMenu(english_language ? &ReadDef1 : &ReadDef1_Rus);
}

void M_ReadThis2(int choice)
{
    choice = 0;
    M_SetupNextMenu(english_language ? &ReadDef2 : &ReadDef2_Rus);
}

void M_FinishReadThis(int choice)
{
    choice = 0;
    M_SetupNextMenu(english_language ? &MainDef : &MainDef_Rus);
}


//
// M_QuitDOOM
//

int quitsounds[8] =
{
    sfx_pldeth,
    sfx_dmpain,
    sfx_popain,
    sfx_slop,
    sfx_telept,
    sfx_posit1,
    sfx_posit3,
    sfx_sgtatk
};

int quitsounds2[8] =
{
    sfx_vilact,
    sfx_getpow,
    sfx_boscub,
    sfx_slop,
    sfx_skeswg,
    sfx_kntdth,
    sfx_bspact,
    sfx_sgtatk
};


void M_QuitResponse (int ch)
{
    if (ch != 'y')
    {
        return;
    }

    // [JN] No need to play exit sfx if it's volume set to 0.
    if (!netgame && play_exit_sfx && sfxVolume > 0)
    {
        if (commercial)
        {
            S_StartSound(NULL,quitsounds2[(gametic>>2)&7]);
        }
        else
        {
            S_StartSound(NULL,quitsounds[(gametic>>2)&7]);
        }
        I_WaitVBL(105);
    }
    I_Quit ();
}


void M_QuitDOOM (int choice)
{
    // We pick index 0 which is language sensitive,
    //  or one at random, between 1 and maximum number.
    if (commercial)
    {
        if (english_language)
        {
            sprintf(endstring, "%s\n\n"DOSY, 
                    endmsg2[(gametic >> 2) % NUM_QUITMESSAGES]);
        }
        else
        {
            sprintf(endstring, "%s\n\n"DOSY_RUS, 
                    endmsg2_rus[(gametic >> 2) % NUM_QUITMESSAGES]);
        }
    }
    else
    {
        if (english_language)
        {
            sprintf(endstring, "%s\n\n"DOSY, 
                    endmsg1[(gametic >> 2) % NUM_QUITMESSAGES]);
        }
        else
        {
            sprintf(endstring, "%s\n\n"DOSY_RUS,
                    endmsg1_rus[(gametic >> 2) % NUM_QUITMESSAGES]);
        }
    }

    if (devparm)
    {
        // [JN] Quit immediately
        I_Quit ();
    }
    else
    {
        M_StartMessage(endstring, M_QuitResponse, true);
    }
}


//
// Menu Functions
//
void M_DrawThermo (int x, int y, int thermWidth, int thermDot)
{
    int xx;
    int i;

    xx = x;

    V_DrawShadowDirect (xx+1,y+1,0,W_CacheLumpName("M_THERML",PU_CACHE));
    V_DrawPatchDirect (xx,y,0,W_CacheLumpName("M_THERML",PU_CACHE));

    xx += 8;

    for (i=0 ; i<thermWidth ; i++)
    {
        V_DrawShadowDirect (xx+1,y+1,0,W_CacheLumpName("M_THERMM",PU_CACHE));
        V_DrawPatchDirect (xx,y,0,W_CacheLumpName("M_THERMM",PU_CACHE));
        xx += 8;
    }

    V_DrawShadowDirect (xx+1,y+1,0,W_CacheLumpName("M_THERMR",PU_CACHE));
    V_DrawPatchDirect (xx,y,0,W_CacheLumpName("M_THERMR",PU_CACHE));

    // [crispy] do not crash anymore if value exceeds thermometer range
    if (thermDot >= thermWidth)
    {
        thermDot = thermWidth - 1;
        V_DrawPatchDirect ((x+8) + thermDot*8,y,0,W_CacheLumpName("M_THERMW",PU_CACHE));
    }
    else if (thermDot == 0)
    {
        V_DrawPatchDirect ((x+8) + thermDot*8,y,0,W_CacheLumpName("M_THERMD",PU_CACHE));
    }
    else
    {
        V_DrawPatchDirect ((x+8) + thermDot*8,y,0,W_CacheLumpName("M_THERMO",PU_CACHE));
    }
}


// -----------------------------------------------------------------------------
// [JN] Draw small thermo for RD options menu
// -----------------------------------------------------------------------------
void M_DrawThermo_Small (int x, int y, int thermWidth, int thermDot)
{
    int		xx;
    int		i;

    xx = x;
    V_DrawShadowDirect (xx+1,y+1,0,W_CacheLumpName("RD_THRML",PU_CACHE));
    V_DrawPatchDirect (xx,y,0,W_CacheLumpName("RD_THRML",PU_CACHE));
    xx += 8;
    for (i=0;i<thermWidth;i++)
    {
        V_DrawShadowDirect (xx+1,y+1,0,W_CacheLumpName("RD_THRMM",PU_CACHE));
        V_DrawPatchDirect (xx,y,0,W_CacheLumpName("RD_THRMM",PU_CACHE));
        xx += 8;
    }
    V_DrawShadowDirect (xx+1,y+1,0,W_CacheLumpName("RD_THRMR",PU_CACHE));
    V_DrawPatchDirect (xx,y,0,W_CacheLumpName("RD_THRMR",PU_CACHE));

    // [crispy] do not crash anymore if value exceeds thermometer range
    // [JN] Draw red slider instead of blue.
    if (thermDot >= thermWidth)
    {
        thermDot = thermWidth - 1;
        V_DrawPatchDirect ((x+8) + thermDot*8,y,0,W_CacheLumpName("RD_THRMW",PU_CACHE));
    }
    else
    {
        V_DrawPatchDirect ((x+8) + thermDot*8,y,0,W_CacheLumpName("RD_THRMO",PU_CACHE));
    }
}


void M_StartMessage (char *string, void *routine, boolean input)
{
    messageLastMenuActive = menuactive;
    messageToPrint = 1;
    messageString = string;
    messageRoutine = routine;
    messageNeedsInput = input;
    menuactive = true;
    return;
}


void M_StopMessage (void)
{
    menuactive = messageLastMenuActive;
    messageToPrint = 0;
}


//
// Find string width from hu_font chars
//
int M_StringWidth(char *string)
{
    int i;
    int w = 0;
    int c;

    for (i = 0 ; i < strlen(string) ; i++)
    {
        c = toupper(string[i]) - HU_FONTSTART;
        if (c < 0 || c >= HU_FONTSIZE)
        {
            w += 4;
        }
        else
        {
            if (english_language || currentMenu == &SaveDef
            ||  currentMenu == &SaveDef_Rus)
            {
                w += SHORT (hu_font[c]->width);
            }
            else
            {
                w += SHORT (hu_font_small_rus[c]->width);
            }
        }
    }

    return w;
}


//
// Find string height from hu_font chars
//
int M_StringHeight(char *string)
{
    int i;
    int h;
    int height = SHORT(hu_font[0]->height);

    h = height;

    for (i = 0 ; i < strlen(string) ; i++)
        if (string[i] == '\n')
            h += height;

    return h;
}


//
// CONTROL PANEL
//

//
// M_Responder
//
boolean M_Responder (event_t *ev)
{
    int          ch;
    int          i;
    static int   joywait = 0;
    static int   mousewait = 0;
    static int   mousey = 0;
    static int   lasty = 0;
    static int   mousex = 0;
    static int   lastx = 0;
    byte        *pal;

    ch = -1;

    if (ev->type == ev_joystick && joywait < I_GetTime())
    {
        if (ev->data3 == -1)
        {
            ch = KEY_UPARROW;
            joywait = I_GetTime() + 5;
        }
        else if (ev->data3 == 1)
        {
            ch = KEY_DOWNARROW;
            joywait = I_GetTime() + 5;
        }

        if (ev->data2 == -1)
        {
            ch = KEY_LEFTARROW;
            joywait = I_GetTime() + 2;
        }
        else if (ev->data2 == 1)
        {
            ch = KEY_RIGHTARROW;
            joywait = I_GetTime() + 2;
        }

        if (ev->data1&1)
        {
            ch = KEY_ENTER;
            joywait = I_GetTime() + 5;
        }

        if (ev->data1&2)
        {
            ch = KEY_BACKSPACE;
            joywait = I_GetTime() + 5;
        }
    }
    else
    {
        if (ev->type == ev_mouse && mousewait < I_GetTime())
        {
            mousey += ev->data3;

            if (mousey < lasty-30)
            {
                ch = KEY_DOWNARROW;
                mousewait = I_GetTime() + 5;
                mousey = lasty -= 30;
            }
            else if (mousey > lasty+30)
            {
                ch = KEY_UPARROW;
                mousewait = I_GetTime() + 5;
                mousey = lasty += 30;
            }

            mousex += ev->data2;

            if (mousex < lastx-30)
            {
                ch = KEY_LEFTARROW;
                mousewait = I_GetTime() + 5;
                mousex = lastx -= 30;
            }
            else if (mousex > lastx+30)
            {
                ch = KEY_RIGHTARROW;
                mousewait = I_GetTime() + 5;
                mousex = lastx += 30;
            }

            if (ev->data1&1)
            {
                ch = KEY_ENTER;
                mousewait = I_GetTime() + 15;
            }

            if (ev->data1&2)
            {
                ch = KEY_BACKSPACE;
                mousewait = I_GetTime() + 15;
            }
        }
        else if (ev->type == ev_keydown)
        {
            ch = ev->data1;
        }
    }

    if (ch == -1)
    {
        return false;
    }

    // Save Game string input
    if (saveStringEnter)
    {
        switch(ch)
        {
            case KEY_BACKSPACE:
            if (saveCharIndex > 0)
            {
                saveCharIndex--;
                savegamestrings[saveSlot][saveCharIndex] = 0;
            }
            break;

            case KEY_ESCAPE:
            saveStringEnter = 0;
            strcpy(&savegamestrings[saveSlot][0],saveOldString);
            break;

            case KEY_ENTER:
            saveStringEnter = 0;
            if (savegamestrings[saveSlot][0])
            {
                M_DoSave(saveSlot);
            }
            break;

            default:
            ch = toupper(ch);
            if (ch != 32)
            if (ch-HU_FONTSTART < 0 || ch-HU_FONTSTART >= HU_FONTSIZE)
		    break;
            if (ch >= 32 && ch <= 127 && saveCharIndex < SAVESTRINGSIZE-1 
            &&  M_StringWidth(savegamestrings[saveSlot]) < (SAVESTRINGSIZE-2)*8)
            {
                savegamestrings[saveSlot][saveCharIndex++] = ch;
                savegamestrings[saveSlot][saveCharIndex] = 0;
            }
            break;
        }
        return true;
    }

    // Take care of any messages that need input
    if (messageToPrint)
    {
        if (messageNeedsInput == true 
        && !(ch == ' ' || ch == 'n' || ch == 'y' || ch == KEY_ESCAPE))
        {
            return false;
        }
        menuactive = messageLastMenuActive;
        messageToPrint = 0;
        if (messageRoutine)
        {
            messageRoutine(ch);
        }
        menuactive = false;
        S_StartSound(NULL,sfx_swtchx);
        return true;
    }

    if (devparm && ch == KEY_F1)
    {
        G_ScreenShot ();
        return true;
    }

    // F-Keys
    if (!menuactive)
    switch(ch)
    {
        case KEY_MINUS:         // Screen size down
        if (automapactive || chat_on)
        {
            return false;
        }
        M_RD_Change_ScreenSize(0);
        S_StartSound(NULL,sfx_stnmov);
        return true;

        case KEY_EQUALS:        // Screen size up
        if (automapactive || chat_on)
        {
            return false;
        }
        M_RD_Change_ScreenSize(1);
        S_StartSound(NULL,sfx_stnmov);
        return true;

        case KEY_F1:            // Help key
	    M_StartControlPanel ();
	    currentMenu = english_language ? &ReadDef2 : &ReadDef2_Rus;
        itemOn = 0;
        S_StartSound(NULL,sfx_swtchn);
        return true;

        case KEY_F2:            // Save
        QuickSaveTitle = false;
        M_StartControlPanel();
        S_StartSound(NULL,sfx_swtchn);
        M_SaveGame(0);
        return true;

        case KEY_F3:            // Load
        M_StartControlPanel();
        S_StartSound(NULL,sfx_swtchn);
        M_LoadGame(0);
        return true;

        case KEY_F4:            // Sound Volume
        M_StartControlPanel ();
        currentMenu = english_language ? &RD_Audio_Def : &RD_Audio_Def_Rus;
        itemOn = rd_audio_sfxvolume;
        S_StartSound(NULL,sfx_swtchn);
        return true;

        case KEY_F5:            // Detail toggle
        M_RD_Change_Detail(0);
        S_StartSound(NULL,sfx_swtchn);
        return true;

        case KEY_F6:            // Quicksave
        QuickSaveTitle = true;
        S_StartSound(NULL,sfx_swtchn);
        M_QuickSave();
        return true;

        case KEY_F7:            // End game
        S_StartSound(NULL,sfx_swtchn);
        M_EndGame(0);
        return true;

        case KEY_F8:            // Toggle messages
        M_RD_Change_Messages(0);
        S_StartSound(NULL,sfx_swtchn);
        return true;

        case KEY_F9:            // Quickload
        S_StartSound(NULL,sfx_swtchn);
        M_QuickLoad();
        return true;

        case KEY_F10:           // Quit DOOM
        S_StartSound(NULL,sfx_swtchn);
        M_QuitDOOM(0);
        return true;
    }

    if (ch == KEY_F11)          // gamma toggle
    {
        usegamma++;
        if (usegamma > 17)
        usegamma = 0;

        players[consoleplayer].message = english_language ?
                                         gammamsg[usegamma] :
                                         gammamsg_rus[usegamma];
        pal = (byte *) W_CacheLumpName (usegamma <= 8 ?  "PALFIX" : "PLAYPAL",
                                        PU_CACHE) + st_palette * 768;
        I_SetPalette (pal);
        return true;
    }

    // Pop-up menu?
    if (!menuactive)
    {
        if (ch == KEY_ESCAPE)
        {
            M_StartControlPanel ();
            S_StartSound(NULL,sfx_swtchn);
            return true;
        }
        return false;
    }

    // Keys usable within menu
    switch (ch)
    {
        case KEY_DOWNARROW:
        do
        {
            if (itemOn+1 > currentMenu->numitems-1)
            {
                itemOn = 0;
            }
            else
            {
                itemOn++;
            }
            S_StartSound(NULL,sfx_pstop);
        } while(currentMenu->menuitems[itemOn].status==-1);
        return true;

        case KEY_UPARROW:
        do
        {
            if (!itemOn)
            {
                itemOn = currentMenu->numitems-1;
            }
            else
            {
                itemOn--;
            }
            S_StartSound(NULL,sfx_pstop);
        } while(currentMenu->menuitems[itemOn].status==-1);
        return true;

        case KEY_LEFTARROW:
        if (currentMenu->menuitems[itemOn].routine
        &&  currentMenu->menuitems[itemOn].status == 2)
        {
            S_StartSound(NULL,sfx_stnmov);
            currentMenu->menuitems[itemOn].routine(0);
        }
        return true;

        case KEY_RIGHTARROW:
        if (currentMenu->menuitems[itemOn].routine
        &&  currentMenu->menuitems[itemOn].status == 2)
        {
            S_StartSound(NULL,sfx_stnmov);
            currentMenu->menuitems[itemOn].routine(1);
        }
        return true;

        case KEY_ENTER:
        if (currentMenu->menuitems[itemOn].routine
        &&  currentMenu->menuitems[itemOn].status)
        {
            currentMenu->lastOn = itemOn;
            if (currentMenu->menuitems[itemOn].status == 2)
            {
                currentMenu->menuitems[itemOn].routine(1); // right arrow
                S_StartSound(NULL,sfx_stnmov);
            }
            else
            {
                currentMenu->menuitems[itemOn].routine(itemOn);
                S_StartSound(NULL,sfx_pistol);
            }
        }
        return true;

        case KEY_ESCAPE:
        currentMenu->lastOn = itemOn;
        M_ClearMenus ();
        S_StartSound(NULL,sfx_swtchx);
        return true;

        case KEY_BACKSPACE:
        currentMenu->lastOn = itemOn;
        if (currentMenu->prevMenu)
        {
            currentMenu = currentMenu->prevMenu;
            itemOn = currentMenu->lastOn;
            S_StartSound(NULL,sfx_swtchn);
        }
        return true;

        // [JN] Scroll Gameplay features menu by PgUp/PgDn keys
        case KEY_PGUP:
        {
            currentMenu->lastOn = itemOn;

            if (currentMenu == &RD_Gameplay_Def_1
            ||  currentMenu == &RD_Gameplay_Def_1_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_4 :
                                &RD_Gameplay_Def_4_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
            if (currentMenu == &RD_Gameplay_Def_2
            ||  currentMenu == &RD_Gameplay_Def_2_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_1 :
                                &RD_Gameplay_Def_1_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
            if (currentMenu == &RD_Gameplay_Def_3
            ||  currentMenu == &RD_Gameplay_Def_3_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_2 :
                                &RD_Gameplay_Def_2_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
            if (currentMenu == &RD_Gameplay_Def_4
            ||  currentMenu == &RD_Gameplay_Def_4_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_3 :
                                &RD_Gameplay_Def_3_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
        }
        case KEY_PGDN:
        {
            currentMenu->lastOn = itemOn;
        
            if (currentMenu == &RD_Gameplay_Def_1
            ||  currentMenu == &RD_Gameplay_Def_1_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_2 :
                                &RD_Gameplay_Def_2_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
            if (currentMenu == &RD_Gameplay_Def_2
            ||  currentMenu == &RD_Gameplay_Def_2_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_3 :
                                &RD_Gameplay_Def_3_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
            if (currentMenu == &RD_Gameplay_Def_3
            ||  currentMenu == &RD_Gameplay_Def_3_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_4 :
                                &RD_Gameplay_Def_4_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
            if (currentMenu == &RD_Gameplay_Def_4
            ||  currentMenu == &RD_Gameplay_Def_4_Rus)
            {
                M_SetupNextMenu(english_language ?
                                &RD_Gameplay_Def_1 :
                                &RD_Gameplay_Def_1_Rus);
                S_StartSound(NULL,sfx_pistol);
                return true;
            }
        }

        case 0:
        break;

        default:
        for (i = itemOn+1;i < currentMenu->numitems;i++)
        if (currentMenu->menuitems[i].alphaKey == ch)
        {
            itemOn = i;
            S_StartSound(NULL,sfx_pstop);
            return true;
        }
        for (i = 0;i <= itemOn;i++)
        if (currentMenu->menuitems[i].alphaKey == ch)
        {
            itemOn = i;
            S_StartSound(NULL,sfx_pstop);
            return true;
        }
        break;
    }
    return false;
}


//
// M_StartControlPanel
//
void M_StartControlPanel (void)
{
    // intro might call this repeatedly
    if (menuactive)
    {
        return;
    }

    menuactive = 1;
    currentMenu = english_language ? &MainDef : &MainDef_Rus;   // JDC
    itemOn = currentMenu->lastOn;                               // JDC
}


//
// M_Drawer
// Called after the view has been rendered,
// but before it has been blitted.
//
void M_Drawer (void)
{
    int          start;
    char         string[80];
    short        i;
    short        max;
    static short x;
    static short y;

    inhelpscreens = false;

    // Horiz. & Vertically center string and print it.
    if (messageToPrint)
    {
        start = 0;
        y = 100 - M_StringHeight(messageString)/2;

        while(*(messageString+start))
        {
            for (i = 0 ; i < strlen(messageString+start) ; i++)
            if (*(messageString+start+i) == '\n')
            {
                memset(string,0,40);
                strncpy(string,messageString+start,i);
                start += i+1;
                break;
            }

            if (i == strlen(messageString+start))
            {
                strcpy(string,messageString+start);
                start += i;
            }

            x = 160 - M_StringWidth(string)/2;

            if (english_language)
            {
                M_WriteText(x,y,string);
                y += SHORT(hu_font[0]->height);
            }
            else
            {
                M_WriteTextSmall_RUS(x, y, string);
                y += SHORT(hu_font_small_rus[0]->height); 
            }
        }
        return;
    }

    if (!menuactive)
    {
        return;
    }

    if (currentMenu->routine)
    {
        // call Draw routine
        currentMenu->routine();
    }

    // DRAW MENU
    x = currentMenu->x;
    y = currentMenu->y;
    max = currentMenu->numitems;

    for (i=0 ; i<max ; i++)
    {
        // ---------------------------------------------------------------------
        // [JN] Write common menus by using standard graphical patches:
        // ---------------------------------------------------------------------
        if (currentMenu == &MainDef                // Main Menu
        ||  currentMenu == &MainDef_Rus            // Main Menu
        ||  currentMenu == &EpiDef                 // Episode selection
        ||  currentMenu == &EpiDef_Rus             // Episode selection
        ||  currentMenu == &NewDef                 // Skill level
        ||  currentMenu == &NewDef_Rus)            // Skill level
        {
            V_DrawShadowDirect (x+1,y+1,0, W_CacheLumpName(currentMenu->menuitems[i].name ,PU_CACHE));
            V_DrawPatchDirect (x,y,0, W_CacheLumpName(currentMenu->menuitems[i].name ,PU_CACHE));

            // DRAW SKULL
            V_DrawShadowDirect(x+1 + SKULLXOFF,currentMenu->y+1 - 5 + itemOn*LINEHEIGHT, 0,
                               W_CacheLumpName(skullName[whichSkull],PU_CACHE));
            V_DrawPatchDirect(x + SKULLXOFF,currentMenu->y - 5 + itemOn*LINEHEIGHT, 0,
                               W_CacheLumpName(skullName[whichSkull],PU_CACHE));

            // [JN] Big vertical spacing
            y += LINEHEIGHT;
        }
        // ---------------------------------------------------------------------
        // [JN] Write English options menu with big English font
        // ---------------------------------------------------------------------
        else if (currentMenu == &RD_Options_Def)
        {
            M_WriteTextBig_ENG(x, y, currentMenu->menuitems[i].name);

            // DRAW SKULL
            V_DrawShadowDirect(x+1 + SKULLXOFF,currentMenu->y+1 - 5 + itemOn*LINEHEIGHT, 0,
                               W_CacheLumpName(skullName[whichSkull],PU_CACHE));
            V_DrawPatchDirect(x + SKULLXOFF,currentMenu->y - 5 + itemOn*LINEHEIGHT, 0,
                               W_CacheLumpName(skullName[whichSkull],PU_CACHE));

            // [JN] Big vertical spacing
            y += LINEHEIGHT;
        }
        // ---------------------------------------------------------------------
        // [JN] Write Russian options menu with big Russian font
        // ---------------------------------------------------------------------
        else if (currentMenu == &RD_Options_Def_Rus)
        {
            M_WriteTextBig_RUS(x, y, currentMenu->menuitems[i].name);

            // DRAW SKULL
            V_DrawShadowDirect(x+1 + SKULLXOFF,currentMenu->y+1 - 5 + itemOn*LINEHEIGHT, 0,
                               W_CacheLumpName(skullName[whichSkull],PU_CACHE));
            V_DrawPatchDirect(x + SKULLXOFF,currentMenu->y - 5 + itemOn*LINEHEIGHT, 0,
                               W_CacheLumpName(skullName[whichSkull],PU_CACHE));

            // [JN] Big vertical spacing
            y += LINEHEIGHT;
        }
        // ---------------------------------------------------------------------
        // [JN] Write English submenus with small English font
        // ---------------------------------------------------------------------
        else 
        if (currentMenu == &RD_Rendering_Def
        ||  currentMenu == &RD_Display_Def
        ||  currentMenu == &RD_Automap_Def
        ||  currentMenu == &RD_Audio_Def
        ||  currentMenu == &RD_Controls_Def
        ||  currentMenu == &RD_Gameplay_Def_1
        ||  currentMenu == &RD_Gameplay_Def_2
        ||  currentMenu == &RD_Gameplay_Def_3
        ||  currentMenu == &RD_Gameplay_Def_4)
        {
            M_WriteTextSmall_ENG(x, y, currentMenu->menuitems[i].name);

            // [JN] Draw blinking ">" symbol
            if (whichSkull == 0)
            dp_translation = cr[CR_DARKRED];
            M_WriteTextSmall_ENG(x + SKULLXOFF + 24, currentMenu->y 
                                   + itemOn*LINEHEIGHT_SML, ">");
            dp_translation = NULL;

            // [JN] Small vertical spacing
            y += LINEHEIGHT_SML;
        }
        // ---------------------------------------------------------------------
        // [JN] Write Russian submenus with small Russian font
        // ---------------------------------------------------------------------
        else
        if (currentMenu == &RD_Rendering_Def_Rus
        ||  currentMenu == &RD_Display_Def_Rus
        ||  currentMenu == &RD_Automap_Def_Rus
        ||  currentMenu == &RD_Audio_Def_Rus
        ||  currentMenu == &RD_Controls_Def_Rus
        ||  currentMenu == &RD_Gameplay_Def_1_Rus
        ||  currentMenu == &RD_Gameplay_Def_2_Rus
        ||  currentMenu == &RD_Gameplay_Def_3_Rus
        ||  currentMenu == &RD_Gameplay_Def_4_Rus)
        {
            M_WriteTextSmall_RUS(x, y, currentMenu->menuitems[i].name);
            
            // [JN] Draw blinking ">" symbol
            if (whichSkull == 0)
            dp_translation = cr[CR_DARKRED];
            M_WriteTextSmall_ENG(x + SKULLXOFF + 24, currentMenu->y 
                                   + itemOn*LINEHEIGHT_SML, ">");
            dp_translation = NULL;
        
            // [JN] Small vertical spacing
            y += LINEHEIGHT_SML;
        }

        // [JN] Saving / Loading menus, help screens. Just a blinking skull.
        if (currentMenu == &LoadDef
        ||  currentMenu == &LoadDef_Rus
        ||  currentMenu == &SaveDef
        ||  currentMenu == &SaveDef_Rus
        ||  currentMenu == &ReadDef1
        ||  currentMenu == &ReadDef1_Rus
        ||  currentMenu == &ReadDef2
        ||  currentMenu == &ReadDef2_Rus)
        {
            // DRAW SKULL
            V_DrawShadowDirect(x+1 + SKULLXOFF,currentMenu->y+1 - 5 + itemOn*LINEHEIGHT, 0,
                    W_CacheLumpName(skullName[whichSkull],PU_CACHE));
            V_DrawPatchDirect(x + SKULLXOFF,currentMenu->y - 5 + itemOn*LINEHEIGHT, 0,
                    W_CacheLumpName(skullName[whichSkull],PU_CACHE));
        }
    }
}


//
// M_ClearMenus
//
void M_ClearMenus (void)
{
    menuactive = 0;
}


//
// M_SetupNextMenu
//
void M_SetupNextMenu(menu_t *menudef)
{
    currentMenu = menudef;
    itemOn = currentMenu->lastOn;
}


//
// M_Ticker
//
void M_Ticker (void)
{
    if (--skullAnimCounter <= 0)
    {
        whichSkull ^= 1;
        skullAnimCounter = 8;
    }
}


//
// M_Init
//
void M_Init (void)
{
    currentMenu = english_language ? &MainDef : &MainDef_Rus;
    menuactive = 0;
    itemOn = currentMenu->lastOn;
    whichSkull = 0;
    skullAnimCounter = 10;
    screenSize = screenblocks - 3;
    messageToPrint = 0;
    messageString = NULL;
    messageLastMenuActive = menuactive;
    quickSaveSlot = -1;

    // [JN] Don't show shareware info screen (HELP2) in non-shareware Doom 1
    // Taken from Chocolate Doom (src/doom/m_menu.c)    
    if (registered || retail)
    {
        MainMenu[readthis].routine = M_ReadThis2;
        MainMenu_Rus[readthis].routine = M_ReadThis2;
        ReadDef2.prevMenu = NULL;
        ReadDef2_Rus.prevMenu = NULL;
    }

    if (commercial)
    {
        MainMenu[readthis] = MainMenu[quitdoom];
        MainMenu_Rus[readthis] = MainMenu_Rus[quitdoom];
        MainDef.numitems--;
        MainDef_Rus.numitems--;
        MainDef.y += 8;
        MainDef_Rus.y += 8;
        NewDef.prevMenu = &MainDef;
        NewDef_Rus.prevMenu = &MainDef_Rus;
        ReadDef1.routine = M_DrawReadThisRetail;
        ReadDef1_Rus.routine = M_DrawReadThisRetail;
        ReadDef1.x = 330;
        ReadDef1.y = 165;
        ReadMenu1[0].routine = M_FinishReadThis;
    }

    // We need to remove the fourth episode.
    if (!retail)
    {
        EpiDef.numitems--;
        EpiDef_Rus.numitems--;
    }
}

