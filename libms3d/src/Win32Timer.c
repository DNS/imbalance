



#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Win32Timer.h"

#ifndef _WIN32
#	error	This file should only be used in a Microsoft Windows build.
#endif

// Disable warning about non-standard extension in Microsoft's header files
//#pragma warning( disable : 4201 )
#include <mmsystem.h>
//#pragma warning( default : 4201 )
//#pragma comment( lib, "winmm.lib" )



Win32Timer *Win32Timer_new () {
	Win32Timer *s = malloc(sizeof(Win32Timer));
	s->base = Timer_new();
	if (QueryPerformanceFrequency((LARGE_INTEGER *) &s->m_frequency) == FALSE ) {
		// No performance counter available
		s->m_usePerformanceCounter = FALSE;

		s->m_mmTimerStart = timeGetTime();
	} else {
		// Performance counter is available, use it instead of the multimedia timer
		s->m_usePerformanceCounter = TRUE;
		
		// Get the current time and store it in pcTimerStart
		QueryPerformanceCounter(( LARGE_INTEGER *) &s->m_pcTimerStart);
		
		// Calculate the timer resolution using the timer frequency
		s->m_resolution = (float) ( 1.0 / (double) s->m_frequency ) * 1000.0f;
	}
	
	s->m_pauseCount = 0;
	s->m_pauseTime = 0;
	
	s->getTime = &Win32Timer_getTime;
	return s;
}



void Win32Timer_reset (Win32Timer *s) {
	if (s->m_usePerformanceCounter == TRUE) {
		QueryPerformanceCounter(( LARGE_INTEGER *) &s->m_pcTimerStart);
	} else {
		s->m_mmTimerStart = timeGetTime();
	}
	s->m_pauseTime = Win32Timer_getTime(s);
}

double Win32Timer_getTime (Win32Timer *s) {
	if (s->m_pauseCount > 0)
		return s->m_pauseTime;

	__int64 timeElapsed;

	if (s->m_usePerformanceCounter == TRUE) {
		QueryPerformanceCounter(( LARGE_INTEGER *) &timeElapsed);
		timeElapsed -= s->m_pcTimerStart;
		return (timeElapsed * s->m_resolution);
	} else {
		timeElapsed = timeGetTime() - s->m_mmTimerStart;
		return (double) timeElapsed;
	}
}

void Win32Timer_pause (Win32Timer *s) {
	if (s->m_pauseCount == 0) {
		s->m_pauseTime = s->getTime(s);
	}
	s->m_pauseCount++;
}

void Win32Timer_unpause (Win32Timer *s) {
	assert(s->m_pauseCount > 0);
	s->m_pauseCount--;
	
	if (s->m_pauseCount == 0 ) {
		if (s->m_usePerformanceCounter == TRUE ) {
			__int64 time;
			QueryPerformanceCounter(( LARGE_INTEGER* ) &time);
			s->m_pcTimerStart = time - (__int64) (s->m_pauseTime / s->m_resolution);
		} else {
			s->m_mmTimerStart = (dword) (timeGetTime() - s->m_pauseTime );
		}
	}
}


void Win32Timer_destroy (Win32Timer *s) {
	free(s->base);
	free(s);
}






