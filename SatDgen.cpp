/* * File name		 : SatDgen.cpp
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
 *	                  Priyank Kumar     November 2009
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
#include "SatDgen.hpp"
#include "PreciseRange.hpp"
#include "GaussianDistribution.hpp"
#include "MiscMath.hpp"
#include "GPSGeoid.hpp"
#include "geometry.hpp"


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
void SatDgen::GetPassiveInput(satDgen_passiveInput &ref)
{
satDgen_probe.s_psvParam = false;
p_passiveData = ref;
satDgen_probe.s_psvParam = true;
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
void SatDgen::GetPassiveParam(satDgen_passiveParam &ref)
{
	satDgen_probe.s_psvParam = false;

	p_passiveControl = ref;

	satDgen_probe.s_psvParam = true;
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

void SatDgen::GetActiveInput(satDgen_activeInput  &ref)
{
satDgen_probe.s_actInput = false;
p_activeData = ref;
satDgen_probe.s_actInput = true;

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

void SatDgen::GetActiveParam(satDgen_activeParam  &ref)
{
satDgen_probe.s_actParam = false;
p_activeControl = ref;
satDgen_probe.s_actParam = true;

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



void SatDgen::SetInitialStateofEngine()
{		DayTime t;
	opDataHeader = print_header();

	// Clear all the satellite code and carrier vector

		satDgen_probe.s_engineControlPsv=true;


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

void SatDgen::SetActiveStateofEngine()
{
	 if(p_activeControl.firstMeas)
		{
		// force other two calls not to be called
		p_activeControl.svRange = false;
		}
		else if (p_activeControl.svRange)
		{
		// force other two calls not to be called
		p_activeControl.firstMeas = false;
		}

	  satDgen_probe.s_engineControlAct=true;

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

void SatDgen:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //satDgen_probe = 0x0000;

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
 * Exception(s) 			: ExcSatDgen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDgen::VerifyIpToEngine() throw(ExcSatDgen)
{


   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(<engine Exception>)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on  parameters and state
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcSatDgen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void SatDgen::MethodOfEngine()throw(ExcSatDgen)
{

	if(p_activeControl.firstMeas)
	{
	satDgen_pdata.pseudoRange = ComputeAtReceiveTime(
							p_activeData.measTime,
							p_activeData.rxPos,
							p_activeData.satid,
							p_activeData.satDb);
	}
	else if(p_activeControl.svRange)
	{
	satDgen_pdata.pseudoRange = ComputeAtTransmitSvTime(
							p_activeData.measTime,
							p_activeData.calculatedRange,
							p_activeData.rxPos,
							p_activeData.satid,
							p_activeData.satDb);
	}
	else
	{
	satDgen_pdata.pseudoRange = ComputeAtTransmitTime(
							p_activeData.measTime,
							p_activeData.calculatedRange,
							p_activeData.rxPos,
							p_activeData.satid,
							p_activeData.satDb);
	}
	satDgen_pdata.prn = p_activeData.satid;


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
void SatDgen::ValidateOpOfEngine() throw(ExcSatDgen)
{
	bool exceptionFlag;
	ExcSatDgen e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	satDgen_probe.s_valEngine= false;
	exceptionFlag=ValidateOp(satDgen_pdata ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			satDgen_probe.s_valEngine= true;
			}
		catch(ExcSatDgen &e1)
	{
		satDgen_probe.s_valEngine= false;
		//SatDb_ExpHandler(e);

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
bool SatDgen::ValidateOp(satDgen_opDataPort &outputPData ,ExcSatDgen &e)
{
	string  content;
	content = "";
	e.setErrorId(4);
	using namespace StringUtils;
	bool exceptionFlag = false;
	content+=leftJustify("Satellite Number \t",10);
	content+=leftJustify(asString(outputPData.prn.id), 6);
	content += leftJustify("\n",3);
	
		/*
		exceptionFlag = false;
		content +=leftJustify("High General Relativity Value" ,40);
		content += leftJustify("\n",3);
		e.addText(content);
		e.addLocation(FILE_LOCATION);
		error_number.push_back(4);*/

	return(exceptionFlag);
}
/*----------------------------------------------------------------------------------------------
 * Function					: ExpHandler(<enginename_Excp &e)
 * Abstract					: This handles all the exceptions at various stages of the engine
 *							: processing and do appropriate decision
 * Formal
 * Parameter(s) 			: exception vector
 * Exception(s) 			: ExcSatDgen
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: String utilities
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/



void SatDgen::ExpHandler(ExcSatDgen &e)
{


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

void SatDgen::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	SatID temp_satid;


	satDgen_oData = satDgen_pdata;
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

	void SatDgen::OutputData()
	{
	satDgen_oData = satDgen_pdata;
	/* Actually this need to be put in validation stage */
	opDataInString = print_content(satDgen_oData);
	satDgen_oData.opDataHeader = opDataHeader;
	satDgen_oData.opDataInString = opDataInString;
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



/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at RECEIVE time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore. Note that this
 *    						  routine does not intrinsicly account for the receiver clock error
 *    						  like the ComputeAtTransmitTime routine does.
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

   double SatDgen::ComputeAtReceiveTime(
       const DayTime& tr_nom,
       const Position& Rx,
       const SatID sat,
       SatDb *satDb)
    {

      try {
          int nit;
          double tof,tof_old,delay;

          GPSGeoid geoid;

          nit = 0;
          tof = 0.07;       // initial guess 70ms
          do {
             // best estimate of transmit time
             satDgen_pdata.transmit = tr_nom;
             satDgen_pdata.transmit -= tof;
             tof_old = tof;
             // get SV position
             try {
                satDgen_pdata.svPosVel = GetXvt(sat, satDgen_pdata.transmit, satDb);
             }
             catch(InvalidRequest& e) {
                GPSTK_RETHROW(e);
             }
			 delay = GetOtherCorrections(satDgen_pdata.svPosVel,Rx);

         	 rotateEarth(satDgen_pdata.svPosVel,Rx);
             // update raw range and time of flight
             satDgen_pdata.rawrange = RSS(satDgen_pdata.svPosVel.x[0]-Rx.X(),
                            satDgen_pdata.svPosVel.x[1]-Rx.Y(),
                            satDgen_pdata.svPosVel.x[2]-Rx.Z());

             tof = satDgen_pdata.rawrange/geoid.c();
             tof -= delay;

          } while(ABS(tof-tof_old)>1.e-13 && ++nit<5);

          updateCER(satDgen_pdata.svPosVel,Rx,satDgen_pdata.rawrange ,satDb);

          double corr_range = satDgen_pdata.rawrange-satDgen_pdata.svclkbias-satDgen_pdata.relativity-satDgen_pdata.relativity2;

          return (corr_range);
       }
       catch(gpstk::Exception& e) {
          GPSTK_RETHROW(e);
       }
    }  // end CorrectedEphemerisRange::ComputeAtReceiveTime

/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at TRANSMIT time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore, as well as
 *
 *
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

    double SatDgen::ComputeAtTransmitTime(
       const DayTime& tr_nom,
       const double& pr,
       const Position& Rx,
       const SatID sat,
       SatDb *satDb)
    {

        double delay;
       try {
          DayTime tt;

          // 0-th order estimate of transmit time = receiver - pseudorange/c
          satDgen_pdata.transmit = tr_nom;
          satDgen_pdata.transmit -= pr/C_GPS_M;
          tt = satDgen_pdata.transmit;

          // correct for SV clock
          for(int i=0; i<2; i++) {
             // get SV position
             try {
                satDgen_pdata.svPosVel = GetXvt(sat,tt,satDb); // this will update satDb output structure
             }
             catch(InvalidRequest& e) {
                GPSTK_RETHROW(e);
             }

             // Add other corrections in time
             // This should be done only once after GetXvt function
             // This fuction uses satDb param which was already updated once the GetXvt is used
             delay = GetOtherCorrections(satDgen_pdata.svPosVel,Rx);
             tt = satDgen_pdata.transmit;
             tt -= delay;      // clock and relativity
          }

		 satDgen_pdata.transmit =tt;
          rotateEarth(satDgen_pdata.svPosVel,Rx);
          // raw range
          satDgen_pdata.rawrange = RSS(satDgen_pdata.svPosVel.x[0]-Rx.X(),
                         satDgen_pdata.svPosVel.x[1]-Rx.Y(),
                         satDgen_pdata.svPosVel.x[2]-Rx.Z());

          updateCER(satDgen_pdata.svPosVel,Rx,satDgen_pdata.rawrange ,satDb);

          double corr_range = satDgen_pdata.rawrange-satDgen_pdata.svclkbias-satDgen_pdata.relativity-satDgen_pdata.relativity2;

          return (corr_range);


       }
       catch(gpstk::Exception& e) {
          GPSTK_RETHROW(e);
       }
    }  // end CorrectedEphemerisRange::ComputeAtTransmitTime

/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at TRANSMIT time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore, as well as
 *
 *
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


    double SatDgen::ComputeAtTransmitSvTime(
       const DayTime& tt_nom,
       const double& pr,
       const Position& rx,
       const SatID sat,
       SatDb *satDb)
    {
       try {
          satDgen_pdata.svPosVel =GetXvt(sat,tt_nom,satDb);

          // compute rotation angle in the time of signal transit
          // While this is quite similiar to rotateEarth, its not the same and jcl doesn't
          // know which is really correct
          GPSGeoid gm;
          double rotation_angle = -gm.angVelocity() * (pr/gm.c() - satDgen_pdata.svPosVel.dtime);
          satDgen_pdata.svPosVel.x[0] = satDgen_pdata.svPosVel.x[0] - satDgen_pdata.svPosVel.x[1] * rotation_angle;
          satDgen_pdata.svPosVel.x[1] = satDgen_pdata.svPosVel.x[1] + satDgen_pdata.svPosVel.x[0] * rotation_angle;
          satDgen_pdata.svPosVel.x[2] = satDgen_pdata.svPosVel.x[2];

          satDgen_pdata.rawrange =rx.slantRange(satDgen_pdata.svPosVel.x);
          updateCER(satDgen_pdata.svPosVel,rx,satDgen_pdata.rawrange,satDb);

         double corr_range = satDgen_pdata.rawrange-satDgen_pdata.svclkbias-satDgen_pdata.relativity;

		 return (corr_range);

       }
       catch (Exception& e) {
          GPSTK_RETHROW(e);
       }
    }

/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at TRANSMIT time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore, as well as
 *
 *
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

    void SatDgen::updateCER(const Xvt& svPosVel, const Position& Rx, const double& rawrange, const SatDb  *satDb)
    {
       satDgen_pdata.relativity = RelativityCorrection(svPosVel) * C_GPS_M;


       satDgen_pdata.svclkbias = svPosVel.ddtime*C_GPS_M - satDgen_pdata.relativity ; // in meters
       satDgen_pdata.svclkdrift = svPosVel.ddtime * C_GPS_M;

       satDgen_pdata.cosines[0] = (Rx.X()-svPosVel.x[0])/rawrange;
       satDgen_pdata.cosines[1] = (Rx.Y()-svPosVel.x[1])/rawrange;
       satDgen_pdata.cosines[2] = (Rx.Z()-svPosVel.x[2])/rawrange;

       Position SV(svPosVel);
       satDgen_pdata.elevation = Rx.elevation(SV);
       satDgen_pdata.azimuth = Rx.azimuth(SV);
       satDgen_pdata.elevationGeodetic = Rx.elevationGeodetic(SV);
       satDgen_pdata.azimuthGeodetic = Rx.azimuthGeodetic(SV);
    }

/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at TRANSMIT time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore, as well as
 *
 *
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

    void SatDgen::rotateEarth(Xvt& svPosVel, const Position& Rx)
    {
       GPSGeoid geoid;
       double tof = RSS(svPosVel.x[0]-Rx.X(),
                        svPosVel.x[1]-Rx.Y(),
                        svPosVel.x[2]-Rx.Z())/geoid.c();
       double wt = geoid.angVelocity()*tof;
       double sx =  cos(wt)*svPosVel.x[0] + sin(wt)*svPosVel.x[1];
       double sy = -sin(wt)*svPosVel.x[0] + cos(wt)*svPosVel.x[1];
       svPosVel.x[0] = sx;
       svPosVel.x[1] = sy;
       sx =  cos(wt)*svPosVel.v[0] + sin(wt)*svPosVel.v[1];
       sy = -sin(wt)*svPosVel.v[0] + cos(wt)*svPosVel.v[1];
       svPosVel.v[0] = sx;
       svPosVel.v[1] = sy;
    }

/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at TRANSMIT time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore, as well as
 *
 *
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

    double RelativityCorrection(const Xvt& svPosVel)
    {
       // relativity correction is added to dtime by the
       // EphemerisStore::getSatXvt routines...
       // dtr = -2*dot(R,V)/(c*c) = -4.4428e-10(s/sqrt(m)) * ecc * sqrt(A(m)) * sinE
       // compute it separately here, in units seconds.
       double dtr = ( -2.0 *( svPosVel.x[0] * svPosVel.v[0]
                              + svPosVel.x[1] * svPosVel.v[1]
                              + svPosVel.x[2] * svPosVel.v[2] ) / C_GPS_M ) / C_GPS_M;
       return dtr;
    }

/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at TRANSMIT time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore, as well as
 *
 *
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


 Xvt SatDgen::GetXvt(const SatID sat, const DayTime& time,SatDb *satDb)
 {


	satDb_activeInput activeInput;

	activeInput.timeForSatPos = time;
	activeInput.satid = sat;

	satDb->activeData=activeInput;
	satDb->Engine(SatDb::RUN_ALL );
	satDb->EngineOutput(SatDb::DATA_HERE);
	bool datahealth = satDb->satDb_probe.h_ofOpData;
	if(!datahealth)
	satDgen_probe.h_ofOpData=false;
	else
	satDgen_probe.h_ofOpData=true;
	
	
	return(satDb->satoData.satXvt);

}

/*----------------------------------------------------------------------------------------------
 * Function					: void ComputeAtTransmitTime(const DayTime& tr_nom,
 *					           const double& pr, const Position& Rx,const SatID sat,
 *					           const SatDb& satDb)
 *
 * Abstract					: Compute the corrected range at TRANSMIT time, from receiver at position Rx,
 *    						  to the GPS satellite given by SatID sat, as well as all the CER quantities,
 *    						  given the nominal receive time tr_nom and an EphemerisStore, as well as
 *
 *
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

double SatDgen::GetOtherCorrections(const Xvt& svPosVel, const Position &Rx)

{
	// Remember here we use ddtime field for correct correction time
	// If this module is used for the ephermeris data, make sure to change the algorithm

	GPSGeoid geoid;
	double otherCorrections = 0;

	otherCorrections = svPosVel.ddtime;
	SatR.setECEF(svPosVel.x[0],svPosVel.x[1],svPosVel.x[2]);

	 // Sagnac effect
	 // ref. Ashby and Spilker, GPS: Theory and Application, 1996 Vol 1, pg 673.
	 // this is w(Earth) * (SatR cross Rx).Z() / c*c in seconds
	 // beware numerical error by differencing very large to get very small
	 satDgen_pdata.sagnac = ( (SatR.X()/geoid.c()) * (Rx.Y()/geoid.c())
	          - (SatR.Y()/geoid.c()) * (Rx.X()/geoid.c()) ) * geoid.angVelocity();
	otherCorrections += satDgen_pdata.sagnac;


	// compute other delays -- very small
	// 2GM/c^2 = 0.00887005608 m^3/s^2 * s^2/m^2 = m
	double rx = Rx.radius();
	double rs = SatR.radius();
	double dr = range(SatR,Rx);
	satDgen_pdata.relativity2 = -0.00887005608 * ::log((rx+rs+dr)/(rx+rs-dr));
	otherCorrections += satDgen_pdata.relativity2 / geoid.c();

	return(otherCorrections);
}

/*----------------------------------------------------------------------------------------------
 * Function					: string print_content(<engine output port> &<engine output data>)                                              )
   
 * Abstract					: This converts all the data on output port into tabbed spaces string 
 *							  and sent out on output port of the data
 *							  
 * Formal
 * Parameter(s) 			: <engine output variables>
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: All Private Members; see the header files
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/	 
std::string SatDgen::print_content(satDgen_opDataPort &satDgen_oData)

{
 string  content;
 content = "";
 using namespace StringUtils;
			   content +=  leftJustify(asString((int)satDgen_oData.prn.id), 13);
			   content += leftJustify("\t",3);
	           content += leftJustify(asString((double)satDgen_oData.pseudoRange), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.rawrange), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.svclkbias), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.svclkdrift), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.relativity), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.sagnac), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.relativity2), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.elevation), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.azimuth), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.elevationGeodetic), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.azimuthGeodetic), 13);
			   content += leftJustify("\t",3);
			    content += leftJustify(asString((double)satDgen_oData.svPosVel.x[0]), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)satDgen_oData.svPosVel.x[1]), 13);
			   content += leftJustify("\t",3);
			    content += leftJustify(asString((double)satDgen_oData.svPosVel.x[2]), 13);
			   content += leftJustify("\t",3);
			    content += leftJustify(asString((double)satDgen_oData.svPosVel.v[0]), 13);
			   content += leftJustify("\t",3);
			    content += leftJustify(asString((double)satDgen_oData.svPosVel.v[1]), 13);
			   content += leftJustify("\t",3);
			    content += leftJustify(asString((double)satDgen_oData.svPosVel.v[2]), 13);
			   content += leftJustify("\t",3);
			    content += leftJustify(asString((double)satDgen_oData.svPosVel.ddtime), 13);
			   content += leftJustify("\t",3);
			     return(content);
}

/*----------------------------------------------------------------------------------------------
 * Function					: string print_header()                                             
 * Abstract					: This prints the header of the data that is output on the output data
 *							  port of the engine
 * Formal
 * Parameter(s) 			: None
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: All Private Members; see the header files
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/	 

std::string SatDgen::print_header()
{
 string  header;
 header = "";
 using namespace StringUtils;
			   header += leftJustify("SatID", 13);	
			   header += leftJustify("\t",3);
			   header += leftJustify("pseudoRange", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("rawrange", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("svclkbias", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("svclkdrift ", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("relativity(m)", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("sagnac", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("relativity2", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("elevation ", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("azimuth", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("elevGeod", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("aziGeod", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("SatPos X", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("SatPos Y", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("SatPos Z", 13);
			   header += leftJustify("\t",3);
			    header += leftJustify("SatVel X", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("SatVel Y", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("SatVel Z", 13);
			   header += leftJustify("\t",3);
			    header += leftJustify("SatClkErr", 13);
			   header += leftJustify("\t",3);
			   			   
return(header);
}


