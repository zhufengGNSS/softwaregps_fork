#ifndef CARRGENTYPES
#define CARRGENTYPES

using namespace gpstk;
//# include "Stats.hpp"
#include "SatDgen.hpp"
 #include "DayTime.hpp"
 #include "Position.hpp"
 # include "Xvt.hpp"
  #include "GlobalMac.hpp"
#include "EngineFramework.hpp"
#include <boost/circular_buffer.hpp>
   #include <assert.h>







// These will be defined as public data members of the Engine

// This defines all datamembers which is fed in runtime: Active Input
typedef struct
{
SatID   satid;
//double initCycles;

double initCoarsePhase;
double initCarrierFinePhase;
double rxClock;
double txCodeClock;
double  satCarrClk;
}carrGen_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
bool useRxclkModel;
bool generateComplex;

}carrGen_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
// Possibly adding the taps vector

}carrGen_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
bool debug;
double IF1mult;
double IF2mult;
double rxConstClk;

}carrGen_passiveParam;




typedef struct
{
public :
SatID  prn;
double   firstCycle_I[CARR_CYCLCE_RESOLUTION];
std::vector<double> lastCycle_I;
double   firstCycle_Q[CARR_CYCLCE_RESOLUTION];
std::vector<double> lastCycle_Q;
double startIndex;

//int   firstBlockSlew;
double   residueCoarsePhase;
double totalCompleteCycles;
double residueFinePhase;
double rxIF;

}carrGen_opDataPort;


typedef struct
{
public :
	EngineFramework::engineHealth carrGen_health;
}carrGen_opHealthPort;

typedef struct
{
//boost::circular_buffer<char> codeArray;
boost::circular_buffer<int> codeArray;
int prvSlew;
double prvSlewPhase;
}carrGen_caCodeArray;

typedef struct
{
//boost::circular_buffer<char> codeArray;
boost::circular_buffer<double> carrArray;
int prvCoarsePhase;
double prvCarrierFinePhase;
}carrGen_carrierArray;

#endif

