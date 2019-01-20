
/* * File name		 : Modulator.cpp
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
#include "Modulator.hpp"
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
void Modulator::GetPassiveInput(modulator_passiveInput &ref)
{
modulator_probe.s_psvParam = false;
p_passiveData = ref;

modulator_probe.s_psvParam = true;


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
void Modulator::GetPassiveParam(modulator_passiveParam &ref)
{
	modulator_probe.s_psvParam = false;

	p_passiveControl = ref;

	modulator_probe.s_psvParam = true;
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

void Modulator::GetActiveInput(modulator_activeInput  &ref)
{
modulator_probe.s_actInput = false;
p_activeData = ref;
modulator_probe.s_actInput = true;

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

void Modulator::GetActiveParam(modulator_activeParam  &ref)
{
modulator_probe.s_actParam = false;
p_activeControl = ref;
modulator_probe.s_actParam = true;
}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcModulator)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcModulator
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/



void Modulator::SetInitialStateofEngine()
{
	modulator_probe.s_engineControlPsv=false;
	
	// This create a dictionary of all PRN CODE so that each time 
	// Code gen need not be calculated  as soon as object is initialized
	// this dictionary is created, Also this dictionary is 
	// available on PORT2 of this engine if incase its needed

	GenerateBaseCarrier(baseCarrierI,baseCarrierMI);
	modulator_probe.s_engineControlPsv=true;


}
/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcModulator)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcModulator
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void Modulator::SetActiveStateofEngine()
{	 modulator_probe.s_engineControlAct=false;
	
	/*map<SatID, boost::circular_buffer<int>>::iterator it_sat;
	it_sat=codeDictionary.find(p_activeData.satid);*/
	currentSatCarrbufferI = baseCarrierI;
	currentSatCarrbufferMI = baseCarrierMI;
    modulator_probe.s_engineControlAct=true;
	modulator_pData.firstBlockIF.clear();
	modulator_pData.lastBlockIF.clear();

}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcModulator)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcModulator
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void Modulator:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //modulator_probe = 0x0000;

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
 * Function					: void VerifyIpToEngine() throw(ExcModulator)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcModulator
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void Modulator::VerifyIpToEngine() throw(ExcModulator)
{
	ExcModulator e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{
	modulator_probe.s_verEngine= false;
	/* Verify  the Input Elevation */


      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;

	modulator_probe.s_verEngine= true;

	}
	catch(ExcModulator &e)
	{
		modulator_probe.s_verEngine= false;
		ExpHandler(e);

	}

   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(ExcModulator)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on active parameter and state
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcModulator
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void Modulator::MethodOfEngine()throw(ExcModulator)
{
int blk_length = 1023;
string  content;
content = "";
ofstream s3;
using namespace StringUtils;
content +=  leftJustify(asString(12), 2);
content += ".txt";
const char* filename =content.c_str(); 

s3.open(filename,std::ios_base::out,ios_base::_Openprot);
//string data;
using std::endl;
using std::setw;
s3<< std::setprecision(4);  
 
for( int i=M_ZERO; i<1023 ;i++)
{
int val = p_activeData.codeInf->firstBlock[i];
if(!val)
currentSatCarrbufferI = currentSatCarrbufferMI;
	  // set the precision of probe output

s3<<GetGeneratedCarr(modulator_pData,currentSatCarrbufferI,p_activeData.initCoarsePhase, p_activeData.initCarrierFinePhase, p_activeData.rxIF);
p_activeData.initCoarsePhase =modulator_pData.residueCoarsePhase;
p_activeData.initCarrierFinePhase =modulator_pData.residueFinePhase;
}
blk_length = p_activeData.codeInf->lastblock.size();
for(int i=0;i<blk_length;i++)
{
	int val=p_activeData.codeInf->lastblock.at(i);
if(!val)
currentSatCarrbufferI = currentSatCarrbufferMI;
	  // set the precision of probe output

s3<<GetGeneratedCarr(modulator_pData,currentSatCarrbufferI,p_activeData.initCoarsePhase, p_activeData.initCarrierFinePhase, p_activeData.rxIF);
p_activeData.initCoarsePhase =modulator_pData.residueCoarsePhase;
p_activeData.initCarrierFinePhase =modulator_pData.residueFinePhase;
}


}


/*----------------------------------------------------------------------------------------------
 * Function					: void ModulatorCA_L1(int *ptr_to_code,int tap1,int tap2)
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



void Modulator::ValidateOpOfEngine() throw(ExcModulator)
{
	bool exceptionFlag;
	ExcModulator e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	modulator_probe.s_valEngine= false;
	exceptionFlag=ValidateOp(modulator_pData ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			modulator_probe.s_valEngine= true;
			}
		catch(ExcModulator &e1)
	{
		modulator_probe.s_valEngine= false;
		//Modulator_ExpHandler(e);

	}




   }



/*----------------------------------------------------------------------------------------------
 * Function					: void ModulatorCA_L1(int *ptr_to_code,int tap1,int tap2)
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

void Modulator::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	SatID temp_satid;
	modulator_Data = modulator_pData;
	s<<el;


    }


	void Modulator::OutputData()
	{
	modulator_Data = modulator_pData;
	}


void Modulator::ExpHandler(ExcModulator &e)
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
bool Modulator::ValidateOp(modulator_opDataPort &outputPData ,ExcModulator &e)
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
void Modulator::GenerateBaseCarrier(boost::circular_buffer<double>& carrArrayI,
								 boost::circular_buffer<double>& carrArrayMI)
{
	
    carrArrayI.clear(); // Clear the buffer
	carrArrayI.set_capacity(CARR_CYCLCE_RESOLUTION);
	carrArrayMI.clear(); // Clear the buffer
	carrArrayMI.set_capacity(CARR_CYCLCE_RESOLUTION);
	

int i;
double temp_cos;
for( i = M_ZERO ; i < CARR_CYCLCE_RESOLUTION ; i++)
	{

		
		//Forcing the value of sin (pi) and sin(0) to hard zero
		
		temp_cos =            cos((M_TWO * gpstk::PI * i)/CARR_CYCLCE_RESOLUTION);
		carrArrayI.push_back(temp_cos);
		carrArrayMI.push_back(-temp_cos);

	}


}


/*----------------------------------------------------------------------------------------------
 * Function					: void ModulatorCA_L1(int *ptr_to_code,int tap1,int tap2)
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
string Modulator:: GetGeneratedCarr(modulator_opDataPort& generatedCarr, 
								boost::circular_buffer<double> & carrArrayI,
								double& initCoarsePhase,
								double& initCarrierFinePhase,
								double& satCarrClk)
{
string  content;
 content = "";
 using namespace StringUtils;


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
phaseIndex = carrArrayI.begin() + int(initCoarsePhase);
carrArrayI.rotate(phaseIndex); // Change the starting point of circularbuffer

for(phaseIndex = carrArrayI.begin();phaseIndex !=carrArrayI.end();)
 {
	 generatedCarr.firstBlockIF.push_back(*phaseIndex);
	content += leftJustify(asString((double)*phaseIndex), 5);
	content += leftJustify("\n",3);
	 phaseIndex++;
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
 
for(phaseIndex = tempArray.begin();phaseIndex !=phaseIndexLast;)
 {
	 generatedCarr.firstBlockIF.push_back(*phaseIndex);
	 content += leftJustify(asString((double)*phaseIndex), 5);
	 content += leftJustify("\n",3);
	 phaseIndex++;
	
 }

generatedCarr.firstBlockIF.push_back(totalCompleteCycles);
content += leftJustify(asString((double)totalCompleteCycles), 5);
content += leftJustify("\n",3);
generatedCarr.residueCoarsePhase = carrEndIndex;
generatedCarr.residueFinePhase = carrierFinePhase;
return(content);
}







