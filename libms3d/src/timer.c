
/*
	timer.c - Timer abstract base class.

*/


#include <assert.h>
#include <stdlib.h>

#include "timer.h"
#include "Win32Timer.h"


Timer *Timer_getNewTimer () {
	Win32Timer_new *wt = Win32Timer_new();
	return wt->base;
}


Timer *Timer_new () {
	Timer *t = malloc(sizeof(Timer));
	TimerLink *pLink = malloc(sizeof(TimerLink));
	pLink->m_pTimer = t;
	pLink->m_pNext = t->m_pTimerList;
	t->m_pTimerList = pLink;
	t->pause = &Win32Timer_pause;
	t->unpause = &Win32Timer_unpause;
	t->getNewTimer = (void *) &Win32Timer_new;
	t->getTime = &Win32Timer_getTime;
	t->getNewTimer = &Timer_getNewTimer;
	return t;
}




void Timer_destroy (Timer *t) {
	TimerLink *pLink = t->m_pTimerList, *pPrev = NULL;
	while (pLink != NULL) {
		if ( pLink->m_pTimer == t ) {
			if (pPrev == NULL)
				t->m_pTimerList = pLink->m_pNext;
			else
				pPrev->m_pNext = pLink->m_pNext;

			free(pLink);
			break;
		}
		pPrev = pLink;
		pLink = pLink->m_pNext;
	}
	assert(pLink != NULL);
}

void Timer_pauseAll (Timer *t) {
	TimerLink *pLink = t->m_pTimerList;
	while (pLink != NULL) {
		pLink->m_pTimer->pause(pLink->m_pTimer);
		pLink = pLink->m_pNext;
	}
}

void Timer_unpauseAll (Timer *t) {
	TimerLink *pLink = t->m_pTimerList;
	while (pLink != NULL) {
		pLink->m_pTimer->unpause(pLink->m_pTimer);
		pLink = pLink->m_pNext;
	}
}







