#include "JSBSIMAircraft.h"

#include "console.h"
extern Console cons;

TestCraft::TestCraft(ColisionManager *colisionManager, Vector initPos)
{
	this->initPos = initPos;

	this->colisionManager = colisionManager;	
	op = new objectPosition(Vector(0, 0, 0), 2);

	
	FDMExec = new JSBSim::FGFDMExec();
	FDMExec->SetDebugLevel(0);
	FDMExec->SetOutputFileName("");
	FDMExec->SetAircraftPath("aircraft");
	FDMExec->SetEnginePath("engine");
	FDMExec->SetSystemsPath("systems");
	
	//FDMExec->LoadModel("B17");

	//FDMExec->GetPropulsion()->InitRunning(-1);

	FDMExec->LoadScript("scripts\\testScript");
	FDMExec->GetPropulsion()->InitRunning(-1);
	FDMExec->GetIC()->SetTerrainElevationFtIC(initPos.z/0.3048);
	/*FDMExec->GetIC()->SetUBodyFpsIC(0.0);
	FDMExec->GetIC()->SetVBodyFpsIC(0.0);
	FDMExec->GetIC()->SetWBodyFpsIC(0.0);

	FDMExec->GetIC()->SetLatitudeDegIC(0);
	FDMExec->GetIC()->SetLongitudeDegIC(0);

	FDMExec->GetIC()->SetPhiDegIC(0.0);
	FDMExec->GetIC()->SetThetaDegIC(0.0);
	FDMExec->GetIC()->SetPsiDegIC(12.0);

	FDMExec->GetIC()->SetAltitudeAGLFtIC(18000);
*/

	/*FDMExec->GetAtmosphere()->SetExTemperature(9.0/5.0*(1+273.15) );
	FDMExec->GetAtmosphere()->SetExDensity(1);
	FDMExec->GetAtmosphere()->SetExPressure(200);*/
	/*FDMExec->GetPropagate()->SetSeaLevelRadius(0);
	FDMExec->GetAtmosphere()->SetWindNED(1,0,0);
	FDMExec->GetAtmosphere()->SetWindspeed(10);*/

	//FDMExec->GetIC()->Load("reset00");
	//FDMExec->RunIC();

	FDMExec->DoTrim(JSBSim::tGround);

	//startEngine();

	//FDMExec->GetPropagate()->GetLocation().
	
	// initial settings
	throttle	= 0.0;
	ailerons	= 0.0;
	elevator	= 0.0;
	ruder		= 0.0;
	flaps		= 0.0;

	propAdvance = 0.0;

	starter		= 0;
	magnetos	= 0;

	mixture		= 1.0;

	speed		= 0.0;

	brakes		= 0.0;
	gearDown	= 1;

	stallWarn	= 0.0;

	pozicija.x = initPos.x;
	pozicija.y = initPos.y;
	pozicija.z = initPos.z;

	copyToJSBSim();
	FDMExec->RunIC();
	copyFromJSBSim();

}

TestCraft::~TestCraft()
{
	delete FDMExec;
}


void TestCraft::startEngine(bool starter)
{
	magnetos = 3;
	this->starter = (starter)?  1 :  0;
}

void TestCraft::stopEngine()
{
	magnetos = 0;
}

void TestCraft::Update(float dt)
{
	FDMExec->GetState()->Setdt(dt);
	copyToJSBSim();
	// info ivedimas
	
	
	FDMExec->Run();
	
	// info istraukimas

	copyFromJSBSim();
	
	op->pozicija.y = FDMExec->GetPropagate()->GetLatitudeDeg() * 111000.0f + initPos.y;
	op->pozicija.x = FDMExec->GetPropagate()->GetLongitudeDeg() * 111000.0f + initPos.x;
	op->pozicija.z = FDMExec->GetPropagate()->GetAltitudeASLmeters();


	pozicija.y = FDMExec->GetPropagate()->GetLatitudeDeg() * 111000.0f + initPos.y;
	pozicija.x = FDMExec->GetPropagate()->GetLongitudeDeg() * 111000.0f + initPos.x;
	pozicija.z = FDMExec->GetPropagate()->GetAltitudeASLmeters();

	
	rotacija.x = RadiansToDegrees(FDMExec->GetPropagate()->GetEuler(1));
	rotacija.y = RadiansToDegrees(FDMExec->GetPropagate()->GetEuler(2));
	rotacija.z = RadiansToDegrees(FDMExec->GetPropagate()->GetEuler(3));


	FGPiston* eng = (FGPiston*)FDMExec->GetPropulsion()->GetEngine(0);
			
			

	//stringstream ss1;//create a stringstream
	//stringstream ss2;
	//stringstream ss3;
	//stringstream ss4;
	//stringstream ss5;
	//stringstream ss6;
	//stringstream ss7;
	//stringstream ss8;
	//
	//ss1 << "Runing: " << eng->GetRunning();
	//ss2 << "RPM: " << eng->getRPM();
	//ss3 << "Starter: " << eng->GetStarter();
	//ss4 << "Starved: " << eng->GetStarved();
	//ss5 << "Throttle: " << eng->GetThrottle();
	//ss6 << "Mixture: " << eng->GetMixture();
	//ss7 << "Flaps: " << flaps;
	//ss8 << "Prop Pitch: " << propAdvance;
	
	
	/*cons.add(ss1.str(),GAME_CONSOLE_WARNING);
	cons.add(ss2.str(),GAME_CONSOLE_WARNING);
	cons.add(ss3.str(),GAME_CONSOLE_WARNING);
	cons.add(ss4.str(),GAME_CONSOLE_WARNING);
	cons.add(ss5.str(),GAME_CONSOLE_WARNING);
	cons.add(ss6.str(),GAME_CONSOLE_WARNING);
	cons.add(ss7.str(),GAME_CONSOLE_WARNING);
	cons.add(ss8.str(),GAME_CONSOLE_WARNING);*/
}


void TestCraft::setElerons(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
	ailerons = pos;
}

void TestCraft::setElevator(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
	elevator = pos;
}

void TestCraft::setRuder(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
	ruder = pos;
}

void TestCraft::setFlaps(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
	flaps = pos;
}


void TestCraft::setThrottle(float pos)
{
	throttle = pos;
	if(throttle > 1.0) throttle = 1.0;
	if(throttle < 0.00000001) throttle = 0.0;
}

void TestCraft::increaseThrottle()
{
	if(throttle < 1.0) throttle += 0.1;
	if(throttle >= 1.0) throttle = 1.0;
}

void TestCraft::decreaseThrottle()
{
	if(throttle > 0.0) throttle -= 0.1;
	if(throttle <= 0.00000001) throttle = 0.0;
}

void TestCraft::increaseMixture()
{
	if(mixture < 1.0) mixture += 0.05;
	if(mixture >= 1.0) mixture = 1.0;
}


void TestCraft::decreaseMixture()
{
	if(mixture > 0.0) mixture -= 0.05;
	if(mixture <= 0.00000001) mixture = 0.0;
}


void TestCraft::increasePropAdvance()
{
	if(propAdvance < 1.0) propAdvance += 0.05;
	if(propAdvance >= 1.0) propAdvance = 1.0;
}


void TestCraft::decreasePropAdvance()
{
	if(propAdvance > 0.0) propAdvance -= 0.05;
	if(propAdvance <= 0.00000001) propAdvance = 0.0;
}

void TestCraft::copyToJSBSim()
{
	FDMExec->GetFCS()->SetDaCmd(ailerons);
	FDMExec->GetFCS()->SetDeCmd(elevator);
	FDMExec->GetFCS()->SetDrCmd(ruder);
	FDMExec->GetFCS()->SetDfCmd(flaps);

	FDMExec->GetFCS()->SetCBrake(brakes);
	FDMExec->GetFCS()->SetLBrake(brakes);
	FDMExec->GetFCS()->SetRBrake(brakes);

	FDMExec->GetFCS()->SetDsCmd(ruder);

	FDMExec->GetFCS()->SetGearCmd(gearDown);


	for(int i = 0; i < FDMExec->GetPropulsion()->GetNumEngines(); i++)
	{
		FDMExec->GetFCS()->SetThrottleCmd(i, throttle);
		FDMExec->GetFCS()->SetMixtureCmd(i, mixture);

		FDMExec->GetFCS()->SetPropAdvance(i, propAdvance);
		
	}

	FDMExec->GetPropulsion()->SetMagnetos( magnetos );
    FDMExec->GetPropulsion()->SetStarter( starter );
	

	//FDMExec->GetGroundReactions()->GetGearUnit(1)->
}


void TestCraft::copyFromJSBSim()
{
	Vector spd = Vector(
						FDMExec->GetPropagate()->GetVel(1),
						FDMExec->GetPropagate()->GetVel(2),
						FDMExec->GetPropagate()->GetVel(3));

	
	speed = spd.Magnitude() * 1.09728; // ft/s to km/h

	stallWarn = FDMExec->GetAerodynamics()->GetStallWarn();

	/*stringstream ss1;
	ss1 << "STALL: " << FDMExec->GetAerodynamics()->GetStallWarn(); 
	cons.add(ss1.str(),GAME_CONSOLE_WARNING);*/
}


void TestCraft::setBrakes(bool b)
{
	if(b)
		brakes = 0.3;
	else
		brakes = 0.0;
}

void TestCraft::gearUpDown()
{
	gearDown = (gearDown == 1)? 0 : 1;
}


float TestCraft::getSpeed()
{
	return speed;
}

bool TestCraft::getStall()
{
	return (stallWarn > 0.001)? true : false;
}

void TestCraft::setElevation(double elev)
{
	//stringstream ss1;//create a stringstream
	
	
	
	//ss1 << "X: " << pozicija.x << " Y: " << pozicija.y <<  " Elevation: " << elev/0.3048;
	
	
	
	//cons.add(ss1.str(),GAME_CONSOLE_WARNING);
	FDMExec->GetPropagate()->SetTerrainElevation(elev/0.3048);
}