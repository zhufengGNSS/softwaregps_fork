#ifndef CARRGEN
#define CARRGEN
/* * File name		 : CarrGen.hpp
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
#include "CarrGenMac.hpp"
#include "CarrGenTypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"



using namespace gpstk;

class CarrGen: public EngineFramework
{
public:


NEW_EXCEPTION_CLASS(ExcCarrGen , gpstk::Exception);


CarrGen(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time")
   {};

void OutputData(std::ostream &s );
void OutputData();
// Public Data memebers
// input section

carrGen_passiveInput passiveData;
carrGen_passiveParam passiveControl;
carrGen_activeInput activeData;
carrGen_activeParam activeControl;


// input section

engineHealth carrGen_probe;
carrGen_opDataPort carrGen_Data;


std::vector<int> error_number;


protected :
/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/
			void EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();

			void GetPassiveInput(carrGen_passiveInput &ref);
			void GetPassiveParam(carrGen_passiveParam &ref);

			void GetActiveInput(carrGen_activeInput  &ref);
			void GetActiveParam(carrGen_activeParam  &ref);

            void VerifyIpToEngine() throw(ExcCarrGen);
			void ValidateOpOfEngine();

			void ExpHandler(ExcCarrGen &e);

/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
// Fromatted File Printing
//std::string print_content(carrGen_opDataPort &ostruct);
//std::string print_header();
//virtual void dump(std::ostream &s);




private :

// Private Data memebers
// input section

carrGen_activeInput p_activeData;
carrGen_activeParam p_activeControl;
carrGen_passiveInput p_passiveData;
carrGen_passiveParam p_passiveControl;


 carrGen_opDataPort carrGen_pData;


 SatID currSatid;
 double currentPhaseValue;
 double rxIF;




 // Private Functions

/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/
 void MethodOfEngine() throw(ExcCarrGen);
void CodeSwitcher(int code_num,boost::circular_buffer<int> *code_buffer);
void GlonassCarrGen(int *ptr_to_code);
void GenerateBaseCarrier(boost::circular_buffer<double>& carrArrayI,
					     boost::circular_buffer<double>& carrArrayQ);
bool ValidateOp(carrGen_opDataPort &outputPData ,ExcCarrGen &e);
void  GenerateBeatFrequency();



//boost::circular_buffer<double>&  GenerateBaseCarrier();
void GetGeneratedCarr(carrGen_opDataPort& generatedCarr, 
								boost::circular_buffer<double> & carrArrayI,
								boost::circular_buffer<double> & carrArrayQ,
								double& initCoarsePhase,
								double& initCarrierFinePhase,
								double& satCarrClk);

//map<SatID, boost::circular_buffer<double>> carrDictionary;
boost::circular_buffer<double> currentSatCarrbufferI;
boost::circular_buffer<double> baseCarrierI;
boost::circular_buffer<double> currentSatCarrbufferQ;
boost::circular_buffer<double> baseCarrierQ;


};


#endif


