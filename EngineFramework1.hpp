 #ifndef GPSTK_ENGINEFRAMEWORK_HPP
 #define GPSTK_ENGINEFRAMEWORK_HPP


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

 //#include "CommandOptionParser.hpp"

 namespace gpstk
 {


    class EngineFramework
    {
    public:


       EngineFramework( const std::string& applName,
                       const std::string& applDesc )
          throw();


       virtual ~EngineFramework() {};

// Input information stream
   //virtual void inputInfToEngine(){};
   virtual void inputParamToEngine(){};

// Engine Working Model

	bool runEngine() throw();
// Loop Engine 

	virtual bool loopEngine();

// Output Processed stream
	virtual void outputInfFromEngine();
   	virtual void outputDumpOfEngine(){};




    protected:

       int debugLevel;
       int verboseLevel;
       std::string argv0;
       std::string appDesc;


       //CommandOptionNoArg debugOption;
       //CommandOptionNoArg verboseOption;
       //CommandOptionNoArg helpOption;

       virtual void verifyIpToEngine() {};
       virtual void prepareIpToEngine() {};

       virtual void methodOfEngine() {};

       virtual void validateOpOfEngine() {};
       virtual void prepareOpOfEngine() {};


      	virtual void shutDownEngine() {};

      	virtual void completeProcessing();






    private:


          // Do not allow the use of the default constructor.
       EngineFramework();

    }; // End of class 'EngineFramework'


 }  // End of namespace gpstk
 #endif   // GPSTK_ENGINEFRAMEWORK_HPP
