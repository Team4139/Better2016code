/*
 * FRC Team 4139 "Easy as Pi"
 * Written by Zack Mudd for the 2015-2016 Season
 * Copyright 2016
 */


#include "WPILib.h"
#include "Joystick.h"
#include "nt_Value.h"
#include "Timer.h"

class PiBot2016: public IterativeRobot
{
public:
	//

private:
	RobotDrive* robot; // Robot drive system
	Joystick* stick; // Joystick
	Talon* shooter;  // Shooter wheel
	Talon* feederA;  // First motor on feeder arm
	Talon* feederB;  // Second motor on feeder arm
	Talon* feederC;  // The hex rod under the shooting panel
	Talon* liftA;    // First motor raising and lowering the feeder
	Talon* liftB;    // Second motor raising and lowering the feeder
	Encoder* encode; // The mag-encoder on one of the lift motors
	Timer* outputTime; // Used to make the rate of console output more reasonable
	Timer* time;     // For autonomous
	bool shoot; // Whether or not the shooter wheel should be spinning
	bool doesEncoderWork; // Default is false because I'm assuming it doesn't
	bool atTop; // True if the lifter arm has exceeded it's upper limit
	bool atBottom; // True if the lifter arm has exceeded it's lower limit
	double MAXVALUE; // Dummy value
	double MINVALUE;   // Dummy value

	void RobotInit()
	{
		robot      = new RobotDrive(4,3,2,1);
		stick      = new Joystick(5);
		shooter    = new Talon(8);
		feederA    = new Talon(0);
		feederB    = new Talon(9);
		feederC    = new Talon(7);
		liftA      = new Talon(5);
		liftB      = new Talon(6);
		encode     = new Encoder(0,1,false,Encoder::EncodingType::k4X);
		outputTime = new Timer();
		time       = new Timer();

		robot->SetExpiration(0.1);
		SmartDashboard::init();
		encode->SetMaxPeriod(0.1);
		encode->SetMinRate(10);
		encode->SetDistancePerPulse(64);
		encode->SetReverseDirection(true);
		encode->SetSamplesToAverage(7);
	}

	void TeleopInit()
	{
		shoot=false; // Whether or not the shooter wheel should be spinning
		doesEncoderWork=false; // Default is false because I'm assuming it doesn't
		atTop=false; // True if the lifter arm has exceeded it's upper limit
		atBottom=false; // True if the lifter arm has exceeded it's lower limit
		encode->Reset(); // Resets the encoder distance to 0
		outputTime->Reset();
		outputTime->Start();
		MAXVALUE=100; // Dummy value
		MINVALUE=0;   // Dummy value
	}

	void TeleopPeriodic()
	{
		// ArcadeDrive: Axis 1 (Left stick up/down) is forward/back - Axis 4 (right stick left/right) is spin
		robot->ArcadeDrive(stick->GetRawAxis(1), stick->GetRawAxis(4), false);

		int distance = encode->GetDistance();

		// feed: current state of feeder. 0: Not moving. 1: Taking ball in. 2: Taking ball out
		int feed = 0;
		if(stick->GetRawButton(1)){ // Button 1: A
			feed=1;
		}
		if(stick->GetRawButton(2)){ // Button 2: B
			feed=2;
		}
		if(stick->GetRawButton(3)){ // Button 3: X
			feed=0;
		}
		if(feed==1)
		{
			// 40% Power forward
			feederA->Set(0.4);
			feederB->Set(0.4);
			feederC->Set(0.4);
		}
		if(feed==2)
		{
			// 40% Power reverse
			feederA->Set(-0.4);
			feederB->Set(-0.4);
			feederC->Set(-0.4);
		}
		if(feed==0)
		{
			// 0% Power
			feederA->Set(0);
			feederB->Set(0);
			feederC->Set(0);
		}


		if(stick->GetRawButton(4)) // Button 4: Y
		{
			shoot=!shoot; // Toggle state of 'shoot'
			Wait(0.25);   // 0.25 second wait to prevent toggle spam
		}
		if(shoot)
			shooter->Set(-0.8); // Sets to 80% power (negative is forward on this motor)
		if(!shoot)
			shooter->Set(0); // Sets to 0% power

		if(!doesEncoderWork)
		{
			if(stick->GetRawAxis(2)>0.2) // If left trigger is pressed more than 20% of full depression
			{
				// Move feeder arm down
				liftA->Set(0.25*stick->GetRawAxis(2));
				liftB->Set(-0.25*stick->GetRawAxis(2)); // liftB is opposite of liftA
			}
			if(stick->GetRawAxis(3)>0.2) // If right trigger is pressed more than 20% of full depression
			{
				// Move feeder arm up
				liftA->Set(-0.25*stick->GetRawAxis(3));
				liftB->Set(0.25*stick->GetRawAxis(3));
			}
			if(stick->GetRawAxis(3)<0.2 && stick->GetRawAxis(2)<0.2) // If neither trigger is pressed more than 20%
			{
				// Set feeder arm to not move up/down
				liftA->Set(0);
				liftB->Set(0);
			}
		}
		if(doesEncoderWork)
		{
			if(stick->GetRawAxis(2)>0.2&&!atBottom) // If left trigger is pressed more than 20% of full depression
			{
				// Move feeder arm down
				liftA->Set(0.25*stick->GetRawAxis(2));
				liftB->Set(-0.25*stick->GetRawAxis(2)); // liftB is opposite of liftA
			}
			if(stick->GetRawAxis(3)>0.2&&!atTop) // If right trigger is pressed more than 20% of full depression
			{
				// Move feeder arm up
				liftA->Set(-0.25*stick->GetRawAxis(3));
				liftB->Set(0.25*stick->GetRawAxis(3));
			}
			if(stick->GetRawAxis(3)<0.2 && stick->GetRawAxis(2)<0.2) // If neither trigger is pressed more than 20%
			{
				// Set feeder arm to not move up/down
				liftA->Set(0);
				liftB->Set(0);
			}
			if(encode->GetDistance()>=MAXVALUE)
				atTop=true;
			if(encode->GetDistance()<=MINVALUE)
				atBottom=true;
			if(encode->GetDistance()<MAXVALUE&&encode->GetDistance()>MINVALUE)
			{
				atTop=false;
				atBottom=false;
			}
		}


		if(outputTime->HasPeriodPassed(1)) // Only returns true once every second
		{
			if(doesEncoderWork) // Output current measured distance of the encoder
				std::cout<<distance<<std::endl;
			if(atTop) // Print an error if the encoder has moved too far
				std::cout<<"The lifter arm has exceeded it's upper limit!"<<std::endl;
			if(atBottom)
				std::cout<<"The lifter arm has exceeded it's lower limit!"<<std::endl;
		}

		Wait(0.005);
	}
};

START_ROBOT_CLASS(PiBot2016)
