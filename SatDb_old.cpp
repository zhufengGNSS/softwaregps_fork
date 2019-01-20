
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
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void SatDb::InputInfToEngine(Kind kind)
{
 switch(kind)
 {
 case PASSIVE : GetPassiveInput();
 break;
 case ACTIVE : GetActiveInput(ref_act);
 break;
 default: break; // Set the flag as null signifying no input
 }


}

/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb::GetPassiveInput()
{

minElev = usr_elev;
yumaReader.read(yumaFileName);

satDb_probe.h_psvInput = true;
}

/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb::GetActiveInput(satDb_actIstruct &ref_act)
{
satDb_ai = ref_act;
gpsTrueTime = inp_time;
satDb_probe.h_actInput = true;

}

/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void SatDb::InputParamToEngine(Kind kind)
{
switch(kind)
 {
 case PASSIVE : GetPassiveParam();
 break;
 case ACTIVE : GetActiveParam();
 break;
 default: break; // Set the flag as null signifying no input
 }
}

/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void SatDb::GetPassiveParam()
{

//satDb_probe = 0;
//model_sat_clock

	/*************************************/
	/*Set the flag */
	/*************************************/
satDb_probe.h_psvParam = true;
}
/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void SatDb::GetActiveParam()
{
	int i=0;
	satDb_probe.h_actParam = true;
}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcSatDb)
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



void SatDb::SetInitialStateofEngine()
{		DayTime t;
		firsttime = false;
	// Clear all the satellite code and carrier vector
		for(int i=1; i <= gpstk::MAX_PRN ; i++)
		{
			SatID temp_satid(i,gpstk::SatID::systemGPS);
			prv_epoch[temp_satid] = t;
		}

		satDb_probe.h_engineControlPsv=true;


}
/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcSatDb)
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


void SatDb::SetActiveStateofEngine()
{


	  satDb_pData.clear();
	  satpData.clear();
	  satDb_probe.h_engineControlAct=true;





}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcSatDb)
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

void SatDb:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //satDb_probe = 0x0000;

	 GetPassiveInput();
	 GetPassiveParam();

	//Set the state depending on input, parameter and initial val
	 SetInitialStateofEngine();
	  break;



	case ACTIVE :
	// These parameters changes during the program execution where its taking input and param from some other
	// Modules

	//<modulename>_actIstruct : Defines active inputs to the module
	 GetActiveInput(ref_act);
	 // Active Parameter : This is basicall user controlled param which user
	 // change in real time program execution
	 GetActiveParam();

	// Set the state and other param of the engine before calling the method of engine
	 SetActiveStateofEngine();

		break;
	default :
		//satDb_probe = 0;
		break;

	}
	// set debug level etc
}


/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcSatDb)
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
	satDb_probe.h_verEngine= false;
	/* Verify  the Input Elevation */
	if(minElev < MIN_ELV_ANGLE)
	{
		e.addText("Elevation Underflow ");
		e.addLocation(FILE_LOCATION);
		error_number.push_back(1);
	}

	if (yumaReader.eph == NULL)
   {
      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;
	}
	satDb_probe.h_verEngine= true;

	}
	catch(ExcSatDb &e)
	{
		satDb_probe.h_verEngine= false;
		SatDb_ExpHandler(e);

	}

   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(ExcSatDb)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on active parameter and state
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
  DayTime epoch = gpsTrueTime;

double range;
double transitTime;
double detla_update;

// Visiblity flag is update from SatMDE
// Visiblity is computed every 80sec and satlite visiblity list is updated

	if(!visiblityFlag)
	{

      for (int prn=1; prn <= MAX_PRN; prn++)
        {

		 SatID temp_satid(prn,gpstk::SatID::systemGPS);
		 range  = (*(satDb_ai.prange_ptr))[temp_satid]; // derefrence
		 transitTime = range/gpstk::C_GPS_M;
		 epoch = epoch - transitTime;

		 getSatParam(SatID(prn, SatID::systemGPS), epoch , yums);

		 prv_epoch[temp_satid] =epoch;
		 prv_epoch[temp_satid].addSeconds((long)1);

	  }
	}
	else
	{
		int size = vislist_ref->size();
		for (int prn=0; prn < size; prn++)
      {
		 SatID satid = vislist_ref->at(prn);
		 range  = (*(satDb_ai.prange_ptr))[satid]; // derefrence
		 transitTime = range/gpstk::C_GPS_M;
         epoch = epoch - transitTime;
         detla_update = epoch - prv_epoch[satid];

		 if(firsttime)
		 { epoch = epoch - detla_update;

		 }

			transitTime = transitTime + detla_update;
			transitTime = range/gpstk::C_GPS_M;

			 //epoch = epoch - transitTime + detla_update;

		  getSatParam(satid, epoch , yums);
		 prv_epoch[satid] =epoch;
		 prv_epoch[satid].addSeconds((long)1);
		}
		if(!firsttime)
		{ firsttime = true;
		}


	  }


	}


/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void SatDb::getSatParam(SatID sat, DayTime& gpstime ,const YumaAlmanacStore& yums)

    {

	 satDb_ostruct temp_sat;
	 Position satR,satV; // can be reduced to ECEF or Triple
	 try
	 {

     AlmOrbit a = yums.findAlmanac(sat, gpstime);
	 sv_xvt     = a.svXvt(gpstime);
	 short week = a.getFullWeek();
	 // Make the output data structure

	 /*these param is needed for carr phase calculation*/

	 //temp_sat.toa = a.getToaTime(); this is wrong value
	 temp_sat.af0 = a.getAF0 ();
	 temp_sat.af1 = a.getAF1 ();

	 double time_elap = gpstime - temp_sat.toa;
	 temp_sat.prn=sat;
	 temp_sat.sathealth=!(a.getSVHealth());


	 temp_sat.satXvt=sv_xvt;

	 long temptoaSow = a.getToaSOW();
	 short tempweek =a.getToaWeek();

	 DayTime correctToa(0.L);
	 correctToa.setGPS(tempweek,(double)temptoaSow);
	 double time_elap1 = gpstime - correctToa;

 	 temp_sat.toa = correctToa;

	 satR.setECEF(sv_xvt.x[M_ZERO],sv_xvt.x[M_ONE],sv_xvt.x[M_TWO]);
	 satV.setECEF(sv_xvt.v[M_ZERO],sv_xvt.v[M_ONE],sv_xvt.v[M_TWO]);

    // Relativity Correction
	// Correction = -2*dot(R,V)/(c*c) ,
	// R & V are position and velocity vector

	 temp_sat.relativity = -2*(satR.X()/C_GPS_M)*(satV.X()/C_GPS_M)
             -2*(satR.Y()/C_GPS_M)*(satV.Y()/C_GPS_M)
             -2*(satR.Z()/C_GPS_M)*(satV.Z()/C_GPS_M);


	 // This take care of second roll over and other time adjustment
	 // as addition operator is overloaded with "addLongDeltaTime" function
	 // of gpstk::DayTime class.
	// correction = af0+af1(gpstime - toc of almanac) + relativity
	// not adding temp_sat.af0 as its the bias term

	 double correction =   time_elap1 * temp_sat.af1 + temp_sat.relativity; // correction with relativity , as in Yuma file it doesnt come
	 temp_sat.satTime = gpstime + correction;
	 //stats[sat].clock.Add(correction);
	 temp_sat.clkCorrection = correction;//a.getAF1()*C_GPS_M;

 // Take care of af0 and af1 of the clock and generate all frequency
	// sat correction is calculated in Satdb engine
	// Orginal Sat clock freq is offset for relativistic correction
	 temp_sat.satOscFreq 	   	= 	RSVCLK + RSVCLK * correction;
	 temp_sat.carrFreqL1 		=	temp_sat.satOscFreq*L1_MULT;
	 temp_sat.carrFreqL2 		=	temp_sat.satOscFreq*L2_MULT;
	 temp_sat.carrFreqL5 		=	temp_sat.satOscFreq*115;
	 temp_sat.chipFreqCA		=   temp_sat.satOscFreq / 10;
	 temp_sat.chipFreqPY		=   time_elap1;





	 // Almorbit satpos computation doesnt add relativity and ddtime param is empty, using it to do relativistic correction
	 temp_sat.satXvt.ddtime = correction; // af0+af1(gpstime - toc of almanac) + relativity

	 if(!temp_sat.sathealth)
	 {temp_sat.dataValid = false;
	 }

	 satDb_pData.push_back(temp_sat);
	 satpData[sat]=temp_sat;

}
	  catch(InvalidRequest &e)
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
		 satDb_pData.push_back(temp_sat);
		  satpData[sat]=temp_sat;
		  //temp_sat.sattime=0;
	  }


 }

void SatDb::ValidateOpOfEngine() throw(ExcSatDb)
{
	bool exceptionFlag;
	ExcSatDb e("Validation" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);

	satDb_probe.h_valEngine= false;
	#if 1
	if(!visiblityFlag)
	{
      for (int prn=1; prn <= MAX_PRN; prn++)
        {
			SatID temp_satid(prn,gpstk::SatID::systemGPS);
			exceptionFlag = false;
			exceptionFlag=CheckPRN(temp_satid ,e);
			try
			{
			if(exceptionFlag)
			throw e;
			else
			satDb_probe.h_valEngine= true;
			}
			catch(ExcSatDb &e)
	{
		satDb_probe.h_valEngine= false;
		//SatDb_ExpHandler(e);

	}
	  }
	}
	else
	{
		int size = vislist_ref->size();
		for (int prn=0; prn < size; prn++)
      {
		 SatID satid = vislist_ref->at(prn);
		 exceptionFlag = false;
			exceptionFlag=CheckPRN(satid ,e);
			try
			{
			if(exceptionFlag)
			throw e;
			else
			satDb_probe.h_valEngine= true;
			}
			catch(ExcSatDb &e)
		{
			satDb_probe.h_valEngine= false;
			//SatDb_ExpHandler(e);

		}
	}
	}
	#endif



   }















/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void SatDb::outputInfFromEngine(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;
	int size,start;
	SatID temp_satid;

	satDb_oData = satDb_pData;
	satoData = satpData;
	el = print_header();
	el += leftJustify("\n",3);
	s<<el;
#if 1
	if(!visiblityFlag)
	{size = MAX_PRN;
	start = 1;}
	else
	{size = vislist_ref->size();
	start =0;
	}

for (int prn=start; prn < size; prn++)
      {
		if(!visiblityFlag)
		{
		temp_satid.id = prn;
		temp_satid.system= gpstk::SatID::systemGPS;
		}
		else
		temp_satid = vislist_ref->at(prn);
		if(satpData[temp_satid].dataValid)
		s << print_content(satpData[temp_satid]);
}



  #endif
#if 0

	vector<satDb_ostruct>::iterator i;

	// Take only the healthy satellite
	for (i=satDb_oData.begin(); i!=satDb_oData.end();)
     {
		if(!(*i).sathealth)
		{
			satDb_oData.erase(i);
		} else i++;
		//if(satDb_oData.empty()) break;
	}




	for (int prn=0; prn < satDb_oData.size(); prn++)
      {

		using std::endl;
		using std::setw;
	   s << std::setprecision(4);    // set the precision of probe output
	   s << print_content(satDb_oData[prn]);
		}
#endif
	el = leftJustify("______________________________",24);
	el += leftJustify("\n",3);
	s<<el;
	s << gpsTrueTime.printf("%Y %03j % 12.6s")<<endl;
	el = leftJustify("______________________________",24);
	el += leftJustify("\n",3);
	s<<el;

    }


void SatDb::SatDb_ExpHandler(ExcSatDb &e)
{
unsigned long eid = error_number[1];


	switch(eid)
	{
	case 2: minElev = MIN_ELV_ANGLE;
		break;
	case 1: e.terminate();
		break;
	case 0: cout<<"over";
		break;
		default: break;
	}

}

/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(int *ptr_to_code,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to C/A code & tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
std::string SatDb::print_content(satDb_ostruct &ostruct)

{
 string  content;
 content = "";
 using namespace StringUtils;
	           content +=  leftJustify(asString(ostruct.prn), 6);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.satXvt.x[M_ZERO]), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.satXvt.x[M_ONE]), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.satXvt.x[M_TWO]), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.satXvt.v[M_ZERO]), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.satOscFreq), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.relativity), 13);
			   content += leftJustify("\t",3);
               content += leftJustify(asString((double)ostruct.clkCorrection),13);
			   content += leftJustify("\n",3);
			  // content += leftJustify(asString(e.sattime),13);
			  // content += leftJustify("\n",3);
			   return(content);
}

std::string SatDb::print_header()
{
 string  header;
 header = "";
 using namespace StringUtils;

 header +=  leftJustify("Sys PRN", 6);
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
			   header += leftJustify("Satvel Z", 13);
			   header += leftJustify("\t",3);
               header += leftJustify("Clock Correction",13);
			   header += leftJustify("\n",3);
			  // header += leftJustify(asString(e.sattime),13);
			  // header += leftJustify("\n",3);
			   return(header);
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
bool SatDb::CheckPRN(SatID satId ,ExcSatDb &e)
{
	string  content;
	content = "";
	e.setErrorId(4);
	using namespace StringUtils;
	bool exceptionFlag = false;
	content+=leftJustify("Satellite Number \t",10);
	content+=leftJustify(asString(satId.id), 6);
	content += leftJustify("\n",3);
	if(satpData[satId].relativity > MAX_REL_DELAY)
	{
		exceptionFlag = true;
		content +=leftJustify("High General Relativity Value" ,40);
		content += leftJustify("\n",3);
		e.addText(content);
		e.addLocation(FILE_LOCATION);
		error_number.push_back(4);
	}

	return(exceptionFlag);
}

