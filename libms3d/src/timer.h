/*
	Timer.h

		Timer abstract base class.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 18 July 2000
	Last Edited: 6 February 2001

	Please see the file ChangeLog.html for a revision history.
*/

#ifndef _TIMER_H
#define _TIMER_H


#include "Win32Timer.h"
#include "typedefs.h"



#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Timer {
	struct _TimerLink {
		struct _Timer *m_pTimer;
		struct _TimerLink *m_pNext;
	};
	
	struct _TimerLink *m_pTimerList;
	
	void (*pause)();
	void (*unpause)();
	void (*reset)();
	double (*getTime)();
	
	struct _Win32Timer *(*getNewTimer)();
} Timer;

typedef struct _TimerLink TimerLink;


/*
typedef struct _TimerLink {
	struct _Timer *m_pTimer;
	struct _TimerLink *m_pNext;
} TimerLink;

typedef struct _Timer {
	struct _TimerLink *m_pTimerList;

	
	void (*pause)();
	void (*unpause)();
	void (*reset)();
	double (*getTime)();
	
	struct _Win32Timer *(*getNewTimer)();
} Timer;
*/



struct _Win32Timer *Timer_getNewTimer ();






#ifdef __cplusplus
}
#endif



#endif


