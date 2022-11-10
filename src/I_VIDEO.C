//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 2021 by bcc2528
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for FM TOWNS Native Version.
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <msdos.cf>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"
#include "doomdef.h"
#include "z_zone.h"
#include "w_wad.h"


//High C Lib for FM TOWNS Graphics
#include <EGB.H>

char work[EgbWorkSize] ;

int reg_ds;


//
// I_InitforConsole
//
void I_InitforConsole()
{
    EGB_resolution( work, 0, 1 );
    EGB_resolution( work, 1, 1 );

    EGB_displayPage(work, 0, 3 );
    EGB_writePage( work, 0 );

    EGB_color( work, 0, 0x00 );
    EGB_clearScreen( work );
}

//
// I_SetPalette
// Palette source must use 8 bit RGB elements.
//
void I_SetPalette(byte *palette)
{
    int i;
    int a;
    char para[2064];
    char r, g, b;

    if(novideo)
    {
    	return;
    }


    DWORD(para + 0) = 256;
    for(i = 0 ; i < 256; i++)
    {
    	a = (i * 8) + 4;
    	r = gammatable[usegamma][*palette++];
    	g = gammatable[usegamma][*palette++];
    	b = gammatable[usegamma][*palette++];
    	DWORD(para + a) = i;
    	BYTE(para  + a + 4) = b;
    	BYTE(para  + a + 5) = r;
    	BYTE(para  + a + 6) = g;
    	BYTE(para  + a + 7) = 0;
    }
		
    EGB_palette (work, 0, para);

}

//
// I_VSync
//
void I_VSync()
{
	char para[16];

	if(novideo)
	{
		return;
	}

	DWORD(para) = 0;
	EGB_palette (work, 1, para);
}

//
// I_StartFrame
//
void I_StartFrame (void)
{
    I_VSync();
}

//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
    // Not use in FM TOWNS version.
}

//
// I_FinishUpdate
//
void I_FinishUpdate (void)
{
    static int	lasttic;
    int		tics;
    int		i;
    char    para[16];
    //static int page = 0;
    //static int x = 0;

    // UNUSED static unsigned char *bigscreen=0;

    if(novideo)
    {
    	return;
    }

    // draws little dots on the bottom of the screen
    if (devparm)
    {
        i = I_GetTime();
        tics = i - lasttic;
        lasttic = i;
        if (tics > 20) tics = 20;

        for (i = 0; i < tics*2; i+=2)
        {
            screens[0][ (SCREENHEIGHT-1) * SCREENWIDTH + i] = 0xff;
        }
        for (; i < 20*2; i+=2)
        {
            screens[0][ (SCREENHEIGHT-1) * SCREENWIDTH + i] = 0x0;
        }

    }

	//Page Flip
	/*page = 1 - page;
	
	if(page)
	{
		x = 320;
	}else
	{
		x = 0;
	}*/

	//Write in screens[0] to VRAM
    DWORD( para+0  ) = screens[0];
    WORD( para+4  ) = reg_ds;
    WORD( para+6  ) = 0;
    WORD( para+8  ) = 19;
    WORD( para+10 ) = SCREENWIDTH - 1;
    WORD( para+12 ) = SCREENHEIGHT + 18;
    EGB_putBlock( work, 0, para );


    //DWORD( para+0  ) = screens[0];
    //WORD( para+4  ) = reg_ds;
    //WORD( para+6  ) = x;
    //WORD( para+8  ) = 19;
    //WORD( para+10 ) = x + SCREENWIDTH - 1;
    //WORD( para+12 ) = SCREENHEIGHT + 18;
    //EGB_putBlock( work, 0, para );

    //Display the current page
    //EGB_displayStart(work, 0, x, 0);

}

//
// I_ReadScreen
//
void
I_ReadScreen
(byte *scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}

//
// I_InitGraphics
//
void I_InitGraphics(void)
{
    if (novideo)
    {
        return;
    }
	

    //Screen set Mode12(640*480,256 Colors, 1 Page only)
    EGB_resolution( work, 0, 12 );

    ///x2 zoom, 640*480->320*240
    EGB_displayStart( work, 2, 2, 2 );
    EGB_displayStart( work, 0, 0, 0 );
    //EGB_displayStart( work, 3, 320, 240);

    EGB_displayPage( work, 0, 3 );
    EGB_writePage( work, 0 );

    EGB_color( work, 0, 0x00 );
    EGB_clearScreen( work );

    I_SetPalette(W_CacheLumpName("PLAYPAL", PU_CACHE));

    reg_ds = getds();
}

//
// I_ShutdownGraphics
//
void I_ShutdownGraphics(void)
{
    if (novideo)
    {
        return;
    }

    EGB_init( work, EgbWorkSize );
}