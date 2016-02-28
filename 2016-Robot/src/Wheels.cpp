#include "WPILib.h"

struct Wheels_In
{
	float yMovement, rotation;
	//int turboMode; // 1: slow, 2: normal, 3: turbo
};
struct Wheels_Out
{

};
class Wheels
{

public:
	RobotDrive* drive;
	MotorSafetyHelper* driveSafety;
	Wheels()
	{
		drive = new RobotDrive(4,3,2,1); // FrontLeft, RearLeft, FrontRight, RearRight
		drive->SetExpiration(0.1);
		drive->SetSafetyEnabled(true);
		driveSafety = new MotorSafetyHelper(drive);
	}
	Wheels_Out Run(Wheels_In input)
	{
		Wheels_Out output;
		driveSafety->Feed();//feeding motor safety
		drive->ArcadeDrive(input.yMovement,input.rotation);
		return output;
	}
	// Code here
};
