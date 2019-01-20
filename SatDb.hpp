#ifndef SATDB
#define SATDB

/* * File name		 : SatDb.hpp
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
#include "SatDbMac.hpp"
#include "SatDbtypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"



using namespace gpstk;

class SatDb: public EngineFramework
{
public:


NEW_EXCEPTION_CLASS(ExcSatDb , gpstk::Exception);


SatDb(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time")
   {};

void OutputData(std::ostream &s );
void OutputData();
// Public Data memebers
// input section

satDb_passiveInput passiveData;
satDb_passiveParam passiveControl;
satDb_activeInput activeData;
satDb_activeParam activeControl;


// input section

engineHealth satDb_probe;


satDb_opDataPort satoData;


std::vector<int> error_number;


protected :
/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/
			void EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();

			void GetPassiveInput(satDb_passiveInput &ref);
			void GetPassiveParam(satDb_passiveParam &ref);

			void GetActiveInput(satDb_activeInput  &ref);
			void GetActiveParam(satDb_activeParam  &ref);

            void VerifyIpToEngine() throw(ExcSatDb);
			void ValidateOpOfEngine();
 
			void ExpHandler(ExcSatDb &e);

/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
// Fromatted File Printing
//std::string print_content(satDb_ostruct &ostruct);
//std::string print_header();
//virtual void dump(std::ostream &s);




private :

// Public Data memebers
// input section

satDb_activeInput p_activeData;
satDb_activeParam p_activeControl;
satDb_passiveInput p_passiveData;
satDb_passiveParam p_passiveControl;

EphReader yumaReader; // Reads all type of possible input file ( YUMA , Sem, RINEX etc , currently implemented for YUMA)
 satDb_opDataPort satpData;


 // Private Functions

/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/
 void MethodOfEngine() throw(ExcSatDb);
 satDb_opDataPort getSatParam(SatID &sat, DayTime& time ,const YumaAlmanacStore& yums);
 bool ValidateOp(satDb_opDataPort &outputPData ,ExcSatDb &e);

};


#endif
