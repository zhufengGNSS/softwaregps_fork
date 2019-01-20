
/* * File name		 : ChannelGPS.cpp
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
#include "ChannelGPS.hpp"
#include "PreciseRange.hpp"
#include "GaussianDistribution.hpp"
//#include "Stats.hpp"

//boost lib for themalnoise
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
void ChannelGPS::GetPassiveInput(channelGPS_passiveInput &ref)
{
channelGPS_probe.s_psvParam = false;
p_passiveData = ref;

channelGPS_probe.s_psvParam = true;


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
void ChannelGPS::GetPassiveParam(channelGPS_passiveParam &ref)
{
	channelGPS_probe.s_psvParam = false;

	p_passiveControl = ref;

	channelGPS_probe.s_psvParam = true;
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

void ChannelGPS::GetActiveInput(channelGPS_activeInput  &ref)
{
channelGPS_probe.s_actInput = false;
p_activeData = ref;
channelGPS_probe.s_actInput = true;

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

void ChannelGPS::GetActiveParam(channelGPS_activeParam  &ref)
{
channelGPS_probe.s_actParam = false;
p_activeControl = ref;
channelGPS_probe.s_actParam = true;

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



void ChannelGPS::SetInitialStateofEngine()
{		
	channelGPS_probe.s_engineControlPsv=false;
	opDataHeader = print_header();

	if(!passiveControl.useExtIono)
		{
	p_passiveData.alpha[0] = 0.010e-6;
	p_passiveData.alpha[1] = 0.000e-6;
	p_passiveData.alpha[2] = -0.060e-6;
	p_passiveData.alpha[3] = 0.000e-6;
	p_passiveData.beta[0] =90e3;
	p_passiveData.beta[1] =0e3;
	p_passiveData.beta[2] =-197e3;
	p_passiveData.beta[3] =0e3;
		}

	
		channelGPS_probe.s_engineControlPsv=true;


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


void ChannelGPS::SetActiveStateofEngine()
{

	   channelGPS_probe.s_engineControlAct=true;

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

void ChannelGPS:: EngineControl(Kind kind)

{


	switch(kind)
	{ case PASSIVE : // These parameters change only once during initialization or during Exception handling
     //channelGPS_probe = 0x0000;

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
 * Exception(s) 			: ExcChannelGPS
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void ChannelGPS::VerifyIpToEngine() throw(ExcChannelGPS)
{
	ExcChannelGPS e("Verification" , 0 ,gpstk::Exception::recoverable);
	e.setErrorId(1);
	try
	{
	channelGPS_probe.s_verEngine= false;
	/* Verify  the Input Elevation */

	if (yumaReader.eph == NULL)
   {
      e.addText("Didn't get any Orbital data from the Yuma files. ");
	  e.addLocation(FILE_LOCATION);
	  error_number.push_back(2);
	     throw e;
	}
	channelGPS_probe.s_verEngine= true;

	}
	catch(ExcChannelGPS &e)
	{
		channelGPS_probe.s_verEngine= false;
		ExpHandler(e);

	}

   }

/*----------------------------------------------------------------------------------------------
 * Function					: void MethodOfEngine() throw(<engine Exception>)
 * Abstract					: This perform the algorithm of engine under particular state decided
 *							  by Engine controller based on  parameters and state
 * Formal
 * Parameter(s) 			: None               
 * Exception(s) 			: ExcChannelGPS
 * Return value 			: None
 * System Call				: Exception handler
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/


void ChannelGPS::MethodOfEngine()throw(ExcChannelGPS)
{
   	// ionocorrection in meters
	double ionoError =  getIonoCorrections( p_passiveData.alpha,
										    p_passiveData.beta,
										    p_activeData.epochTime,
											p_activeData.rxPos,
											p_activeData.elevation,
											p_activeData.azimuth,
											gpstk::IonoModel::L1);
   	// ionocorrection in seconds
   channelGPS_pData.ionoDelay	= 		ionoError;
 
   channelGPS_pData.phasewindup = ComputePhaseWindUpDelay(p_activeData.satid,
												p_activeData.epochTime,
												p_activeData.satPos,
												p_activeData.rxPos);
    
	//generate the thermal noise w.r.t C/NO and chip spacing
	
	getThermalNoise(p_passiveData.loopBWpll,p_passiveData.c_n0,
					p_passiveData.loopBWdll,p_passiveData.chipWidth,
				    p_passiveData.armSpacing ,p_passiveData.predetectIT);

					channelGPS_pData.pllThermalNoise =noisePll;
					channelGPS_pData.dllThermalNoise =noiseDll; 	
					

					
												
			

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



void ChannelGPS::ValidateOpOfEngine() throw(ExcChannelGPS)
{
	bool exceptionFlag;
	ExcChannelGPS e1("Validation" , 0 ,gpstk::Exception::recoverable);
	e1.setErrorId(1);

	channelGPS_probe.s_valEngine= false;
	exceptionFlag=ValidateOp(channelGPS_oData ,e1);
			try
			{
			if(exceptionFlag)
			throw e1;
			else
			channelGPS_probe.s_valEngine= true;
			}
		catch(ExcChannelGPS &e1)
	{
		channelGPS_probe.s_valEngine= false;
		//ChannelGPS_ExpHandler(e);

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

void ChannelGPS::OutputData(std::ostream & 	s )

    {
	string  el;
	el = "";
	using namespace StringUtils;

	SatID temp_satid;


	channelGPS_oData = channelGPS_pData;
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

	void ChannelGPS::OutputData()
	{
	channelGPS_oData = channelGPS_pData;
	/* Actually this need to be put in validation stage */
	opDataInString = print_content(channelGPS_oData);
	channelGPS_oData.opDataHeader = opDataHeader;
	channelGPS_oData.opDataInString = opDataInString;
	
	}


/*----------------------------------------------------------------------------------------------
 * Function					: ExpHandler(<enginename_Excp &e)
 * Abstract					: This handles all the exceptions at various stages of the engine
 *							: processing and do appropriate decision
 * Formal
 * Parameter(s) 			: exception vector
 * Exception(s) 			: ExcChannelGPS
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: String utilities
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/

void ChannelGPS::ExpHandler(ExcChannelGPS &e)
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
 * Exception(s) 			: ExcChannelGPS
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: gpstk exception library
 * Member variables accessed: All output dataport members; see the header files <enginename>Types.hpp
 * Assumptions 				: The range check is just sanity check it only ensures that output is not
 *							: beyond the range, it doesnot guarntee the logical value of output
 ---------------------------------------------------------------------------------------------*/
bool ChannelGPS::ValidateOp(channelGPS_opDataPort &outputPData ,ExcChannelGPS &e)
{
	string  content;
	content = "";
	e.setErrorId(4);
	using namespace StringUtils;
	bool exceptionFlag = false;
	/*content+=leftJustify("Satellite Number \t",10);
	content+=leftJustify(asString(outputPData.prn.id), 6);
	content += leftJustify("\n",3);

		exceptionFlag = true;
		content +=leftJustify("High General Relativity Value" ,40);
		content += leftJustify("\n",3);
		e.addText(content);
		e.addLocation(FILE_LOCATION);
		error_number.push_back(4);*/

	return(exceptionFlag);
}

double ChannelGPS::ComputePhaseWindUpDelay(SatID &satid ,DayTime &time , Xvt  &satPos, Position &rxPos)
{

SunPosition sunPosition;
Triple sunPos(sunPosition.getPosition(time));
Triple svPos 	=   satPos.x;
//Returns phase Windup in radians
return(getWindUp(satid, time, svPos, sunPos ,rxPos));
}


/*----------------------------------------------------------------------------------------------
 * Function					: void getWindUp( const SatID& satid,
 *                           const DayTime& time,
 *                           const Triple& sat,
 *                           const Triple& sunPosition,
 *							const Position& posRx)
 * Abstract					: This function generate phase windup error for the satellite
 *							  given the satellite taps.
 * Formal
 * Parameter(s) 			: time,sat postion ,sunPosition,posRx
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: gpstk getWindUp
 * Member variables accessed: None
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
// Returns in radians
double ChannelGPS::getWindUp( const SatID& satid,
                                    const DayTime& time,
                                    const Triple& sat,
                                    const Triple& sunPosition,
									const Position& posRx)
   {

         // Unitary vector from satellite to Earth mass center
      Triple rk( ( (-1.0)*(sat.unitVector()) ) );

         // Unitary vector from Earth mass center to Sun
      Triple ri( sunPosition.unitVector() );

         // rj = rk x ri
      Triple rj(rk.cross(ri));

         // ri = rj x rk
      ri = rj.cross(rk);

         // Let's convert ri, rj to unitary vectors.
         // Now ri, rj, rk form a base in the ECEF reference frame
      ri = ri.unitVector();
      rj = rj.unitVector();


         // Get satellite rotation angle

         // Get vector from Earth mass center to receiver
      Triple rxPos(posRx.X(), posRx.Y(), posRx.Z());

         // Compute unitary vector vector from satellite to RECEIVER
      Triple rrho( (rxPos-sat).unitVector() );

         // Vector from SV to Sun center of mass
      Triple gps_sun( sunPosition-sat );

         // Redefine rk: Unitary vector from SV to Earth mass center
      rk = (-1.0)*(sat.unitVector());

         // Redefine rj: rj = rk x gps_sun, then make sure it is unitary
      rj = (rk.cross(gps_sun)).unitVector();

         // Redefine ri: ri = rj x rk, then make sure it is unitary
         // Now, ri, rj, rk form a base in the satellite body reference
         // frame, expressed in the ECEF reference frame
      ri = (rj.cross(rk)).unitVector();


         // Projection of "rk" vector to line of sight vector (rrho)
      double zk(rrho.dot(rk));

         // Get a vector without components on rk (i.e., belonging
         // to ri, rj plane)
      Triple dpp(rrho-zk*rk);

         // Compute dpp components in ri, rj plane
      double xk(dpp.dot(ri));
      double yk(dpp.dot(rj));

         // Compute satellite rotation angle, in radians
      double alpha1(std::atan2(yk,xk));


         // Get receiver rotation angle

         // Redefine rk: Unitary vector from Receiver to Earth mass center
      rk = (-1.0)*(rxPos.unitVector());

         // Let's define a NORTH unitary vector in the Up, East, North
         // (UEN) topocentric reference frame
      Triple delta(0.0, 0.0, 1.0);

         // Rotate delta to XYZ reference frame
      delta =
         (delta.R2(tempPos.geodeticLatitude())).R3(-tempPos.longitude());


         // Computation of reference trame unitary vectors for receiver
         // rj = rk x delta, and make it unitary
      rj = (rk.cross(delta)).unitVector();

         // ri = rj x rk, and make it unitary
      ri = (rj.cross(rk)).unitVector();

         // Projection of "rk" vector to line of sight vector (rrho)
      zk = rrho.dot(rk);

         // Get a vector without components on rk (i.e., belonging
         // to ri, rj plane)
      dpp = rrho-zk*rk;

         // Compute dpp components in ri, rj plane
      xk = dpp.dot(ri);
      yk = dpp.dot(rj);

         // Compute receiver rotation angle, in radians
      double alpha2(std::atan2(yk,xk));

      double wind_up(0.0);

         // Find out if satellite belongs to block "IIR", because
         // satellites of block IIR have a 180 phase shift
int IIR_Sat[12]= {2,11,13,14,16,18,19,20,21,22,23,28};

 for(int i=0;i<12;i++)
 {
	 IIR = false;
	 if(satid.id == IIR_Sat[i])
	 {
		IIR = true;
		break;
	 }
 }


      if(IIR)
      {
         wind_up = PI;
      }

      alpha1 = alpha1 + wind_up;

      double da1(alpha1-phase_satellite[satid].previousPhase);

      double da2(alpha2-phase_station[satid].previousPhase);

         // Let's avoid problems when passing from 359 to 0 degrees.
      phase_satellite[satid].previousPhase += std::atan2( std::sin(da1),
                                                          std::cos(da1) );

      phase_station[satid].previousPhase += std::atan2( std::sin(da2),
                                                        std::cos(da2) );

         // Compute wind up effect in radians
      wind_up = phase_satellite[satid].previousPhase -
                phase_station[satid].previousPhase;

      return wind_up;

   }  // End of method 'ComputeWindUp::getWindUp()'



   /*----------------------------------------------------------------------------------------------
 * Function					: double getIonoCorrections(double alpha[],double beta[],DayTime transmit,
 *                                                  Position &rxPos,
 *                                                  double elevation,
 *                                                  double azimuth,gpstk::IonoModel::Frequency )
    
 * Abstract					: This function generate ionospheris delay w.rt to rxpos state
 *							  and model based on gpstk library
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
   double ChannelGPS::getIonoCorrections(double alpha[],double beta[],DayTime transmit,
                                                   Position &rxPos,
                                                   double elevation,
                                                   double azimuth,gpstk::IonoModel::Frequency )
    {
	   // Convert Position rxPos to Geodetic rxGeo
		Geodetic rxGeo( rxPos.getGeodeticLatitude(),
					rxPos.getLongitude(),
					rxPos.getAltitude() );


       IonoModel tempIono(alpha,beta);
	   double ionoCorr(0.0);

       try
       {
		   ionoCorr = tempIono.getCorrection(transmit, rxGeo, elevation, azimuth,gpstk::IonoModel::L1);
       }
       catch(IonoModel::InvalidIonoModel& e)
       {
          ionoCorr = 0.0;
       }

       return ionoCorr;

    }  // End of method 'ModeledReferencePR::getIonoCorrections()'
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
 * Function					: double getThermalNoise(const double &loopBWpll,const double &c_n0,
 *									  const	double &loopBWdll,const double &chipWidth,
 *									  const double &armSpacing , const double &predetectIT)
 *   
 * Abstract					: This function generate thermal noise delay at receiver front end
 *							  given the bandwidth of the PLL and DLL used in the receiver along with
 *							  C/N0 for the tracking loops.
 * Formal
 * Parameter(s) 			: loopBWpll,loopBWdll,armSpacing,c_n0,chipWidth,predetectIT
 * Return value 			: None
 * System Call				: None
 * Functions called			: None
 * Reference				: None
 * Specific library calls	: None
 * Member variables accessed: All Private Members; see the header files
 * Assumptions 				: None
 *
 ----------------------------------------------------------------------------------------------*/
   void ChannelGPS::getThermalNoise(const double &loopBWpll,const double &c_n0,
									  const	double &loopBWdll,const double &chipWidth,
									  const double &armSpacing , const double &predetectIT)
                                                 
												   
	{
	
	
	// This is a typedef for a random number generator.
	// Try boost::mt19937 or boost::ecuyer1988 instead of boost::minstd_rand
	// Define the generator type
	double signalDll = chipWidth;
	double const1 = (loopBWdll * armSpacing)/(M_TWO*chipWidth*c_n0);
	double const2 = M_ONE + M_ONE/(predetectIT * c_n0);
	signalDll *= std::sqrt(const1*const2);
	
	double signalPll = std::sqrt(loopBWpll/(M_TWO*c_n0));
	signalPll *= (M_ONE/(M_TWO*gpstk::PI));
	
	// Define the generator type
	typedef boost::mt19937 base_generator_type1;
	typedef boost::mt11213b base_generator_type2;
	// Using 42u generator typs
	base_generator_type1 generator1(42u);
	base_generator_type2 generator2(48u);
	// Take the seed from the PC clock
	generator1.seed(static_cast<unsigned int>(std::time(0)));

	boost::normal_distribution<>  normal1(M_ZERO,signalDll);
	boost::normal_distribution<>  normal2(M_ZERO,signalPll);

	// Generate two iid  Normal Random number generator with mean = 0 , variance =sigmaDLL;
    boost::variate_generator<base_generator_type1&, boost::normal_distribution<> > norm_gen1(generator1, normal1);

	generator2.seed(static_cast<unsigned int>(std::time(0)));

	boost::variate_generator<base_generator_type2&, boost::normal_distribution<> > norm_gen2(generator2, normal2);

	// Define the two integrater for discrete gaussian noise process = Weiner Process
	noiseDll = norm_gen1();
	noisePll = norm_gen2();


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
std::string ChannelGPS::print_content(channelGPS_opDataPort &channelGPS_oData)

{
 string  content;
 content = "";
 using namespace StringUtils;
	           content += leftJustify(asString((double)channelGPS_oData.ionoDelay), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)channelGPS_oData.phasewindup), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)channelGPS_oData.pllThermalNoise), 13);
			   content += leftJustify("\t",3);
			   content += leftJustify(asString((double)channelGPS_oData.dllThermalNoise), 13);
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

std::string ChannelGPS::print_header()
{
 string  header;
 header = "";
 using namespace StringUtils;

			   header += leftJustify("IonoDelay(m)", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("PhaseWindup(rad)", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("PLLNoise(rad)", 13);
			   header += leftJustify("\t",3);
			   header += leftJustify("DLLNoise(rad) Z", 13);
			   header += leftJustify("\t",3);
return(header);
}

