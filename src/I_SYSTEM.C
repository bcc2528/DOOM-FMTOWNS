// Emacs style mode select   -*- C++ -*- 
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
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"

#include "d_net.h"
#include "g_game.h"

////High C Lib for FM TOWNS Keyboard scan
#include <fmcfrb.h>

//HIS Lib for VSync interrupt(from oh!FM TOWNS August 1995 - Appendix CD)
#include <his.h>

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"

byte	keyboard_scan[128] = {	0,KEY_ESCAPE,'1','2','3','4','5','6',	  /* 00`07 */
				'7','8','9','0','-','^','@',KEY_BACKSPACE,  /* 08`0F */
				KEY_TAB,'q','w','e','r','t','y','u',    /* 10`17 */
				'i','o','p',0,'[',KEY_ENTER,'a','s', 	  /* 18`1F */
				'd','f','g','h','j','k','l',';',	  /* 20`27 */
				':',']','z','x','c','v','b','n',	  /* 28`2F */
				'm',',','.','/','\\',' ','*','/',	/* 30`37 */
				'+','-','7','8','9','=','4','5',/* 38`3F */
				'6',0,'1','2','3',KEY_ENTER,'0','.',	/* 40`47 */
				0,0,0,0,0,KEY_UPARROW,0,KEY_LEFTARROW,	/* 48`4F */
				KEY_DOWNARROW,KEY_RIGHTARROW,KEY_RCTRL,KEY_RSHIFT,0,0,0,0,	/* 50`57 */
				0,0,0,KEY_F12,KEY_RALT,KEY_F1,KEY_F2,KEY_F3,		/* 58`5F */
				KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,0,	  /* 60`67 */
				KEY_RSHIFT,KEY_F11,0,0,0,0,0,0,	  /* 68`6F */
				0,0,0,0,0,0,0,0,	 	  /* 70`77 */
				0,0,0,0,KEY_ESCAPE,0,0,0		  /* 78`7F */
};

#define stackSize 1000
char EGB_stack[ stackSize ];

int	mb_used = 6;

#define VSYNCclear 0x05ca
#define VSYNCintNumber 11

int ticcount;
byte* mem_zone;

void I_ReadMouse(void);

void I_StartScreen()
{
	I_InitforConsole();
	VDB_init();
}

void VSYNChandler( void )
{
	static int VSyncfrag = 1;

	VSyncfrag = 1 - VSyncfrag;

	if(VSyncfrag)
		ticcount++;

	/******** ‚u‚r‚x‚m‚bŠ„‚èž‚ÝŒ´ˆöƒNƒŠƒAƒŒƒWƒXƒ^‚Ö‚Ì‘‚«ž‚Ý ********/
	_outb( VSYNCclear, 0 );
}

void I_Tactile ( int	on, int	off, int total )
{
  // UNUSED.
  on = off = total = 0;
}

ticcmd_t	emptycmd;
ticcmd_t*	I_BaseTiccmd(void)
{
    return &emptycmd;
}


int  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{

	//8MB
    *size = 8*1024*1024;
	mem_zone = (byte *) malloc (*size);
	if(mem_zone!=NULL)
	{
		printf("I_ZoneBase: Secure 8MB in Main Memory for Zone Memory\n");
		return mem_zone;
	}

	//6MB
    *size = 6*1024*1024;
	mem_zone = (byte *) malloc (*size);
	if(mem_zone!=NULL)
	{
		printf("I_ZoneBase: Secure 6MB in Main Memory for Zone Memory\n");
		return mem_zone;
	}

	//4MB
	*size = 4*1024*1024;
	mem_zone = (byte *) malloc (*size);
	if(mem_zone!=NULL)
	{
		printf("I_ZoneBase: Secure 4MB in Main Memory for Zone Memory\n");
		return mem_zone;
	}

	printf("I_ZoneBase: Insufficient Memory.\nYou need to have at least free 6 MB Main Memory for FM Towns DOOM.\n");
	exit(-1);

	//return (byte *) malloc (*size);
}

//
// I_GetTime
// Returns time in 1/30th second tics.
//
int I_GetTime(void)
{
	return ticcount;
}

//
// I_WaitVBL
//
void I_WaitVBL(int vbls)
{
    //if (novideo)
    //{
    //    return;
    //}

    while (vbls > 0)
    {
        I_VSync();
		vbls--;
    }
}

// draw disk icon
void I_BeginRead(void)
{

}

// erase disk icon
void I_EndRead(void)
{

}

//
// I_StartTic
//
void I_StartTic (void)
{
    int k;
    event_t ev;
    int		bit;
    unsigned int    keyscan, encode; 

    I_ReadMouse();

    //
    // keyboard events
    //

    while(1)
    {
		keyscan = KYB_read( 1, &encode );

		if ( (keyscan >> 8 ) == 0xff )
		{
			break;
		}
		else if( (keyscan >> 7 ) == 0)
		{
			ev.type = ev_keydown;
		}
		else
		{
			ev.type = ev_keyup;
		}

		ev.data1 = keyboard_scan[keyscan & 127];
		D_PostEvent(&ev);
	
    }

}


//
// I_ReadMouse
//
void I_ReadMouse(void)
{
    return;
}

//
// I_Init
//
void  I_Init (void)
{
	printf("I_Init: Initialize FM Towns System Interface\n");

    //Keyboard Interface Initialize for Scan mode.
    KYB_init();
    KYB_setcode( 0x4000 );
    KYB_clic ( 1 );

    I_InitSound ();

    ticcount = 0;

	HIS_stackArea( EGB_stack , stackSize );
	HIS_setHandler( VSYNCintNumber , VSYNChandler );
	HIS_enableInterrupt( VSYNCintNumber );

}

//
// I_Quit
//
void I_Quit (void)
{
    D_QuitNetGame ();
    I_ShutdownSound();
    I_ShutdownMusic();
    M_SaveDefaults ();
    I_ShutdownGraphics();

    KYB_clrbuf();
    KYB_init();
    KYB_clic ( 1 );

    HIS_detachHandler( VSYNCintNumber );

    free(mem_zone);

    I_StartScreen();

    exit(0);
}

byte*	I_AllocLow(int length)
{
    byte*	mem;
        
    mem = (byte *)malloc (length);
    memset (mem,0,length);
    return mem;
}

//
// I_Error
//
extern boolean demorecording;

void I_Error (char *error, ...)
{
    va_list	argptr;

    I_ShutdownGraphics();

    I_ShutdownSound();

    I_StartScreen();

    // Message first.
    va_start (argptr,error);
    fprintf (stderr, "Error: ");
    vfprintf (stderr,error,argptr);
    fprintf (stderr, "\n");
    va_end (argptr);

    fflush( stderr );

    // Shutdown. Here might be other errors.
    if (demorecording)
	G_CheckDemoStatus();

    D_QuitNetGame ();

    KYB_clrbuf();
    KYB_init();
    KYB_clic ( 1 );

    HIS_detachHandler( VSYNCintNumber );

    free(mem_zone);
    
    exit(-1);
}
