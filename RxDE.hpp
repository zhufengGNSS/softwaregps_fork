#ifndef RXDE
#define RXDE
/* * File name		 : RxDE.hpp
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
#include "Position.hpp"
#include "RxDEMac.hpp"
#include "RxDEtypes.hpp"


/*C++ Systems Include files*/
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*GPSGyan Include files*/
#include "EngineFramework.hpp"



using namespace gpstk;

class RxDE: public EngineFramework
{
public:	


NEW_EXCEPTION_CLASS(ExcRxDE , gpstk::Exception);
/*RxDE(const string& applName) throw()
   : EngineFramework(
      applName,
      "hello")
   {};*/
RxDE(){};


void OutputData(std::ostream &s );
void OutputData();
// Public Data memebers
// input section


rxDE_passiveInput passiveData;
rxDE_passiveParam passiveControl;
rxDE_activeInput activeData;
rxDE_activeParam activeControl;


// input section

engineHealth rxDE_probe;
rxDE_opDataPort rxDE_oData;

std::vector<int> error_number;


protected : 
/*----------------------------*/
/* Framework related functions*/
/*----------------------------*/
			void EngineControl(Kind kind);
			void SetInitialStateofEngine();
			void SetActiveStateofEngine ();


			void GetPassiveInput(rxDE_passiveInput &ref);
			void GetPassiveParam(rxDE_passiveParam &ref);

			void GetActiveInput(rxDE_activeInput  &ref);
			void GetActiveParam(rxDE_activeParam  &ref);


            void VerifyIpToEngine() throw(ExcRxDE);
			void ValidateOpOfEngine();

			void ExpHandler(ExcRxDE &e);

/*----------------------------------------*/
/* Framework related output file functions*/
/*----------------------------------------*/
std::string opDataHeader;
std::string opDataInString;
std::string print_content(rxDE_opDataPort &rxDE_oData);
std::string print_header();

private :
rxDE_activeInput p_activeData;
rxDE_activeParam p_activeControl;
rxDE_passiveInput p_passiveData;
rxDE_passiveParam p_passiveControl;
rxDE_opDataPort rxDE_pData;

double norm1,norm2;
double rxClkerr_time[M_TWO] ,rxClkerr_phase[M_TWO] ;
/*-------------------------------------*/
/* Engine/algorithm specific  functions*/
/*-------------------------------------*/
void MethodOfEngine() throw(ExcRxDE);
double GenerateRxClkErr(double& update_interval);
bool ValidateOp(rxDE_opDataPort &outputPData ,ExcRxDE &e);


//DayTime  trueGPStime;
	SatID    gps_sv;
	// Error Value

	double minElev;

};
#endif


