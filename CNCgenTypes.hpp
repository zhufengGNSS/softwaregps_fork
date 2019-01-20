using namespace gpstk;
//# include "Stats.hpp"
#include "SatDgen.hpp"
 #include "DayTime.hpp"
 #include "Position.hpp"
 # include "Xvt.hpp"
#include "EngineFramework.hpp"
#include <boost/circular_buffer.hpp>
   #include <assert.h>



#ifndef cncGentypes
#define cncGentypes



// These will be defined as public data members of the Engine

// This defines all datamembers which is fed in runtime: Active Input
typedef struct
{
DayTime timeForSatPos;
SatID   satid;
SatDgen  *satDgen;
Position rx;



}cncGen_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
}cncGen_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
string yumaFileName;
double alpha[4];
double beta[4];


}cncGen_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
bool useFullGpsWeek;
bool useExtIono;
}cncGen_passiveParam;




typedef struct
{
public :
SatID  prn;
int   firstBlock[1023];
int   firstBlockSlew;
std::vector<int> lastblock;
int   totalCompleteCodeBlocks;
double lastBlockPhase;
int    lastBlockSlew;

double   firstCycle[1023];
long   firstCycleCoarsePhase;
std::vector<double> lastCycle;
double   totalCompleteCycles;
double lastCycleFinePhase;
long   lastCycleCoarsePhase;

}cncGen_opDataPort;


typedef struct
{
public :
	EngineFramework::engineHealth cncGen_health;
}cncGen_opHealthPort;

typedef struct
{
//boost::circular_buffer<char> codeArray;
boost::circular_buffer<int> codeArray;
int prvSlew;
double prvSlewPhase;
}cncGen_caCodeArray;

typedef struct
{
//boost::circular_buffer<char> codeArray;
boost::circular_buffer<double> carrArray;
int prvCoarsePhase;
double prvCarrierFinePhase;
}cncGen_carrierArray;

#endif

