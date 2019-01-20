#ifndef SATDGEN
#define SATDGEN

/* * File name		 : SatDgen.hpp
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
#include "YumaAlmanacStore.hpp"
#include "AlmOrbit.hpp"
#include "DayTime.hpp"
#include "SatID.hpp"
#include "EphReader.hpp"
#include "MSCData.hpp"
#include "MSCStream.hpp"
#include "FFIdentifier.hpp"
#include "Position.hpp"
#include "SatDgenMac.hpp"
#include "SatDgenTypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"


using namespace gpstk;

class SatDgen: public EngineFramework
{
public:


 NEW_EXCEPTION_CLASS(ExcSatDgen , gpstk::Exception);


 SatDgen(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time"){};



void OutputData(std::ostream &s );
void OutputData();

// Public Data memebers
// input section

satDgen_passiveInput passiveData;
satDgen_passiveParam passiveControl;
satDgen_activeInput activeData;
satDgen_activeParam activeControl;


// input section

engineHealth satDgen_probe;

satDgen_opDataPort satDgen_oData;


std::vector<int> error_number;


protected :

/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/

	void EngineControl(Kind kind);
	void SetInitialStateofEngine();
	void SetActiveStateofEngine ();

	void GetPassiveInput(satDgen_passiveInput &ref);
	void GetPassiveParam(satDgen_passiveParam &ref);

	void GetActiveInput(satDgen_activeInput  &ref);
	void GetActiveParam(satDgen_activeParam  &ref);

	void VerifyIpToEngine() throw(ExcSatDgen);
	void ValidateOpOfEngine();

	void ExpHandler(ExcSatDgen &e);



/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
// Fromatted File Printing
std::string opDataHeader;
std::string opDataInString;
std::string print_content(satDgen_opDataPort& satDgen_oData);
std::string print_header();



/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/



			double ComputeAtReceiveTime(
			          const DayTime& tr_nom,
			          const Position& Rx,
			          const SatID sat,
			           SatDb *satDb);

			       double ComputeAtTransmitTime(
			          const DayTime& tr_nom,
			          const double& pr,
			          const Position& Rx,
			          const SatID sat,
			           SatDb *satDb);

			       double ComputeAtTransmitSvTime(
			          const DayTime& tt_nom,
			          const double& pr,
			          const Position& Rx,
			          const SatID sat,
           SatDb *satDb);



private :

// Public Data memebers
// input section

// These are just helper functions to keep from repeating code
void updateCER(const Xvt& svPosVel, const Position& Rx, const double& rawrange, const SatDb *satDb);
void rotateEarth(Xvt& svPosVel, const Position& Rx);
void MethodOfEngine() throw(ExcSatDgen);
Xvt GetXvt(const SatID sat, const DayTime& time,SatDb *satDb);
double GetOtherCorrections(const Xvt& svPosVel, const Position &Rx);

bool ValidateOp(satDgen_opDataPort &outputPData ,ExcSatDgen &e);



Position SatR, SatV;




satDgen_activeInput p_activeData;
satDgen_activeParam p_activeControl;
satDgen_passiveInput p_passiveData;
satDgen_passiveParam p_passiveControl;

satDgen_opDataPort satDgen_pdata;

};


double RelativityCorrection(const Xvt& svPosVel);

#endif
