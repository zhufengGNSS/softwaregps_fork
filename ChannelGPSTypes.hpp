using namespace gpstk;
//# include "Stats.hpp"
#include "SatDgen.hpp"
 #include "DayTime.hpp"
 #include "Position.hpp"
 # include "Xvt.hpp"
#include "EngineFramework.hpp"



#ifndef CHANNELGPSOPDATAPORT
#define CHANNELGPSOPDATAPORT



// These will be defined as public data members of the Engine

// This defines all datamembers which is fed in runtime: Active Input
typedef struct
{
DayTime epochTime; // Time when errors would be computed
SatID   satid;		// Satellite ID
Position rxPos;		// Rx position @ epoch time
Xvt      satPos;
double   elevation; // Elevation @ epoch Time
double  azimuth; 	// Azimututh angle
double  carrFreq;   // Carrier Frequency @ epoch

}channelGPS_activeInput;


// This defines all control parameter which is fed in runtime: Active control

typedef struct
{
bool debug;
}channelGPS_activeParam;



// This defines all inputs which is fed during initialization: Passive Input

typedef struct
{
double alpha[4];
double beta[4];
double loopBWpll;
double predetectIT;
double armSpacing;
double loopBWdll;
double c_n0;
double chipWidth;
}channelGPS_passiveInput;

// This defines all controls which is fed during initialization: Passive Control
typedef struct
{
bool useFullGpsWeek;
bool useExtIono;
}channelGPS_passiveParam;




typedef struct
{
public :
double   ionoDelay;
double   phasewindup;
double   pllThermalNoise;
double   dllThermalNoise;
/*Formatted String for output*/
std::string opDataHeader;
std::string opDataInString;

}channelGPS_opDataPort;


typedef struct
{
public :
	EngineFramework::engineHealth channelGPS_health;
}channelGPS_opHealthPort;

struct phaseData
{
  // Default constructor initializing the data in the structure
phaseData() : previousPhase(0.0) {};
double previousPhase;
};
#endif

