
/* * File name		 : CNCgen.cpp
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
#include "CNCgen.hpp"
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
void CNCgen::GetPassiveInput(cncGen_passiveInput &ref)
{
cncGen_probe.s_psvParam = false;
p_passiveData = ref;

cncGen_probe.s_psvParam = true;


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
void CNCgen::GetPassiveParam(cncGen_passiveParam &ref)
{
	cncGen_probe.s_psvParam = false;

	p_passiveControl = ref;

	cncGen_probe.s_psvParam = true;
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

void CNCgen::GetActiveInput(cncGen_activeInput  &ref)
{
cncGen_probe.s_actInput = false;
p_activeData = ref;
cncGen_probe.s_actInput = true;

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

void CNCgen::GetActiveParam(cncGen_activeParam  &ref)
{
cncGen_probe.s_actParam = false;
p_activeControl = ref;
cncGen_probe.s_actParam = true;

}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCNCgen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCNCgen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/



void CNCgen::SetInitialStateofEngine()
{
	cncGen_probe.s_engineControlPsv=false;



		cncGen_probe.s_engineControlPsv=true;


}
/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCNCgen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCNCgen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void CNCgen::SetActiveStateofEngine()
{

	   cncGen_probe.s_engineControlAct=true;

}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCNCgen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCNCgen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void CNCgen:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //cncGen_probe = 0x0000;

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
 * Function					: void VerifyIpToEngine() throw(ExcCNCgen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCNCgen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void CNCgen::VerifyIpToEngine() throw(ExcCNCgen)
{
	ExcCNCgen e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{
	cncGen_probe.s_verEngine= false;
	/* Verify  the Input Elevation */


      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;

	cncGen_probe.s_verEngine= true;

	}
	catch(ExcCNCgen &e)
	{
		cncGen_probe.s_verEngine= false;
		ExpHandler(e);

	}

   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(ExcCNCgen)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on active parameter and state
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCNCgen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void CNCgen::MethodOfEngine()throw(ExcCNCgen)
{




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



void CNCgen::ValidateOpOfEngine() throw(ExcCNCgen)
{
	bool exceptionFlag;
	ExcCNCgen e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	cncGen_probe.s_valEngine= false;
	exceptionFlag=ValidateOp(cncGen_pData ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			cncGen_probe.s_valEngine= true;
			}
		catch(ExcCNCgen &e1)
	{
		cncGen_probe.s_valEngine= false;
		//CNCgen_ExpHandler(e);

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

void CNCgen::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	SatID temp_satid;


	cncGen_Data = cncGen_pData;
	s<<el;


    }


	void CNCgen::OutputData()
	{
	cncGen_Data = cncGen_pData;
	}


void CNCgen::ExpHandler(ExcCNCgen &e)
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
bool CNCgen::ValidateOp(cncGen_opDataPort &outputPData ,ExcCNCgen &e)
{
	string  content;
	content = "";
	e.setErrorId(4);
	using namespace StringUtils;
	bool exceptionFlag = false;
	content+=leftJustify("Satellite Number \t",10);
	content+=leftJustify(asString(outputPData.prn.id), 6);
	content += leftJustify("\n",3);

		exceptionFlag = true;
		content +=leftJustify("High General Relativity Value" ,40);
		content += leftJustify("\n",3);
		e.addText(content);
		e.addLocation(FILE_LOCATION);
		error_number.push_back(4);

	return(exceptionFlag);
}

/*----------------------------------------------------------------------------------------------
 * Function					: void CodeGenCA_L1(boost::circular_buffer<int> *prnCodeBuffer,int tap1,int tap2)
 * Abstract					: This function generate the GPS CA code for L1 GPS satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: pointer to prnCodeBuffer& tap values
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
void CNCgen:: CodeGenCA_L1(boost::circular_buffer<int> *prnCodeBuffer,int tap1,int tap2)
{
	int i,j;
	int inp1,inp2;
	int temp1,temp2;
	int g1_reg[10],g2_reg[10]; //G1 and G2 generators 10 bits

    int tempint ;
	for(i=0;i<10;i++)
	{
		g1_reg[i]=1;//Initialise G1 and G2 registers
		g2_reg[i]=1;
	}

	tap1--;
	tap2--;

	for(i=0;i<CA_CODE_LENGTH;i++)
	{
		temp1=g1_reg[9];
		temp2=g2_reg[tap1]^g2_reg[tap2];
		tempint= temp1^temp2;
		if(!tempint)
		tempint = -1;

		//tempint = i; //Debug
		prnCodeBuffer->push_back(tempint);


		inp1= g1_reg[2]^g1_reg[9];
		inp2=g2_reg[1]^g2_reg[2]^g2_reg[5]^g2_reg[7]^g2_reg[8]^g2_reg[9];

		for(j=9;j>=1;j--)
		{
			g1_reg[j]=g1_reg[j-1];//Shift the registers
			g2_reg[j]=g2_reg[j-1];
		}

		g1_reg[0]=inp1;
		g2_reg[0]=inp2;
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
void CNCgen:: GlonassCodeGen(int *ptr_to_code)
{
	int i,j;
	int inpbit;
	int g1_reg[9];

	for(i=0;i<9;i++)
	{
		g1_reg[i]=1;//Initialise G1 register
	}

	for(i=0;i<GLONASS_CODE_SIZE;i++)
	{
		ptr_to_code[i]=g1_reg[6];
		inpbit=g1_reg[4]^g1_reg[8];
		for(j=8;j>=1;j--)
			g1_reg[j] = g1_reg[j-1];

		g1_reg[0]=inpbit;
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

void CNCgen:: CodeSwitcher(int sat_index,boost::circular_buffer<int> *code_buffer)
{

	int taps1[]={2,3,4,5,1,2 ,1,2,3 ,2,3,5,6,7,8,9 ,1,2,3,4,5,6,1,4,5,6,7,8 ,1,2,3,4,5 ,4, 1 ,2,4};
	int taps2[]={6,7,8,9,9,10,8,9,10,3,4,6,7,8,9,10,4,5,6,7,8,9,3,6,7,8,9,10,6,7,8,9,10,10,7,8,10};

    CodeGenCA_L1(code_buffer,taps1[sat_index -1],taps2[sat_index -1]);


		//GlonassCodeGen(code_buffer);

}

/*----------------------------------------------------------------------------------------------
 * Function					: void GenerateCAcodeArray(int satId ,double initCodePhase)
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

cncGen_caCodeArray& CNCgen:: GenerateCAcodeArray(int satId ,double initCodePhase)
{
	prnCodeBuffer.codeArray.clear(); // Clear the buffer
	prnCodeBuffer.codeArray.set_capacity(CA_CODE_LENGTH);

	double tempSlew = initCodePhase * CA_CODE_LENGTH;
	int coarseSlew = int(floor(tempSlew));
	double fineSlew = tempSlew - coarseSlew;
// Set Initial code phase for the satellite code
	prnCodeBuffer.prvSlew=coarseSlew;
	prnCodeBuffer.prvSlewPhase=fineSlew;


	CodeSwitcher(satId,&prnCodeBuffer.codeArray);

	return(prnCodeBuffer);

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
void CNCgen:: GetGeneratedCode(cncGen_caCodeArray &tempCodeBuffer, double codeFrequency, cncGen_opDataPort &cncGen_Data)
{
/*int satNum = prn-1;
cncGen_caCodeArray tempCodeBuffer;
tempCodeBuffer = bigCodeArray[satNum];*/

boost::circular_buffer<int> tempArray(tempCodeBuffer.codeArray);

double tempCodeblocks = codeFrequency/CA_CODE_LENGTH;
int totalCompleteBlocks = int(std::floor(tempCodeblocks));
double fractionalBlocks = tempCodeblocks - floor(tempCodeblocks);
double totalChipsInFracBlock = fractionalBlocks * CA_CODE_LENGTH;
int integerSlew = int(std::floor(totalChipsInFracBlock));
double fracSlew = totalChipsInFracBlock - integerSlew;

/* Resolve for fractional slew*/
double slewPhase = fracSlew + tempCodeBuffer.prvSlewPhase;
if(slewPhase > 1)
{
	slewPhase = slewPhase - 1;
    integerSlew = integerSlew + 1;
}

int codeStartIndex = tempCodeBuffer.prvSlew;

boost::circular_buffer<int>::iterator slewIndex , slewIndexLast;
slewIndex = tempCodeBuffer.codeArray.begin() + codeStartIndex;

//cncGen_Data.firstBlockSlew=slewIndex;
tempCodeBuffer.codeArray.rotate(slewIndex); // Change the starting point of circularbuffer

int i;
 for(slewIndex = tempCodeBuffer.codeArray.begin(),i=0;slewIndex !=tempCodeBuffer.codeArray.end();)
 {
	 cncGen_Data.firstBlock[i] = *slewIndex;
	 slewIndex++;
	 i++;
 }



slewIndex = tempCodeBuffer.codeArray.begin();
slewIndexLast =  tempCodeBuffer.codeArray.begin()+ integerSlew;
tempArray  = tempCodeBuffer.codeArray;
tempArray.assign(slewIndex,slewIndexLast);


 for(slewIndex = tempArray.begin();slewIndex !=tempArray.end();)
 {

	 cncGen_Data.lastblock.push_back(*slewIndex);
	 slewIndex++;
 }



tempCodeBuffer.prvSlew = integerSlew ; // start with the next completed slew and count for
tempCodeBuffer.prvSlewPhase = slewPhase;
cncGen_Data.lastBlockSlew = integerSlew;
cncGen_Data.lastBlockPhase = slewPhase;
cncGen_Data.totalCompleteCodeBlocks=totalCompleteBlocks;
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
cncGen_carrierArray& CNCgen:: GenerateBaseCarrier(double initCarrPhase)
{

    prnCarrBuffer.carrArray.clear(); // Clear the buffer
	prnCarrBuffer.carrArray.set_capacity(CARR_CYCLCE_RESOLUTION);
	double tempPhase = initCarrPhase * CARR_CYCLCE_RESOLUTION;
	int coarsePhase = int(std::floor(tempPhase));
	double finePhase = tempPhase - coarsePhase;

	prnCarrBuffer.prvCoarsePhase=coarsePhase ;
	prnCarrBuffer.prvCarrierFinePhase=finePhase;


int i;
double temp_cos;
for( i = 0 ; i < CARR_CYCLCE_RESOLUTION ; i++)
	{

		temp_cos =  cos((2.0 * gpstk::PI * i)/CARR_CYCLCE_RESOLUTION);
		prnCarrBuffer.carrArray.push_back(temp_cos);

	}

return(prnCarrBuffer);
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
void CNCgen:: GetGeneratedCarrier(cncGen_carrierArray &tempCarrBuffer, double carrierFrequency, cncGen_opDataPort &cncGen_Data)
{
/*int satNum = prn-1;
cncGen_carrierArray tempCarrBuffer;
tempCarrBuffer = bigCarrArray[satNum];*/
boost::circular_buffer<double> tempArray(tempCarrBuffer.carrArray);

double tempCarrCycles = carrierFrequency;
double totalCompleteCycles = (std::floor(tempCarrCycles));
double fractionalCycles = tempCarrCycles - totalCompleteCycles;
double totalPhaseInFracCycle = fractionalCycles * CARR_CYCLCE_RESOLUTION; // 1023 points make a cycle
int integerPhase = int(std::floor(totalPhaseInFracCycle));
double fracphase = totalPhaseInFracCycle - integerPhase;

/* Resolve for fractional slew*/
double carrierFinePhase = fracphase + tempCarrBuffer.prvCarrierFinePhase;
if(carrierFinePhase > 1)
{
	carrierFinePhase = carrierFinePhase - 1;
    integerPhase = integerPhase + 1;



	if(integerPhase > CARR_CYCLCE_RESOLUTION)
	{
		integerPhase = integerPhase - CARR_CYCLCE_RESOLUTION;
	    totalCompleteCycles = totalCompleteCycles + 1;
}


}

int carrStartIndex = tempCarrBuffer.prvCoarsePhase;






boost::circular_buffer<double>::iterator phaseIndex , phaseIndexLast;
phaseIndex = tempCarrBuffer.carrArray.begin() + carrStartIndex;
//cncGen_Data.firstCycleCoarsePhase = phaseIndex;
tempCarrBuffer.carrArray.rotate(phaseIndex); // Change the starting point of circularbuffer

int i;
 for(phaseIndex = tempCarrBuffer.carrArray.begin(),i=0;phaseIndex !=tempCarrBuffer.carrArray.end();)
 {
	 cncGen_Data.firstCycle[i] = *phaseIndex;
	 phaseIndex++;
	 i++;
 }


phaseIndex = tempCarrBuffer.carrArray.begin();
phaseIndexLast =  tempCarrBuffer.carrArray.begin()+ integerPhase;
tempArray  = tempCarrBuffer.carrArray;
tempArray.assign(phaseIndex,phaseIndexLast);


 for(phaseIndex = tempArray.begin();phaseIndex !=tempArray.end();)
 {
	 cncGen_Data.lastCycle.push_back(*phaseIndex);
	 phaseIndex++;
 }

//cncGen_Data.prn.id=satNum + 1;

tempCarrBuffer.prvCoarsePhase = integerPhase ; // start with the next completed slew and count for
tempCarrBuffer.prvCarrierFinePhase = carrierFinePhase;
cncGen_Data.lastCycleCoarsePhase = integerPhase;
cncGen_Data.lastCycleFinePhase = carrierFinePhase;
cncGen_Data.totalCompleteCycles = totalCompleteCycles;

}
