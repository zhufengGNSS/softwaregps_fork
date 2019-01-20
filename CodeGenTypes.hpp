#ifndef CODEGENTYPES
#define CODEGENTYPES
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
double  initSlewPhase;
int     initSlew;
double  satCodeClk;
}codeGen_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
}codeGen_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
// Possibly adding the taps vector


}codeGen_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
bool debug;
}codeGen_passiveParam;




typedef struct
{
public :
SatID  prn;
int   firstBlock[1023];
std::vector<int> lastblock;
int   firstBlockSlew;
int   residueSlew;
double totalCompleteCodeBlocks;
double residueSlewPhase;


}codeGen_opDataPort;


typedef struct
{
public :
	EngineFramework::engineHealth codeGen_health;
}codeGen_opHealthPort;

typedef struct
{
//boost::circular_buffer<char> codeArray;
boost::circular_buffer<int> codeArray;
int prvSlew;
double prvSlewPhase;
}codeGen_caCodeArray;

typedef struct
{
//boost::circular_buffer<char> codeArray;
boost::circular_buffer<double> carrArray;
int prvCoarsePhase;
double prvCarrierFinePhase;
}codeGen_carrierArray;

#endif

