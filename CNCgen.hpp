#ifndef CNCGEN
#define CNCGEN
/* * File name		 : CNCgen.hpp
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

#include "IonoModel.hpp"
#include "DayTime.hpp"
#include "SatID.hpp"
#include "Position.hpp"
#include "SunPosition.hpp"
#include "CNCgenMac.hpp"
#include "CNCgenTypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"



using namespace gpstk;

class CNCgen: public EngineFramework
{
public:


NEW_EXCEPTION_CLASS(ExcCNCgen , gpstk::Exception);


CNCgen(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time")
   {};

void OutputData(std::ostream &s );
void OutputData();
// Public Data memebers
// input section

cncGen_passiveInput passiveData;
cncGen_passiveParam passiveControl;
cncGen_activeInput activeData;
cncGen_activeParam activeControl;


// input section

engineHealth cncGen_probe;


cncGen_opDataPort cncGen_Data;


std::vector<int> error_number;


protected :
/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/
			void EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();

			void GetPassiveInput(cncGen_passiveInput &ref);
			void GetPassiveParam(cncGen_passiveParam &ref);

			void GetActiveInput(cncGen_activeInput  &ref);
			void GetActiveParam(cncGen_activeParam  &ref);

            void VerifyIpToEngine() throw(ExcCNCgen);
			void ValidateOpOfEngine();

			void ExpHandler(ExcCNCgen &e);

/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
// Fromatted File Printing
//std::string print_content(cncGen_opDataPort &ostruct);
//std::string print_header();
//virtual void dump(std::ostream &s);




private :

// Public Data memebers
// input section

cncGen_activeInput p_activeData;
cncGen_activeParam p_activeControl;
cncGen_passiveInput p_passiveData;
cncGen_passiveParam p_passiveControl;

EphReader yumaReader; // Reads all type of possible input file ( YUMA , Sem, RINEX etc , currently implemented for YUMA)
 cncGen_opDataPort cncGen_pData;

 cncGen_caCodeArray prnCodeBuffer;
 cncGen_carrierArray prnCarrBuffer;
 SatID currSatid;
 double currentPhaseValue;



 // Private Functions

/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/
 void MethodOfEngine() throw(ExcCNCgen);
 void CodeSwitcher(int code_num,boost::circular_buffer<int> *code_buffer);
void GlonassCodeGen(int *ptr_to_code);
void  CodeGenCA_L1(boost::circular_buffer<int> *ptr_to_code,int tap1,int tap2);
bool ValidateOp(cncGen_opDataPort &outputPData ,ExcCNCgen &e);



cncGen_caCodeArray&  GenerateCAcodeArray(int satID, double initCodePhase);
cncGen_carrierArray& GenerateBaseCarrier(double initCarrPhase);

void  GetGeneratedCode(cncGen_caCodeArray &tempCodeBuffer, double codeFrequency,cncGen_opDataPort & );
void  GetGeneratedCarrier(cncGen_carrierArray &tempCarrBuffer, double carrierFrequency, cncGen_opDataPort &op_struct);




};





#endif