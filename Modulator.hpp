#ifndef MODULATOR
#define MODULATOR
/* * File name		 : Modulator.hpp
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


#include "SatID.hpp"
#include "ModulatorMac.hpp"
#include "ModulatorTypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"



using namespace gpstk;

class Modulator: public EngineFramework
{
public:


NEW_EXCEPTION_CLASS(ExcModulator , gpstk::Exception);


Modulator(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time")
   {};

void OutputData(std::ostream &s );
void OutputData();
// Public Data memebers
// input section

modulator_passiveInput passiveData;
modulator_passiveParam passiveControl;
modulator_activeInput activeData;
modulator_activeParam activeControl;


// input section

engineHealth modulator_probe;
modulator_opDataPort modulator_Data;


std::vector<int> error_number;


protected :
/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/
			void EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();

			void GetPassiveInput(modulator_passiveInput &ref);
			void GetPassiveParam(modulator_passiveParam &ref);

			void GetActiveInput(modulator_activeInput  &ref);
			void GetActiveParam(modulator_activeParam  &ref);

            void VerifyIpToEngine() throw(ExcModulator);
			void ValidateOpOfEngine();

			void ExpHandler(ExcModulator &e);

/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
// Fromatted File Printing
//std::string print_content(modulator_opDataPort &ostruct);
//std::string print_header();
//virtual void dump(std::ostream &s);




private :

// Public Data memebers
// input section

modulator_activeInput p_activeData;
modulator_activeParam p_activeControl;
modulator_passiveInput p_passiveData;
modulator_passiveParam p_passiveControl;


 modulator_opDataPort modulator_pData;


 SatID currSatid;
 double currentPhaseValue;



 // Private Functions

/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/
 void MethodOfEngine() throw(ExcModulator);
void CodeSwitcher(int code_num,boost::circular_buffer<int> *code_buffer);
void GlonassModulator(int *ptr_to_code);
void GenerateBaseCarrier(boost::circular_buffer<double>& carrArrayI,
					     boost::circular_buffer<double>& carrArrayQ);
bool ValidateOp(modulator_opDataPort &outputPData ,ExcModulator &e);



//boost::circular_buffer<double>&  GenerateBaseCarrier();
string GetGeneratedCarr(modulator_opDataPort& generatedCarr, 
								boost::circular_buffer<double> & carrArrayI,
								double& initCoarsePhase,
								double& initCarrierFinePhase,
								double& satCarrClk);

//map<SatID, boost::circular_buffer<double>> carrDictionary;
boost::circular_buffer<double> currentSatCarrbufferI;
boost::circular_buffer<double> baseCarrierI;
boost::circular_buffer<double> currentSatCarrbufferMI;
boost::circular_buffer<double> baseCarrierMI;


};



#endif


