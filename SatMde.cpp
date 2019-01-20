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
#include "SatMde.hpp"
#include "PreciseRange.hpp"


using namespace gpstk;
using namespace std;



void SatMDE::InputInfToEngine(Kind kind )
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



void SatMDE::GetPassiveInput(satMDE_psvIstruct &ref_psv)
{
satMDE_pi = ref_psv;
visSV = 0;

}


void SatMDE::GetActiveInput(satMDE_actIstruct &ref_act)
{

satMDE_ai = ref_act;


}



void SatMDE::InputParamToEngine(Kind kind)
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


void SatMDE::GetPassiveParam()
{




}

void SatMDE::GetActiveParam()
{


}



void SatMDE:: EngineControl(Kind kind)

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




void SatMDE::SetInitialStateofEngine()
{
	// Clear and set initial state variable;
	
	visSV= M_ZERO;
	timeToCompute = true;
	visiblityComputed = false;
	visCounter = M_ZERO;

// Clear / initialize the data base 
	for(int i=1; i <= gpstk::MAX_PRN ; i++)
	{
	satMDE_prvMeas tempPrvMeas;	// this is constructed with default values
	SatID satid(i,gpstk::SatID::systemGPS);
	prvMeas[satid]=tempPrvMeas;
	}
	
  
/* Clear or initialize anydata which is feed back to other Engine in their active state*/

/* Since initial Estimate of transit time is fed back to SatDb engine which is called earlier*/
	
//carrgen.Engine(CarrGen::INITIALIZE);



}


void SatMDE::SetActiveStateofEngine()
{
// clear all the old value of the private data structure;
satMDE_pData.clear();
satMDE_oData.clear();
//visSV = M_ZERO;  

// set the global var for current active inputs that would be used by Engine Algo


// Take care of any counter and flag that are dependedent on Active input
// or Engine independent counters and flag

if(timeToCompute) // Time to Compute Visiblity
{
timeToCompute = false;
// Clear all the database
rejectedSV.resize(0);
visibleSV.resize(0);
visSV = M_ZERO;// reset the visible sv = 0 for visblity computation
// Compute the visiblity and store in database
ComputeVisibility();
}
else
{	
visCounter++;
if(visCounter == TIME_FOR_ELV_COMP)
{
//reset counter
visiblityComputed = false;
visCounter = M_ZERO;
timeToCompute = true;
}

}

}




void SatMDE::VerifyIpToEngine() throw(ExcSatMDE)
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


void SatMDE::MethodOfEngine()
{

satMDE_ostruct temp_mdpdata;
DayTime measTime = satMDE_ai.measTime;
Modulator		modulator("ModulatorSection");
modulator.Engine(CodeGen::INITIALIZE);

totalSat = visibleSV.size();
	
for (int i=0; i<totalSat; i++)
	{
	SatID satid = visibleSV.at(i) ;
	bool visible = true;
	satMDE_clkFreq initClock;
	// initialize locals to zero
	clkL1=initClock;
	clkL2c=initClock;
	clkL5 =initClock;
	double tempRangeRate = M_ZERO;
	
	

     bool measHealth = getMeasurement(satid,satMDE_ai.rxXvt,measTime,satMDE_ai.satDb,satMDE_ai.satDgen,0,true,false);
	// Compute range with only visible sat
	double range =satMDE_ai.satDgen->satDgen_oData.pseudoRange;
	
	double clk = satMDE_ai.satDgen->satDgen_oData.svPosVel.ddtime;
    // Add ionodelay
    LoadChannel(satMDE_ai,satid);
	range += satMDE_ai.channel->channelGPS_oData.ionoDelay;

	if(prvMeas[satid].prvPseudoRange == M_ZERO)
	{ // Put health of measurement = zero
	temp_mdpdata.measHealth = false;
	tempRangeRate = M_ZERO;

	}
	else
	{ // the prvpseudorange is available 
		//RangeRate Calculation
	tempRangeRate = range - prvMeas[satid].prvPseudoRange;
	}
	ComputePhaseAndDoppler(tempRangeRate,clk);
	double codefrequency=clkL1.codeFreq+ clkL1.codeDoppler;
	double carrfrequency=clkL1.carrFreq+ clkL1.carrDoppler;
	CodeGenerator(satMDE_ai.codegen,satid,codefrequency);
	CarrGenerator(satMDE_ai.carrgen,satid,carrfrequency);
	double rxIF_2_txCode = satMDE_ai.carrgen->carrGen_Data.rxIF/codefrequency;
	/*if(satid.id==3)
	{
	modulator.activeData.initCarrierFinePhase=0;
	modulator.activeData.initCoarsePhase=0;
	modulator.activeData.rxIF=rxIF_2_txCode;
	modulator.activeData.codeInf = &satMDE_ai.codegen->codeGen_Data;
	modulator.Engine(Modulator::RUN_METHOD );
	modulator.EngineOutput(Modulator::DATA_HERE);
	}*/
	
	
	prvMeas[satid].prvPseudoRange = range;
	prvMeas[satid].prvSlew =satMDE_ai.codegen->codeGen_Data.residueSlew;
	prvMeas[satid].prvSlewPhase =satMDE_ai.codegen->codeGen_Data.residueSlewPhase;
	prvMeas[satid].prvCoarsePhase =satMDE_ai.carrgen->carrGen_Data.residueCoarsePhase;
	prvMeas[satid].prvCarrierFinePhase =satMDE_ai.carrgen->carrGen_Data.residueFinePhase;



	//Doppler, Phase
	
	//ComputePhaseAndDoppler(tempRangeRate,satMDE_ai.satDb->satoData.clkCorrection);
    
	temp_mdpdata.prn =satid;
	temp_mdpdata.dataNmeas = satMDE_ai.satDgen->satDgen_oData;
	temp_mdpdata.dataCarr=satMDE_ai.carrgen->carrGen_Data;
	temp_mdpdata.dataChannel = satMDE_ai.channel->channelGPS_oData;
	temp_mdpdata.clksL1 =clkL1;
	temp_mdpdata.clksL2c =clkL2c;
	temp_mdpdata.clksL5 =clkL5;
	temp_mdpdata.rangeRate =tempRangeRate;
	temp_mdpdata.rxIF_2_txCode =rxIF_2_txCode;
	satMDE_pData.push_back(temp_mdpdata);
	satMDE_pData1[satid]=temp_mdpdata;

	
	}

}

void SatMDE::ComputePhaseAndDoppler(const double &rangeRate,const double &clkCorrection)
{

	//Get the IONO Correction w.r.t to current satellite frequency
	//Remember Satellite Frequency L1 or L2 or L5 is not fixed as they
	//suffer from the satellite clock bias and jitter
	//Xvt rxXvt = satMDE_ai.rxXvt;

	/* Calculate Param for L1 */
	double satOscFreq = gpstk::OSC_FREQ;
	//satMDE_ai.satDgen->satDgen_oData
    
	satOscFreq =  satOscFreq*(1 + clkCorrection);


	clkL1.carrFreq  = satOscFreq * gpstk::L1_MULT;
	clkL1.codeFreq  = satOscFreq*CODECLKMULT_L1;
	double lambda = C_GPS_M/clkL1.carrFreq ;
	double gpsCar2CodeRatio = clkL1.carrFreq  / clkL1.codeFreq;

	//gpstk::IonoModel::Frequency  freq = L1;

	clkL1.carrDoppler = rangeRate / lambda;
	clkL1.codeDoppler  = clkL1.carrDoppler/gpsCar2CodeRatio;

	/* Calculate Param for L2 */

	clkL2c.carrFreq  = satOscFreq * gpstk::L2_MULT;
	clkL2c.codeFreq  = satOscFreq*CODECLKMULT_L2;
	lambda = C_GPS_M/clkL2c.carrFreq ;
	gpsCar2CodeRatio = clkL2c.carrFreq  / clkL2c.codeFreq;

	//gpstk::IonoModel::Frequency  freq = L2c;

	clkL2c.carrDoppler = rangeRate / lambda;
	clkL2c.codeDoppler  = clkL2c.carrDoppler/gpsCar2CodeRatio;

	 /* Calculate Param for L5 */

	clkL5.carrFreq  = satOscFreq * L5_MULT;
	clkL5.codeFreq  = satOscFreq*CODECLKMULT_L5;
	lambda = C_GPS_M/clkL5.carrFreq ;
	gpsCar2CodeRatio = clkL5.carrFreq  / clkL5.codeFreq;

	//gpstk::IonoModel::Frequency  freq = L5;

	clkL5.carrDoppler = rangeRate / lambda;
	clkL5.codeDoppler  = clkL5.carrDoppler/gpsCar2CodeRatio;

	}



bool SatMDE::getMeasurement (SatID &temp_satid,
							 Position &rxXvt,
							 DayTime  &measTime,
							 SatDb    *satDb,
							 SatDgen *satDgen,
							 double range,
							 bool firstMeas,
							 bool svRange)
 {

satDgen_activeInput activeInput;
satDgen_activeParam activeControl;
activeInput.measTime = measTime;
activeInput.satid = temp_satid;
activeInput.rxPos = rxXvt;
activeInput.calculatedRange = range;
activeInput.satDb = satDb;

activeControl.firstMeas =firstMeas;
activeControl.svRange =svRange;

satDgen->activeControl=activeControl;
satDgen->activeData=activeInput;
satDgen->Engine(SatDgen::RUN_ALL );
satDgen->EngineOutput(SatDgen::DATA_HERE);
return(satDgen->satDgen_probe.h_ofOpData);

 }
 
 
void SatMDE::ComputeVisibility()

 {
 		// Recalculating visiblily for all the satellites
	for(int i=1; i <= gpstk::MAX_PRN ; i++)
	{
	satMDE_prvMeas tempPrvMeas;	
	SatID satid(i,gpstk::SatID::systemGPS);
	bool measHealth =getMeasurement(satid,satMDE_ai.rxXvt,satMDE_ai.measTime,satMDE_ai.satDb,satMDE_ai.satDgen,0,true,false);
	/* neglect SV if the elevation is less than min elv or SV is unhealthy*/
	if(satMDE_ai.satDgen->satDgen_oData.elevation < satMDE_pi.minElev || (!measHealth))
	{
	rejectedSV.push_back(satid);
	prvMeas[satid]=tempPrvMeas;
	//prvMeas[satid].prvPseudoRange=0;
	continue;
	}
	else
	{
	visSV ++;
	visibleSV.push_back(satid);
	}
	}
	visiblityComputed = true;
}

void SatMDE::CodeGenerator(CodeGen   *codegen, SatID &satid ,double& codefrequency)
{
codeGen_activeInput activeData;
activeData.initSlew = prvMeas[satid].prvSlew;
activeData.initSlewPhase = prvMeas[satid].prvSlewPhase;
activeData.satCodeClk = codefrequency;
activeData.satid= satid;

	codegen->activeData=activeData;
	codegen->Engine(CodeGen::RUN_METHOD );
	codegen->EngineOutput(CodeGen::DATA_HERE);
	/*bool datahealth = satDb->satDb_probe.h_ofOpData;
	if(!datahealth)
	satDgen_probe.h_ofOpData=false;
	else
	satDgen_probe.h_ofOpData=true;*/
	

}

void SatMDE::CarrGenerator(CarrGen   *carrgen, SatID &satid ,double& carrfrequency)
{
carrGen_activeInput activeData;
carrGen_activeParam activeControl;
activeData.initCoarsePhase = prvMeas[satid].prvCoarsePhase;
activeData.initCarrierFinePhase = prvMeas[satid].prvCarrierFinePhase;
activeData.satCarrClk = carrfrequency;
activeData.rxClock=0;// modify it
activeData.satid= satid;
activeControl.useRxclkModel = false;



	carrgen->activeData=activeData;
    carrgen->activeControl=activeControl;
	carrgen->Engine(CarrGen::RUN_METHOD );
	carrgen->EngineOutput(CarrGen::DATA_HERE);
	/*bool datahealth = satDb->satDb_probe.h_ofOpData;
	if(!datahealth)
	satDgen_probe.h_ofOpData=false;
	else
	satDgen_probe.h_ofOpData=true;*/
	

}

void SatMDE::LoadChannel(satMDE_actIstruct &p_activeData, SatID &satid)
{
	channelGPS_activeInput activeInput;

activeInput.azimuth = p_activeData.satDgen->satDgen_oData.azimuth;
activeInput.elevation = p_activeData.satDgen->satDgen_oData.elevation;
activeInput.epochTime =p_activeData.measTime;
activeInput.rxPos =p_activeData.rxXvt;
activeInput.satid = satid;
activeInput.satPos =  p_activeData.satDgen->satDgen_oData.svPosVel;
p_activeData.channel->activeData=activeInput;
p_activeData.channel->Engine(ChannelGPS::RUN_METHOD );
p_activeData.channel->EngineOutput(ChannelGPS::DATA_HERE);
}




void SatMDE::outputInfFromEngine(std::ostream &s, SatID &satid , int type )

    {
	string  el;
	el = "";
	

	using namespace StringUtils;

	satMDE_oData = satMDE_pData1;
	
	switch(type)
	{
	case 0: 
	//el += leftJustify("\t",3);
	el += leftJustify("TimeTag",15);
	el += leftJustify("\t",3);
	el += satMDE_oData[satid].dataNmeas.opDataHeader;
	el += satMDE_oData[satid].dataChannel.opDataHeader;
	el += leftJustify("\n",3);
	s<<el;
	
	break;
	
	case 1:
	el += satMDE_ai.measTime.printf("%5.10s");
	el += leftJustify("\t",3);
	el += satMDE_oData[satid].dataNmeas.opDataInString;
	el += satMDE_oData[satid].dataChannel.opDataInString;
	el += leftJustify("\n",3);
	s<<el;
	break;
	
	default :
	el =leftJustify("Wrongparam",3);
	s<<el;
	}
	
	

	/*for (unsigned int prn=0; prn <satMDE_oData.size(); prn++)
      {

		using std::endl;
		using std::setw;
       s << std::setprecision(4);    // set the precision of probe output
	   s << print_content(satMDE_oData[prn]);
	}
	/*el = leftJustify("______________________________",24);
	el += leftJustify("\n",3);
	s<<el;
	s << satTime.printf("%Y %03j % 12.6s")<<endl;
	el = leftJustify("______________________________",24);
	el += leftJustify("\n",3);
	s<<el;*/



    }


void SatMDE::SatMDE_ExpHandler(ExcSatMDE &e)
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


std::string SatMDE::print_content(satMDE_ostruct &ostruct)

{
 string  content;
 content = "";
 using namespace StringUtils;
	           content +=  leftJustify(asString(ostruct.prn), 6);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.dataNmeas.pseudoRange), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.dataNmeas.rawrange), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.rangeRate), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.dataChannel.dllThermalNoise), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.dataChannel.phasewindup), 13);
			   content += leftJustify("\t",3);

			   content += leftJustify(asString((double)ostruct.dataChannel.pllThermalNoise), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)ostruct.dataChannel.ionoDelay), 13);
			   content += leftJustify("\n",3);
			  // content += leftJustify(asString(e.sattime),13);
			  // content += leftJustify("\n",3);
			   return(content);
}

std::string SatMDE::print_header()
{
 string  header;
 header = "";
 using namespace StringUtils;

 header +=  leftJustify("Sys PRN", 6);
			   header += leftJustify("\t",3);
			   header += leftJustify("RawRange X", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("Corr_range Y", 13);
			   header += leftJustify("\t",3);
			    header += leftJustify("Delta Range", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("elevation Z", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("azimuth", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("IONO Delay", 13);

			   header += leftJustify("\n",3);
			  // header += leftJustify(asString(e.sattime),13);
			  // header += leftJustify("\n",3);
			   return(header);
}




 
	
	



