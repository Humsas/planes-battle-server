#ifndef _TESTCRAFT
#define _TESTCRAFT

#include "colisionManager.h"

#include "FGFDMExec.h"
#include "FGState.h"
#include "models\propulsion\FGPiston.h"
//#include "math\FGLocation.h"
using namespace JSBSim;


class TestCraft
{
private:
	FGFDMExec *FDMExec;
	
	Vector initPos;


	ColisionManager *colisionManager;

	// controls
	float throttle;
	float ailerons;
	float elevator;
	float ruder;
	float flaps;

	float propAdvance;

	int starter;
	int magnetos;

	float mixture;
	

	float speed;

	float brakes;

	int gearDown;

	// returns
	double stallWarn;

	void copyToJSBSim();
	void copyFromJSBSim();

public:
	objectPosition *op;
	Vector pozicija;
	Vector rotacija;

	TestCraft(ColisionManager *colisionManager, Vector initPos);
	~TestCraft();


	void startEngine(bool starter);
	void stopEngine();

	void setElevation(double elev);

	void Update(float dt);


	// controls
	void setElerons(float pos);
	void setElevator(float pos);
	void setRuder(float pos);
	void setFlaps(float pos);

	void setThrottle(float pos);
	void increaseThrottle();
	void decreaseThrottle();

	void increaseMixture();
	void decreaseMixture();

	void increasePropAdvance();
	void decreasePropAdvance();

	void setBrakes(bool b);
	void gearUpDown();

	float getSpeed();
	bool getStall();

};


#endif