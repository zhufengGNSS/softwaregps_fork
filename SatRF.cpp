#pragma ident "$Id$"
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
#include "SatRf.hpp"
#include "WGS84Geoid.hpp"
#include "PreciseRange.hpp"


using namespace gpstk;
using namespace std;





void SatRF::InputInfToEngine(Kind kind )
{
 switch(kind)
 {
 case PASSIVE : GetPassiveInput(ref_psv);
 break;
 case ACTIVE : GetActiveInput(ref_act);
 break;
 default: break; // Set the flag as null signifying no input
 }


}



void SatRF::GetPassiveInput(satRF_psvIstruct &ref_psv)
{
satRF_pi = ref_psv;
visSV = 0;
alpha[0] = 0.010e-6;
alpha[1] = 0.000e-6;
alpha[2] = -0.060e-6;
alpha[3] = 0.000e-6;
beta[0] =90e3;
beta[1] =0e3;
beta[2] =-197e3;
beta[3] =0e3;
}


void SatRF::GetActiveInput(satRF_actIstruct &ref_act)
{

satRF_ai = ref_act;


}



void SatRF::InputParamToEngine(Kind kind)
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


void SatRF::GetPassiveParam()
{

int i = 0;



}

void SatRF::GetActiveParam()
{
	satRF_pData.clear();

}


void SatRF:: EngineControl(Kind kind)

{
	switch(kind)
	{ case PASSIVE :

	SetInitialStateofEngine();
	  // Initialize GPS true time to system time then increment the second based on timer expiration

	  break;

	case ACTIVE :
	 SetActiveStateofEngine();

		break;
	default :
		//isThisFirstTick = false;
		break;

	}
	// set debug level etc
}



void SatRF::SetInitialStateofEngine()
{
	// Clear all the satellite code and carrier vector


	for(int i=1; i <= gpstk::MAX_PRN ; i++)
	{
		SatID temp_satid(i,gpstk::SatID::systemGPS);
		codeAndCarrierDictionary[temp_satid].codeVector.codeArray.clear();
		codeAndCarrierDictionary[temp_satid].codeVector.prvSlew = M_ZERO;
		codeAndCarrierDictionary[temp_satid].codeVector.prvSlewPhase = M_ZERO;

		codeAndCarrierDictionary[temp_satid].carrierVector.carrArray.clear();
		codeAndCarrierDictionary[temp_satid].carrierVector.prvCarrierFinePhase =M_ZERO;
		codeAndCarrierDictionary[temp_satid].carrierVector.prvCoarsePhase = M_ZERO;
		codeAndCarrierDictionary[temp_satid].codeCarrierGenerated = false;

		instantCarrierDictionary[temp_satid].carrArray.clear();
		instantCarrierDictionary[temp_satid].prvCarrierFinePhase = M_ZERO;
		instantCarrierDictionary[temp_satid].prvCoarsePhase = M_ZERO;

	}
}


void SatRF::SetActiveStateofEngine()
{
	int N =	satRF_ai.satMde_ref->size();
	satRF_caCodeArray tempcodebuff;
	satRF_carrierArray tempcarrierbuff;
	bool temp_visiblity[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	satRF_pData.clear();

	for (int i=0; i<N; i++)
	{
		currSatid = satRF_ai.satMde_ref->at(i).prn; // This feed only visible satellites
		temp_visiblity[currSatid.id] = true;
		if(!codeAndCarrierDictionary[currSatid].codeCarrierGenerated)
		{
			codeAndCarrierDictionary[currSatid].codeCarrierGenerated = true;
			tempcodebuff = GenerateCAcodeArray(currSatid.id,0);
			tempcarrierbuff=GenerateBaseCarrier(0);
			codeAndCarrierDictionary[currSatid].codeVector = tempcodebuff;
			codeAndCarrierDictionary[currSatid].carrierVector = tempcarrierbuff;
			instantCarrierDictionary[currSatid] =tempcarrierbuff;
			// add the instant carrier dictionary with code phase and cycle;
		}
	}

	for (int i=1; i<= gpstk::MAX_PRN; i++)
	{
	SatID temp_satid(i,gpstk::SatID::systemGPS);
	codeAndCarrierDictionary[temp_satid].codeCarrierGenerated = temp_visiblity[i];

	}

}


void SatRF::VerifyIpToEngine() throw(ExcSatRF)
{
#if 0
	ExcSatMDE e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{

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


	}
	catch(ExcSatMDE &e)
	{
			SatMDE_ExpHandler(e);
	}
	 #endif
   }


void SatRF::MethodOfEngine()
{
	satRF_ostruct temp_data;

	int N =	satRF_ai.satMde_ref->size();
	double code_freq ,carr_freq;
	satRF_caCodeArray tempcodebuff;
satRF_carrierArray tempcarrierbuff;


	for (int i=0; i<N; i++)
	{
		currSatid = satRF_ai.satMde_ref->at(i).prn;
		code_freq = satRF_ai.satDb_ref->at(i).chipFreqCA;
		code_freq =code_freq + satRF_ai.satMde_ref->at(i).codeDopplerCaL1;
		carr_freq = satRF_ai.satDb_ref->at(i).carrFreqL1;
		carr_freq = carr_freq+satRF_ai.satMde_ref->at(i).carrierDopplerL1;
		tempcodebuff = codeAndCarrierDictionary[currSatid].codeVector;
		tempcarrierbuff = codeAndCarrierDictionary[currSatid].carrierVector;
		GetGeneratedCode(tempcodebuff, code_freq,temp_data);
		GetGeneratedCarrier(tempcarrierbuff, carr_freq,temp_data);
		satRF_pData.push_back(temp_data);


		//codeAndCarrierDictionary[currSatid].codeVector.prvSlew = tempcodebuff.prvSlew;
		//codeAndCarrierDictionary[currSatid].codeVector.prvSlewPhase = tempcodebuff.prvSlewPhase;


		//codeAndCarrierDictionary[currSatid].carrierVector.prvCarrierFinePhase =tempcarrierbuff.prvCarrierFinePhase;
		//codeAndCarrierDictionary[currSatid].carrierVector.prvCoarsePhase = tempcarrierbuff.prvCoarsePhase ;

		//codeAndCarrierDictionary[currSatid].codeVector = tempcodebuff ;
		 //codeAndCarrierDictionary[currSatid].carrierVector =tempcarrierbuff ;

	}

CarrierPhaseComputation();


	}

void SatRF::CarrierPhaseComputation()
{

    long integerCycles;
	int N =	satRF_ai.satMde_ref->size();
	double af0 ,af1 , relativity;
	DayTime   toa;
	double delay, transitDelay , ionodelay;


satRF_carrierArray 		tempcarrierbuff;
satRF_carrierInitVals	tempcarrierInitVals;


	for (int i=0; i<N; i++)
	{
		 currSatid = satRF_ai.satMde_ref->at(i).prn;
		 tempcarrierbuff = instantCarrierDictionary[currSatid];
		 toa = satRF_ai.satDb_ref->at(i).toa;
		 af0 = satRF_ai.satDb_ref->at(i).af0;
		 af1 = satRF_ai.satDb_ref->at(i).af1;

		/*
		relativity = satRF_ai.satDb_ref->at(i).relativity;

		 // Phase delay due to satellite clock bias + relativity  //
		 delay = (af0 + relativity);
		 integerCycles = ResolveCyclesAndPhase(delay , gpstk::L1_FREQ ,tempcarrierbuff);

		 // Phase delay due to transit time //
		 transitDelay =  (satRF_ai.satMde_ref->at(i).corrRange)/gpstk::C_GPS_M;
		 double temp = satRF_ai.gpsTrueTime - toa;
		 double temp1 = 1 - transitDelay;
		 temp = temp - transitDelay;
		 temp = temp * af1;


		 delay = temp + temp1;//transitDelay +  af1( gpsTrueTime - toa -transitDelay);

		 integerCycles += ResolveCyclesAndPhase(delay , gpstk::L1_FREQ ,tempcarrierbuff);*/

		  /* Phase delay due to ionosphere */

		 ionodelay = (satRF_ai.satMde_ref->at(i).ionoDelayL1)/(gpstk::C_GPS_M);
		 delay = ionodelay*(1+af1); // total delay w.r.t to sat clock and rate of change of satellite clock
		 integerCycles = ResolveCyclesAndPhase(delay , gpstk::L1_FREQ ,tempcarrierbuff);



		/* Phase delay due to phasewindup */
		//delay  = (satRF_ai.satMde_ref->at(i).phaseWindUp);
		//integerCycles += ResolveCyclesAndPhase(delay , gpstk::L1_FREQ ,tempcarrierbuff);

		/* Phase delay due to multipath */

	    tempcarrierInitVals.initCycles = integerCycles;
	    tempcarrierInitVals.initCoarsePhase = tempcarrierbuff.prvCoarsePhase;
	    tempcarrierInitVals.initCarrierFinePhase = tempcarrierbuff.prvCarrierFinePhase;
		carrierInitVal[currSatid]=tempcarrierInitVals;




	}




	}









void SatRF::outputInfFromEngine(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	satRF_oData = satRF_pData;
	el = print_header();
	el += leftJustify("\n",3);
	s<<el;

	for (int prn=0; prn <satRF_oData.size(); prn++)
      {

		using std::endl;
		using std::setw;
       s << std::setprecision(4);    // set the precision of probe output
	   s << print_content(satRF_oData[prn]);
	}
	/*el = leftJustify("______________________________",24);
	el += leftJustify("\n",3);
	s<<el;
	s << satTime.printf("%Y %03j % 12.6s")<<endl;
	el = leftJustify("______________________________",24);
	el += leftJustify("\n",3);
	s<<el;*/



    }


void SatRF::SatMDE_ExpHandler(ExcSatRF &e)
{
	int i=0;
/*
unsigned long eid = error_number[1];


	switch(eid)
	{
	case 2: minElev = MIN_ELV_ANGLE;
		break;
	case 1: e.terminate();
		break;
	case 0: cout<<"over";
		break;
	}
	*/

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

std::string SatRF::print_content(satRF_ostruct &ostruct)

{
 string  content;
 content = "";
 using namespace StringUtils;
	           content +=  leftJustify(asString(ostruct.prn), 6);
			   content += leftJustify("\t",3);
			     content += leftJustify(asString((double)ostruct.totalCompleteCodeBlocks), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.lastBlockSlew), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.lastBlockPhase), 13);
			   content += leftJustify("\t",3);
			  content += leftJustify(asString((double)ostruct.totalCompleteCycles), 13);
			   content += leftJustify("\t",3);

			   content += leftJustify(asString((double)ostruct.lastCycleCoarsePhase), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.lastCycleFinePhase), 13);
			   content += leftJustify("\n",3);
			  // content += leftJustify(asString(e.sattime),13);
			  // content += leftJustify("\n",3);
			   return(content);
}

std::string SatRF::print_header()
{
 string  header;
 header = "";
 using namespace StringUtils;

 header +=  leftJustify("Sys PRN", 6);
			   header += leftJustify("\t",3);
			   header += leftJustify("First X", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("lase Slew Y", 13);
			   header += leftJustify("\t",3);
			    header += leftJustify("slewphase", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("charrier phase", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("last carr ph", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("last fine phase", 13);

			   header += leftJustify("\n",3);
			  // header += leftJustify(asString(e.sattime),13);
			  // header += leftJustify("\n",3);
			   return(header);
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
void SatRF:: CodeGenCA_L1(boost::circular_buffer<int> *prnCodeBuffer,int tap1,int tap2)
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
void SatRF:: GlonassCodeGen(int *ptr_to_code)
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

void SatRF:: CodeSwitcher(int sat_index,boost::circular_buffer<int> *code_buffer)
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

satRF_caCodeArray& SatRF:: GenerateCAcodeArray(int satId ,double initCodePhase)
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
void SatRF:: GetGeneratedCode(satRF_caCodeArray &tempCodeBuffer, double codeFrequency, satRF_ostruct &op_struct)
{
/*int satNum = prn-1;
satRF_caCodeArray tempCodeBuffer;
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

//op_struct.firstBlockSlew=slewIndex;
tempCodeBuffer.codeArray.rotate(slewIndex); // Change the starting point of circularbuffer

int i;
 for(slewIndex = tempCodeBuffer.codeArray.begin(),i=0;slewIndex !=tempCodeBuffer.codeArray.end();)
 {
	 op_struct.firstBlock[i] = *slewIndex;
	 slewIndex++;
	 i++;
 }



slewIndex = tempCodeBuffer.codeArray.begin();
slewIndexLast =  tempCodeBuffer.codeArray.begin()+ integerSlew;
tempArray  = tempCodeBuffer.codeArray;
tempArray.assign(slewIndex,slewIndexLast);


 for(slewIndex = tempArray.begin();slewIndex !=tempArray.end();)
 {

	 op_struct.lastblock.push_back(*slewIndex);
	 slewIndex++;
 }



tempCodeBuffer.prvSlew = integerSlew ; // start with the next completed slew and count for
tempCodeBuffer.prvSlewPhase = slewPhase;
op_struct.lastBlockSlew = integerSlew;
op_struct.lastBlockPhase = slewPhase;
op_struct.totalCompleteCodeBlocks=totalCompleteBlocks;
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
satRF_carrierArray& SatRF:: GenerateBaseCarrier(double initCarrPhase)
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
void SatRF:: GetGeneratedCarrier(satRF_carrierArray &tempCarrBuffer, double carrierFrequency, satRF_ostruct &op_struct)
{
/*int satNum = prn-1;
satRF_carrierArray tempCarrBuffer;
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
//op_struct.firstCycleCoarsePhase = phaseIndex;
tempCarrBuffer.carrArray.rotate(phaseIndex); // Change the starting point of circularbuffer

int i;
 for(phaseIndex = tempCarrBuffer.carrArray.begin(),i=0;phaseIndex !=tempCarrBuffer.carrArray.end();)
 {
	 op_struct.firstCycle[i] = *phaseIndex;
	 phaseIndex++;
	 i++;
 }


phaseIndex = tempCarrBuffer.carrArray.begin();
phaseIndexLast =  tempCarrBuffer.carrArray.begin()+ integerPhase;
tempArray  = tempCarrBuffer.carrArray;
tempArray.assign(phaseIndex,phaseIndexLast);


 for(phaseIndex = tempArray.begin();phaseIndex !=tempArray.end();)
 {
	 op_struct.lastCycle.push_back(*phaseIndex);
	 phaseIndex++;
 }

//op_struct.prn.id=satNum + 1;

tempCarrBuffer.prvCoarsePhase = integerPhase ; // start with the next completed slew and count for
tempCarrBuffer.prvCarrierFinePhase = carrierFinePhase;
op_struct.lastCycleCoarsePhase = integerPhase;
op_struct.lastCycleFinePhase = carrierFinePhase;
op_struct.totalCompleteCycles = totalCompleteCycles;

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
long SatRF:: ResolveCyclesAndPhase(double delay , double baseFreq ,satRF_carrierArray &tempCarrBuffer )
{
	double tempCarrCycles = baseFreq;

long totalCompleteCycles = long(std::floor(tempCarrCycles * delay));
double fractionalCycles = tempCarrCycles*delay - totalCompleteCycles;

double totalPhaseInFracCycle = fractionalCycles * CARR_CYCLCE_RESOLUTION; // 1023 points make a cycle
int integerPhase = int(std::floor(totalPhaseInFracCycle));
double fracphase = totalPhaseInFracCycle - integerPhase;

/* Resolve for fractional slew*/
double carrierFinePhase = fracphase + tempCarrBuffer.prvCarrierFinePhase;
if(carrierFinePhase > 1)
{
	carrierFinePhase = carrierFinePhase - 1;
    integerPhase = integerPhase + 1;
}

int carrStartIndex = tempCarrBuffer.prvCoarsePhase + integerPhase;

if(carrStartIndex > CARR_CYCLCE_RESOLUTION)
{
	carrStartIndex = carrStartIndex - CARR_CYCLCE_RESOLUTION;
    totalCompleteCycles = totalCompleteCycles + 1;
}

boost::circular_buffer<double>::iterator phaseIndex;
phaseIndex = tempCarrBuffer.carrArray.begin() + carrStartIndex;

currentPhaseValue = *phaseIndex;
tempCarrBuffer.prvCoarsePhase = integerPhase ; // start with the next completed slew and count for
tempCarrBuffer.prvCarrierFinePhase = carrierFinePhase;

return(totalCompleteCycles);
}

