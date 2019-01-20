#ifndef TIMER_CLASS
#define TIMER_CLASS
#include <windows.h>


class Timer_class
{
public : 
		
	void timerset(UINT timerms);
	void kill_timer();
	bool time_expired;
	static VOID CALLBACK TimerProc_Wrapper( HWND hwnd, UINT uMsg, 
                                  UINT idEvent, DWORD dwTime );
	VOID CALLBACK TimerProc( HWND hwnd, 
                       UINT uMsg, UINT idEvent, DWORD dwTime );
	
private :
	static void * pObject;

	UINT_PTR m_timerID;
	int timer_ms;

};


#endif


