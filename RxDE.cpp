//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Copyright 2008, The University of Texas at Austin
//
//============================================================================
//
// Compute number of stations visible to a set of space vehicles (SV)
// over a requested period (default 23:56).  Accept FIC
// almanac format, FIC ephemeris, Rinex nav, Yuma almanac, SEM almanac,
// or SP3 as input
//
// Assumptions:
//
// System
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>

// gpstk
#include "StringUtils.hpp"
#include "GPSAlmanacStore.hpp"
#include "icd_200_constants.hpp"
#include "gps_constants.hpp"
#include "RxDE.hpp"
#include "PreciseRange.hpp"
#include <ctime>            // std::time

#include <boost/random/linear_congruential.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/additive_combine.hpp>
#include <boost/random.hpp>
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC > 0x530)
#include <boost/generator_iterator.hpp>
#endif


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
void RxDE::GetPassiveInput(rxDE_passiveInput &ref)
{
rxDE_probe.s_psvParam = false;
p_passiveData = ref;

rxDE_probe.s_psvParam = true;


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
void RxDE::GetPassiveParam(rxDE_passiveParam &ref)
{

	rxDE_probe.s_psvParam = false;

	p_passiveControl = ref;

	rxDE_probe.s_psvParam = true;
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

void RxDE::GetActiveInput(rxDE_activeInput  &ref)
{
rxDE_probe.s_actInput = false;
p_activeData = ref;
rxDE_probe.s_actInput = true;

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

void RxDE::GetActiveParam(rxDE_activeParam  &ref)
{
rxDE_probe.s_actParam = false;
p_activeControl = ref;
rxDE_probe.s_actParam = true;
}

/*----------------------------------------------------------------------------------------------
 * Function					: void SetInitialStateofEngine() throw(ExcRxDE)
 * Abstract					: This perform the sets the initial  input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcRxDE
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void RxDE::SetInitialStateofEngine()
{
opDataHeader = print_header();
rxClkerr_time[0] = 0;
rxClkerr_time[1] = 0;
rxClkerr_phase[0] = 0;
rxClkerr_phase[1]= 0;
//model_sat_clock
}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcRxDE)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcRxDE
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void RxDE::SetActiveStateofEngine()
{	 rxDE_probe.s_engineControlAct=false;
	
	
}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcRxDE)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcRxDE
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void RxDE:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //rxDE_probe = 0x0000;

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
 * Function					: void VerifyIpToEngine() throw(ExcRxDE)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcRxDE
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void RxDE::VerifyIpToEngine() throw(ExcRxDE)
{

	ExcRxDE e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{

	rxDE_probe.s_verEngine= false;
	/* Verify  the Input Elevation */


      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;


	rxDE_probe.s_verEngine= true;

	}
	catch(ExcRxDE &e)
	{
		rxDE_probe.s_verEngine= false;
		ExpHandler(e);

	}


   }


void RxDE::MethodOfEngine()
{
  double update_interval =1;
  double rx_correction =GenerateRxClkErr(update_interval);
  rxDE_pData.rx_time = p_activeData.trueGPStime + rx_correction;
  rxDE_pData.trueGPStime = p_activeData.trueGPStime ;
  rxDE_pData.rxClkBias =rxClkerr_time[M_ZERO] ;
  rxDE_pData.rxClkDrift = rxClkerr_phase[M_ZERO] ;

}

double RxDE::GenerateRxClkErr(double& update_interval)

    {

	// This is a typedef for a random number generator.
	// Try boost::mt19937 or boost::ecuyer1988 instead of boost::minstd_rand
	// Define the generator type
	double dt = update_interval;
	typedef boost::mt19937 base_generator_type1;
	typedef boost::mt11213b base_generator_type2;
	// Using 42u generator typs
	base_generator_type1 generator1(42u);
	base_generator_type2 generator2(48u);
	// Take the seed from the PC clock
	generator1.seed(static_cast<unsigned int>(std::time(0)));

	boost::normal_distribution<>  normal(M_ZERO,M_ONE);

	// Generate two iid  Normal Random number generator with mean = 0 , variance =1;
    boost::variate_generator<base_generator_type1&, boost::normal_distribution<> > norm_gen1(generator1, normal);

	generator2.seed(static_cast<unsigned int>(std::time(0)));

	boost::variate_generator<base_generator_type2&, boost::normal_distribution<> > norm_gen2(generator2, normal);

	// Define the two integrater for discrete gaussian noise process = Weiner Process
	norm1 = norm_gen1();
	norm2 = norm_gen2();


	double gaussian_walk1 = std::sqrt(CLK_SF*dt + CLK_SGB_12*pow(dt,(double)3))*norm1 + std::sqrt( CLK_SGB_4*pow(dt,(double)3))*norm2;
	double gaussian_walk2 = std::sqrt( CLK_SG*dt)*norm2;

    rxClkerr_time[M_ONE] = rxClkerr_time[M_ZERO] + dt*rxClkerr_phase[M_ZERO] + gaussian_walk1;
    rxClkerr_phase[M_ONE] = rxClkerr_phase[M_ZERO] + gaussian_walk2;



   double ret_rxClk_err = rxClkerr_time[M_ZERO];
   // Update the stochastic Model for next tick


   rxClkerr_time[M_ZERO]	=rxClkerr_time[M_ONE];
   rxClkerr_phase[M_ZERO]	=rxClkerr_phase[M_ONE];
   return(ret_rxClk_err);

 }


/*----------------------------------------------------------------------------------------------
 * Function					: void RxDECA_L1(int *ptr_to_code,int tap1,int tap2)
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


void RxDE::ValidateOpOfEngine() throw(ExcRxDE)
{
	bool exceptionFlag;
	ExcRxDE e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	rxDE_probe.s_valEngine= false;
	exceptionFlag=ValidateOp(rxDE_pData ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			rxDE_probe.s_valEngine= true;
			}
		catch(ExcRxDE &e1)
	{
		rxDE_probe.s_valEngine= false;
		//RxDE_ExpHandler(e);

	}









   }




/*----------------------------------------------------------------------------------------------
 * Function					: void RxDECA_L1(int *ptr_to_code,int tap1,int tap2)
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

void RxDE::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;




	SatID temp_satid;
	rxDE_oData = rxDE_pData;
	s<<el;















    }


	void RxDE::OutputData()
	{
	rxDE_oData = rxDE_pData;
	opDataInString = print_content(rxDE_oData);
	rxDE_oData.opDataHeader = opDataHeader;
	rxDE_oData.opDataInString = opDataInString;
	
	}


void RxDE::ExpHandler(ExcRxDE &e)
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
bool RxDE::ValidateOp(rxDE_opDataPort &outputPData ,ExcRxDE &e)
{
	string  content;
	content = "";
	e.setErrorId(4);
	using namespace StringUtils;



	bool exceptionFlag = false;
	/*content+=leftJustify("Satellite Number \t",10);
	content+=leftJustify(asString(outputPData.prn.id), 6);


	content += leftJustify("\n",3);

		exceptionFlag = false;
		content +=leftJustify("High General Relativity Value" ,40);
		content += leftJustify("\n",3);
		e.addText(content);
		e.addLocation(FILE_LOCATION);
		error_number.push_back(4);*/

	return(exceptionFlag);
}

std::string RxDE::print_content(rxDE_opDataPort &ostruct)

{
 string  content;
 content = "";
 using namespace StringUtils;
	           content += leftJustify(asString((double)norm1), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(ostruct.rx_time.printf("%5.12s"),17);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.rxClkBias), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.rxClkDrift), 19);
			   content += leftJustify("\t",3);
			    return(content);
}

std::string RxDE::print_header()
{
 string  header;
 header = "";
 using namespace StringUtils;

 			   header += leftJustify("AWGN PROCESS 1 ", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("Time", 17);
			   header += leftJustify("\t",3);
			   header += leftJustify("Receiver Time corr", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("Receiver Phase cor", 19);
			   header += leftJustify("\t",3);
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
