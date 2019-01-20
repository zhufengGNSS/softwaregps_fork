#include "timer.hpp"


void * Timer_class::pObject;

VOID CALLBACK Timer_class::TimerProc_Wrapper( HWND hwnd, UINT uMsg, 
                                           UINT idEvent, DWORD dwTime ) {
 Timer_class *pSomeClass = (Timer_class*)pObject; // cast the void pointer

 pSomeClass->TimerProc(hwnd, uMsg, idEvent, dwTime); // call non-static function

}


VOID CALLBACK Timer_class::TimerProc( HWND hwnd, 
                       UINT uMsg, UINT idEvent, DWORD dwTime )
{
	
	
	{time_expired = true;
	}
}
void Timer_class ::kill_timer()
{
 KillTimer(NULL, m_timerID);
}
void Timer_class :: timerset(UINT timerms)
{
pObject = this; 
Timer_class *pSomeClass = (Timer_class*)pObject;	
	
	timer_ms =timerms ;
m_timerID = SetTimer(NULL, 0, 1000,TimerProc_Wrapper );

}



