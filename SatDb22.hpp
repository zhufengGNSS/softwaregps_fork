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



//void InputInfToEngine(Kind kind);  // think of argv and argc implementation
//virtual void runEngine();
void InputParamToEngine(Kind kind);// throw(InvalidParameter);
//bool runEngine() throw();
//void outputInfFromEngine(std::ostream & s ,const std::string & 	data);
void outputInfFromEngine(std::ostream & s );
//void OutputDumpOfEngine();

// Public Data memebers
// input section

satDb_activeInput activeData;
satDb_activeParam activeControl;
satDb_passiveInput passiveData;
satDb_passiveParam passiveControl;

// input section





engineHealth satDb_probe;

std::vector<satDb_ostruct> satDb_oData;
map<SatID ,satDb_ostruct> satoData;

std::vector<SatID> *vislist_ref;
bool visiblityFlag;

satDb_actIstruct ref_act;



string yumaFileName;
DayTime inp_time ;
double  transitTime;
double usr_elev;

std::vector<int> error_number;
//std::string op_inf;
protected :
			void  EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();

			void GetPassiveInput(satDb_activeInput &ref);
			void GetActiveInput(satDb_activeInput  &ref);
			void GetPassiveParam(satDb_activeInput &ref);
			void GetActiveParam(satDb_activeInput  &ref);





            void VerifyIpToEngine() throw(ExcSatDb);
 //         void prepareIpToEngine() ;

			void MethodOfEngine() throw(ExcSatDb);
			void getSatParam(SatID sat, DayTime& t ,const YumaAlmanacStore& yums);
			void SatDb_ExpHandler(ExcSatDb &e);

			// Fromatted File Printing
			std::string print_content(satDb_ostruct &ostruct);
		    std::string print_header();
			bool CheckPRN(SatID prn ,ExcSatDb &e);

        void ValidateOpOfEngine();
		//	void PrepareOpOfEngine()	;

			//virtual void dump(std::ostream &s);




private :

// Public Data memebers
// input section

satDb_activeInput p_activeData;
satDb_activeParam p_activeControl;
satDb_passiveInput p_passiveData;
satDb_passiveParam p_passiveControl;



    EphReader yumaReader; // Reads all type of possible input file ( YUMA , Sem, RINEX etc , currently implemented for YUMA)
	DayTime  gpsTrueTime;
	map<SatID ,DayTime> prv_epoch;
	//map<SatID ,satDb_stat> stats;
	SatID    gps_sv;

	std::vector<satDb_ostruct> satDb_pData;
	map<SatID ,satDb_ostruct> satpData;
	//std::vector<satDb_ostruct> satDb_pData;
	Xvt      sv_xvt;
	bool firsttime;
	// Error Value

	double minElev;

};


