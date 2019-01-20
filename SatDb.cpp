
/* * File name		 : SatDb.cpp
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



#include <stdio.h>
#include <iostream>
#include <string>
#include <list>

// gpstk
#include "StringUtils.hpp"
#include "GPSAlmanacStore.hpp"
#include "icd_200_constants.hpp"
#include "gps_constants.hpp"
#include "SatDb.hpp"
#include "PreciseRange.hpp"
#include "GaussianDistribution.hpp"
//#include "Stats.hpp"


using namespace gpstk;
using namespace std;



/*----------------------------------------------------------------------------------------------
 * Function					: GetPassiveInput(<enginename>_passiveInput &ref)
 * Abstract					: This function takes the passive input structure to the engine
 *							  which is normally done during initialization of engine. Input can
 *							  be prepared in this function which as suitable to method of engine
 * Formal
 * Parameter(s) 			: reference to the passive input structure
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: OOSDM core framework
 * Specific library calls	: None
 * Member variables accessed: status flag of passive input
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void SatDb::GetPassiveInput(satDb_passiveInput &ref)
{
satDb_probe.s_psvParam = false;
p_passiveData = ref;
yumaReader.read(p_passiveData.yumaFileName);
satDb_probe.s_psvParam = true;
}

/*----------------------------------------------------------------------------------------------
 * Function					: GetPassiveParam(<enginename>_passiveParam &ref)
 * Abstract					: This function takes the passive controls structure to the engine
 *							  which is normally done during initialization of engine. dependent
 *							  parameters can be created in this module.
 *
 * Formal
 * Parameter(s) 			: reference to the passive parameter structure
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: OOSDM core framework
 * Specific library calls	: None
 * Member variables accessed: status flag of passive param
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void SatDb::GetPassiveParam(satDb_passiveParam &ref)
{
	satDb_probe.s_psvParam = false;

	p_passiveControl = ref;

	satDb_probe.s_psvParam = true;
}


/*----------------------------------------------------------------------------------------------
 * Function					: GetActiveInput(<enginename>_activeInput &ref)
 * Abstract					: This function takes the active input structure to the engine
 *							  which is normally done during run phase of engine. Input can
 *							  be prepared in this function as per  method of engine
 * Formal
 * Parameter(s) 			: reference to the passive input structure
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: OOSDM core framework
 * Specific library calls	: None
 * Member variables accessed: status flag of passive input
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb::GetActiveInput(satDb_activeInput  &ref)
{
satDb_probe.s_actInput = false;
p_activeData = ref;
satDb_probe.s_actInput = true;

}



/*----------------------------------------------------------------------------------------------
 * Function					: GetActiveParam(<enginename>_activeParam &ref)
 * Abstract					: This function takes the active controls structure to the engine
 *							  which is normally done during runtime of engine. dependent
 *							  parameters can be created in this module.
 *
 * Formal
 * Parameter(s) 			: reference to the active parameter structure
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: OOSDM core framework
 * Specific library calls	: None
 * Member variables accessed: status flag of active param
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void SatDb::GetActiveParam(satDb_activeParam  &ref)
{
satDb_probe.s_actParam = false;
p_activeControl = ref;
satDb_probe.s_actParam = true;

}

/*----------------------------------------------------------------------------------------------
 * Function					: void SetInitialStateofEngine()
 * Abstract					: This fuction processes the input passive parameters and internal
 *							  variable of engine to bring the algorithm / engine to initial known
 *							  state.
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: None
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/



void SatDb::SetInitialStateofEngine()
{		DayTime t;

	// Clear all the satellite code and carrier vector

		satDb_probe.s_engineControlPsv=true;


}

/*----------------------------------------------------------------------------------------------
 * Function					: void SetActiveStateofEngine()
 * Abstract					: This fuction processes the input active parameters and internal
 *							  variable of engine to change the operating mode or to make appropiate
 *							  change to engine for processing active inputs
 *							  state.
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: None
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void SatDb::SetActiveStateofEngine()
{
		//make health of data from o/p port as unhealthy(=false)
		// Validate output( Engine will make it true/false)
	   satDb_probe.h_ofOpData = false;
	   satDb_probe.s_engineControlAct=true;

}

/*----------------------------------------------------------------------------------------------
 * Function					: void EngineControl()
 * Abstract					: This is the main controller of the engine, its responsible for
 *							  fetching and applying all input and controls and making sure
 *							  that engine performs well under all good condition and catches
 *							  exceptions if some wrong input if fed or wrong result is generated
 *							  This is not main algorithm of engine, it acts as manager of the algorithm
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcSatDb
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: GetPassiveInput,GetPassiveParam,SetInitialStateofEngine
 *							: GetActiveInput,GetActiveParam,SetActiveStateofEngine
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //satDb_probe = 0x0000;

	 GetPassiveInput(passiveData);
	 GetPassiveParam(passiveControl);

	//Set the state depending on input, parameter and initial val
	 SetInitialStateofEngine();
	  break;



	case ACTIVE :
	// These parameters changes during the program execution where its taking input and param from some other
	// Modules

	//<modulename>_actInput : Defines active inputs to the module
	 GetActiveInput(activeData);
	 // Active Parameter : This is basicall user controlled param which user
	 // change in real time program execution
	 GetActiveParam(activeControl);

	// Set the state and other param of the engine before calling the method of engine
	 SetActiveStateofEngine();

		break;
	default :

		break;

	}
	// set debug level etc
}


/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(<engine Exception>)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcSatDb
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb::VerifyIpToEngine() throw(ExcSatDb)
{
	ExcSatDb e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{
	satDb_probe.s_verEngine= false;
	/* Verify  the Input Elevation */

	if (yumaReader.eph == NULL)
   {
      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;
	}
	satDb_probe.s_verEngine= true;

	}
	catch(ExcSatDb &e)
	{
		satDb_probe.s_verEngine= false;
		ExpHandler(e);

	}

   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(<engine Exception>)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on  parameters and state
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcSatDb
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void SatDb::MethodOfEngine()throw(ExcSatDb)
{
   	gpstk::XvtStore<SatID>& almStore = *yumaReader.eph;
  	const YumaAlmanacStore& yums = dynamic_cast<const YumaAlmanacStore&>(*yumaReader.eph);
  	satpData = getSatParam(p_activeData.satid, p_activeData.timeForSatPos,yums);
   
	


	}

/*----------------------------------------------------------------------------------------------
 * Function					: getSatParam(SatID &sat, DayTime& time ,const YumaAlmanacStore& yums)
 * Abstract					: This function generate satellite pvt and healt for given satid and
 *							  GPStime.
 * Formal
 * Parameter(s) 			: satid , time , pointer to the almanac database
 * Return value 			: satDb_opDataPort structure which contain all data that this function
 * System Call				: None
 * Functions called			: None
 * Reference				: Spilker, Kaplan
 * Specific library calls	: gpstk
 * Member variables accessed: None
 * Assumptions 				: gpstk doesnot read the week from the almanac file as full week
 *							: so the algorithm is modified accordingly
 ----------------------------------------------------------------------------------------------*/


satDb_opDataPort SatDb::getSatParam(SatID &sat, DayTime& time ,const YumaAlmanacStore& yums)

    {
 Xvt sv_xvt;
	 satDb_opDataPort temp_sat;
	 Position satR,satV; // can be reduced to ECEF or Triple

     AlmOrbit a = yums.findAlmanac(sat, time);
	 sv_xvt     = a.svXvt(time);
	 // Make the output data structure

	 /*these param is needed for carr phase calculation*/

	 //temp_sat.toa = a.getToaTime(); this is wrong value
	 temp_sat.af0 = a.getAF0 ();
	 temp_sat.af1 = a.getAF1 ();
	 temp_sat.prn=sat;
	 temp_sat.sathealth=!(a.getSVHealth());
	 temp_sat.satXvt=sv_xvt;

	 long temptoaSow = a.getToaSOW();
	 short tempweek =a.getToaWeek();

	 DayTime correctToa(0.L);
	 correctToa.setGPS(tempweek,(double)temptoaSow);
	 double time_elap1 = time - correctToa;

 	 temp_sat.toa = correctToa;

	 satR.setECEF(sv_xvt.x[M_ZERO],sv_xvt.x[M_ONE],sv_xvt.x[M_TWO]);
	 satV.setECEF(sv_xvt.v[M_ZERO],sv_xvt.v[M_ONE],sv_xvt.v[M_TWO]);

    // Relativity Correction
	// Correction = -2*dot(R,V)/(c*c) ,
	// R & V are position and velocity vector

	/* temp_sat.relativity = -2*(satR.X()/C_GPS_M)*(satV.X()/C_GPS_M)
             -2*(satR.Y()/C_GPS_M)*(satV.Y()/C_GPS_M)
             -2*(satR.Z()/C_GPS_M)*(satV.Z()/C_GPS_M);*/


	 // This take care of second roll over and other time adjustment
	 // as addition operator is overloaded with "addLongDeltaTime" function
	 // of gpstk::DayTime class.
	// correction = af0+af1(gpstime - toc of almanac) + relativity
	// not adding temp_sat.af0 as its the bias term

	 double correction = temp_sat.af0 + time_elap1 * temp_sat.af1 ;//+ temp_sat.relativity;
	 temp_sat.clkCorrection = correction;//a.getAF1()*C_GPS_M;

	 // Almorbit satpos computation doesnt add relativity and ddtime param is empty, using it to do relativistic correction
	 temp_sat.satXvt.ddtime = correction; // af0+af1(gpstime - toc of almanac) + relativity


	/*  catch(InvalidRequest &e)
	 {
		 temp_sat.prn=sat;
		 temp_sat.sathealth=false;
		 temp_sat.satXvt.x[M_ZERO] = (double)M_ZERO;
		 temp_sat.satXvt.x[M_ONE] = (double)M_ZERO;
		 temp_sat.satXvt.x[M_TWO] = (double)M_ZERO;
	     temp_sat.satXvt.v[M_ZERO] = (double)M_ZERO;
		 temp_sat.satXvt.v[M_ONE] = (double)M_ZERO;
		 temp_sat.satXvt.v[M_TWO] = (double)M_ZERO;
		 temp_sat.af0=M_ZERO;
		  temp_sat.af1=M_ZERO;
		 temp_sat.relativity=M_ZERO;
		 temp_sat.clkCorrection=(double)M_ZERO;
	 }
*/
	 return(temp_sat);


 }

/*----------------------------------------------------------------------------------------------
 * Function					: void ValidateOpOfEngine() throw(<engine Exception>)
 * Abstract					: This perform the validation of output data generated by method of
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcSatDb
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void SatDb::ValidateOpOfEngine() throw(ExcSatDb)
{
	bool exceptionFlag;
	ExcSatDb e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	satDb_probe.s_valEngine= false;
	
	exceptionFlag=ValidateOp(satpData ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			satDb_probe.s_valEngine= true;
			}
		catch(ExcSatDb &e1)
	{
		satDb_probe.s_valEngine= false;
		//SatDb_ExpHandler(e);

	}

   }


/*----------------------------------------------------------------------------------------------
 * Function					: OutputData(std::ostream &s )
 * Abstract					: This dumps the output data to a stream "s"
 *
 * Formal
 * Parameter(s) 			: stream s
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: String utilities
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	SatID temp_satid;


	satoData = satpData;
	s<<el;

    }

/*----------------------------------------------------------------------------------------------
 * Function					: OutputData( )
 * Abstract					: This dumps the verified output data to the public data port
 *							: This function is a place where formating on the output data
 *							: can be done
 * Formal
 * Parameter(s) 			:
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: String utilities
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

	void SatDb::OutputData()
	{
	satoData = satpData;
	}


/*----------------------------------------------------------------------------------------------
 * Function					: ExpHandler(<enginename_Excp &e)
 * Abstract					: This handles all the exceptions at various stages of the engine
 *							: processing and do appropriate decision
 * Formal
 * Parameter(s) 			: exception vector
 * Exception(s) 			: ExcSatDb
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: String utilities
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb::ExpHandler(ExcSatDb &e)
{
unsigned long eid = error_number[1];


	switch(eid)
	{
	case 1: e.terminate();
		break;
	case 0: cout<<"over";
		break;
		default: break;
	}

}


/*----------------------------------------------------------------------------------------------
 * Function					: void ValidateOp(satDb_opDataPort &outputPData ,ExcSatDb &e)
 * Abstract					: This is internal validation function that compares all the generated
 *							  output value to known range defined in <enginenameMac.hpp>
 *
 * Formal
 * Parameter(s) 			: File name where dump of engine is stored
 * Exception(s) 			: ExcSatDb
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: gpstk exception library
 * Member variables accessed: All output dataport members; see the header files <enginename>Types.hpp
 * Assumptions 				: The range check is just sanity check it only ensures that output is not
 *							: beyond the range, it doesnot guarntee the logical value of output
 ----------------------------------------------------------------------------------------------*/
bool SatDb::ValidateOp(satDb_opDataPort &outputPData ,ExcSatDb &e)
{
	string  content;
	content = "";
	e.setErrorId(4);
	using namespace StringUtils;
	bool exceptionFlag = false;
	content+=leftJustify("Satellite Number \t",10);
	content+=leftJustify(asString(outputPData.prn.id), 6);
	content += leftJustify("\n",3);
	 if(!satpData.sathealth)
	satDb_probe.h_ofOpData=false;
	else
	satDb_probe.h_ofOpData=true;

		/*exceptionFlag = false;
		content +=leftJustify("High General Relativity Value" ,40);
		content += leftJustify("\n",3);
		e.addText(content);
		e.addLocation(FILE_LOCATION);
		error_number.push_back(4);*/

	return(exceptionFlag);
}

/*----------------------------------------------------------------------------------------------
 * Function					: void dump(std::ostream& s)
 * Abstract					: This function generate the dump of the private variables and state
 *							  of the Engines given the debug level
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: File name where dump of engine is stored
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: All Private Members; see the header files
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

