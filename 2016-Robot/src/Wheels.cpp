#include "WPILib.h"

struct Wheels_In
{
	float xMovement, yMovement, rotation, gyroAngle;
	int turboMode; // 1: slow, 2: normal, 3: turbo
};
struct Wheels_Out
{

};
class Wheels
{
private:
	RobotDrive* drive;
	MotorSafetyHelper* driveSafety;
public:
	Wheels()
{
		drive = new RobotDrive(0,1,2,3); // FrontLeft, FrontRight, RearLeft, RearRight
		drive->SetExpiration(0.1);
		drive->SetSafetyEnabled(true);
		driveSafety = new MotorSafetyHelper(drive);
}
	Wheels_Out Run(Wheels_In input)
	{
		Wheels_Out output;

		// Code here

		return output;
	}
};
