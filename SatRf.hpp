/* * File name		 : satRF.hpp
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


/* GPSTk Library*/
#include "SatRfTypes.hpp"
#include "DayTime.hpp"
#include "SatID.hpp"
#include "Position.hpp"
#include "SatRfMac.hpp"

#include "MSCData.hpp"
#include "MSCStream.hpp"
#include "FFIdentifier.hpp"
//#include "TropModel.hpp"
#include "IonoModel.hpp"
//#include "IonoModelStore.hpp"
#include "Geodetic.hpp"
#include "SunPosition.hpp"



/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"




using namespace gpstk;

class SatRF: public EngineFramework
{
public:


	 NEW_EXCEPTION_CLASS(ExcSatRF , gpstk::Exception);


	SatRF(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time")
   {};

	 // bool initialize(int argc, char *argv[]) throw();
	 // virtual void process();

void GetPassiveInput(satRF_psvIstruct &ref_psv);
void GetActiveInput(satRF_actIstruct &ref_act);
void GetPassiveParam();
void GetActiveParam();


void InputInfToEngine(Kind kind);  // think of argv and argc implementation
//virtual void runEngine();
void InputParamToEngine(Kind kind);// throw(InvalidParameter);
//bool runEngine() throw();
//void outputInfFromEngine(std::ostream & s ,const std::string & 	data);
void outputInfFromEngine(std::ostream & s );
//void OutputDumpOfEngine();

// Public Data memebers
engineHealth satRF_probe;

std::vector<satRF_ostruct> satRF_oData;
std::vector<SatID> visibleSV;
bool visiblityComputed;

satRF_actIstruct ref_act;
satRF_psvIstruct ref_psv;
double alpha[4],beta[4],iono_freq;


DayTime inp_time ;
double usr_elev;
std::vector<int> error_number;
//std::string op_inf;
protected :
			void  EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();


            void VerifyIpToEngine() throw(ExcSatRF);
 //         void prepareIpToEngine() ;

			void MethodOfEngine() throw(ExcSatRF);


			void SatMDE_ExpHandler(ExcSatRF &e);

			// Fromatted File Printing
			std::string print_content(satRF_ostruct &ostruct);
		    std::string print_header();


		   void CodeSwitcher(int code_num,boost::circular_buffer<int> *code_buffer);
		   void GlonassCodeGen(int *ptr_to_code);
		   void  CodeGenCA_L1(boost::circular_buffer<int> *ptr_to_code,int tap1,int tap2);



		   satRF_caCodeArray&  GenerateCAcodeArray(int satID, double initCodePhase);
		   satRF_carrierArray& GenerateBaseCarrier(double initCarrPhase);

		   void  GetGeneratedCode(satRF_caCodeArray &tempCodeBuffer, double codeFrequency,satRF_ostruct & );
		   void  GetGeneratedCarrier(satRF_carrierArray &tempCarrBuffer, double carrierFrequency, satRF_ostruct &op_struct);


		void CarrierPhaseComputation();
		long  ResolveCyclesAndPhase(double delay , double baseFreq ,satRF_carrierArray &tempCarrBuffer );

         // void validateOpOfEngine();
		//	void PrepareOpOfEngine()	;

			//virtual void dump(std::ostream &s);




private :

satRF_caCodeArray prnCodeBuffer;
satRF_carrierArray prnCarrBuffer;
SatID currSatid;
double currentPhaseValue;



map<SatID, codeAndCarrierDB> codeAndCarrierDictionary;
map<SatID, satRF_carrierArray> instantCarrierDictionary;
map<SatID, satRF_carrierInitVals> carrierInitVal;

std::vector<satRF_caCodeArray> bigCodeArray;
std::vector<satRF_carrierArray> bigCarrArray;
//boost::circular_buffer<bool>::iterator it = cb.begin();

satRF_actIstruct satRF_ai;
satRF_psvIstruct satRF_pi;
unsigned int visSV;
std::vector<satRF_ostruct> satRF_pData;




};


