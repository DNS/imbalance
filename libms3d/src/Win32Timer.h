/*
	Win32Timer.h

		Timer class under Microsoft Windows.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 18 July 2000
	Last Edited: 19 December 2000

	Please see the file ChangeLog.html for a revision history.
*/

#ifndef _WIN32TIMER_H
#define _WIN32TIMER_H

#ifndef _WIN32
#	error	This file should only be used in a Microsoft Windows build.
#endif


#include <windows.h>

#include "typedefs.h"
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Win32Timer {
	// base inheritance
	Timer *base;
	
	// private attr
	__int64 m_frequency;
	float m_resolution;
	dword m_mmTimerStart;
	__int64 m_pcTimerStart;
	BOOL m_usePerformanceCounter;
	int m_pauseCount;
	double m_pauseTime;
	
	// public method
	void (*Win32Timer_new)();
	void (*reset)();
	double (*getTime)();
	void (*pause)();
	void (*unpause)();
} Win32Timer;


Win32Timer *Win32Timer_new ();
double Win32Timer_getTime (Win32Timer *);
void Win32Timer_pause (Win32Timer *);
void Win32Timer_unpause (Win32Timer *);





#ifdef __cplusplus
}
#endif 


#endif	// ndef PORTALIB3D_WIN32TIMER_H


