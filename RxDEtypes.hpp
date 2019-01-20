 #include "DayTime.hpp"
#include "Xvt.hpp"
#include "EngineFramework.hpp"

#ifndef rxDETYPES
#define rxDETYPES

using namespace gpstk;

// These will be defined as public data members of the Engine

// This defines all datamembers which is fed in runtime: Active Input
typedef struct
{
DayTime   trueGPStime;
}rxDE_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
bool useRxclkModel;

}rxDE_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
// Possibly adding the taps vector

}rxDE_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
bool debug;
double IF1mult;
double IF2mult;
double rxConstClk;

}rxDE_passiveParam;

 
typedef struct 
{
DayTime		trueGPStime;
DayTime		rx_time;
double 		rxClkBias;
double 		rxClkDrift;
/*Formatted String for output*/
std::string opDataHeader;
std::string opDataInString;


}rxDE_opDataPort;

typedef struct
{
public :
EngineFramework::engineHealth rxDE_health;
}rxDE_opHealthPort;


#endif