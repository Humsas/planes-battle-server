#include "AircraftB17Entity.h"

#include "console.h"
extern Console gCons;


//AircraftB17::AircraftB17(ColisionManager *colisionManager, Vector initPos)
//{
//	this->initPos = initPos;
//
//	this->colisionManager = colisionManager;	
//	op = new objectPosition(Vector(0, 0, 0), 2);
//
//
//	FDMExec = new JSBSim::FGFDMExec();
//	FDMExec->SetDebugLevel(0);
//	FDMExec->SetOutputFileName("");
//	FDMExec->SetAircraftPath("aircraft");
//	FDMExec->SetEnginePath("engine");
//	FDMExec->SetSystemsPath("systems");
//
//	//FDMExec->LoadModel("B17");
//
//	//FDMExec->GetPropulsion()->InitRunning(-1);
//
//	FDMExec->LoadScript("scripts\\testScript");
//	FDMExec->GetPropulsion()->InitRunning(-1);
//	FDMExec->GetIC()->SetTerrainElevationFtIC(initPos.z/0.3048);
//	/*FDMExec->GetIC()->SetUBodyFpsIC(0.0);
//	FDMExec->GetIC()->SetVBodyFpsIC(0.0);
//	FDMExec->GetIC()->SetWBodyFpsIC(0.0);
//
//	FDMExec->GetIC()->SetLatitudeDegIC(0);
//	FDMExec->GetIC()->SetLongitudeDegIC(0);
//
//	FDMExec->GetIC()->SetPhiDegIC(0.0);
//	FDMExec->GetIC()->SetThetaDegIC(0.0);
//	FDMExec->GetIC()->SetPsiDegIC(12.0);
//
//	FDMExec->GetIC()->SetAltitudeAGLFtIC(18000);
//	*/
//
//	/*FDMExec->GetAtmosphere()->SetExTemperature(9.0/5.0*(1+273.15) );
//	FDMExec->GetAtmosphere()->SetExDensity(1);
//	FDMExec->GetAtmosphere()->SetExPressure(200);*/
//	/*FDMExec->GetPropagate()->SetSeaLevelRadius(0);
//	FDMExec->GetAtmosphere()->SetWindNED(1,0,0);
//	FDMExec->GetAtmosphere()->SetWindspeed(10);*/
//
//	//FDMExec->GetIC()->Load("reset00");
//	//FDMExec->RunIC();
//
//	FDMExec->DoTrim(JSBSim::tGround);
//
//	//startEngine();
//
//	//FDMExec->GetPropagate()->GetLocation().
//
//	// initial settings
//	throttle	= 0.0;
//	ailerons	= 0.0;
//	elevator	= 0.0;
//	ruder		= 0.0;
//	flaps		= 0.0;
//
//	propAdvance = 0.0;
//
//	starter		= 0;
//	magnetos	= 0;
//
//	mixture		= 1.0;
//
//	speed		= 0.0;
//
//	brakes		= 0.0;
//	gearDown	= 1;
//
//	stallWarn	= 0.0;
//
//	position.x = initPos.x;
//	position.y = initPos.y;
//	position.z = initPos.z;
//
//	copyToJSBSim();
//	FDMExec->RunIC();
//	copyFromJSBSim();
//
//}

AircraftB17::~AircraftB17()
{
	delete FDMExec;
}

void AircraftB17::reset(bool fullReset)
{
	if(fullReset)
	{
		delete FDMExec;


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
		FDMExec->GetIC()->SetLongitudeDegIC(0);*/

		FDMExec->GetIC()->SetPhiDegIC(-initRotation.z);
		FDMExec->GetIC()->SetThetaDegIC(initRotation.y);
		FDMExec->GetIC()->SetPsiDegIC(-(initRotation.x+90));

		/*FDMExec->GetIC()->SetAltitudeAGLFtIC(18000);*/
		

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

		this->position.x = initPos.x;
		this->position.y = initPos.y;
		this->position.z = initPos.z;

		copyToJSBSim();
		FDMExec->RunIC();
		copyFromJSBSim();

	}
	else
	{
		FDMExec->ResetToInitialConditions();

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
	}
}

void AircraftB17::startEngine(bool starter)
{
	magnetos = 3;
	this->starter = (starter)?  1 :  0;
}

void AircraftB17::stopEngine()
{
	magnetos = 0;
}

void AircraftB17::Update(float dt)
{
	bool reset = false;
	if(!isClientStub)
	{
		mProjectileBombCooldown.UpdateCooldowns(dt);

		setElevation(CM->getMapHeightAtPoint(position));

		FDMExec->GetState()->Setdt(dt);
		copyToJSBSim();
		// info ivedimas


		FDMExec->Run();
		/*{
		reset();
		}*/


		FGJSBBase::Message* msg;
		while ((msg = FDMExec->ProcessMessage()) != NULL) {
			//      msg = fdmex->ProcessNextMessage();
			switch (msg->type) {
			case FGJSBBase::Message::eText:
				if (msg->text == "Crash Detected: Simulation FREEZE.")
				{
					reset = true;
				}
				break;
			}
		}



		// info istraukimas

		copyFromJSBSim();

		/*op->pozicija.y = FDMExec->GetPropagate()->GetLatitudeDeg() * 111000.0f + initPos.y;
		op->pozicija.x = FDMExec->GetPropagate()->GetLongitudeDeg() * 111000.0f + initPos.x;
		op->pozicija.z = FDMExec->GetPropagate()->GetAltitudeASLmeters();*/


		position.x = FDMExec->GetPropagate()->GetLatitudeDeg() * 111000.0f + initPos.x;
		position.y = FDMExec->GetPropagate()->GetLongitudeDeg() * 111000.0f + initPos.y;
		position.z = FDMExec->GetPropagate()->GetAltitudeASLmeters();



		rotarionYawPitchRoll.x = -RadiansToDegrees(FDMExec->GetPropagate()->GetEuler(3))-90;
		rotarionYawPitchRoll.y = RadiansToDegrees(FDMExec->GetPropagate()->GetEuler(2));
		rotarionYawPitchRoll.z = -RadiansToDegrees(FDMExec->GetPropagate()->GetEuler(1));


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


		/*gCons.add(ss1.str(),GAME_CONSOLE_WARNING);
		gCons.add(ss2.str(),GAME_CONSOLE_WARNING);
		gCons.add(ss3.str(),GAME_CONSOLE_WARNING);
		gCons.add(ss4.str(),GAME_CONSOLE_WARNING);
		gCons.add(ss5.str(),GAME_CONSOLE_WARNING);
		gCons.add(ss6.str(),GAME_CONSOLE_WARNING);
		gCons.add(ss7.str(),GAME_CONSOLE_WARNING);
		gCons.add(ss8.str(),GAME_CONSOLE_WARNING);*/
	}
	rotateYPR(&rotarionYawPitchRoll);
	combine();

	if(reset)
	{
		this->reset(true);
	}
	
}


void AircraftB17::setElerons(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
		ailerons = pos;
}

void AircraftB17::setElevator(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
		elevator = pos;
}

void AircraftB17::setRuder(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
		ruder = pos;
}

void AircraftB17::setFlaps(float pos)
{
	if(-1.0 <= pos && pos <= 1.0)
		flaps = pos;
}


void AircraftB17::setThrottle(float pos)
{
	throttle = pos;
	if(throttle > 1.0) throttle = 1.0;
	if(throttle < 0.00000001) throttle = 0.0;
}

void AircraftB17::increaseThrottle()
{
	if(throttle < 1.0) throttle += 0.1;
	if(throttle >= 1.0) throttle = 1.0;
}

void AircraftB17::decreaseThrottle()
{
	if(throttle > 0.0) throttle -= 0.1;
	if(throttle <= 0.00000001) throttle = 0.0;
}

void AircraftB17::increaseMixture()
{
	if(mixture < 1.0) mixture += 0.05;
	if(mixture >= 1.0) mixture = 1.0;
}


void AircraftB17::decreaseMixture()
{
	if(mixture > 0.0) mixture -= 0.05;
	if(mixture <= 0.00000001) mixture = 0.0;
}


void AircraftB17::increasePropAdvance()
{
	if(propAdvance < 1.0) propAdvance += 0.05;
	if(propAdvance >= 1.0) propAdvance = 1.0;
}


void AircraftB17::decreasePropAdvance()
{
	if(propAdvance > 0.0) propAdvance -= 0.05;
	if(propAdvance <= 0.00000001) propAdvance = 0.0;
}

void AircraftB17::copyToJSBSim()
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


void AircraftB17::copyFromJSBSim()
{
	Vector spd = Vector(
		FDMExec->GetPropagate()->GetVel(1),
		FDMExec->GetPropagate()->GetVel(2),
		FDMExec->GetPropagate()->GetVel(3));


	speed = spd.Magnitude() * 1.09728; // ft/s to km/h

	stallWarn = FDMExec->GetAerodynamics()->GetStallWarn();

	/*stringstream ss1;
	ss1 << "STALL: " << FDMExec->GetAerodynamics()->GetStallWarn(); 
	gCons.add(ss1.str(),GAME_CONSOLE_WARNING);*/
}


void AircraftB17::setBrakes(bool b)
{
	if(b)
		brakes = 0.3;
	else
		brakes = 0.0;
}

void AircraftB17::gearUpDown()
{
	gearDown = (gearDown == 1)? 0 : 1;
}


float AircraftB17::getSpeed()
{
	return speed;
}

bool AircraftB17::getStall()
{
	return (stallWarn > 0.001)? true : false;
}

void AircraftB17::setElevation(double elev)
{
	FDMExec->GetPropagate()->SetTerrainElevation(elev/0.3048);
}