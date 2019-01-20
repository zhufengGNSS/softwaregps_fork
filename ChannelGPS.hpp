#ifndef CHANNELGPS
#define CHANNELGPS
/* * File name		 : ChannelGPS.hpp
 *
 * Abstract          : This file contains definitions of structures and Global classes
 *                     Struchers  used in the Gpsgyan project.

 *
 * Compiler          : Visual C++ Version 9.0
 *
 * Compiler options  : Default
 *
 * Pragmas           : None used
 *
 * H/W Platform      : IBM PC
 *
 * Portability       : No Operating system call used.
 *                     No machine specific instruction used.
 *                     No compiler specific routines used.
 *                     Hence Portable.
 *
 * Authors			: Priyank Kumar
 *
 *
 * Version history	: Base version
 *	                   Priyank Kumar     November 2009
 *
 * References        : None
 *
 * Functions called  : None
 * Library Used 	 : GPStk
 *  				   Copyright 2009, The University of Texas at Austin
 *
 */
/* GPSTk Library*/

#include "IonoModel.hpp"
#include "DayTime.hpp"
#include "SatID.hpp"
#include "Position.hpp"
#include "SunPosition.hpp"
#include "ChannelGPSMac.hpp"
#include "ChannelGPStypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"


using namespace gpstk;

class ChannelGPS: public EngineFramework
{
public:


NEW_EXCEPTION_CLASS(ExcChannelGPS , gpstk::Exception);


ChannelGPS(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time")
   {};

void OutputData(std::ostream &s );
void OutputData();
// Public Data memebers
// input section

channelGPS_passiveInput passiveData;
channelGPS_passiveParam passiveControl;
channelGPS_activeInput activeData;
channelGPS_activeParam activeControl;


// input section

engineHealth channelGPS_probe;


channelGPS_opDataPort channelGPS_oData;


std::vector<int> error_number;


protected :


/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/
			void EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();

			void GetPassiveInput(channelGPS_passiveInput &ref);
			void GetPassiveParam(channelGPS_passiveParam &ref);

			void GetActiveInput(channelGPS_activeInput  &ref);
			void GetActiveParam(channelGPS_activeParam  &ref);

            void VerifyIpToEngine() throw(ExcChannelGPS);
			void ValidateOpOfEngine();

			void ExpHandler(ExcChannelGPS &e);

/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
// Fromatted File Printing
std::string opDataHeader;
std::string opDataInString;
std::string print_content(channelGPS_opDataPort &channelGPS_oData);
std::string print_header();
//virtual void dump(std::ostream &s);




private :

// Public Data memebers
// input section

channelGPS_activeInput p_activeData;
channelGPS_activeParam p_activeControl;
channelGPS_passiveInput p_passiveData;
channelGPS_passiveParam p_passiveControl;

EphReader yumaReader; // Reads all type of possible input file ( YUMA , Sem, RINEX etc , currently implemented for YUMA)
 channelGPS_opDataPort channelGPS_pData;


 // Private Functions

/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/
 void MethodOfEngine() throw(ExcChannelGPS);
 channelGPS_opDataPort getSatParam(SatID &sat, DayTime& time ,const YumaAlmanacStore& yums);
 bool ValidateOp(channelGPS_opDataPort &outputPData ,ExcChannelGPS &e);
 double getWindUp( const SatID& satid,
                   const DayTime& time,
                   const Triple& sat,
                   const Triple& sunPosition,
				   const Position& posRx);

double getIonoCorrections(double alpha[],double beta[],DayTime transmit,
                                                   Position &rxPos,
                                                   double elevation,
                                                   double azimuth,gpstk::IonoModel::Frequency );

double ComputePhaseWindUpDelay(SatID &satid ,DayTime &time , Xvt  &satPos, Position &rxPos);
void getThermalNoise(const double &loopBWpll,const double &c_n0,
									  const	double &loopBWdll,const double &chipWidth,
									  const double &armSpacing , const double &predetectIT);


    Position tempPos;
	double noiseDll,noisePll;
    map<SatID, phaseData> phase_station;
       map<SatID, phaseData> phase_satellite;


 	  //int IIR_Sat[12] ;
	  bool IIR;

};





#endif