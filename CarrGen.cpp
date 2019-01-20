
/* * File name		 : CarrGen.cpp
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
#include "CarrGen.hpp"
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
void CarrGen::GetPassiveInput(carrGen_passiveInput &ref)
{
carrGen_probe.s_psvParam = false;
p_passiveData = ref;

carrGen_probe.s_psvParam = true;


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
void CarrGen::GetPassiveParam(carrGen_passiveParam &ref)
{
	carrGen_probe.s_psvParam = false;

	p_passiveControl = ref;

	carrGen_probe.s_psvParam = true;
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

void CarrGen::GetActiveInput(carrGen_activeInput  &ref)
{
carrGen_probe.s_actInput = false;
p_activeData = ref;
carrGen_probe.s_actInput = true;

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

void CarrGen::GetActiveParam(carrGen_activeParam  &ref)
{
carrGen_probe.s_actParam = false;
p_activeControl = ref;
carrGen_probe.s_actParam = true;
}

/*----------------------------------------------------------------------------------------------
 * Function					: void SetInitialStateofEngine() throw(ExcCarrGen)
 * Abstract					: This perform the sets the initial  input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCarrGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/



void CarrGen::SetInitialStateofEngine()
{
	carrGen_probe.s_engineControlPsv=false;
	
	// This create a dictionary of all PRN CODE so that each time 
	// Code gen need not be calculated  as soon as object is initialized
	// this dictionary is created, Also this dictionary is 
	// available on PORT2 of this engine if incase its needed

	GenerateBaseCarrier(baseCarrierI,baseCarrierQ);
	carrGen_probe.s_engineControlPsv=true;


}
/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCarrGen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCarrGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void CarrGen::SetActiveStateofEngine()
{	 carrGen_probe.s_engineControlAct=false;
	
	/*map<SatID, boost::circular_buffer<int>>::iterator it_sat;
	it_sat=codeDictionary.find(p_activeData.satid);*/
	currentSatCarrbufferI = baseCarrierI;
	currentSatCarrbufferQ = baseCarrierQ;
    carrGen_probe.s_engineControlAct=true;
	carrGen_pData.lastCycle_I.clear();
	carrGen_pData.lastCycle_Q.clear();

}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCarrGen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCarrGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void CarrGen:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //carrGen_probe = 0x0000;

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
 * Function					: void VerifyIpToEngine() throw(ExcCarrGen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCarrGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void CarrGen::VerifyIpToEngine() throw(ExcCarrGen)
{
	ExcCarrGen e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{
	carrGen_probe.s_verEngine= false;
	/* Verify  the Input Elevation */


      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;

	carrGen_probe.s_verEngine= true;

	}
	catch(ExcCarrGen &e)
	{
		carrGen_probe.s_verEngine= false;
		ExpHandler(e);

	}

   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(ExcCarrGen)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on active parameter and state
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCarrGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void CarrGen::MethodOfEngine()throw(ExcCarrGen)
{
// This updates the Rx IF frequency depending on the downconverter used
GenerateBeatFrequency();

GetGeneratedCarr(carrGen_pData,currentSatCarrbufferI,currentSatCarrbufferQ, p_activeData.initCoarsePhase, p_activeData.initCarrierFinePhase, rxIF);
carrGen_pData.rxIF = rxIF;
}


/*----------------------------------------------------------------------------------------------
 * Function					: void CarrGenCA_L1(int *ptr_to_code,int tap1,int tap2)
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



void CarrGen::ValidateOpOfEngine() throw(ExcCarrGen)
{
	bool exceptionFlag;
	ExcCarrGen e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	carrGen_probe.s_valEngine= false;
	exceptionFlag=ValidateOp(carrGen_pData ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			carrGen_probe.s_valEngine= true;
			}
		catch(ExcCarrGen &e1)
	{
		carrGen_probe.s_valEngine= false;
		//CarrGen_ExpHandler(e);

	}




   }



/*----------------------------------------------------------------------------------------------
 * Function					: void CarrGenCA_L1(int *ptr_to_code,int tap1,int tap2)
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

void CarrGen::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	SatID temp_satid;
	carrGen_Data = carrGen_pData;
	s<<el;


    }


	void CarrGen::OutputData()
	{
	carrGen_Data = carrGen_pData;
	}


void CarrGen::ExpHandler(ExcCarrGen &e)
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
bool CarrGen::ValidateOp(carrGen_opDataPort &outputPData ,ExcCarrGen &e)
{
	string  content;
	content = "";
	e.setErrorId(4);
	using namespace StringUtils;
	bool exceptionFlag = false;
	content+=leftJustify("Satellite Number \t",10);
	content+=leftJustify(asString(outputPData.prn.id), 6);
	content += leftJustify("\n",3);

		exceptionFlag = false;
		content +=leftJustify("High General Relativity Value" ,40);
		content += leftJustify("\n",3);
		e.addText(content);
		e.addLocation(FILE_LOCATION);
		error_number.push_back(4);

	return(exceptionFlag);
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
void CarrGen::GenerateBaseCarrier(boost::circular_buffer<double>& carrArrayI,
								 boost::circular_buffer<double>& carrArrayQ)
{
	
    carrArrayI.clear(); // Clear the buffer
	carrArrayI.set_capacity(CARR_CYCLCE_RESOLUTION);
	carrArrayQ.clear(); // Clear the buffer
	carrArrayQ.set_capacity(CARR_CYCLCE_RESOLUTION);
	

int i;
double temp_cos,temp_sin;
for( i = M_ZERO ; i < CARR_CYCLCE_RESOLUTION ; i++)
	{

		
		//Forcing the value of sin (pi) and sin(0) to hard zero
		
		if((i==CARR_CYCLCE_RESOLUTION/4)||(i==3*CARR_CYCLCE_RESOLUTION/4))
		temp_cos= M_ZERO;
		else
		temp_cos =            cos((M_TWO * gpstk::PI * i)/CARR_CYCLCE_RESOLUTION);
		if((i==M_ZERO)||(i==CARR_CYCLCE_RESOLUTION/M_TWO))
		temp_sin = M_ZERO;
		else
		temp_sin =  sin((M_TWO * gpstk::PI * i)/CARR_CYCLCE_RESOLUTION);
		
		
		
		carrArrayI.push_back(temp_cos);
		carrArrayQ.push_back(temp_sin);

	}


}


/*----------------------------------------------------------------------------------------------
 * Function					: void CarrGenCA_L1(int *ptr_to_code,int tap1,int tap2)
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
void CarrGen:: GetGeneratedCarr(carrGen_opDataPort& generatedCarr, 
								boost::circular_buffer<double> & carrArrayI,
								boost::circular_buffer<double> & carrArrayQ,
								double& initCoarsePhase,
								double& initCarrierFinePhase,
								double& satCarrClk)
{

boost::circular_buffer<double> tempArray(carrArrayI);
double tempCarrCycles = satCarrClk;

double totalCompleteCycles = std::floor(tempCarrCycles);
double fractionalCycles = tempCarrCycles - totalCompleteCycles;

double totalPhaseInFracCycle = fractionalCycles * CARR_CYCLCE_RESOLUTION; // 1023 points make a cycle
double integerPhase = std::floor(totalPhaseInFracCycle);
double fracphase = totalPhaseInFracCycle - integerPhase;

/* Resolve for fractional slew*/
double carrierFinePhase = fracphase + initCarrierFinePhase;
if(carrierFinePhase >= M_ONE)
{
	carrierFinePhase = carrierFinePhase - M_ONE;
    integerPhase = integerPhase + M_ONE;
}


boost::circular_buffer<double>::iterator phaseIndex , phaseIndexLast,phaseIndexQ;

tempArray =carrArrayI;

phaseIndex = carrArrayI.begin() + int(initCoarsePhase);
phaseIndexQ =carrArrayQ.begin() + int(initCoarsePhase);

carrArrayI.rotate(phaseIndex); // Change the starting point of circularbuffer
carrArrayQ.rotate(phaseIndexQ); // Change the starting point of circularbuffer
//double tempfirst[CARR_CYCLCE_RESOLUTION];

int i;
for(phaseIndex = carrArrayI.begin(),i=0;phaseIndex !=carrArrayI.end();)
 {
	 //phaseIndexLast = phaseIndex +Q_OFFSET;
	 generatedCarr.firstCycle_I[i] = *phaseIndex;
	 //tempfirst[i] = *phaseIndex;
	 phaseIndex++;
	 i++;
 }
for(phaseIndexQ = carrArrayQ.begin(),i=0;phaseIndexQ !=carrArrayQ.end();)
 {
	 	 generatedCarr.firstCycle_Q[i] = *(phaseIndexQ);
		phaseIndexQ++;
	 i++;
 }

double carrEndIndex = initCoarsePhase + integerPhase;
if(carrEndIndex >= CARR_CYCLCE_RESOLUTION)
{
	carrEndIndex = carrEndIndex - CARR_CYCLCE_RESOLUTION;
    totalCompleteCycles = totalCompleteCycles + M_ONE;
}


tempArray  = carrArrayI;
phaseIndex = tempArray.begin();
phaseIndexLast =  tempArray.begin()+ int(integerPhase);
//double tempLast[CARR_CYCLCE_RESOLUTION];
 
for(phaseIndex = tempArray.begin();phaseIndex !=phaseIndexLast;)
 {
	 generatedCarr.lastCycle_I.push_back(*phaseIndex);
	 //generatedCarr.lastCycle_Q.push_back(*phaseIndex);
//	 tempLast[i]=*phaseIndex;
	 phaseIndex++;
	
 }
 
tempArray  = carrArrayQ;
phaseIndex = tempArray.begin();
phaseIndexLast =  tempArray.begin()+ int(integerPhase);
for(phaseIndex = tempArray.begin();phaseIndex !=phaseIndexLast;)
 {
	 generatedCarr.lastCycle_Q.push_back(*phaseIndex);
//	 tempLast[i]=*phaseIndex;
	 phaseIndex++;
	
 }
 
generatedCarr.startIndex = initCoarsePhase;
generatedCarr.residueCoarsePhase = carrEndIndex;
generatedCarr.residueFinePhase = carrierFinePhase;
generatedCarr.totalCompleteCycles=totalCompleteCycles;

}

void CarrGen::GenerateBeatFrequency()
{

double rfTCXO =  p_passiveControl.rxConstClk;
if( p_activeControl.useRxclkModel)
rfTCXO = p_activeData.rxClock;
 
double rxBeatFreq1 =  p_passiveControl.IF1mult * rfTCXO;
double rxIF1 =  p_activeData.satCarrClk - rxBeatFreq1;
double rxBeatFreq2 =  p_passiveControl.IF2mult * rfTCXO;
rxIF =  rxIF1 - rxBeatFreq2;
// this gives no of carrier cycles to be generated per code bit

}
