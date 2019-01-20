using namespace gpstk;
//# include "Stats.hpp"

 #include "DayTime.hpp"
 #include "Position.hpp"
 # include "Xvt.hpp"
#include "EngineFramework.hpp"
#include "SatDb.hpp"

#ifndef SATDGENTYPES
#define SATDGENTYPES



// These will be defined as public data members of the Engine

// This defines all datamembers which is fed in runtime: Active Input
typedef struct
{

SatID   satid;
DayTime measTime;
Position rxPos;
double calculatedRange;
SatDb *satDb;

}satDgen_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
bool firstMeas;
bool svRange;

}satDgen_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
// if there is no passive input for a particular engine
// Set flag psvInput to false
bool psvInputChk;


}satDgen_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
int debugLevel;
}satDgen_passiveParam;




typedef struct
{
public :
SatID  prn;
double pseudoRange;
double rawrange;
double svclkbias;
double svclkdrift;
double relativity;
double sagnac;
double  relativity2;
double elevation;
double azimuth;
double elevationGeodetic;
double azimuthGeodetic;
DayTime transmit;
Triple cosines;
Xvt svPosVel;
/*Formatted String for output*/
std::string opDataHeader;
std::string opDataInString;

}satDgen_opDataPort;


typedef struct
{
public :
	EngineFramework::engineHealth satDgen_health;
}satDgen_opHealthPort;


#endif
