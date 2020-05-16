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
//	Printed strings for translation.
//	English language support (default).
//

// ����⪠ �� ᨬ�����:
// --------------------
// < = ,
// > = .
// $ = BFG
// # = y 
// @ = n
// ^ = "

#ifndef __D_ENGLSH__
#define __D_ENGLSH__

//
//	Printed strings for translation
//

//
// D_Main.C
//
#define D_DEVSTR	"��⨢�஢�� ०�� ࠧࠡ��稪�.\n"
#define D_CDROM		"��᪮��� �����: 䠩�� ���䨣��樨 ��࠭���� � ����� c:\\doomdata\n"

//
//	M_Menu.C
//
#define PRESSKEY    "(yf;vbnt k.,e. rkfdbie)"   // (������ ���� �������.)
#define PRESSYN     "(yf;vbnt ^#^ bkb ^@^)"     // (������ "Y" ��� "N".)
#define DOSY        "(yf;vbnt ^#^ xnj,s dsqnb)" // (������ "Y" �⮡� ���.)

#define QUITMSG     "ds ltqcndbntkmyj \[jnbnt dsqnb\nbp 'njq pfvtxfntkmyjq buhs?"            // �� ����⢨⥫쭮 ��� ��� \n �� �⮩ �����⥫쭮� ����?
#define LOADNET     "ytdjpvj;yj pfuhepbnm cj[hfytyye.\nbuhe d ctntdjq buht!\n\n"PRESSKEY    // ���������� ����㧨�� ��࠭���� \n ���� � �⥢�� ���!
#define QLOADNET    "ytdjpvj;yj pfuhepbnm ,scnhjt\ncj[hfytybt d ctntdjq buht!\n\n"PRESSKEY  // ���������� ����㧨�� ����஥ \n ��࠭���� � �⥢�� ���!
#define QSAVESPOT   "e dfc yt ds,hfy ckjn ,scnhjuj cj[hfytybz!\n\n"PRESSKEY                 // � ��� �� ��࠭ ᫮� ����ண� ��࠭����!
#define SAVEDEAD    "ytdjpvj;yj cj[hfybnm buhe< yt yfxfd tt!\n\n"PRESSKEY                   // ���������� ��࠭��� ����, �� ��砢 ��!
#define QSPROMPT    "gthtpfgbcfnm ,scnhjt cj[hfytybt\n^%s^?\n\n"PRESSYN                     // quicksave over your game named\n\n'%s'?\n\n
#define QLPROMPT    "pfuhepbnm ,scnhjt cj[hfytybt\n^%s^?\n\n"PRESSYN                        // do you want to quickload the game named\n'%s'?\n\n

#define RD_ON       "drk"       // ���
#define RD_OFF      "dsrk"      // �몫
#define RD_NEXT     "lfktt )"   // ����� >
#define RD_PREV     "( yfpfl"   // < �����

// ����ਨ�஢� ����ன�� ����
// ��襭� �� �⠭����� ���祭��.
//
// ���७�, �� ��� �த������?
#define RD_DEFAULTS                     \
"dyenhbbuhjdst yfcnhjqrb ,elen\n"       \
"c,hjitys yf cnfylfhnyst pyfxtybz>\n"   \
"\n"                                    \
"edthtys< xnj [jnbnt ghjljk;bnm?\n\n"   \
PRESSYN

// ���������� ����� ����� ����
// �� ��⨢��� �⥢�� ���.
#define NEWGAME	\
"ytdjpvj;yj yfxfnm yjde. buhe\n"\
"ghb frnbdyjq ctntdjq buht>\n\n"PRESSKEY 

// ���७�? ��� �஢��� ᫮�����
// ���� �� ������ � ��⭮��.
#define NIGHTMARE	\
"edthtys? 'njn ehjdtym ckj;yjcnb\n"\
"lf;t yt ,kbpjr r xtcnyjve>\n\n"PRESSYN 

// ������� �� ���������, ������
// �� ᪮���� � �������� �஭�,
// ����ᨬ��� ��ப�, 㢥��祭�.
//
// ���७�, �� ᬮ��� �릨��?
#define ULTRANM	\
"vjycnhs yt djcrhtif.ncz< jlyfrj\n"\
"b[ crjhjcnm b lbfgfpjy ehjyf<\n"\
"yfyjcbvjuj buhjre< edtkbxtys>\n\n"\
"edthtys< xnj cvj;tnt ds;bnm?\n\n"PRESSYN

// �� ��������樮���� ����� ����.
// ��� ����室��� �ਮ���� ��� �ਫ����.
#define SWSTRING	\
"'nj ltvjycnhfwbjyyfz dthcbz buhs>\n\n"\
"dfv ytj,[jlbvj ghbj,htcnb dc. nhbkjub.>\n\n"PRESSKEY 

#define MSGOFF  "cjj,otybz dsrk.xtys"                                   // ����饭�� �몫�祭�
#define MSGON   "cjj,otybz drk.xtys"                                    // ����饭�� ����祭�
#define NETEND  "ytdjpvj;yj pfrjyxbnm ctntde. buhe!\n\n"PRESSKEY        // ���������� �������� �⥢�� ����!
#define ENDGAME "ds ltqcndbntkmyj [jnbnt pfrjyxbnm buhe?\n\n"PRESSYN    // �� ����⢨⥫쭮 ��� �������� ����?

#define DETAILHI    "dscjrfz ltnfkbpfwbz"       // ��᮪�� ��⠫�����
#define DETAILLO    "ybprfz ltnfkbpfwbz"        // ������ ��⠫�����

// Improved (PALFIX) and standard (PLAYPAL) gamma-correction
#define GAMMA_IMPROVED_OFF  "ekexityyfz ufvvf-rjhhtrwbz jnrk.xtyf"     // ����襭��� �����-���४�� �⪫�祭�
#define GAMMA_IMPROVED_05   "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 0>5"   // �஢��� ���襭��� �����-���४樨 0.5
#define GAMMA_IMPROVED_1    "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 1>0"   // �஢��� ���襭��� �����-���४樨 1.0
#define GAMMA_IMPROVED_15   "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 1>5"   // �஢��� ���襭��� �����-���४樨 1.5
#define GAMMA_IMPROVED_2    "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 2>0"   // �஢��� ���襭��� �����-���४樨 2.0
#define GAMMA_IMPROVED_25   "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 2>5"   // �஢��� ���襭��� �����-���४樨 2.5
#define GAMMA_IMPROVED_3    "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 3>0"   // �஢��� ���襭��� �����-���४樨 3.0
#define GAMMA_IMPROVED_35   "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 3>5"   // �஢��� ���襭��� �����-���४樨 3.5
#define GAMMA_IMPROVED_4    "ehjdtym ekexityyjq ufvvf-rjhhtrwbb 4>0"   // �஢��� ���襭��� �����-���४樨 4.0
#define GAMMA_ORIGINAL_OFF  "cnfylfhnyfz ufvvf-rjhhtrwbz jnrk.xtyf"    // �⠭���⭠� �����-���४�� �⪫�祭�
#define GAMMA_ORIGINAL_05   "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 0>5"  // �஢��� �⠭���⭮� �����-���४樨 0.5
#define GAMMA_ORIGINAL_1    "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 1>0"  // �஢��� �⠭���⭮� �����-���४樨 1.0
#define GAMMA_ORIGINAL_15   "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 1>5"  // �஢��� �⠭���⭮� �����-���४樨 1.5
#define GAMMA_ORIGINAL_2    "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 2>0"  // �஢��� �⠭���⭮� �����-���४樨 2.0
#define GAMMA_ORIGINAL_25   "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 2>5"  // �஢��� �⠭���⭮� �����-���४樨 2.5
#define GAMMA_ORIGINAL_3    "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 3>0"  // �஢��� �⠭���⭮� �����-���४樨 3.0
#define GAMMA_ORIGINAL_35   "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 3>5"  // �஢��� �⠭���⭮� �����-���४樨 3.5
#define GAMMA_ORIGINAL_4    "ehjdtym cnfylfhnyjq ufvvf-rjhhtrwbb 4>0"  // �஢��� �⠭���⭮� �����-���४樨 4.0

#define EMPTYSTRING "gecnjq ckjn"                  // ���⮩ ᫮�
#define SECRETFOUND "j,yfhe;ty nfqybr!"            // �����㦥� ⠩���!
#define CHEATDENIED "dsgjkytybt rjvfyls pfghtotyj" // �믮������ ������� ����饭�

//
//	P_inter.C
//

#define GOTARMOR    "gjkextyf ,hjyz>"           // ����祭� �஭�.
#define GOTMEGA     "gjkextyf vtuf,hjyz!"       // ����祭� �����஭�!
#define GOTHTHBONUS "gjkexty ,jyec pljhjdmz>"   // ����祭 ����� ���஢��.
#define GOTARMBONUS "gjkexty ,jyec ,hjyb>"      // ����祭 ����� �஭�.
#define GOTSTIM     "gjkexty cnbvekznjh>"       // ����祭 �⨬����.
#define GOTMEDIKIT  "gjkextyf fgntxrf>"         // ����祭� ���窠.
#define GOTSUPER    "cdth[pfhzl!"               // ����姠��!
#define GOTMEDINEED "gjkextyf rhfqyt ytj,[jlbvfz fgntxrf!"  // ����祭� �ࠩ�� ����室���� ���窠!

#define GOTBLUECARD "gjkextyf cbyzz rk.x-rfhnf>"    // ����祭� ᨭ�� ����-����.
#define GOTYELWCARD "gjkextyf ;tknfz rk.x-rfhnf>"   // ����祭� ����� ����-����
#define GOTREDCARD  "gjkextyf rhfcyfz rk.x-rfhnf>"  // ����祭� ��᭠� ����-����.
#define GOTBLUESKUL "gjkexty cbybq rk.x-xthtg>"     // ����祭 ᨭ�� ����-�९.
#define GOTYELWSKUL "gjkexty ;tknsq rk.x-xthtg>"    // ����祭 ����� ����-�९.
#define GOTREDSKULL "gjkexty rhfcysq rk.x-xthtg>"   // ����祭 ���� ����-�९.

#define GOTINVUL    "ytezpdbvjcnm!"                 // ���梨������!
#define GOTBERSERK  ",thcthr!"                      // �����!
#define GOTINVIS    "xfcnbxyfz ytdblbvjcnm>"        // ����筠� �����������.
#define GOTSUIT	    "rjcn.v hflbfwbjyyjq pfobns>"   // ����� ࠤ��樮���� �����.
#define GOTMAP      "rjvgm.nthyfz rfhnf ehjdyz>"    // �������ୠ� ���� �஢��.
#define GOTVISOR    "dbpjh ecbktybz jcdtotybz>"     // ����� �ᨫ���� �ᢥ饭��.
#define GOTMSPHERE  "vtufcathf!"                    // �������!

#define GOTCLIP     "gjkextyf j,jqvf>"                          // ����祭� ������.
#define GOTCLIPBOX  "gjkextyf rjhj,rf gfnhjyjd>"                // ����祭� ��஡�� ���஭��.
#define GOTROCKET   "gjkextyf hfrtnf>"                          // ����祭� ࠪ��.
#define GOTROCKET2  "gjkextyj ldt hfrtns>"                      // ����祭� ��� ࠪ���.
#define GOTROCKBOX  "gjkexty zobr hfrtn>"                       // ����祭 �騪 ࠪ��.
#define GOTCELL     "gjkextyf 'ythuj,fnfhtz>"                   // ����祭� �࣮�����.
#define GOTCELLBOX  "gjkexty 'ythujfrrevekznjh>"                // ����祭 �࣮��������.
#define GOTSHELLS   "gjkextyj 4 gfnhjyf lkz lhj,jdbrf>"         // ����祭� 4 ���஭� ��� �஡�����.
#define GOTSHELLS8  "gjkextyj 8 gfnhjyjd lkz lhj,jdbrf>"        // ����祭� 8 ���஭�� ��� �஡�����.
#define GOTSHELLBOX "gjkextyf rjhj,rf gfnhjyjd lkz lhj,jdbrf>"  // ����祭� ��஡�� ���஭�� ��� �஡�����.
#define GOTBACKPACK "gjkexty h.rpfr c fvveybwbtq!"              // ����祭 ��� � ��㭨樥�!

#define GOTBFG9000  "gjkextyj $9000! j lf>"         // ����祭� BFG9000! � ��.
#define GOTCHAINGUN "gjkexty gektvtn!"              // ����祭 �㫥���!
#define GOTCHAINSAW ",typjgbkf! yfqlb ytvyjuj vzcf!"// ���������! ����� ������� ���!
#define GOTLAUNCHER "gjkextyf hfrtnybwf!"           // ����祭� ࠪ�⭨�!
#define GOTPLASMA   "gjkextyf gkfpvtyyfz geirf!"    // ����祭� ���������� ��誠!
#define GOTSHOTGUN  "gjkexty lhj,jdbr!"             // ����祭 �஡����!
#define GOTSHOTGUN2 "gjkextyj ldecndjkmyjt he;mt!"  // ����祭� ����⢮�쭮� ���!

//
// P_Doors.C
//

#define PD_BLUEO    "lkz frnbdfwbb ye;ty cbybq rk.x>"   // ��� ��⨢�樨 �㦥� ᨭ�� ����.
#define PD_REDO     "lkz frnbdfwbb ye;ty rhfcysq rk.x>" // ��� ��⨢�樨 �㦥� ���� ����.
#define PD_YELLOWO  "lkz frnbdfwbb ye;ty ;tknsq rk.x>"  // ��� ��⨢�樨 �㦥� ����� ����.
#define PD_BLUEK    "lkz jnrhsnbz ye;ty cbybq rk.x>"    // ��� ������ �㦥� ᨭ�� ����.
#define PD_REDK     "lkz jnrhsnbz ye;ty rhfcysq rk.x>"  // ��� ������ �㦥� ���� ����.
#define PD_YELLOWK  "lkz jnrhsnbz ye;ty ;tknsq rk.x>"   // ��� ������ �㦥� ���� ����.

//
//	G_game.C
//

#define GGSAVED     "buhf cj[hfytyf>"   // ��� ��࠭���.
#define GGLOADED    "buhf pfuhe;tyf>"   // ��� ����㦥��.

//
//	HU_stuff.C
//

#define HUSTR_MSGU  "(cjj,otybt yt jnghfdktyj)"                 // (����饭�� �� ��ࠢ����)

//
// DOOM 1
//

#define HUSTR_E1M1  "t1v1: fyufh"                               // E1M1: �����
#define HUSTR_E1M2  "t1v2: fnjvyfz 'ktrnhjcnfywbz"              // E1M2: �⮬��� �����⠭��
#define HUSTR_E1M3  "t1v3: pfdjl gj gththf,jnrt zlj[bvbrfnjd"   // E1M3: ����� �� ���ࠡ�⪥ 冷娬���⮢
#define HUSTR_E1M4  "t1v4: rjvfylysq geyrn"                     // E1M4: �������� �㭪�
#define HUSTR_E1M5  "t1v5: kf,jhfnjhbz yf aj,jct"               // E1M5: �������� �� �����
#define HUSTR_E1M6  "t1v6: wtynhfkmysq geyrn j,hf,jnrb"         // E1M6: ����ࠫ�� �㭪� ��ࠡ�⪨
#define HUSTR_E1M7  "t1v7: dsxbckbntkmysq wtynh"                // E1M7: ���᫨⥫�� 業��
#define HUSTR_E1M8  "t1v8: fyjvfkbz yf aj,jct"                  // E1M8: �������� �� �����
#define HUSTR_E1M9  "t1v9: djtyyfz ,fpf"                        // E1M9: ������� ����

#define HUSTR_E2M1  "t2v1: fyjvfkbz yf ltqvjct"                 // E2M1: �������� �� ������
#define HUSTR_E2M2  "t2v2: [hfybkbot"                           // E2M2: �࠭����
#define HUSTR_E2M3  "t2v3: jxbcnbntkmysq pfdjl"                 // E2M3: ����⥫�� �����
#define HUSTR_E2M4  "t2v4: kf,jhfnjhbz yf ltqvjct"              // E2M4: �������� �� ������
#define HUSTR_E2M5  "t2v5: rjvfylysq wtynh"                     // E2M5: �������� 業��
#define HUSTR_E2M6  "t2v6: pfks ghjrkzns["                      // E2M6: ���� �ப�����
#define HUSTR_E2M7  "t2v7: ythtcnbkbot"                         // E2M7: ����⨫��
#define HUSTR_E2M8  "t2v8: dfdbkjycrfz ,fiyz"                   // E2M8: �������᪠� ����
#define HUSTR_E2M9  "t2v9: rhtgjcnm nfqy"                       // E2M9: �९���� ⠩�

#define HUSTR_E3M1  "t3v1: rhtgjcnm flf"                        // E3M1: �९���� ���
#define HUSTR_E3M2  "t3v2: nhzcbyf jnxfzybz"                    // E3M2: ���ᨭ� ���ﭨ�
#define HUSTR_E3M3  "t3v3: gfyltvjybq"                          // E3M3: ����������
#define HUSTR_E3M4  "t3v4: ljv ,jkb"                            // E3M4: ��� ����
#define HUSTR_E3M5  "t3v5: ytxtcnbdsq cj,jh"                    // E3M5: ����⨢� ᮡ��
#define HUSTR_E3M6  "t3v6: ujhf 'ht,"                           // E3M6: ��� �ॡ
#define HUSTR_E3M7  "t3v7: dhfnf d kbv,"                        // E3M7: ��� � ����
#define HUSTR_E3M8  "t3v8: lbn"                                 // E3M8: ���
#define HUSTR_E3M9  "t3v9: rhjkbxbq cfl"                        // E3M9: �஫�稩 ᠤ

#define HUSTR_E4M1  "t4v1: gjl fljv"                            // E4M1: ��� ����
#define HUSTR_E4M2  "t4v2: bcnbyyfz ytyfdbcnm"                  // E4M2: ��⨭��� ���������
#define HUSTR_E4M3  "t4v3: bcnzpfybt ytxtcnbds["                // E4M3: ���易��� ����⨢��
#define HUSTR_E4M4  "t4v4: ytelth;bvjt pkj"                     // E4M4: ��㤥ন��� ���
#define HUSTR_E4M5  "t4v5: jyb gjrf.ncz"                        // E4M5: ��� ��������
#define HUSTR_E4M6  "t4v6: ceghjnbd cb[ ytxtcnbds["             // E4M6: ���⨢ �� ����⨢��
#define HUSTR_E4M7  "t4v7: b gjcktljdfk fl"                     // E4M7: � ��᫥����� ��
#define HUSTR_E4M8  "t4v8: lj ,tccthltxbz"                      // E4M8: �� ����थ��
#define HUSTR_E4M9  "t4v9: cnhf["                               // E4M9: ����

//
// DOOM2: �� �� �����
//

#define HUSTR_1     "ehjdtym 1: gfhflysq d[jl"                  // �஢��� 1: ��ࠤ�� �室
#define HUSTR_2     "ehjdtym 2: gjlptvyst pfks"                 // �஢��� 2: �������� ����
#define HUSTR_3     "ehjdtym 3: dspjd ,hjity"                   // �஢��� 3: �맮� ��襭
#define HUSTR_4     "ehjdtym 4: chtljnjxbt"                     // �஢��� 4: �।��稥
#define HUSTR_5     "ehjdtym 5: cnjxyst neyytkb"                // �஢��� 5: ���� �㭭���
#define HUSTR_6     "ehjdtym 6: ghtcc"                          // �஢��� 6: ����
#define HUSTR_7     "ehjdtym 7: cvthntkmyj ghjcnjq"             // �஢��� 7: ����⥫쭮 ���⮩
#define HUSTR_8     "ehjdtym 8: ekjdrb b kjdeirb"               // �஢��� 8: ������ � ����誨
#define HUSTR_9     "ehjdtym 9: zvf"                            // �஢��� 9: ���
#define HUSTR_10    "ehjdtym 10: pfghfdjxyfz ,fpf"              // �஢��� 10: ���ࠢ�筠� ����
#define HUSTR_11    "ehjdtym 11: ^j^ hfpheitybz!"               // �஢��� 11: "o" ࠧ��襭��!

#define HUSTR_12    "ehjdtym 12: af,hbrf"                       // �஢��� 12: ���ਪ�
#define HUSTR_13    "ehjdtym 13: ltkjdjq hfqjy"                 // �஢��� 13: ������� ࠩ��
#define HUSTR_14    "ehjdtym 14: uke,jxfqibt kjujdbof"          // �஢��� 14: ��㡮砩訥 �������
#define HUSTR_15    "ehjdtym 15: ghjvsiktyyfz pjyf"             // �஢��� 15: �஬�諥���� ����
#define HUSTR_16    "ehjdtym 16: ghbujhjl"                      // �஢��� 16: �ਣ�த 
#define HUSTR_17    "ehjdtym 17: dkfltybz"                      // �஢��� 17: �������� 
#define HUSTR_18    "ehjdtym 18: dyenhtyybq ldjh"               // �஢��� 18: ����७��� ����
#define HUSTR_19    "ehjdtym 19: wbnfltkm"                      // �஢��� 19: ��⠤��� 
#define HUSTR_20    "ehjdtym 20: gjgfkcz!"                      // �஢��� 20: �������!

#define HUSTR_21    "ehjdtym 21: ybhdfyf"                       // �஢��� 21: ��ࢠ�� 
#define HUSTR_22    "ehjdtym 22: rfnfrjv,s"                     // �஢��� 22: ��⠪���� 
#define HUSTR_23    "ehjdtym 23: wtkst ,jxrb dtctkmz"           // �஢��� 23: ���� ��窨 ��ᥫ��
#define HUSTR_24    "ehjdtym 24: ghjgfcnm"                      // �஢��� 24: �ய���� 
#define HUSTR_25    "ehjdtym 25: rhjdjgfls"                     // �஢��� 25: �஢�����
#define HUSTR_26    "ehjdtym 26: pf,hjityyst if[ns"             // �஢��� 26: ����襭�� ����
#define HUSTR_27    "ehjdtym 27: rjyljvbybev vjycnhjd"          // �஢��� 27: ���������� �����஢
#define HUSTR_28    "ehjdtym 28: vbh le[jd"                     // �஢��� 28: ��� ��客
#define HUSTR_29    "ehjdtym 29: rjytw dctuj ;bdjuj"            // �஢��� 29: ����� �ᥣ� ������
#define HUSTR_30    "ehjdtym 30: brjyf uht[f"                   // �஢��� 30: ����� ���

#define HUSTR_31    "ehjdtym 31: djkmatyinfqy"                  // �஢��� 31: ����䥭�⠩�
#define HUSTR_32    "ehjdtym 32: uhjcct"                        // �஢��� 32: ����

//
// DOOM2: ��� ����� ��� �����
//

#define NHUSTR_1	"ehjdtym 1: ,fpf yf ptvkt"      // �஢��� 1: ���� �� �����
#define NHUSTR_2	"ehjdtym 2: kf,jhfnjhbb ,jkb"   // �஢��� 2: ������ਨ ����
#define NHUSTR_3	"ehjdtym 3: rfymjy vthndtwjd"   // �஢��� 3: ���쮭 ���⢥殢
#define NHUSTR_4	"ehjdtym 4: flcrfz ujhf"        // �஢��� 4: ��᪠� ���
#define NHUSTR_5	"ehjdtym 5: dbdbctrwbz"         // �஢��� 5: ����ᥪ��
#define NHUSTR_6	"ehjdtym 6: rhjdm ghtbcgjlyzz"  // �஢��� 6: �஢� �२ᯮ����
#define NHUSTR_7	"ehjdtym 7: ,fyrtn e ,fhjyf"    // �஢��� 7: ������ � ��஭�
#define NHUSTR_8	"ehjdtym 8: uhj,ybwf pkj,s"     // �஢��� 8: �஡��� �����
#define NHUSTR_9	"ehjdtym 9: itcndbt ltvjyjd"    // �஢��� 9: ���⢨� �������

//
// �����-�஢�� ��� DOOM 2
//

#define MLSTR_1     "yfgfltybt"                // ATTACK.WAD   - ���������
#define MLSTR_2     "xthyfz ,fiyz"             // BLACKTWR.WAD - ��ୠ� ����
#define MLSTR_3     "rhtgjcnm d rhjdfdjv vjht" // BLOODSEA.WAD - �९���� � �஢���� ���
#define MLSTR_4     "rfymjy"                   // CANYON.WAD   - ���쮭
#define MLSTR_5     "gjvjcn"                   // CATWALK.WAD  - ������
#define MLSTR_6     "rjv,byfn"                 // COMBINE.WAD  - ��������
#define MLSTR_7     "abcnekf"                  // FISTULA.WAD  - ����㫠
#define MLSTR_8     "ufhybpjy"                 // GARRISON.WAD - ��୨���
#define MLSTR_9     "uthbjy"                   // GERYON.WAD   - ��ਮ�
#define MLSTR_10    "gjvtcnmt ubufynf"         // MANOR.WAD    - ������� ������
#define MLSTR_11    "vfdpjktq vtabcnjatkz"     // MEPHISTO.WAD - �������� �����䥫�
#define MLSTR_12    "ghbujdjh vbyjcf"          // MINOS.WAD    - �ਣ���� �����
#define MLSTR_13    "ytcc"                     // NESSUS.WAD   - ����
#define MLSTR_14    "gfhfljrc"                 // PARADOX.WAD  - ��ࠤ���
#define MLSTR_15    "gjlghjcnhfycndj"          // SUBSPACE.WAD - �������࠭�⢮
#define MLSTR_16    "gjlptvtkmt"               // SUBTERRA.WAD - ���������
#define MLSTR_17    "neh,jkban d ghtbcgjly.."  // TEETH.WAD    - ��࡮���� � �२ᯮ����
#define MLSTR_18    "lehyjq cjy"               //              - ��୮� ᮭ
#define MLSTR_19    "pfcnhzdibq yf nbnfyt"     // TTRAP.WAD    - ������訩 �� ��⠭�
#define MLSTR_20    "dtxth"                    // VESPERAS.WAD - ����
#define MLSTR_21    "djkz dthubkbz"            // VIRGIL.WAD   - ���� ��ࣨ���

//
// Final DOOM: ��ᯥਬ��� "���⮭��"
//

#define PHUSTR_1    "ehjdtym 1: rjyuj"                  // �஢��� 1: �����
#define PHUSTR_2    "ehjdtym 2: rjkjltw lei"            // �஢��� 2: ������� ���
#define PHUSTR_3    "ehjdtym 3: fwntr"                  // �஢��� 3: ��⥪
#define PHUSTR_4    "ehjdtym 4: pfgthnsq d rktnrt"      // �஢��� 4: ������� � ���⪥
#define PHUSTR_5    "ehjdtym 5: ujhjl-ghbphfr"          // �஢��� 5: ��த-�ਧࠪ
#define PHUSTR_6    "ehjdtym 6: kjujdj ,fhjyf"          // �஢��� 6: ������ ��஭�
#define PHUSTR_7    "ehjdtym 7: ldjh-kjdeirf"           // �஢��� 7: ����-����誠
#define PHUSTR_8    "ehjdtym 8: wfhcndj"                // �஢��� 8: ����⢮
#define PHUSTR_9    "ehjdtym 9: f,,fncndj"              // �஢��� 9: ������⢮
#define PHUSTR_10   "ehjdtym 10: yfnbcr"                // �஢��� 10: ����
#define PHUSTR_11   "ehjdtym 11: ghtcktletvsq"          // �஢��� 11: ��᫥�㥬�

#define PHUSTR_12   "ehjdtym 12: crjhjcnm"              // �஢��� 12: �������
#define PHUSTR_13   "ehjdtym 13: crktg"                 // �஢��� 13: �����
#define PHUSTR_14   "ehjdtym 14: pfhj;ltybt"            // �஢��� 14: ��஦�����
#define PHUSTR_15   "ehjdtym 15: cevthrb"               // �஢��� 15: �㬥ન
#define PHUSTR_16   "ehjdtym 16: ghtlpyfvtyjdfybt"      // �஢��� 16: �।������������
#define PHUSTR_17   "ehjdtym 17: rjvgfeyl"              // �஢��� 17: �����㭤
#define PHUSTR_18   "ehjdtym 18: ytqhjcathf"            // �஢��� 18: �������
#define PHUSTR_19   "ehjdtym 19: d>h>f>u>"              // �஢��� 19: �.�.�.�.
#define PHUSTR_20   "ehjdtym 20: j,bntkm cvthnb"        // �஢��� 20: ���⥫� �����

#define PHUSTR_21   "ehjdtym 21: e,bqwf"                // �஢��� 21: �����
#define PHUSTR_22   "ehjdtym 22: ytdsgjkybvjt pflfybt"  // �஢��� 22: ���믮������ �������
#define PHUSTR_23   "ehjdtym 23: yfluhj,bt"             // �஢��� 23: ����஡��
#define PHUSTR_24   "ehjdtym 24: gjcktlybq he,t;"       // �஢��� 24: ��᫥���� �㡥�
#define PHUSTR_25   "ehjdtym 25: [hfv nmvs"             // �஢��� 25: �ࠬ ���
#define PHUSTR_26   "ehjdtym 26: ,eyrth"                // �஢��� 26: �㭪��
#define PHUSTR_27   "ehjdtym 27: fynb[hbcn"             // �஢��� 27: �������
#define PHUSTR_28   "ehjdtym 28: rfyfkbpfwbz"           // �஢��� 28: ����������
#define PHUSTR_29   "ehjdtym 29: jlbcctz ievjd"         // �஢��� 29: ������ �㬮�
#define PHUSTR_30   "ehjdtym 30: dhfnf flf"             // �஢��� 30: ��� ���

#define PHUSTR_31   "ehjdtym 31: rb,thkjujdj"           // �஢��� 31: ����૮����
#define PHUSTR_32   "ehjdtym 32: blb r ctve"            // �஢��� 32: ��� � ᥬ�

//
// Final DOOM: TNT - ������
//

#define THUSTR_1    "ehjdtym 1: wtynh eghfdktybz cbcntvjq"      // �஢��� 1: ����� �ࠢ����� ��⥬��
#define THUSTR_2    "ehjdtym 2: ,fh,tr. bp xtkjdtxbys"          // �஢��� 2: ��ࡥ�� �� 祫���稭�
#define THUSTR_3    "ehjdtym 3: wtynh eghfdktybz gbnfybtv"      // �஢��� 3: ����� �ࠢ����� ��⠭���
#define THUSTR_4    "ehjdtym 4: lshf dj dhtvtyb"                // �஢��� 4: ��� �� �६���
#define THUSTR_5    "ehjdtym 5: dbctkbwf"                       // �஢��� 5: ��ᥫ��
#define THUSTR_6    "ehjdtym 6: jnrhsnsq ctpjy"                 // �஢��� 6: ������ ᥧ��
#define THUSTR_7    "ehjdtym 7: n.hmvf"                         // �஢��� 7: ���쬠
#define THUSTR_8    "ehjdtym 8: vtnfkk"                         // �஢��� 8: ��⠫�
#define THUSTR_9    "ehjdtym 9: rhtgjcnm"                       // �஢��� 9: �९����
#define THUSTR_10   "ehjdtym 10: bcregktybt"                    // �஢��� 10: ��㯫����
#define THUSTR_11   "ehjdtym 11: crkfl"                         // �஢��� 11: �����

#define THUSTR_12   "ehjdtym 12: rhfnth"                        // �஢��� 12: ����
#define THUSTR_13   "ehjdtym 13: gththf,jnrf zlthys[ jn[jljd"   // �஢��� 13: ���ࠡ�⪠ 拉��� ��室��
#define THUSTR_14   "ehjdtym 14: cnfktkbntqysq pfdjl"           // �஢��� 14: C⠫���⥩�� �����
#define THUSTR_15   "ehjdtym 15: vthndfz pjyf"                  // �஢��� 15: ���⢠� ����
#define THUSTR_16   "ehjdtym 16: uke,jxfqibt ljcnb;tybz"        // �஢��� 16: ��㡮砩訥 ���⨦����
#define THUSTR_17   "ehjdtym 17: pjyf j,hf,jnrb"                // �஢��� 17: ���� ��ࠡ�⪨
#define THUSTR_18   "ehjdtym 18: pfdjl"                         // �஢��� 18: �����
#define THUSTR_19   "ehjdtym 19: gjuheprf/jnghfdrf"             // �஢��� 19: ����㧪�/��ࠢ��
#define THUSTR_20   "ehjdtym 20: wtynhfkmysq geyrn j,hf,jnrb"   // �஢��� 20: ����ࠫ�� �㭪� ��ࠡ�⪨

#define THUSTR_21   "ehjdtym 21: flvbybcnhfnbdysq wtynh"        // �஢��� 21: ���������⨢�� 業��
#define THUSTR_22   "ehjdtym 22: j,bnfkbot"                     // �஢��� 22: ���⠫��
#define THUSTR_23   "ehjdtym 23: keyysq ujhysq ghjtrn"          // �஢��� 23: �㭭� ���� �஥��
#define THUSTR_24   "ehjdtym 24: rfhmth"                        // �஢��� 24: �����
#define THUSTR_25   "ehjdtym 25: kjujdj ,fhjyf"                 // �஢��� 25: ������ ��஭�
#define THUSTR_26   "ehjdtym 26: ,fkkbcnbrc"                    // �஢��� 26: ������⨪�
#define THUSTR_27   "ehjdtym 27: ujhf ,jkm"                     // �஢��� 27: ��� ����
#define THUSTR_28   "ehjdtym 28: xthnjdobyf"                    // �஢��� 28: ���⮢騭�
#define THUSTR_29   "ehjdtym 29: htrf cnbrc"                    // �஢��� 29: ���� �⨪�
#define THUSTR_30   "ehjdtym 30: gjcktlybq dspjd"               // �஢��� 30: ��᫥���� �맮�

#define THUSTR_31   "ehjdtym 31: afhfjy"                        // �஢��� 31: ��࠮�
#define THUSTR_32   "ehjdtym 32: rfhb,s"                        // �஢��� 32: ��ਡ�

#define HUSTR_CHATMACRO1    "ujnjd r ,j.!"                  // ��⮢ � ���!
#define HUSTR_CHATMACRO2    "z d gjhzlrt>"                  // � � ���浪�.
#define HUSTR_CHATMACRO3    "z hfyty!"                      // � ࠭��!
#define HUSTR_CHATMACRO4    "yf gjvjom!"                    // �� ������!
#define HUSTR_CHATMACRO5    "ns - jncnjq!"                  // �� - ���⮩!
#define HUSTR_CHATMACRO6    "d cktle.obq hfp< gjljyjr>>>"   // � ᫥���騩 ࠧ, �������...
#define HUSTR_CHATMACRO7    "c.lf!"                         // �!
#define HUSTR_CHATMACRO8    "z j, 'njv gjpf,jxecm>"         // � �� �⮬ ����������.
#define HUSTR_CHATMACRO9    "lf."                           // ��.
#define HUSTR_CHATMACRO0    "ytn>"                          // ���.

#define HUSTR_TALKTOSELF1   "ythfp,jhxbdjt ,jhvjnfybt>>>"   // ��ࠧ���稢�� ��ମ⠭��...
#define HUSTR_TALKTOSELF2   "rnj nfv?"                      // �� ⠬?
#define HUSTR_TALKTOSELF3   "xnj 'nj ,skj?"                 // �� �� �뫮?
#define HUSTR_TALKTOSELF4   "ds ,htlbnt>"                   // �� �।��.
#define HUSTR_TALKTOSELF5   "rfrfz ljcflf!"                 // ����� ��ᠤ�!

#define HUSTR_MESSAGESENT   "(cjj,otybt jnghfdktyj)"        // (ᮮ�饭�� ��ࠢ����)

// The following should NOT be changed unless it seems
// just AWFULLY necessary
//
// [JN] ������� ����室������ � ���� ���� :)

#define HUSTR_PLRGREEN  "uhby: "    // �ਭ:
#define HUSTR_PLRINDIGO "bylbuj: "  // ������:
#define HUSTR_PLRBROWN  ",hfey: "   // ���:
#define HUSTR_PLRRED    "htl: "     // ���:

#define HUSTR_KEYGREEN  'g'
#define HUSTR_KEYINDIGO 'i'
#define HUSTR_KEYBROWN  'b'
#define HUSTR_KEYRED    'r'

//
//	AM_map.C
//

#define AMSTR_FOLLOWON  "ht;bv cktljdfybz drk.xty"  // ����� ᫥������� ����祭
#define AMSTR_FOLLOWOFF "ht;bv cktljdfybz dsrk.xty" // ����� ᫥������� �몫�祭

#define AMSTR_GRIDON    "ctnrf drk.xtyf"            // ��⪠ ����祭�
#define AMSTR_GRIDOFF   "ctnrf jnrk.xtyf"           // ��⪠ �몫�祭�

#define AMSTR_MARKEDSPOT    "jnvtnrf"               // �⬥⪠
#define AMSTR_MARKSCLEARED  "jnvtnrb jxbotys"       // �⬥⪨ ��饭�

//
//	ST_stuff.C
//

#define STSTR_MUS       "cvtyf vepsrb"                              // ����� ��모
#define STSTR_NOMUS     "ds,jh ytdjpvj;ty"                          // �롮� ����������
#define STSTR_DQDON     "ytezpdbvjcnm frnbdbhjdfyf"                 // ���梨������ ��⨢�஢���
#define STSTR_DQDOFF    "ytezpdbvjcnm ltfrnbdbhjdfyf"               // ���梨������ ����⨢�஢���

#define STSTR_KFAADDED  ",jtpfgfc gjgjkyty"                         // �������� ��������
#define STSTR_FAADDED   ",jtpfgfc gjgjkyty (,tp rk.xtq)"            // �������� �������� (��� ���祩)

#define STSTR_NCON      "ghj[j;ltybt xthtp cntys frnbdbhjdfyj"      // ��宦����� �१ �⥭� ��⨢�஢���
#define STSTR_NCOFF     "ghj[j;ltybt xthtp cntys ltfrnbdbhjdfyj"    // ��宦����� �१ �⥭� ��ॠ�⨢�஢���

#define STSTR_CHOPPERS  ">>> yt lehcndtyyj - u>v>"                  // ... �� ����⢥��� - �.�.
#define STSTR_CLEV      "cvtyf ehjdyz>>>"                           // ����� �஢��...

#define STSTR_BEHOLD    "v=,tccv>< s=,thc>< i=ytd>< r=rjc>< a=rhn>< l=dbp>" // m=����., �=����., �=���., �=���., �=���., �=���.
#define STSTR_BEHOLDX   "ghtlvtn frnbdbhjdfy"                       // �।��� ��⨢�஢��
#define STSTR_BEHOLDZ   "ghtlvtn ltfrnbdbhjdfy"                     // �।��� ����⨢�஢��

#define STSTR_MASSACRE  "dct vjycnhs eybxnj;tys"                    // �� ������� 㭨�⮦���

//
//	F_Finale.C
//
#define E1TEXT \
"eybxnj;bd ,fhjyjd flf b pfxbcnbd keyye.\n"\
",fpe< ds ljk;ys ,skb gj,tlbnm< yt nfr kb?\n"\
"yt nfr kb? ult pfcke;tyyfz yfuhflf b\n"\
",bktn ljvjq? xnj 'nj pf xthnjdobyf?\n"\
"nfr yt ljk;yj ,skj dct pfrjyxbnmcz!\n"\
"\n" \
"'nj vtcnj gf[ytn rfr uybkjt vzcj< yj\n"\
"dsukzlbn rfr gjnthzyyfz ,fpf yf ltqvjct>\n"\
"gj[j;t< ds pfcnhzkb d ghb,ht;mt flf< b\n"\
"tlbycndtyysq genm ntgthm - ghjqnb tuj>\n"\
"\n"\
"xnj,s ghjljk;bnm gjuhe;tybt d buhe<\n"\
"ghjqlbnt 'gbpjl ^ghb,ht;mt flf^ b tuj\n"\
"pfvtxfntkmysq cbrdtk ^byathyj^!\n"

// ����⮦�� ��஭�� ��� � ����⨢ �㭭��
// ����, �� ������ �뫨 ��������, �� ⠪ ��?
// �� ⠪ ��? ��� ���㦥���� ���ࠤ� �
// ����� �����? �� �� �� ��⮢騭�?
// ��� �� ������ �뫮 �� ����������!
// 
// �� ���� ��孥� ��� ������ ���, ��
// �룫廊� ��� ����ﭭ�� ���� �� ������.
// ��宦�, �� �����﫨 � "�ਡ०� ���", �
// �����⢥��� ���� ⥯��� - �ன� ���.
//
// �⮡� �த������ ����㦥��� � ����,
// �ன��� ���� �ਡ०� ��� � ���
// �����⥫�� ᨪ��� "���୮"!

#define E2TEXT \
"e dfc gjkexbkjcm! e;fcysq kjhl-\n"\
"rb,thltvjy< ghfdzobq gjnthzyyjq\n"\
"ltqvjcjdcrjq ,fpjq< ,sk gjdth;ty\n"\
"b ds njh;tcndetnt! yj>>> ult ds?\n"\
"gjlj,hfdibcm r rhf. cgenybrf< ds\n"\
"j,hfoftnt dpjh dybp< xnj,s\n"\
"edbltnm e;fcye. ghfdle>\n" \
"\n"\
"ltqvjc gksdtn yfl cfvbv fljv!\n"\
"ds ybrjulf yt cksifkb< xnj,s rnj-yb,elm\n"\
"c,tufk bp flf< yj ds pfcnfdbnt e,k.lrjd \n"\
"gj;fktnm j njv< xnj jyb ghjcksifkb j dfc>\n"\
"gjcgtiyj j,dzpfdibcm dthtdrjq< ds\n"\
"cgecrftntcm yf gjdth[yjcnm flf>\n"\
"\n" \
"Ntgthm yfcnegftn dhtvz gjcktlytq ukfds\n"\
"- ^byathyj^>"

// � ��� ����稫���! ����� ���-
// ����थ���, �ࠢ�騩 ����ﭭ�� 
// �����ᮢ᪮� �����, �� ����থ� 
// � �� �থ����! ��... ��� ��? 
// �����ࠢ��� � ��� ��⭨��, ��
// ���頥� ���� ����, �⮡�  
// 㢨���� 㦠��� �ࠢ��. 
// 
// ������ ����� ��� ᠬ�� ����! 
// �� ������� �� ��蠫�, �⮡� ��-�����
// ᡥ��� �� ���, �� �� ���⠢�� 㡫��
// �������� � ⮬, �� ��� ����蠫� � ���.
// ��ᯥ譮 ���易���� ���񢪮�, �� 
// ��᪠���� �� �����孮��� ���.
//
// ������ ����㯠�� �६� ��᫥���� ����� 
// - "���୮". 

#define E3TEXT \
"jvthpbntkmysq gferjltvjy<\n"\
"herjdjlbdibq dnjh;tybtv yf keyyst\n"\
",fps b ghbytcibq cnjkm vyjuj \n"\
"cvthntq< ,sk jrjyxfntkmyj gjdth;ty>\n"\
"\n"\
"jnrhsdftncz ctrhtnyfz ldthm b ds\n"\
"d[jlbnt d ytt> ds ljrfpfkb< xnj\n"\
"ckbirjv rhens lkz flf< b gj'njve\n"\
"fl< yfrjytw< gjcnegftn cghfdtlkbdj -\n"\
"ds ds[jlbnt bp ldthb ghzvj yf\n"\
"ptktyst gjkz ptvkb> yfrjytw-nj ljv>\n" \
"\n"\
"ds cghfibdftnt ct,z: xnj ghjbc[jlbkj\n"\
"yf ptvkt< gjrf ds chf;fkbcm c \n"\
"dscdj,jlbdibvcz pkjv? [jhjij< xnj yb\n"\
"jlyj gjhj;ltybt flf yt cvjukj ghjqnb \n"\
"xthtp 'ne ldthm dvtcnt c dfvb>>>"

// ���২⥫�� ��㪮�����,
// �㪮�����訩 ��থ���� �� �㭭�
// ���� � �ਭ��訩 �⮫� �����
// ᬥ�⥩, �� �����⥫쭮 ����থ�.
//
// ���뢠���� ᥪ�⭠� ����� � ��
// �室�� � ���. �� ��������, ��
// ᫨誮� ����� ��� ���, � ���⮬�
// ��, �������, ����㯠�� �ࠢ������ -
// �� ��室�� �� ���� ��אַ ��
// ������ ���� �����. �������-� ���.
//
// �� ��訢��� ᥡ�: �� �ந�室���
// �� �����, ���� �� �ࠦ����� �
// ��᢮�����訬�� ����? ����, �� ��
// ���� ��஦����� ��� �� ᬮ��� �ன�
// �१ ��� ����� ����� � ����...

#define E4TEXT \
"gfer-ghtldjlbntkm< ljk;yj ,snm< gjckfk\n"\
"dgthtl cdjb ktubjys gjhj;ltybq flf\n"\
"gthtl dfibv gjcktlybv chf;tybtv c 'nbv\n"\
"e;fcysv jnhjlmtv> yj ds gjikb lj rjywf\n"\
"b chfpbkb dtxysv ghjrkznmtv b cnhflfybtv\n"\
"'ne jhle nfr< rfr gjcnegbk ,s yfcnjzobq\n"\
"uthjq gthtl kbwjv cvthntkmyjq jgfcyjcnb>\n"\
"\n"\
"rhjvt njuj< rnj-nj ljk;ty ,sk pfgkfnbnm\n"\
"pf nj< xnj ckexbkjcm c ltqpb< dfitq\n"\
"ljvfiytq rhjkmxb[jq>\n"\
"\n"\
"ntgthm ds dblbnt< crjkm vyjuj ,jkb b\n"\
"rhjdb dfv ghjhjxfn gjkxbof ltvjyjd<\n"\
"ytbcnde.ob[ d yfib[ ujhjlf[>\n"\
"\n"\
"cktle.ofz jcnfyjdrf - fl yf ptvkt!"

// ���-�।����⥫�, ������ ����, ��᫠� 
// ����� ᢮� ������� ��஦����� ��� ��।
// ��訬 ��᫥���� �ࠦ����� � �⨬ 
// 㦠�� ��த쥬. �� �� ��諨 �� ���� 
// � �ࠧ��� ���� �ப���쥬 � ��ࠤ����� 
// ��� ��� ⠪, ��� ����㯨� �� �����騩 
// ��ன ��। ��殬 ᬥ�⥫쭮� ���᭮᭮��.
//
// �஬� ⮣�, ��-� ������ �� �������� 
// �� �, �� ��稫��� � ���, ��襩
// ����譥� �஫��宩. 
// 
// ������ �� �����, ᪮�� ����� ���� �
// �஢� ��� ����� ����� �������, 
// ��������� � ���� ��த��.
//
// �������� ��⠭���� - �� �� �����!

// after level 6, put this:

#define C1TEXT \
"ds ghjikb duke,m gjhf;tyyjuj rjcvjgjhnf<\n" \
"yj xnj-nj pltcm ytghfdbkmyj> vjycnhs\n" \
"ghbytckb c cj,jq cj,cndtyye. htfkmyjcnm<\n" \
"b nt[yjkjubz rjcvjgjhnf \n" \
"nhfycajhvbhetncz jn b[ ghbcencndbz>\n" \
"\n"\
"dgthtlb ds dblbnt fdfygjcn flf>\n" \
"tckb dfv elfcncz ghj,hfnmcz xthtp ytuj<\n" \
"ds cvj;tnt ghjybryenm d yfctktyysq\n" \
"ltvjyfvb wtynh ,fps b yfqnb eghfdkz.obq\n" \
"dsrk.xfntkm< lth;fobq yfctktybt ptvkb\n" \
"d pfkj;ybrf[>"

// �� ��諨 ����� ��ࠦ񭭮�� ��ᬮ����,
// �� ��-� ����� ���ࠢ��쭮. �������
// �ਭ�᫨ � ᮡ�� ᮡ�⢥���� ॠ�쭮���,
// � �孮����� ��ᬮ���� 
// �࠭��ନ����� �� �� ������⢨�.
//
// ���।� �� ����� �������� ���.
// �᫨ ��� 㤠���� �஡����� �१ ����,
// �� ᬮ��� �஭������ � ��ᥫ�� 
// �������� 業�� ���� � ���� �ࠢ���騩 
// �몫��⥫�, ��ঠ騩 ��ᥫ���� ����� 
// � ����������.

// After level 11, put this:

#define C2TEXT \
"ds gj,tlbkb! dfif gj,tlf gjpdjkbkf\n" \
"xtkjdtxtcnde 'dfrebhjdfnmcz c ptvkb\n"\
"b cgfcnbcm jn rjivfhf> ntgthm ds -\n"\
"tlbycndtyysq xtkjdtr< jcnfdibqcz yf\n"\
"gkfytnt< b k.ljtls-venfyns< [boyst\n"\
"byjgkfytnzyt b pkst le[b - dfib\n"\
"tlbycndtyyst cjctlb> eljdktndjhtyysq\n"\
"ntv< xnj cgfckb cdjq dbl< ds cgjrjqyj\n"\
"lj;blftntcm ytvbyetvjq ub,tkb>\n"\
"\n"\
"yj dcrjht herjdjlcndj ptvkb gthtlftn\n"\
"cjj,otybt c jh,bns: ^ctycjhs j,yfhe;bkb\n"\
"bcnjxybr byjgkfytnyjuj dnjh;tybz> dfif\n"\
"pflfxf - kbrdblbhjdfnm tuj^> byjgkfytnyfz\n"\
",fpf yf[jlbncz d wtynht dfituj ujhjlf<\n"\
"ytlfktrj jn rjcvjgjhnf> vtlktyyj b\n"\
"vexbntkmyj ds djpdhfoftntcm d ,jq>\n"\

// �� ��������! ��� ������ ��������� 
// 祫������� ���஢����� � �����
// � ᯠ���� �� ��謠�. ������ �� - 
// �����⢥��� 祫����, ��⠢訩�� �� 
// ������, � ����-��⠭��, ��� 
// ���������ﭥ � ��� ��� - ���
// �����⢥��� �ᥤ�. ������⢮��� 
// ᯠᥭ��� ᢮��� ����, �� ᯮ�����
// ���������� �����㥬�� ������.
//
// �� �᪮� �㪮����⢮ ����� ��।���
// ᮮ�饭�� � �ࡨ��: "������ �����㦨��
// ���筨� ��������⭮�� ��থ���. ���
// ����� - �������஢��� ���". ��������⭠� 
// ���� ��室���� � 業�� ��襣� ��த�, 
// �������� �� ��ᬮ����. �������� � 
// ���⥫쭮 �� �����頥��� � ���.


// After level 20, put this:

#define C3TEXT \
"ds yf[jlbntcm d hfpkfuf.obvcz cthlwt\n"\
"ujhjlf< d jrhe;tybb nhegjd cdjb[ dhfujd>\n"\
"ds yt dblbnt ybrfrjuj cgjcj,f eybxnj;bnm\n"\
"gjhnfk yf 'njq cnjhjyt< b gj'njve< cnbcyed\n"\
"pe,s< ghj[jlbnt crdjpm ytuj>\n"\
"\n"\
"Ljk;ty ,snm cgjcj, pfrhsnm gjhnfk\n"\
"yf lheujq cnjhjyt> b rfrjt dfv ltkj lj\n"\
"njuj< xnj ghbltncz ghjqnb xthtp fl< xnj,s\n"\
"lj,hfnmcz lj 'njuj gjhnfkf?"

// �� ��室���� � ࠧ�����饬�� ���
// ��த�, � ���㦥��� ��㯮� ᢮�� �ࠣ��. 
// �� �� ����� �������� ᯮᮡ� 㭨�⮦���
// ���⠫ �� �⮩ ��஭�, � ���⮬�, ���� 
// ���, ��室�� ᪢��� ����. 

// ������ ���� ᯮᮡ ������� ���⠫ 
// �� ��㣮� ��஭�. � ����� ��� ���� ��
// ⮣�, �� �ਤ���� �ன� �१ ��, �⮡�
// �������� �� �⮣� ���⠫�?



// After level 29, put this:

#define C4TEXT \
";enxfqibq kbr cfvjuj ,jkmijuj ltvjyf<\n"\
"rjnjhjuj ds rjulf-kb,j dbltkb< heibncz\n"\
"yf dfib[ ukfpf[ gjckt njuj< rfr ds\n"\
"yfrfxfkb hfrtnfvb tuj ytpfobotyysq\n"\
"vjpu> vjycnh eufcftn b ub,ytn<\n"\
"hfpheifz ,tcxbcktyyst vbkb gjdth[yjcnb\n"\
"flf>\n"\
"\n"\
"ds cltkfkb 'nj> dnjh;tyb. rjytw> ptvkz\n"\
"cgfctyf> fl gjdth;ty> ds cghfibdftnt\n"\
"ct,z: relf ntgthm gjckt cvthnb ,elen\n"\
"gjgflfnm gkj[bt k.lb? enthtd gjn\n"\
"cj k,f< ds yfxbyftnt ljkujt gentitcndbt\n"\
"j,hfnyj ljvjq> djccnfyjdktybt ptvkb\n"\
"ljk;yj ,snm ujhfplj ,jktt bynthtcysv\n"\
"pfyznbtv< yt;tkb tt hfpheitybt>"

// ���砩訩 ��� ᠬ��� ����讣� ������,
// ���ண� �� �����-���� ������, ������
// �� ���� ������ ��᫥ ⮣�, ��� �� 
// ����砫� ࠪ�⠬� ��� �����饭��
// ����. ������ 㣠ᠥ� � ������, 
// ࠧ���� ����᫥��� ���� �����孮�� 
// ���.
//
// �� ᤥ���� ��. ��থ��� �����. ����� 
// ᯠᥭ�. �� ����থ�. �� ��訢��� 
// ᥡ�: �㤠 ⥯��� ��᫥ ᬥ�� ����
// �������� ���娥 �? ��ॢ ��� 
// � ���, �� ��稭��� ������ �����⢨�
// ���⭮ �����. ����⠭������� ����� 
// ������ ���� ��ࠧ�� ����� ������
// ����⨥�, ������ �� ࠧ��襭��. 


// Before level 31, put this:

#define C5TEXT \
"gjplhfdkztv< ds yfikb ctrhtnysq ehjdtym!\n"\
"gj[j;t< jy ,sk gjcnhjty k.lmvb< f yt\n"\
"ltvjyfvb> k.,jgsnyj< rnj ;t yfctkztn\n"\
"'njn eujkjr flf?\n"\

// ����ࠢ�塞, �� ��諨 ᥪ��� �஢���! 
// ��宦�, �� �� ����஥� ��쬨, 
// � �� ��������. ���⭮, �� �� ��ᥫ��
// ��� 㣮��� ���?

// Before level 32, put this:

#define C6TEXT \
"gjplhfdkztv< ds yfikb cdth[ctrhtnysq\n"\
"ehjdtym! kexit ,s dfv cnhtvbntkmyj\n"\
"ghjhdfnmcz crdjpm ytuj!"

// ����ࠢ�塞, �� ��諨 ᢥ��ᥪ��� 
// �஢���! ���� �� ��� ��६�⥫쭮
// ��ࢠ���� ᪢��� ����!

// after map 06	

#define P1TEXT  \
"ds cj pkjhflcndjv cvjnhbnt yf ujhzobq\n"\
"rfhrfc cnhf;f> c tuj cvthnm. ds dshdfkb\n"\
"ecrjhbntkm bp pkjdjyys[ rjuntq flf>\n"\
"hfcckf,bdibcm< ds jrblsdftnt dpukzljv\n"\
"gjvtotybt> ghjrkznmt! nen ljk;ty ,snm\n"\
"[jnz ,s jlby hf,jxbq ghjnjnbg< yj \n"\
"ltvjys< ljk;yj ,snm< pf,hfkb tuj c cj,jq>\n"\
"\n"\
"ds ljk;ys yfqnb ghjnjnbg< byfxt dct dfib\n"\
"ghjikst ecbkbz jrf;encz yfghfcysvb>\n"\
"ghjljk;fqnt ldbufnmcz< ghjljk;fqnt\n"\
"chf;fnmcz< ghjljk;fqnt e,bdfnm>\n"\
"b lf< ghjljk;fqnt ds;bdfnm>"

// �� � ���ࠤ�⢮� ᬮ��� �� ����騩 
// ��ઠ� ��ࠦ�. � ��� ᬥ���� �� ��ࢠ��
// �᪮�⥫� �� ��������� ���⥩ ���.
// ���᫠������, �� ����뢠�� ����冷�
// ����饭��. �ப����! ��� ������ ����
// ��� �� ���� ࠡ�稩 ���⨯, �� 
// ������, ������ ����, ���ࠫ� ��� � ᮡ��.
//
// �� ������ ���� ���⨯, ���� �� ���
// ���� �ᨫ�� �������� �����묨.
// �த������ ���������, �த������
// �ࠦ�����, �த������ 㡨����. � ��,
// �த������ �릨����.

// after map 11

#define P2TEXT \
"lf;t cvthntkmysq kf,bhbyn fhxdfqkjd yt\n"\
"cvju jcnfyjdbnm dfc> ds lj,hfkbcm lj\n"\
"ghjnjnbgf ecrjhbntkz< rjnjhsq dcrjht\n"\
",sk eybxnj;ty hfp b yfdctulf>\n"\
"\n"\
"eybxnj;tybt - dfif cgtwbfkmyjcnm>"

// ���� ᬥ�⥫�� ����ਭ� ��碠���� ��
// ᬮ� ��⠭����� ���. �� ���ࠫ��� ��
// ���⨯� �᪮�⥫�, ����� �᪮�
// �� 㭨�⮦�� ࠧ � ���ᥣ��.
//
// ����⮦���� - ��� ᯥ樠�쭮���.

// after map 20

#define P3TEXT \
"ds ghj,bkb b ghjrjkjnbkb cdjq genm d\n"\
"cthlwt lmzdjkmcrjuj ekmz> yfcnfkj dhtvz\n"\
"lkz vbccbb ^yfqnb b eybxnj;bnm^<\n"\
"j,]trnjv rjnjhjq cnfytn ghbdhfnybr<\n"\
"xmb ytxbcnbdst jnghscrb ybpdthuf.ncz\n"\
"yf ptvk.>\n"\
"\n"\
"lf< jy gkj[jq< yj ds pyftnt rnj tot [e;t!\n"\
"\n"\
"pkj,yj e[vskzzcm< ds ghjdthztnt cdjt\n"\
"cyfhz;tybt< b ujnjdbntcm ghtgjlfnm\n"\
"e,k.lre ytvyjuj flf dfituj\n"\
"cj,cndtyyjuj bpujnjdktybz!"

// �� �஡��� � �ப���⨫� ᢮� ���� �
// ��� ����᪮�� ���. ���⠫� �६� 
// ��� ���ᨨ "���� � 㭨�⮦���", 
// ��ꥪ⮬ ���ன �⠭�� �ਢ�⭨�, 
// �� ����⨢� ����᪨ �����࣠���� 
// �� �����. 
//
// ��, �� ���宩. �� �� ����� �� ��� �㦥!
//
// ������ �������, �� �஢���� ᢮� 
// ᭠�殮���, � ��⮢���� �९����� 
// 㡫� ������� ��� ��襣� 
// ᮡ�⢥����� ����⮢�����!

// after map 30

#define P4TEXT \
"gjckt eybxnj;tybz kbrf ghbdhfnybrf\n"\
"yfxbyf.n ajhvbhjdfnmcz bydthnbhjdfyyst\n"\
"dhfnf< rjnjhst pfcfcsdf.n d ct,z\n"\
"gjcktlybt j,kjvrb ecrjhbntkz b\n"\
"ytcrjkmrb[ jcnfdib[cz ltvjyjd>\n"\
"\n"\
"ujnjdj> fl dthyekcz yf rheub cdjz<\n"\
"gjukfofz kbim uhtiys[ k.ltq< yt;tkb\n"\
"ghfdtlys[>\n"\
"\n"\
"yt pf,elmnt gjghjcbnm dyerjd gjkj;bnm\n"\
"hfrtnybwe d dfi uhj,> Tckb gjckt cvthnb\n"\
"ds gjgfltnt d fl< jyf gjyflj,bncz\n"\
"lkz yt,jkmijq gjcktlytq pfxbcnrb>>>"

// ��᫥ 㭨�⮦���� ���� �ਢ�⭨��
// ��稭��� �ନ஢����� ������஢���� 
// ���, ����� ������� � ᥡ� 
// ��᫥���� ������� �᪮�⥫� � 
// ��᪮�쪨� ��⠢���� �������.
//
// ��⮢�. �� ������ �� ��㣨 ᢮�,
// ������� ���� ����� ��, ������ 
// �ࠢ�����.
//
// �� ������ ������� ��㪮� ��������
// ࠪ�⭨�� � ��� �஡. �᫨ ��᫥ ᬥ��
// �� ������� � ��, ��� �����������
// ��� ������让 ��᫥���� ����⪨...



// before map 31

#define P5TEXT \
"ds yfikb dnjhjq gj ckj;yjcnb ehjdtym<\n"\
"rjnjhsq e yfc tcnm> yflttvcz< ds\n"\
"cj[hfybkb buhe yf ghtlsleotv ehjdyt\n"\
"bkb tot hfymit> tckb ytn< ghbujnjdmntcm\n"\
"vyjuj evbhfnm>\n"\
"\n"\
"ehjdtym hfccxbnfy bcrk.xbntkmyj yf\n"\
"ghjatccbjyfkjd>"

// �� ��諨 ��ன �� ᫮����� �஢���,
// ����� � ��� ����. ��������, �� 
// ��࠭��� ���� �� �।��饬 �஢�� 
// ��� ��� ࠭��. �᫨ ���, �ਣ�⮢���� 
// ����� 㬨���.
//
// �஢��� �c��⠭ �᪫��⥫쭮 �� 
// ����ᨮ�����.

// before map 32

#define P6TEXT \
"cgjhbv< ds elbdkzkbcm< rfrjq ;t ehjdtym\n"\
"cfvsq ckj;ysq? ntgthm ds pyftnt>\n"\
"ybrnj yt ds,thtncz ;bdsv>"

// ���ਬ, �� 㤨��﫨��, ����� �� �஢���
// ����� ᫮���? ������ �� �����.
// ���� �� �롥���� ����.

// after map 06	

#define T1TEXT \
"chf;fzcm< ds ds,hfkbcm bp pfhf;tyys[\n"\
"'rcgthtvtynfkmys[ kf,jhfnjhbq>\n"\
"gj[j;t< jfr hfcnhfy;bhbkf b b[< ytcvjnhz\n"\
"yf juhjvyst rjhgjhfnbdyst lj[jls> jyb\n"\
"lf;t yt gjpf,jnbkbcm j gjregrt cnhf[jdjr\n"\
"lkz cdjb[ cjnhelybrjd>>>\n"\
"\n"\
"dgthtlb hfcgjkj;ty djtyysq rjvgktrc<\n"\
"rbifobq dbhecfvb b njkmrj ;leobq\n"\
"rfr ,s duhspnmcz d dfie gkjnm> xnj ;<\n"\
"tckb gjdtptn< d rjvgktrct dct tot\n"\
"ljk;ys jcnfdfnmcz ,jtghbgfcs>"

// �ࠦ����, �� ��ࠫ��� �� ��ࠦ���
// �ᯥ६��⠫��� ������਩.
// ��宦�, ��� ���࠭��ਫ� � ��, ��ᬮ���
// �� ��஬�� ��௮�⨢�� ��室�. ���
// ���� �� ������⨫��� � ���㯪� ���客��
// ��� ᢮�� ���㤭����...
//
// ���।� �ᯮ����� ������ ��������, 
// ���騩 ����ᠬ� � ⮫쪮 ���騩 
// ��� �� �������� � ���� �����. �� �,
// �᫨ �������, � �������� ��� ��� 
// ������ ��⠢����� ����ਯ���.

// after map 11

#define T2TEXT \
"dgthtlb cksity vtnfkkbxtcrbq crht;tn\n"\
"nz;tks[ vt[fybpvjd> ds edthtys< xnj jyb\n"\
"yt infvge.n jxthtlye. gfhnb.\n"\
"lmzdjkmcrb[ jnhjlbq< yj lf;t tckb 'nj\n"\
"b nfr< ds r 'njve ujnjds>\n"\
"\n"\
"'nb pderb vjuen jpyfxfnm jxthtlyjq\n"\
"rhjdfdsq atcnbdfkm< yj ds-nj e;t pyftnt<\n"\
"rfrjuj 'nj joeofnm nsczxb ujkjdjhtpjd<\n"\
"cj,hfyys[ d jlyjv ,tpevyjv e,bqwt>\n"\
"\n"\
"ds nfr ghjcnj yt clflbntcm>"

// ���।� ��襭 ��⠫���᪨� �०�� 
// ����� ��堭�����. �� 㢥७�, �� ���
// �� �⠬���� ��।��� ����� 
// ����᪨� ��த��, �� ���� �᫨
// � ⠪, �� � �⮬� ��⮢�. 
// �� ��㪨 ����� ������� ��।��� 
// �஢��� ��⨢���, �� ��-� 㦥 �����, 
// ������ �� ������ ����� ������१��, 
// ᮡ࠭��� � ����� ���㬭�� 㡨��.
//
// �� ⠪ ���� �� ᤠ�����.

#define T3TEXT \
"jnrhsdf.ofzcz gthcgtrnbdf dsukzlbn\n"\
"xthnjdcrb pyfrjvjq b gf[ytn \n"\
"ckjdyj pf;fhtyyst 'rcrhbvtyns>\n"\
"'nj vtcnj yt yhfdbkjcm dfv hfymit<\n"\
"b ds xthnjdcrb edthtys< xnj yt\n"\
"gjyhfdbncz b ctqxfc> xtv ,jkmit ds\n"\
"hfpvsikztnt yfl 'nbv< ntv gtxfkmytt\n"\
"dct cnfyjdbncz>\n"\
"\n"\
"dpdtcbd cdjt jhe;bt< ds pkjdtot\n"\
"e[vskztntcm> yfcnfkj dhtvz dcthmtp\n"\
"yflhfnm rjt-rjve pflybwe>"

// ���뢠����� ���ᯥ�⨢� �룫廊� 
// ��⮢᪨ �������� � ��孥� 
// ᫮��� ����७�� ��ਬ����. 
// �� ���� �� �ࠢ����� ��� ࠭��, 
// � �� ��⮢᪨ 㢥७�, �� �� 
// ���ࠢ���� � ᥩ��. ��� ����� �� 
// ࠧ����� ��� �⨬, ⥬ ��砫쭥� 
// ��� �⠭������.
// 
// ����ᨢ ᢮� ��㦨�, �� ������ 
// �������. ���⠫� �६� ���쥧 
// ������ ���-���� �������.

#define T4TEXT \
"dytpfgyj dct jrenfkjcm nbibyjq lj\n"\
"cfvjuj ujhbpjynf> fujybpbhe.ott flcrjt\n"\
"'[j cnb[kj< rjivfhyjt yt,j cnfkj dyjdm\n"\
"ujke,sv< nhegs vjycnhjd yfxfkb\n"\
"hfpkfufnmcz cj pkjdjyysv cvhfljv>\n"\
",j;t< yte;tkb ds b ghfdlf gj,tlbkb?\n"\
"\n"\
"gjckt rjhjnrjuj ptvktnhzctybz< bp\n"\
"hfpuhjvktyyjuj xthtgf bphsufntkz\n"\
"ltvjyjd yfxbyftn ghj,bdfnmcz zhrjt\n"\
"cbytt cdtxtybt>"

// �������� ��� ���⠫��� �設�� ��
// ᠬ��� ��ਧ���. ����������饥 ��᪮�
// �� ��嫮, ��謠୮� ���� �⠫� �����
// �����, ���� �����஢ ��砫� 
// ࠧ�������� � �������� �ࠤ��.
// ����, ��㦥�� �� � �ࠢ�� ��������?
//
// ��᫥ ���⪮�� ��������ᥭ��, ��
// ࠧ�஬������� �९� ���룠⥫� 
// ������� ��稭��� �஡������� �મ�
// ᨭ�� ᢥ祭��.


#define T5TEXT \
"b xnj ntgthm? dct dsukzlbn cjdthityyj\n"\
"gj-lheujve< ckjdyj gjrjb dkflsrb\n"\
"nenfy[jvjyf>\n"\
"\n"\
"xnj ;< xnj ,s yt j;blfkj dfc dyenhb<\n"\
"[e;t e;t ,snm yt vj;tn< yt nfr kb?\n"\
"bkb vj;tn kexit yt nhtdj;bnm\n"\
"cgzob[ ,jujd>>>"

// � �� ⥯���? ��� �룫廊� ᮢ��襭��
// ��-��㣮��, ᫮��� ����� ����모
// ��⠭堬���.
//
// �� �, �� �� �� ������� ��� �����,
// �㦥 㦥 ���� �� �����, �� ⠪ ��?
// ��� ����� ���� �� �ॢ����� 
// ���� �����...

#define T6TEXT \
"ghbikj dhtvz lkz jngecrf> tq-,jue<\n"\
"djhjif ytlhf flf< ds njkmrj j ytv\n"\
"b vtxnfkb! Gecnm rnj-yb,elm lheujq\n"\
"ntgthm ,jhtncz c gjkxbofvb ltvjyjd>\n"\
"\n"\
"dgthtlb hfcgjkj;ty nb[bq ujhjljr c\n"\
"ytcgtiyj ntreotq djljq< ghbxelkbdsvb\n"\
"ljvbrfvb< b< dthjznyj< yt yfctktyysq\n"\
"flcrbv jnhjlmtv>\n"\
"\n"\
"gjrbyed nhfycgjhnyjt chtlcndj<\n"\
"ds cksibnt njgjn ;tktpyjuj rjgsnf\n"\
"rb,thltvjyf>"

// ��諮 �६� ��� ���᪠. ��-����, 
// ���� ���� ���, �� ⮫쪮 � ���
// � ���⠫�! ����� ��-����� ��㣮� 
// ⥯��� ������ � ����頬� �������.
//
// ���।� �ᯮ�������� �娩 ��த�� �
// ��ᯥ譮 ⥪�饩 �����, ���㤫��묨
// ��������, �, ����⭮, �� ��ᥫ����
// ��᪨� ��த쥬.
//
// ������ �࠭ᯮ�⭮� �।�⢮,
// �� ���� ⮯�� ��������� ����� 
// ����थ����.


/******************************************** 

[JN] NRFTL text. Not used in DOS version. 

#define N1TEXT \
"ytghbznyjcnb< rfpfkjcm< yfcnbukb d cfvsq\n"\
"ytgjl[jlzobq vjvtyn>>>\n"\
"ytcvjnhz yf hfpuhjvyjt gjhf;tybt cbk\n"\
"flf< j,tpevtdibq rb,thltvjy htibk< xnj\n"\
"cvj;tn bpvtybnm [jl cj,snbq b dyjdm\n"\
"gjdthuyenm ptvk. d ltvjybxtcrbq [fjc>\n"\
"\n" \
"b vexbntkmyfz cvthnm cnfkf gkfnjq pf 'ne\n"\
"hjrjde. jib,re> cntys tuj j,bntkb tot\n"\
"ljkuj ,elen gjvybnm ghtlcvthnye. fujyb.\n"\
"'njuj gjhj;ltybz gjnecnjhjyyb[ cbk>\n"\
"b gecnm tuj uyb.obt jcnfyrb cnfyen\n" \
"dtxysv yfgjvbyfybtv lkz nt[< rnj dyjdm\n" \
"jcvtkbncz yfheibnm gjrjq yfituj vbhf>\n" \
"\n" \
"'nf vbccbz pfdthityf>"

// �����⭮��, ��������, ���⨣�� � ᠬ� 
// �����室�騩 ������...
// ��ᬮ��� �� ࠧ�஬��� ��ࠦ���� ᨫ 
// ���, ����㬥�訩 ����थ��� �訫, �� 
// ᬮ��� �������� 室 ᮡ�⨩ � ����� 
// ����࣭��� ����� � �������᪨� 堮�.
// 
// � ���⥫쭠� ᬥ��� �⠫� ���⮩ �� ��� 
// ப���� �訡��. �⥭� ��� ���⥫� ��� 
// ����� ���� ������� �।ᬥ���� ������ 
// �⮣� ��஦����� ������஭��� ᨫ.
// � ����� ��� ����騥 ��⠭�� �⠭��  
// ���� ������������ ��� ��, �� �����
// �ᬥ����� ������� ����� ��襣� ���.
// 
// �� ����� �����襭�.


[JN] The Master Levels. Not used.

#define M1TEXT \
"CONGRATULATIONS YOU HAVE FINISHED... \n\n"\
"THE MASTER LEVELS\n"

********************************************/

//
// F_FINALE.C
//

#define CC_ZOMBIE   "pjv,b"                 // �����
#define CC_SHOTGUN  "pjv,b-cth;fyn"         // �����-�ঠ��
#define CC_HEAVY    "gektvtnxbr"            // �㫥���稪
#define CC_IMP      ",tc"                   // ���
#define CC_DEMON    "ltvjy"                 // �����
#define CC_LOST     "gjnthzyyfz leif"       // ����ﭭ�� ���
#define CC_CACO     "rfrjltvjy"             // ���������
#define CC_HELL     "hswfhm flf"            // ����� ���
#define CC_BARON    ",fhjy flf"             // ��஭ ���
#define CC_ARACH    "fhf[yjnhjy"            // ��孮�஭
#define CC_PAIN     "'ktvtynfkm ,jkb"       // ������⠫� ����
#define CC_REVEN    "htdtyfyn"              // ��������
#define CC_MANCU    "vfyre,ec"              // �������
#define CC_ARCH     "fhxdfqk"               // ��碠��
#define CC_SPIDER   "gfer-ghtldjlbntkm"     // ���-�।����⥫�
#define CC_CYBER    "rb,thltvjy"            // ����थ���
#define CC_HERO     "yfi uthjq"             // ��� ��ன


// Port-specific strings
#define STSTR_VERSION   "dthcbz 1>7 (2019-08-04)"                   // ����� 1.7 (2019-08-04)

#define STSTR_KAADDED   "rk.xb gjkextys"                            // ���� ����祭�

#define STSTR_MLOOK_ON  "j,pjh vsim. drk.xty"                       // ����� ����� ����祭
#define STSTR_MLOOK_OFF "j,pjh vsim. dsrk.xty"                      // ����� ����� �몫�祭

#define STSTR_CROSSHAIR_ON  "ghbwtk drk.xty"                        // ��楫 ����祭
#define STSTR_CROSSHAIR_OFF "ghbwtk dsrk.xty"                       // ��楫 �몫�祭


#endif
