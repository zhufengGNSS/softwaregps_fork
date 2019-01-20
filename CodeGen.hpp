#ifndef CODEGEN
#define CODEGEN
/* * File name		 : CodeGen.hpp
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
#include "CodeGenMac.hpp"
#include "CodeGenTypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"



using namespace gpstk;

class CodeGen: public EngineFramework
{
public:


NEW_EXCEPTION_CLASS(ExcCodeGen , gpstk::Exception);


CodeGen(const string& applName) throw()
   : EngineFramework(
      applName,
      "this computes satellite Position , Velocity and time for the specified receiever position and time")
   {};

void OutputData(std::ostream &s );
void OutputData();
// Public Data memebers
// input section

codeGen_passiveInput passiveData;
codeGen_passiveParam passiveControl;
codeGen_activeInput activeData;
codeGen_activeParam activeControl;


// input section

engineHealth codeGen_probe;
codeGen_opDataPort codeGen_Data;


std::vector<int> error_number;


protected :
/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/
			void EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();

			void GetPassiveInput(codeGen_passiveInput &ref);
			void GetPassiveParam(codeGen_passiveParam &ref);

			void GetActiveInput(codeGen_activeInput  &ref);
			void GetActiveParam(codeGen_activeParam  &ref);

            void VerifyIpToEngine() throw(ExcCodeGen);
			void ValidateOpOfEngine();

			void ExpHandler(ExcCodeGen &e);

/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
// Fromatted File Printing
//std::string print_content(codeGen_opDataPort &ostruct);
//std::string print_header();
//virtual void dump(std::ostream &s);




private :

// Public Data memebers
// input section

codeGen_activeInput p_activeData;
codeGen_activeParam p_activeControl;
codeGen_passiveInput p_passiveData;
codeGen_passiveParam p_passiveControl;


 codeGen_opDataPort codeGen_pData;


 SatID currSatid;
 double currentPhaseValue;



 // Private Functions

/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/
 void MethodOfEngine() throw(ExcCodeGen);
void CodeSwitcher(int code_num,boost::circular_buffer<int> *code_buffer);
void GlonassCodeGen(int *ptr_to_code);
void CodeGenCA_L1(boost::circular_buffer<int> *ptr_to_code,int tap1,int tap2);
bool ValidateOp(codeGen_opDataPort &outputPData ,ExcCodeGen &e);



void GenerateCAcodeArray(SatID satid,boost::circular_buffer<int>& codeArray);
void GetGeneratedCode(codeGen_opDataPort& generatedCode,
					boost::circular_buffer<int>& codeArray,
					int& initSlew,
					double& initSlewPhase,
					double& satCodeClk);


map<SatID, boost::circular_buffer<int>> codeDictionary;
boost::circular_buffer<int> currentSatCodebuffer;


};





#endif