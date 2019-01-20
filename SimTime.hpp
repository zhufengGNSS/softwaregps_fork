/* * File name		 : SimTimeEngine.hpp
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
 * H/W Platform      : IBM PC1
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

//#include <winshock.h>

/* GPSTk Library*/
#include "Exception.hpp"
#include "DayTime.hpp"

/*GPSGyan Include files*/
#include "EngineFramework.hpp"
#include "CommandOptionWithTimeArg.hpp"

/*C++ Systems Include files*/
#include "MSCData.hpp"
#include "MSCStream.hpp"
#include <string>


#ifndef SIMTIMEENGINE
#define SIMTIMEENGINE

using namespace gpstk;

class SimTimeEngine: public EngineFramework
{
public:
	 NEW_EXCEPTION_CLASS(ExcSimTimeEngine , gpstk::Exception);

	SimTimeEngine(const string& applName) 
   : EngineFramework(
      applName,
      "This modules contain the base simulation time")
   {};

	  //bool initialize(int argc, char *argv[]) throw();
	//template <class X>
void InputInfToEngine(DayTime &time);  // think of argv and argc implementation
//virtual void runEngine();
void InputParamToEngine() throw(InvalidParameter);
//bool runEngine() throw();
//void outputInfFromEngine();
void OutputDumpOfEngine();

//public data Members
DayTime gpsTrueTime,rxModelledTime ;
bool simTimerExpired;


protected :

		 void  EngineControl(Kind kind);
          void VerifyIpToEngine() throw(InvalidParameter);
 //         void prepareIpToEngine() ;

          void MethodOfEngine() throw(ExcSimTimeEngine);

         // void validateOpOfEngine();
          void PrepareOpOfEngine()	;

         virtual void dump(std::ostream &s);



private :
    DayTime simStartTime, simEndTime,simTimebase;
	
	bool simtimeIsSystemTime, startSimTime ,stopSimTime,pauseSimTime,isThisFirstTick,isThisFirstTimerTick;
	bool simtimeIsTimer,simTimerReset;
//	static int timeCount=100;
};


#endif
