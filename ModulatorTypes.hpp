#ifndef MODULATORTYPES
#define MODULATORTYPES

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
#include "CodeGenTypes.hpp"







// These will be defined as public data members of the Engine

// This defines all datamembers which is fed in runtime: Active Input
typedef struct
{
codeGen_opDataPort *codeInf;
double rxIF;
double initCoarsePhase;
double initCarrierFinePhase;

}modulator_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
int numberofms;
}modulator_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
// Possibly adding the taps vector


}modulator_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
bool debug;
}modulator_passiveParam;




typedef struct
{
public :
SatID  prn;
std::vector<double> firstBlockIF;
std::vector<double> lastBlockIF;
double   residueCoarsePhase;
double residueFinePhase;

}modulator_opDataPort;


typedef struct
{
public :
	EngineFramework::engineHealth modulator_health;
}modulator_opHealthPort;


#endif

