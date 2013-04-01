#ifndef _AIRCRAFTPHYSICS
#define _AIRCRAFTPHYSICS

#include "mymath.h"
#include "colisionManager.h"
#define	_DTHRUST	100.0f
#define	_MAXTHRUST	50000.0f


//------------------------------------------------------------------------//
// Rigid body structure
//------------------------------------------------------------------------//

struct AircraftStruct {
    float fMass; // total mass (constant)
    Matrix3x3 mInertia; // mass moment of inertia in body coordinates (constant)
    Matrix3x3 mInertiaInverse; // inverse of mass moment of inertia matrix	(constant)

    Vector vPosition; // position in earth coordinates
    Vector vVelocity; // velocity in earth coordinates
    Vector vVelocityBody; // velocity in body coordinates
    Vector vAngularVelocity; // angular velocity in body coordinates
    Vector vEulerAngles; // Euler angles in body coordinates
    float fSpeed; // speed (magnitude of the velocity)

    Quaternion qOrientation; // orientation in earth coordinates
    //Matrix3x3	mRotation;		// rotation matrix

    Vector vForces; // total force on body
    Vector vMoments; // total moment (torque) on body

    Matrix3x3 mIeInverse; // inverse of moment of inertia in earth coordinates

    // testing...
    //Vector p1, p2;
    // ... end testing

	int flapChanger;

};

struct AircraftBodyElement {
    float fMass;
    Vector vDCoords;
    Vector vCGCoords;
    Vector vLocalInertia;
    float fIncidence;
    float fDihedral;
    Vector vNormal;
    float fArea;
    int iFlap;
};

class AircraftPhysics
{

private:
	float speed;
	//Vector axisSpeed;
	
	ColisionManager *colisionManager;
	

public: 
	AircraftStruct	Airplane;			// Rigid body properties of our airplane
	AircraftBodyElement	Element[8];		// Mass, inertia and lifting surface properties of our airplane
	Vector		Thrust;					// Thrust vector, assumed to act through the plane's CG
	float		ThrustForce;			// Magnitude of the thrust force
	bool		Stalling;				// Flag to let us know if we are in a stalled condition
	bool		Flaps;					// Flag to let us know if the flaps are down
	objectPosition *op;

public:
	AircraftPhysics(ColisionManager *colMan)
	{ 
		InitializeAirplane(); 
		colisionManager = colMan;
		speed = 0.0;
		op = new objectPosition(Vector(0, 0, 0), 2);
	}
	~AircraftPhysics() {};

	void InitializeAirplane(void);
	void CalcAirplaneMassProperties(void); // calcs total mass and inertia
	void CalcAirplaneLoads(void); // calcs total forces and moments
	void StepSimulation(float dt); // step dt time in the simulation
	Vector GetBodyZAxisVector(void);
	Vector GetBodyXAxisVector(void);
	Matrix3x3 MakeAngularVelocityMatrix(Vector u);
	float LiftCoefficient(float angle, int flaps);
	float DragCoefficient(float angle, int flaps);
	float RudderLiftCoefficient(float angle);
	float RudderDragCoefficient(float angle);
	void IncThrust(void);
	void DecThrust(void);
	void LeftRudder(void);
	void RightRudder(void);
	void ZeroRudder(void);
	void RollLeft(void);
	void RollRight(void);
	void PitchUp(void);
	void PitchDown(void);
	void ZeroAilerons(void);
	void ZeroElevators(void);
	void FlapsDown(void);
	void ZeroFlaps(void);
	float GetSpeed(void);
	Vector GetAxisSpeed();
	void Stabilizacija(float speed);

	void Autopilot();
	void reset();
};

#endif