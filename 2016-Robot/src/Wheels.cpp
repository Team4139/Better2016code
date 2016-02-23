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

public:
	RobotDrive* drive;
		MotorSafetyHelper* driveSafety;
		Joystick stick;
	int stickport;
	Wheels()
{
		drive = new RobotDrive(0,1,2,3); // FrontLeft, FrontRight, RearLeft, RearRight
		drive->SetExpiration(0.1);
		drive->SetSafetyEnabled(true);
		driveSafety = new MotorSafetyHelper(drive);
		stick(stickport);
}
	Wheels_Out Run(Wheels_In input)
	{
		Wheels_Out output;
int mode;
if(stick.GetRawButton(1)==1){//tank drive
	mode=1;
	Wait(0.5);
	std::cout<<"mode 1"<<std::endl;
}
else if(stick.GetRawButton(2)==1){//arcade drive
	mode=2;
	Wait(0.5);
	std::cout<<"mode 2"<<std::endl;
}

float xMovement=input.xMovement*(input.turboMode /2);
float yMovement=input.yMovement*(input.turboMode /2);
if(mode==1){
	drive->TankDrive(xMovement,yMovement);
}
else if(mode==2){
	drive->ArcadeDrive(yMovement,input.rotation);
}
		// Code here

		return output;
	}
};
