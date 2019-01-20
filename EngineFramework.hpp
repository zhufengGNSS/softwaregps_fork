#ifndef ENGINEFRAMEWORK
#define ENGINEFRAMEWORK


 //============================================================================
 /* * File name		 : EngineFramework.hpp
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
  * Library Used 	 : GPStk ,Copyright 2009, The University of Texas at Austin
  */
 //============================================================================

 #include "CommandOptionParser.hpp"
#include <fstream>


 namespace gpstk
 {


    class EngineFramework
    {
    public:
	enum Mode { INITIALIZE =0,
				RUN_ALL =1,
				RUN_METHOD=2,
				RUN_WITH_VnV=3,
				RUN_FROM_START =4};


enum OpMode { DATA_HERE =0,
				DATA_INFILE =1,
				DATA_N_DEBUG_INFILE=2,
				DEBUG_INFILE=3,
				DEBUG_HERE =4};


    enum Kind{ PASSIVE =0 ,
			ACTIVE =1};


	typedef struct
	{
	unsigned s_psvInput:1;
	unsigned s_psvParam:1;
	unsigned s_actInput:1;
	unsigned s_actParam:1;
	unsigned s_engineControlAct:1;
	unsigned s_engineControlPsv:1;
	unsigned s_engine :2;
	unsigned s_prepEngine :1;
	unsigned s_verEngine :1;
	unsigned s_methodEngine:1;
	unsigned s_valEngine:1;
	unsigned s_opEngine:1;
	unsigned h_ofOpData:1;
	unsigned :19;
	}engineHealth;




// Default Constructor
			Mode mode;
			Kind kind;
EngineFramework( const std::string& applName,
                 const std::string& applDesc )
                 throw();
 // Do not allow the use of the default constructor.
       EngineFramework();

// Default Destructor
       virtual ~EngineFramework() {};

// Input information stream
   //virtual void inputInfToEngine(){};
   virtual void InputParamToEngine(){};

// Engine Running Modes Model

	bool Engine(Mode mode) throw();
	//bool EngineOutput(OpMode mode,std::ostream *s)throw();
	bool EngineOutput(OpMode mode)throw();
	


// Output Processed stream
	virtual void OutputData(){};
	virtual void OutputData(std::ostream &s ){};
   	virtual void OutputDebug(){};

/***********************************/
    protected:

       int debugLevel;
       int verboseLevel;
       std::string argv0;
       std::string appDesc;


       //CommandOptionNoArg debugOption;
       //CommandOptionNoArg verboseOption;
       //CommandOptionNoArg helpOption;

	   virtual void EngineControl(Kind kind){} ;

       virtual void VerifyIpToEngine() {};
       virtual void PrepareIpToEngine() {};



       virtual void MethodOfEngine() {};

       virtual void ValidateOpOfEngine() {};
       virtual void PrepareOpOfEngine() {};


      	virtual void ShutDownEngine() {};

      	virtual void CompleteProcessing();






    private:


         
    }; // End of class 'EngineFramework'


 }  // End of namespace gpstk
 #endif   // GPSTK_ENGINEFRAMEWORK_HPP
