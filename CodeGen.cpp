
/* * File name		 : CodeGen.cpp
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
#include "CodeGen.hpp"
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
void CodeGen::GetPassiveInput(codeGen_passiveInput &ref)
{
codeGen_probe.s_psvParam = false;
p_passiveData = ref;

codeGen_probe.s_psvParam = true;


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
void CodeGen::GetPassiveParam(codeGen_passiveParam &ref)
{
	codeGen_probe.s_psvParam = false;

	p_passiveControl = ref;

	codeGen_probe.s_psvParam = true;
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

void CodeGen::GetActiveInput(codeGen_activeInput  &ref)
{
codeGen_probe.s_actInput = false;
p_activeData = ref;
codeGen_probe.s_actInput = true;

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

void CodeGen::GetActiveParam(codeGen_activeParam  &ref)
{
codeGen_probe.s_actParam = false;
p_activeControl = ref;
codeGen_probe.s_actParam = true;

}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCodeGen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCodeGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/



void CodeGen::SetInitialStateofEngine()
{
	codeGen_probe.s_engineControlPsv=false;
	boost::circular_buffer<int> codeArray;
	// This create a dictionary of all PRN CODE so that each time 
	// Code gen need not be calculated  as soon as object is initialized
	// this dictionary is created, Also this dictionary is 
	// available on PORT2 of this engine if incase its needed
	
	for(int i=1; i <= gpstk::MAX_PRN ; i++)
	{
	SatID satid(i,gpstk::SatID::systemGPS);
	GenerateCAcodeArray(satid,codeArray);
	codeDictionary[satid]=codeArray;
	}

	codeGen_probe.s_engineControlPsv=true;


}
/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCodeGen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCodeGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void CodeGen::SetActiveStateofEngine()
{	 codeGen_probe.s_engineControlAct=false;
	
	/*map<SatID, boost::circular_buffer<int>>::iterator it_sat;
	it_sat=codeDictionary.find(p_activeData.satid);*/
	currentSatCodebuffer = codeDictionary[p_activeData.satid];
	codeGen_pData.prn=p_activeData.satid;
	
	 codeGen_pData.lastblock.clear();
	 codeGen_probe.s_engineControlAct=true;

}

/*----------------------------------------------------------------------------------------------
 * Function					: void VerifyIpToEngine() throw(ExcCodeGen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCodeGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void CodeGen:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //codeGen_probe = 0x0000;

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
 * Function					: void VerifyIpToEngine() throw(ExcCodeGen)
 * Abstract					: This perform the verification of the input & Parameter fed to the
 *							  Engine
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCodeGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void CodeGen::VerifyIpToEngine() throw(ExcCodeGen)
{
#if 0
	ExcCodeGen e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{
	codeGen_probe.s_verEngine= false;
	/* Verify  the Input Elevation */


      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;

	codeGen_probe.s_verEngine= true;

	}
	catch(ExcCodeGen &e)
	{
		codeGen_probe.s_verEngine= false;
		//ExpHandler(e);

	}
#endif
   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(ExcCodeGen)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on active parameter and state
 * Formal
 * Parameter(s) 			: None
 * Exception(s) 			: ExcCodeGen
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void CodeGen::MethodOfEngine()throw(ExcCodeGen)
{

GetGeneratedCode(codeGen_pData,
				currentSatCodebuffer, 
				p_activeData.initSlew,
				p_activeData.initSlewPhase,
				p_activeData.satCodeClk);
//GetGeneratedCode(currentSatCodebuffer);


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



void CodeGen::ValidateOpOfEngine() throw(ExcCodeGen)
{
	bool exceptionFlag;
	ExcCodeGen e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	codeGen_probe.s_valEngine= false;
	exceptionFlag=ValidateOp(codeGen_pData ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			codeGen_probe.s_valEngine= true;
			}
		catch(ExcCodeGen &e1)
	{
		codeGen_probe.s_valEngine= false;
		//CodeGen_ExpHandler(e);

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

void CodeGen::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	SatID temp_satid;


	codeGen_Data = codeGen_pData;
	s<<el;


    }


	void CodeGen::OutputData()
	{
	codeGen_Data = codeGen_pData;
	}


void CodeGen::ExpHandler(ExcCodeGen &e)
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
bool CodeGen::ValidateOp(codeGen_opDataPort &outputPData ,ExcCodeGen &e)
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
void CodeGen:: CodeGenCA_L1(boost::circular_buffer<int> *prnCodeBuffer,int tap1,int tap2)
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
void CodeGen:: GlonassCodeGen(int *ptr_to_code)
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

void CodeGen:: CodeSwitcher(int sat_index,boost::circular_buffer<int> *code_buffer)
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

void CodeGen::GenerateCAcodeArray(SatID satid,boost::circular_buffer<int>& codeArray)
{
	
	codeArray.clear();
	codeArray.set_capacity(CA_CODE_LENGTH);
	
	// See the type of sat ID and decide the code length
	//*** For demo purpose it GPSL1 is chosen
	
	codeArray.set_capacity(CA_CODE_LENGTH);
	int satnum = satid.id;
	CodeSwitcher(satnum,&codeArray);

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
 void CodeGen:: GetGeneratedCode(codeGen_opDataPort& generatedCode,
									boost::circular_buffer<int>& codeArray, 
									int& initSlew, double& initSlewPhase, 
									double& satCodeClk)
{
/*int satNum = prn-1;
codeGen_caCodeArray tempCodeBuffer;
tempCodeBuffer = bigCodeArray[satNum];*/

boost::circular_buffer<int> tempArray(codeArray);


double tempCodeblocks = satCodeClk/CA_CODE_LENGTH;
double totalCompleteBlocks = (std::floor(tempCodeblocks));
double fractionalBlocks = tempCodeblocks - totalCompleteBlocks;
double totalChipsInFracBlock = fractionalBlocks * CA_CODE_LENGTH;
int integerSlew = int(std::floor(totalChipsInFracBlock));
double fracSlew = totalChipsInFracBlock - integerSlew;

/* Resolve for fractional slew*/
double slewPhase = fracSlew + initSlewPhase;
if(slewPhase > M_ONE)
{
	slewPhase = slewPhase - M_ONE;
    integerSlew = integerSlew + M_ONE;
}

int codeStartIndex = initSlew;

boost::circular_buffer<int>::iterator slewIndex , slewIndexLast;
slewIndex = codeArray.begin() + initSlew;

generatedCode.firstBlockSlew=initSlew;
codeArray.rotate(slewIndex); // Change the starting point of circularbuffer

int i;
 for(slewIndex = codeArray.begin(),i=0;slewIndex !=codeArray.end();)
 {
	 generatedCode.firstBlock[i] = *slewIndex;
	 slewIndex++;
	 i++;
 }

 double codeEndIndex = initSlew + integerSlew;
if(codeEndIndex >= CA_CODE_LENGTH)
{
	codeEndIndex = codeEndIndex - CA_CODE_LENGTH;
    totalCompleteBlocks = totalCompleteBlocks + M_ONE;
}
 
tempArray  = codeArray;
slewIndex = tempArray.begin();
slewIndexLast =  tempArray.begin()+ int(integerSlew);

 
 /*slewIndex = codeArray.begin();
slewIndexLast =codeArray.begin()+ integerSlew;
tempArray  = codeArray;
tempArray.assign(slewIndex,slewIndexLast);*/


 for(slewIndex = tempArray.begin();slewIndex !=slewIndex;)
 {

	 generatedCode.lastblock.push_back(*slewIndex);
	 slewIndex++;
 }

generatedCode.residueSlew = codeEndIndex;
generatedCode.residueSlewPhase = slewPhase;
generatedCode.totalCompleteCodeBlocks=totalCompleteBlocks;

}
