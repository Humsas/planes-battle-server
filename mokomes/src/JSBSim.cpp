/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 Module:       JSBSim.cpp
 Author:       Jon S. Berndt
 Date started: 08/17/99
 Purpose:      Standalone version of JSBSim.
 Called by:    The USER.

 ------------- Copyright (C) 1999  Jon S. Berndt (jsb@hal-pc.org) -------------

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 details.

 You should have received a copy of the GNU Lesser General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place - Suite 330, Boston, MA  02111-1307, USA.

 Further information about the GNU Lesser General Public License can also be found on
 the world wide web at http://www.gnu.org.

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------

This class Handles calling JSBSim standalone. It is set up for compilation under
Borland C+Builder or other compiler.

HISTORY
--------------------------------------------------------------------------------
08/17/99   JSB   Created

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "FGFDMExec.h"
#include "FGState.h"
using namespace JSBSim;
//#include <initialization/FGTrimAnalysis.h>

//#if !defined(__GNUC__) && !defined(sgi) && !defined(_MSC_VER)
//#  include <time>
//#else
//#  include <time.h>
//#endif
//
//#if defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW32__)
//#  define WIN32_LEAN_AND_MEAN
//#  include <windows.h>
//#  include <mmsystem.h>
//#  include <regstr.h>
//#  include <sys/types.h>
//#  include <sys/timeb.h>
//#else
//#  include <sys/time.h>
//#endif
//
///*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
//
//static const char *IdSrc = "$Id: JSBSim.cpp,v 1.45 2009/05/17 13:51:23 jberndt Exp $";
//
///*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//GLOBAL DATA
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
//
//string RootDir = "";
//string ScriptName;
//string AircraftName;
//string ResetName;
//string LogOutputName;
//string LogDirectiveName;
//vector <string> CommandLineProperties;
//vector <double> CommandLinePropertyValues;
//JSBSim::FGFDMExec* FDMExec;
//bool realtime;
//bool play_nice;
//bool suspend;
//bool catalog;
//
//double end_time = -1.0;
//
///*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//FORWARD DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
//
//bool options(int, char**);
//void PrintHelp(void);
//
//#if defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW32__)
//  double getcurrentseconds(void)
//  {
//    struct timeb tm_ptr;
//    ftime(&tm_ptr);
//    return tm_ptr.time + tm_ptr.millitm*0.001;
//  }
//#else
//  double getcurrentseconds(void)
//  {
//    struct timeval tval;
//    struct timezone tz;
//
//    gettimeofday(&tval, &tz);
//    return (tval.tv_sec + tval.tv_usec*1e-6);
//  }
//#endif
//
//#if defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW32__)
//  void sim_nsleep(long nanosec)
//  {
//    Sleep(nanosec*1e-6); // convert nanoseconds (passed in) to milliseconds for Win32.
//  }
//#else
//  void sim_nsleep(long nanosec)
//  {
//    struct timespec ts, ts1;
//
//    ts.tv_sec = 0;
//    ts.tv_nsec = nanosec;
//    nanosleep(&ts, &ts1);
//  }
//#endif
//
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** \mainpage JSBSim
 * An Open Source, Object-Oriented, Cross-Platform Flight Dynamics Model in C++

 * \section intro Introduction
 *
 * JSBSim is an open source, multi-platform, object-oriented flight dynamics
 * model (FDM) framework written in the C++ programming language.  It is
 * designed to support simulation modeling of any aerospace craft without the
 * need for specific compiled and linked program code, instead relying on a
 * relatively simple model specification written in a XML format. The format is
 * formally known as JSBSim-ML (JSBSim Markup Language).
 *
 * JSBSim (www.jsbsim.org) was created initially for the open source FlightGear
 * flight simulator (www.flightgear.org), where it replaced LaRCSim (Langley
 * Research Center Simulation) as the default FDM.  JSBSim also maintains the
 * ability to run in a standalone, batch mode.  This is useful for running tests
 * or sets of tests automatically using the internal scripting capability.
 *
 * Differently from LaRCSim, JSBSim does not model specific aircraft in program
 * code. The aircraft itself is defined in a file written in an XML-based format
 * where the aircraft mass and geometric properties are specified.  Additional
 * statements define:
 *
 * - Landing gear location and properties.
 * - Pilot eyepoint
 * - Additional point masses (passengers, cargo, etc.)
 * - Propulsion system (engines, fuel tanks, and "thrusters")
 * - Flight control system
 * - Autopilot
 * - Aerodynamic stability derivatives and coefficients
 *
 * The configuration file format is set up to be easily comprehensible, for
 * instance featuring textbook-like coefficients, which enables newcomers to
 * become immediately fluent in describing vehicles, and requiring only prior
 * basic theoretical aero knowledge.
 *
 * One of the more unique features of JSBSim is its method of modeling flight
 * control systems and an autopilot.  These are modeled by assembling strings
 * of components that represent filters, switches, summers, gains, sensors, etc.
 *
 * Another unique feature is displayed in the use of "properties".  Properties
 * essentially expose chosen variables as nodes in a tree, in a directory-like
 * hierarchy.  This approach facilitates plugging in different FDMs into
 * FlightGear, but it also is a fundamental tool in allowing a wide range of
 * aircraft to be modeled, each having its own unique control system,
 * aerosurfaces, and flight deck instrument panel.  The use of properties allows
 * all these items for a craft to be modeled and integrated without the need for
 * specific and unique program source code.
 *
 * The equations of motion are modeled essentially as they are presented in
 * aerospace textbooks for the benefit of student users, but quaternions are
 * used to track orientation, avoiding "gimbal lock". While JSBSim is
 * designed to model primarily atmospheric flight at lower speeds, coriolis and
 * centripetal accelerations are incorporated into the EOM to
 * permit a wider range of vehicles to be simulated.
 *
 * Currently under development is an expansion of the atmospheric modeling for
 * JSBSim.  The existing model approximates the standard atmosphere of 1976.
 * Recently, source code for the NRLMSISE-00 model was obtained and this is
 * being implemented as a C++ class that can optionally be used.  Also, a simple
 *  Mars atmosphere is being implemented.
 *
 * JSBSim can output (log) data in a configurable way.  Sets of data that are
 * logically related can be selected to be output at a chosen rate, and
 * individual properties can be selected for output.  The output can be streamed
 * to the console, or to a file, and can also be transmitted through a socket.
 *
 * JSBSim has been used in a variety of ways:
 *
 * - For developing control laws for a sounding rocket
 * - For crafting an aircraft autopilot as part of a thesis project
 * - As a flight model for FlightGear
 * - As an FDM that drives motion base simulators for some
 *   commercial/entertainment simulators
 *
 * \section Supported Platforms:
 * JSBSim has been built on the following platforms:
 *
 *   - Linux (x86)
 *   - Windows (MSVC, Cygwin, Mingwin)
 *   - SGI (native compilers)
 *   - Mac OS X
 *   - FreeBSD
 *
 * \section depends Dependencies
 *
 * JSBSim has no dependencies at present.
 *
 * \section license Licensing
 *
 * JSBSim is licensed under the terms of the Lesser GPL (LGPL)
 *
 * \section website Website
 *
 * For more information, see the JSBSim web site: www.jsbsim.org.
 */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

//int main(int argc, char* argv[])
//{
//  // *** INITIALIZATIONS *** //
//
//  ScriptName = "";
//  AircraftName = "";
//  ResetName = "";
//  LogOutputName = "";
//  LogDirectiveName = "";
//  bool result = false, success;
//  bool was_paused = false;
//  
//  double frame_duration;
//
//  double new_five_second_value = 0.0;
//  double actual_elapsed_time = 0;
//  double initial_seconds = 0;
//  double current_seconds = 0.0;
//  double paused_seconds = 0.0;
//  double sim_time = 0.0;
//  double sim_lag_time = 0;
//  double cycle_duration = 0.0;
//  long sleep_nseconds = 0;
//
//  realtime = false;
//  play_nice = false;
//  suspend = false;
//  catalog=false;
//
//  // *** PARSE OPTIONS PASSED INTO THIS SPECIFIC APPLICATION: JSBSim *** //
//  success = options(argc, argv);
//  if (!success) {
//    PrintHelp();
//    exit(-1);
//  }
//
//  // *** SET UP JSBSIM *** //
//  FDMExec = new JSBSim::FGFDMExec();
//  FDMExec->SetAircraftPath(RootDir + "aircraft");
//  FDMExec->SetEnginePath(RootDir + "engine");
//  FDMExec->SetSystemsPath(RootDir + "systems");
//  FDMExec->GetPropertyManager()->Tie("simulation/frame_start_time", &actual_elapsed_time);
//  FDMExec->GetPropertyManager()->Tie("simulation/cycle_duration", &cycle_duration);
//
//  // *** OPTION A: LOAD A SCRIPT, WHICH LOADS EVERYTHING ELSE *** //
//  if (!ScriptName.empty()) {
//
//    ScriptName = RootDir + ScriptName;
//    result = FDMExec->LoadScript(ScriptName);
//
//    if (!result) {
//      cerr << "Script file " << ScriptName << " was not successfully loaded" << endl;
//      delete FDMExec;
//      exit(-1);
//    }
//
//  // *** OPTION B: LOAD AN AIRCRAFT AND A SET OF INITIAL CONDITIONS *** //
//  } else if (!AircraftName.empty() || !ResetName.empty()) {
//
//    if (catalog) FDMExec->SetDebugLevel(0);
//
//    if ( ! FDMExec->LoadModel( RootDir + "aircraft",
//                               RootDir + "engine",
//                               RootDir + "systems",
//                               AircraftName)) {
//      cerr << "  JSBSim could not be started" << endl << endl;
//      delete FDMExec;
//      exit(-1);
//    }
//
//    if (catalog) {
//      FDMExec->PrintPropertyCatalog();
//      delete FDMExec;
//      return 0;
//    }
//
//    JSBSim::FGInitialCondition *IC = FDMExec->GetIC();
//    if ( ! IC->Load(ResetName)) {
//      delete FDMExec;
//      cerr << "Initialization unsuccessful" << endl;
//      exit(-1);
//    }
//
//  } else {
//    cout << "  No Aircraft, Script, or Reset information given" << endl << endl;
//    delete FDMExec;
//    exit(-1);
//  }
//
//  // Load output directives file, if given
//  if (!LogDirectiveName.empty()) {
//    if (!FDMExec->SetOutputDirectives(LogDirectiveName)) {
//      cout << "Output directives not properly set" << endl;
//      delete FDMExec;
//      exit(-1);
//    }
//  }
//
//  // OVERRIDE OUTPUT FILE NAME. THIS IS USEFUL FOR CASES WHERE MULTIPLE
//  // RUNS ARE BEING MADE (SUCH AS IN A MONTE CARLO STUDY) AND THE OUTPUT FILE
//  // NAME MUST BE SET EACH TIME TO AVOID THE PREVIOUS RUN DATA FROM BEING OVER-
//  // WRITTEN. THIS OVERRIDES ONLY THE FILENAME FOR THE FIRST FILE.
//  if (!LogOutputName.empty()) {
//    string old_filename = FDMExec->GetOutputFileName();
//    if (!FDMExec->SetOutputFileName(LogOutputName)) {
//      cout << "Output filename could not be set" << endl;
//    } else {
//      cout << "Output filename change from " << old_filename << " from aircraft"
//              " configuration file to " << LogOutputName << " specified on"
//              " command line" << endl;
//    }
//  }
//
//  // SET PROPERTY VALUES THAT ARE GIVEN ON THE COMMAND LINE
//
//  for (unsigned int i=0; i<CommandLineProperties.size(); i++) {
//
//    if (!FDMExec->GetPropertyManager()->GetNode(CommandLineProperties[i])) {
//      cerr << endl << "  No property by the name " << CommandLineProperties[i] << endl;
//      goto quit;
//    } else {
//      FDMExec->SetPropertyValue(CommandLineProperties[i], CommandLinePropertyValues[i]);
//    }
//  }
//
//  cout << endl << JSBSim::FGFDMExec::fggreen << JSBSim::FGFDMExec::highint
//       << "---- JSBSim Execution beginning ... --------------------------------------------"
//       << JSBSim::FGFDMExec::reset << endl << endl;
//
//  result = FDMExec->Run();  // MAKE AN INITIAL RUN
//
//  if (suspend) FDMExec->Hold();
//
//  JSBSim::FGJSBBase::Message* msg;
//
//  // Print actual time at start
//  char s[100];
//  time_t tod;
//  time(&tod);
//  strftime(s, 99, "%A %B %d %Y %X", localtime(&tod));
//  cout << "Start: " << s << " (HH:MM:SS)" << endl;
//
//  frame_duration = FDMExec->GetDeltaT();
//  if (realtime) sleep_nseconds = (long)(frame_duration*1e9);
//  else          sleep_nseconds = (10000000);           // 0.01 seconds
//
//  tzset(); 
//  current_seconds = initial_seconds = getcurrentseconds();
//
//  // *** CYCLIC EXECUTION LOOP, AND MESSAGE READING *** //
//  while (result) {
//    while (FDMExec->SomeMessages()) {
//      msg = FDMExec->ProcessMessage();
//      switch (msg->type) {
//      case JSBSim::FGJSBBase::Message::eText:
//        cout << msg->messageId << ": " << msg->text << endl;
//        break;
//      case JSBSim::FGJSBBase::Message::eBool:
//        cout << msg->messageId << ": " << msg->text << " " << msg->bVal << endl;
//        break;
//      case JSBSim::FGJSBBase::Message::eInteger:
//        cout << msg->messageId << ": " << msg->text << " " << msg->iVal << endl;
//        break;
//      case JSBSim::FGJSBBase::Message::eDouble:
//        cout << msg->messageId << ": " << msg->text << " " << msg->dVal << endl;
//        break;
//      default:
//        cerr << "Unrecognized message type." << endl;
//        break;
//      }
//    }
//
//    // if running realtime, throttle the execution, else just run flat-out fast
//    // unless "playing nice", in which case sleep for a while (0.01 seconds) each frame.
//    // If suspended, then don't increment cumulative realtime "stopwatch".
//
//    if ( ! FDMExec->Holding()) {
//      if ( ! realtime ) {         // ------------ RUNNING IN BATCH MODE
//
//        result = FDMExec->Run();
//
//        if (play_nice) sim_nsleep(sleep_nseconds);
//
//      } else {                    // ------------ RUNNING IN REALTIME MODE
//
//        // "was_paused" will be true if entering this "run" loop from a paused state.
//        if (was_paused) {
//          initial_seconds += paused_seconds;
//          was_paused = false;
//        }
//        current_seconds = getcurrentseconds();                      // Seconds since 1 Jan 1970
//        actual_elapsed_time = current_seconds - initial_seconds;    // Real world elapsed seconds since start
//        sim_lag_time = actual_elapsed_time - FDMExec->GetSimTime(); // How far behind sim-time is from actual
//                                                                    // elapsed time.
//        for (int i=0; i<(int)(sim_lag_time/frame_duration); i++) {  // catch up sim time to actual elapsed time.
//          result = FDMExec->Run();
//          cycle_duration = getcurrentseconds() - current_seconds;   // Calculate cycle duration
//          current_seconds = getcurrentseconds();                    // Get new current_seconds
//          if (FDMExec->Holding()) break;
//        }
//
//        if (play_nice) sim_nsleep(sleep_nseconds);
//
//        if (FDMExec->GetSimTime() >= new_five_second_value) { // Print out elapsed time every five seconds.
//          cout << "Simulation elapsed time: " << FDMExec->GetSimTime() << endl;
//          new_five_second_value += 5.0;
//        }
//      }
//    } else { // Suspended
//      was_paused = true;
//      paused_seconds = getcurrentseconds() - current_seconds;
//      sim_nsleep(sleep_nseconds);
//      result = FDMExec->Run();
//    }
//
//  }
//
//quit:
//
//  // PRINT ENDING CLOCK TIME
//  time(&tod);
//  strftime(s, 99, "%A %B %d %Y %X", localtime(&tod));
//  cout << "End: " << s << " (HH:MM:SS)" << endl;
//
//  // CLEAN UP
//  delete FDMExec;
//
//  return 0;
//}
//
////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//bool options(int count, char **arg)
//{
//  int i;
//  bool result = true;
//
//  if (count == 1) {
//    PrintHelp();
//    exit(0);
//  }
//
//#define gripe cerr << "Option '" << keyword 	\
//	<< "' requires a value, as in '"	\
//	<< keyword << "=something'" << endl << endl;/**/
//
//  for (i=1; i<count; i++) {
//    string argument = string(arg[i]);
//    string keyword(argument);
//    string value("");
//    unsigned int n=argument.find("=");
//    if (n != string::npos && n > 0) {
//      keyword = argument.substr(0, n);
//      value = argument.substr(n+1);
//    }
//
//    if (keyword == "--help") {
//      PrintHelp();
//      exit(0);
//    } else if (keyword == "--version") {
//      cout << endl << "  JSBSim Version: " << FDMExec->GetVersion() << endl << endl;
//      exit (0);
//    } else if (keyword == "--realtime") {
//      realtime = true;
//    } else if (keyword == "--nice") {
//      play_nice = true;
//    } else if (keyword == "--suspend") {
//      suspend = true;
//    } else if (keyword == "--outputlogfile") {
//      if (n != string::npos) {
//        LogOutputName = value;
//      } else {
//        LogOutputName = "JSBout.csv";
//        cerr << "  Output log file name must be specified with an = sign. Using JSBout.csv as default";
//      }
//    } else if (keyword == "--logdirectivefile") {
//      if (n != string::npos) {
//        LogDirectiveName = value;
//      } else {
//        gripe;
//        exit(1);
//      }
//    } else if (keyword == "--root") {
//      if (n != string::npos) {
//        RootDir = value;
//      } else {
//        gripe;
//        exit(1);
//      }
//    } else if (keyword == "--aircraft") {
//      if (n != string::npos) {
//        AircraftName = value;
//      } else {
//        gripe;
//        exit(1);
//      }
//    } else if (keyword == "--script") {
//      if (n != string::npos) {
//        ScriptName = value;
//      } else {
//        gripe;
//        exit(1);
//      }
//    } else if (keyword == "--initfile") {
//      if (n != string::npos) {
//        ResetName = value;
//      } else {
//        gripe;
//        exit(1);
//      }
//
//    } else if (keyword == "--property") {
//      if (n != string::npos) {
//         string propName = value.substr(0,value.find("="));
//         string propValueString = value.substr(value.find("=")+1);
//         double propValue = atof(propValueString.c_str());
//         CommandLineProperties.push_back(propName);
//         CommandLinePropertyValues.push_back(propValue);
//      } else {
//        gripe;
//        exit(1);
//      }
//
//    } else if (keyword == "--end-time") {
//      if (n != string::npos) {
//        end_time = atof( value.c_str() );
//      } else {
//        gripe;
//        exit(1);
//      }
//
//    } else if (keyword == "--catalog") {
//        catalog = true;
//    } else {
//      cerr << endl << "  Parameter: " << argument << " not understood" << endl;
//      result = false;
//    }
//  }
//
//  // Post-processing for script options. check for incompatible options.
//
//  if (catalog && !ScriptName.empty()) {
//    cerr << "Cannot specify catalog with script option" << endl << endl;
//    result = false;
//  }
//  if (AircraftName.size() > 0 && ResetName.size() == 0 && !catalog) {
//    cerr << "You must specify an initialization file with the aircraft name." << endl << endl;
//    result = false;
//  }
//  if ((ScriptName.size() > 0 && AircraftName.size() > 0) || (ScriptName.size() > 0 && ResetName.size() > 0)) {
//    cerr << "You cannot specify an aircraft or initialization file with a script." << endl;
//    result = false;
//  }
//
//  return result;
//
//}
//
////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//void PrintHelp(void)
//{
//  cout << endl << "  JSBSim version " << FDMExec->GetVersion() << endl << endl;
//  cout << "  Usage: jsbsim <options>" << endl << endl;
//  cout << "  options:" << endl;
//    cout << "    --help  returns this message" << endl;
//    cout << "    --version  returns the version number" << endl;
//    cout << "    --outputlogfile=<filename>  sets (overrides) the name of the data output file" << endl;
//    cout << "    --logdirectivefile=<filename>  specifies (overrides) the name of the data logging directives file" << endl;
//    cout << "    --root=<path>  specifies the JSBSim root directory (where aircraft/, engine/, etc. reside)" << endl;
//    cout << "    --aircraft=<filename>  specifies the name of the aircraft to be modeled" << endl;
//    cout << "    --script=<filename>  specifies a script to run" << endl;
//    cout << "    --realtime  specifies to run in actual real world time" << endl;
//    cout << "    --nice  specifies to run at lower CPU usage" << endl;
//    cout << "    --suspend  specifies to suspend the simulation after initialization" << endl;
//    cout << "    --initfile=<filename>  specifies an initilization file" << endl;
//    cout << "    --catalog specifies that all properties for this aircraft model should be printed" << endl;
//    cout << "    --property=<property_name=property_value> e.g. --property=aero/qbar-psf=3.4" << endl;
//    cout << "    --end-time=<time (double)> specifies the sim end time" << endl << endl;
//
//  cout << "  NOTE: There can be no spaces around the = sign when" << endl;
//  cout << "        an option is followed by a filename" << endl << endl;
//}


int main()
{
	cout << "Start\n";


	//// init

	//JSBSim::FGFDMExec* FDMExec = new JSBSim::FGFDMExec();
	//FDMExec->LoadModel("aircraft","engine","systems","pa28");

	//// try to set initial aircraft's position to 0,100,0

	//JSBSim::FGMassBalance* mass = FDMExec->GetMassBalance();
	//JSBSim::FGColumnVector3 CG;
	//CG.InitMatrix(0,100,0);
	////mass->SetBaseCG(CG);
 // 
	////FDMExec->GetState()->Setdt(1); // constant dt just for test



	//// random values
	//

	///*FDMExec->GetAtmosphere()->SetExTemperature(9.0/5.0*(1+273.15) );
	//FDMExec->GetAtmosphere()->SetExDensity(1);
	//FDMExec->GetAtmosphere()->SetExPressure(1);
	//FDMExec->GetPropagate()->SetSeaLevelRadius(0);
	//FDMExec->GetAtmosphere()->SetWindNED(1,0,0);
	//FDMExec->GetAtmosphere()->SetWindspeed(10);*/



	JSBSim::FGFDMExec *FDMExec = new JSBSim::FGFDMExec();
	FDMExec->SetDebugLevel(0);
	FDMExec->SetAircraftPath("aircraft");
	FDMExec->SetEnginePath("engine");
	FDMExec->SetSystemsPath("systems");
	FDMExec->LoadModel("c182");

	//FDMExec->LoadModel("aircraft","engine","systems","c172r");
	////FDMExec->GetState()->

	//JSBSim::FDState *state = new JSBSim::
	//FDMExec->GetState();
	//FDMExec->GetIC()

	FDMExec->GetIC()->SetUBodyFpsIC(0.0);
	FDMExec->GetIC()->SetVBodyFpsIC(0.0);
	FDMExec->GetIC()->SetWBodyFpsIC(0.0);

	FDMExec->GetIC()->SetLatitudeDegIC(47.0);
	FDMExec->GetIC()->SetLongitudeDegIC(122.0);

	FDMExec->GetIC()->SetPhiDegIC(0.0);
	FDMExec->GetIC()->SetThetaDegIC(0.0);
	FDMExec->GetIC()->SetPsiDegIC(12.0);

	FDMExec->GetIC()->SetAltitudeAGLFtIC(4000.8);


	FDMExec->GetAtmosphere()->SetExTemperature(9.0/5.0*(1+273.15) );
	FDMExec->GetAtmosphere()->SetExDensity(1);
	FDMExec->GetAtmosphere()->SetExPressure(1);
	FDMExec->GetPropagate()->SetSeaLevelRadius(0);
	FDMExec->GetAtmosphere()->SetWindNED(1,0,0);
	FDMExec->GetAtmosphere()->SetWindspeed(10);

	//FDMExec->GetIC()->Load("reset01");
	FDMExec->RunIC();

	//FDMExec->DoTrim(JSBSim::tGround);

	FDMExec->GetState()->Setdt(1);


	
	//
	////FDMExec->DoTrim(mode);
	////
	//
	//FDMExec->GetIC()->Load("reset01");
	//JSBSim::FGFDMExec* FDMExec = new JSBSim::FGFDMExec();
	//FDMExec->LoadScript("scripts\\B747_script1");
	bool result = FDMExec->Run();
	



	

	while (result)
	{
		/*FDMExec->GetFCS()->SetDaCmd(0.2);
		FDMExec->GetFCS()->SetDeCmd(0.3);
		FDMExec->GetFCS()->SetDrCmd(0.4);

		FDMExec->GetFCS()->SetThrottleCmd(0, 0.5);*/


		FGJSBBase::Message* msg;
    while (FDMExec->SomeMessages()) {
      msg = FDMExec->ProcessMessage();
	  //cout << msg->messageId << ": " << msg->text << endl;
      /*switch (msg->type) {
      case FGJSBBase::Message::eText:
        if (msg->text == "Crash Detected: Simulation FREEZE.")
          bool crashed = true;
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text );
        break;
      case FGJSBBase::Message::eBool:
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text << " " << msg->bVal );
        break;
      case FGJSBBase::Message::eInteger:
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text << " " << msg->iVal );
        break;
      case FGJSBBase::Message::eDouble:
        SG_LOG( SG_FLIGHT, SG_INFO, msg->messageId << ": " << msg->text << " " << msg->dVal );
        break;
      default:
        SG_LOG( SG_FLIGHT, SG_INFO, "Unrecognized message type." );
        break;
      }*/
	}
	result = FDMExec->Run();
	//FDMExec->GetAircraft()->GetXYZep(1);
	//float4 pos = float4( mass->GetXYZcg(1), mass->GetXYZcg(2), mass->GetXYZcg(3) ); // (float4 is my custom vector type)
	//pos.Dump(); 

	

	cout << "simtime: "<<FDMExec->GetSimTime() << " " << FDMExec->GetPropagate()->Gethdot() << " " << FDMExec->GetPropagate()->GetEuler(1) << " " << FDMExec->eZ << endl; 
	Sleep(1000);
	}

	



	cin.ignore();
	return 0;
}