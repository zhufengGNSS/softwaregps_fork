
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


 #include "Exception.hpp"
 #include "EngineFramework.hpp"


 namespace gpstk
 {

    using namespace std;


    EngineFramework :: EngineFramework( const string& applName,
                                      const string& applDesc )
       throw()
          : debugLevel(0),
            verboseLevel(0),
            argv0(applName),
            appDesc(applDesc)
            //debugOption('d', "debug", "Increase debug level"),
           // verboseOption('v', "verbose", "Increase verbosity"),
           // helpOption('h', "help", "Print help usage")
    {} // End of constructor 'BasicFramework::BasicFramework()'



 bool EngineFramework::loopEngine()
 
 {   prepareIpToEngine();
	 methodOfEngine();
 return true;

 }

    bool EngineFramework :: runEngine()
       throw()
    {

       try
       {
          completeProcessing();
       }
       catch (Exception& exc)
       {
          cerr << exc;
          return false;
       }
       catch (...)
       {
          cerr << "Caught unknown exception" << endl;
          return false;
       }

       shutDownEngine();

       return true;

    }  // End of method 'BasicFramework::run()'



    void EngineFramework :: completeProcessing()
    {
		prepareIpToEngine();
       		 verifyIpToEngine() ;
	        	         methodOfEngine() ;

    }  // End of method 'EngineFramework::completeProcessing()'


   void EngineFramework::outputInfFromEngine()

 	 {
    	validateOpOfEngine();
    	prepareOpOfEngine();
	 }

 }  // End of namespace gpstk

