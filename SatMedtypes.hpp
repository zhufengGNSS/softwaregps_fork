#ifndef SATMDETYPES
#define SATMDETYPES 

#include "DayTime.hpp"
#include "Position.hpp"
#include "EngineFramework.hpp"
#include "SatDgen.hpp"
#include "CodeGen.hpp"
#include "CarrGen.hpp"
#include "ChannelGPS.hpp"

using namespace gpstk;
typedef struct
{
//std::vector<satDb_ostruct> *satDb_ref;
SatDgen  *satDgen;
SatDb	 *satDb;
CarrGen	 *carrgen;
CodeGen	 *codegen;
ChannelGPS *channel;
Position    rxXvt;
DayTime measTime;
}satMDE_actIstruct;



typedef struct
{
double minElev;

}satMDE_psvIstruct;


struct satMDE_clkFreq
{
satMDE_clkFreq() : carrFreq(0.0), codeFreq(0.0),carrDoppler(0.0),codeDoppler(0.0){};

double carrFreq;
double codeFreq;
double carrDoppler;
double codeDoppler;

};
typedef struct
{
SatID  prn;
satDgen_opDataPort dataNmeas;
carrGen_opDataPort dataCarr;
double rangeRate;
double rxIF_2_txCode;
satMDE_clkFreq clksL1;
satMDE_clkFreq clksL2c;
satMDE_clkFreq clksL5;
channelGPS_opDataPort dataChannel;

bool   measHealth;


}satMDE_ostruct;

struct satMDE_prvMeas
{
satMDE_prvMeas() : prvPseudoRange(0.0), prvSlewPhase(0.0),prvSlew(0),prvCoarsePhase(0.0),
					prvCarrierFinePhase(0.0){};
double prvPseudoRange;
double prvSlewPhase;
int    prvSlew;
double prvCoarsePhase;
double prvCarrierFinePhase;

};



#endif


