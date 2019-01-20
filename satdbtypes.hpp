using namespace gpstk;
//# include "Stats.hpp"

 #include "DayTime.hpp"
 #include "Position.hpp"
 # include "Xvt.hpp"
#include "EngineFramework.hpp"


#ifndef SATDBTYPES
#define SATDBTYPES



// These will be defined as public data members of the Engine

// This defines all datamembers which is fed in runtime: Active Input
typedef struct
{
DayTime timeForSatPos;
SatID   satid;

}satDb_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
}satDb_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
string yumaFileName;

}satDb_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
bool useFullGpsWeek;
}satDb_passiveParam;




typedef struct
{
public :
SatID    prn;
bool	 sathealth;
DayTime  toa;
double   af0;
double   af1;
double   clkCorrection;
//double   relativity;
Xvt      satXvt;
}satDb_opDataPort;


typedef struct
{
public :
	EngineFramework::engineHealth satDb_health;
}satDb_opHealthPort;

#endif
