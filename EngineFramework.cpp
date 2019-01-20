
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

EngineFramework :: EngineFramework()
{int i = 0;}


 /*bool EngineFramework::loopEngine()

 {   prepareIpToEngine();
	 methodOfEngine();
 return true;

 }*/

/*void EngineFramework :: EngineControl(Kind kind)
{
	switch(kind)
	{ case PASSIVE: break;
	case ACTIVE : break;
	//case default : break;
	}
}*/



    bool EngineFramework :: Engine(Mode mode)
       throw()
    {

       try
       { switch (mode)
	   {

		case INITIALIZE :
		    // Take the Passive input and control
		     EngineControl(PASSIVE);
			 break;

		case RUN_ALL :
            // This is for Active  inputs and param
			EngineControl(ACTIVE);
			PrepareIpToEngine();
       		VerifyIpToEngine() ;
			MethodOfEngine() ;
			ValidateOpOfEngine();

			break;

		case RUN_METHOD :
            EngineControl(ACTIVE);
			PrepareIpToEngine();
			MethodOfEngine();
			break;

		/*case RUN_FROM_START :
			 setInitialStateOfEngine();
			 prepareIpToEngine();
			 methodOfEngine() ;
		     break ;

		case RUN_WITH_VnV:
			prepareIpToEngine();
       		verifyIpToEngine() ;
			methodOfEngine() ;
			validateOpOfEngine();
			  break ;*/


		default: PrepareIpToEngine();
			     MethodOfEngine();

	   }


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



       return true;

    }  // End of method 'BasicFramework::run()'


	//bool EngineFramework :: EngineOutput(OpMode mode,std::ostream *s=NULL)
		bool EngineFramework :: EngineOutput(OpMode mode)
       throw()
    {

       try
       { 	ofstream s1 ;	
		   switch (mode)
	   {

	
		case DATA_HERE :
		    // Take the Passive input and control
		     OutputData();
			 break;

		case DATA_INFILE :
			{
            // This is for Active  inputs and param
				//std::ostream 
			OutputData(s1);

			break;
			}
		/*case DATA_N_DEBUG_INFILE :
		OutputDebug();

			break;

		case DEBUG_INFILE :

		     break ;

		case DEBUG_HERE :

		     break ;*/
 			default:OutputData();

	   }


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



       return true;

    }  // End of method 'BasicFramework::run()'





    void EngineFramework :: CompleteProcessing()
    {
		PrepareIpToEngine();
       	VerifyIpToEngine() ;
	    MethodOfEngine() ;

    }  // End of method 'EngineFramework::completeProcessing()'



 }  // End of namespace gpstk

