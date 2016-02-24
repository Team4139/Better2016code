#include "WPILib.h"
#include "Joystick.h"
#include "nt_Value.h"
#include "Math.h"
class Robot: public SampleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Victor shooter;
	Talon feederA;
	Talon feederB;
	Talon feederC;
	Talon liftA;
	Talon liftB;
	bool isMoving=false;

public:
	Robot() :
		myRobot(4,3,2,1),
		stick(5),
		shooter(8),
		feederA(0),
		feederB(9),
		feederC(7),
		liftA(5),
		liftB(6)
{
		myRobot.SetExpiration(0.1);
		SmartDashboard::init();

}
	void OperatorControl()
	{
		bool shoot=false;
		while (IsOperatorControl() && IsEnabled())
		{
			myRobot.ArcadeDrive(stick.GetRawAxis(1), stick.GetRawAxis(4), false);

			int feed = 0;
			if(stick.GetRawButton(1)){
				feed=1;
				//Wait(0.25);
			}
			if(stick.GetRawButton(2)){
				feed=2;
				//Wait(0.25);
			}
			if(stick.GetRawButton(3)){
				feed=0;
				//Wait(0.25);
			}
			if(feed==1)
			{
				feederA.Set(0.4);
				feederB.Set(0.4);
				feederC.Set(0.4);
			}
			if(feed==2)
			{
				feederA.Set(-0.4);
				feederB.Set(-0.4);
				feederC.Set(-0.4);
			}
			if(feed==0)
			{
				feederA.Set(0);
				feederB.Set(0);
				feederC.Set(0);
			}


			if(stick.GetRawButton(4))
			{
				shoot=!shoot;
				Wait(0.25);
			}
			if(shoot)
				shooter.Set(-0.8);
			if(!shoot)
				shooter.Set(0);

			if(stick.GetRawAxis(2)>0.2)
			{
				liftA.Set(0.25*stick.GetRawAxis(2));
				liftB.Set(-0.25*stick.GetRawAxis(2));
			}
			if(stick.GetRawAxis(3)>0.2)
			{
				liftA.Set(-0.25*stick.GetRawAxis(3));
				liftB.Set(0.25*stick.GetRawAxis(3));
			}
			if(stick.GetRawAxis(3)<0.2 && stick.GetRawAxis(2)<0.2)
			{
				liftA.Set(0);
				liftB.Set(0);
			}


			std::cout<<"2: "<<stick.GetRawAxis(2)<<"     3: "<<stick.GetRawAxis(3)<<std::endl;
			Wait(0.005);
		}
	}
};


START_ROBOT_CLASS(Robot)
