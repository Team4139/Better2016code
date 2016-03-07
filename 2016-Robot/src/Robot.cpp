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
	RobotDrive* robot;        // Robot drive system
	Joystick* stick;          // Joystick
	Joystick* pad;            // The gamepad
	Talon* shooter;           // Shooter wheel
	Talon* feederA;           // First motor on feeder arm
	Talon* feederB;           // Second motor on feeder arm
	Talon* feederC;           // The hex rod under the shooting panel
	Talon* liftA;             // First motor raising and lowering the feeder
	Talon* liftB;             // Second motor raising and lowering the feeder
	Encoder* encode;          // The mag-encoder on one of the lift motors
	Timer* outputTime;        // Used to make the rate of console output more reasonable
	Timer* time;              // For autonomous
	Timer* feedTime;          // For the feeder
	bool shoot;               // Whether or not the shooter wheel should be spinning
	bool doesEncoderWork;     // Default is false because I'm assuming it doesn't
	bool atTop;               // True if the lifter arm has exceeded it's upper limit
	bool atBottom;            // True if the lifter arm has exceeded it's lower limit
	double top;               // Post-calibration feeder arm upper limit
	double bottom;            // Post-calibration feeder arm lower limit
	double FEED_DEADZONE;     // Deadzone parameter for feeder arm
	double setting;           // Current desired feeder angle (in linear MRUs)
	double cogX;              // X coordinate of detected object
	int autoStage;            // current stage of autonomous
	float sonarDistance;      // Current distance read by sonar
	AnalogInput* sonic;       // The distance read from the sonar, in MRUs
	Timer* shooterTime;       // Timer for the shooter
	PIDController* feederPID; // PID Controller to remove parkinsons from feeder arm
	PIDSource* feederInput;   // PID Source reading from the encoder
	PIDOutput* feederOutput;  // PID Output reading to liftA
	DigitalInput* button1;    // Digital input reading from Button 1 on kento's thing
	DigitalInput* button2;    // Digital input reading from Button 2 on kento's thing
	double speedMod;          // Modification parameter for the speed of the lifter
	int timesRun;             // Pretty sure this was unused
	void RobotInit()
	{
		robot      = new RobotDrive(4,3,2,1);
		stick      = new Joystick(5);
		pad        = new Joystick(4);
		shooter    = new Talon(8);
		feederA    = new Talon(0);
		feederB    = new Talon(9);
		feederC    = new Talon(7);
		liftA      = new Talon(5);
		liftB      = new Talon(6);
		encode     = new Encoder(0,1,false,Encoder::EncodingType::k4X);
		outputTime = new Timer();
		time       = new Timer();
		setting    = 0;
		sonic      = new AnalogInput(1); // 1 is temp value
		FEED_DEADZONE=5; // DUMMY VALUE
		robot->SetExpiration(0.1);
		SmartDashboard::init();
		encode->SetDistancePerPulse(1);
		shooterTime=new Timer();
		feedTime = new Timer();
		feederPID = new PIDController(5,3,3,encode,liftA,0.005);
		feederPID->SetOutputRange(-130,-20);
		feederPID->SetInputRange(-135,0);
		feederPID->SetSetpoint(-20);
		button1 = new DigitalInput(6);
		button2 = new DigitalInput(5);
		timesRun=0;
		encode->Reset();
	}
	void TestInit()
	{
		time->Stop();
		time->Reset();
		time->Start();
	}
	void TestPeriodic()
	{
		if(time->HasPeriodPassed(0.5))
		{
			time->Reset();
			std::cout<<"Button 1: "<<button1->Get()<<" | Button 2: "<<button2->Get()<<" | Sonar: "<<sonic->GetVoltage()*0.125<<std::endl;
		}
	}
	void AutonomousInit()
	{
		time->Stop();
		time->Reset();
		time->Start();
		autoStage=0;
		encode->Reset();
		feederPID->SetSetpoint(0); // Set the lifter to horizontal (for PID system)
	}
	void AutonomousPeriodic()
	{
		double speedMod=1;
		setting=0; // Set the lifter to horizontal (for parkinson's system)
		cogX=SmartDashboard::GetNumber("COG_X", 0.0);
		std::cout<<"Button 1: "<<button1->Get()<<" | Button 2: "<<button2->Get()<<" | Sonar: "<<sonic->GetVoltage()*0.125<<std::endl;
		if(true){ // Set true to run autonomous (NO VISION VERSION)
			if(encode->GetDistance() < setting-1){
				//move the arm down
				if((setting-1) - encode->GetDistance() < 2){
					speedMod = ((setting-1) - encode->GetDistance())/2;
				}
				liftA->Set(0.4 * speedMod);
				liftB->Set(-0.4 * speedMod);
			}
			if(encode->GetDistance() > setting+1){
				//move the arm up
				if(encode->GetDistance() - (setting+1) < 2){
					speedMod = (encode->GetDistance() - (setting+1))/2;
				}
				liftA->Set(-0.4 * speedMod);
				liftB->Set(0.4 * speedMod);
			}
			switch(autoStage){
			case 0:
				setting=0; // Set lifter to horizontal
				autoStage++;
				break;
			case 1:
				robot->TankDrive(1,1); // drive forward at 80% power
				if(time->HasPeriodPassed(7)){ // Wait for 7 seconds
				robot->TankDrive(0.0,0.0); // Stop the robot
				autoStage++;
				setting=-20;
				}
				break;
			}
		}
		if(false) // put true here to run autonomous (VISION VERSION)
		{
			switch(autoStage)
			{
			case 0: // Initial stuff
				// something to get the feeder off the ground or something
				autoStage++;
				break;
			case 1: // Move forward for 5 seconds
				robot->TankDrive(0.5, 0.5);
				if(time->Get()>5)
				{
					autoStage++;
					robot->TankDrive(0.0,0.0);
					time->Reset();
				}
				break;
			case 2: // Spin until the robot is pointing at the goal
				if(cogX>165||cogX<155)
					robot->TankDrive(0.2,-0.2);
				if(!(cogX>165||cogX<155))
					robot->TankDrive(0.0,0.0);
				autoStage++;
				break;
			case 3: // Move forward until close enough to the goal
				if(false) // Put true here if there is a sonar
				{
					sonarDistance=sonic->GetVoltage()/8;
					if(sonarDistance>100) // 100 is a temp value
						robot->TankDrive(0.3,0.3);
					else if(sonarDistance<=100) // 100 is a temp value
						robot->TankDrive(0.0,0.0);
					if(cogX>165||cogX<155)
					{
						autoStage=2;
						break;
					}
				}
				autoStage++;
				break;
			case 4: // Fire the ball
				shooter->Set(-0.8);
				time->Reset();
				if(time->HasPeriodPassed(10))
				{
					feederC->Set(0.4);
					time->Reset();
					if(time->HasPeriodPassed(1))
					{
						feederC->Set(0);
						shooter->Set(0);
					}
				}
				break;
			}
		}
		Wait(0.005);
	}
	void TeleopInit()
	{
		shoot=false;          // Whether or not the shooter wheel should be spinning
		doesEncoderWork=true; // Default is false because I'm assuming it doesn't
		atTop=false;          // True if the lifter arm has exceeded it's upper limit
		atBottom=false;       // True if the lifter arm has exceeded it's lower limit
		//encode->Reset();    // Resets the encoder distance to 0
		outputTime->Reset();  // Resets the timer for the printing section
		outputTime->Start();  // Starts the timer for the printing section
		feedTime->Reset();    // Resets the timer for the intakes
		feedTime->Start();    // Starts the timer for the intakes
		//top=0;              // I got rid of this for a reason
		shooterTime->Reset(); // Resets the timer for the shooter wheel
		shooterTime->Start(); // Starts the timer for the shooter wheel
		setting=-20;          // Sets the lifter to vertical, from slightly back at robotinit
	}
	void TeleopPeriodic()
	{
		// ArcadeDrive: Axis 1 (Left stick up/down) is forward/back - Axis 4 (right stick left/right) is spin
		robot->ArcadeDrive(-stick->GetRawAxis(1), -stick->GetRawAxis(4), false); // Move robot based on input from the joysticks (negative because they built the robot backwards
		if(stick->GetRawButton(7)){ // Calibrates the bottom to current when 'START' button is pressed
			bottom=encode->GetDistance();
		}
		if(stick->GetRawButton(8)){ // Calibrates the top to current when 'BACK' button is pressed
			top=encode->GetDistance();
		}
		// feed: current state of feeder. 0: Not moving. 1: Taking ball in. 2: Taking ball out
		int feed = 0;
		if(pad->GetRawButton(2)){ // Button 1: A
			feed=1;
		}
		if(pad->GetRawButton(3)){ // Button 2: B
			feed=2;
		}
		if(pad->GetRawButton(4)){ // Button 4: Y
			feed=1;
		}
		if(!pad->GetRawButton(3)&&pad->GetRawButton(4)&&!pad->GetRawButton(5)){ // Button 3: X
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
		if(pad->GetRawButton(5)) // Feeder on/off switch on control board
			shooter->Set(-0.60);
		if(!pad->GetRawButton(5))
			shooter->Set(0);
		if(!doesEncoderWork) // Runs if the encoder does not work
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
		if(true) // Set true to re-enable Kyle's parkinsons code
		{
			liftA->Set(0);
			liftB->Set(0);
			if(stick->GetRawAxis(2)>0.2) // If left trigger is pressed more than 20% of full depression
			{
				// Move feeder arm down
				setting -= 0.75;
			}
			if(stick->GetRawAxis(3)>0.2) // If right trigger is pressed more than 20% of full depression
			{
				// Move feeder arm up
				setting += 0.75;
			}
			if(stick->GetRawButton(1))
				setting=-130;
			int speedMod = 1;
			if(stick->GetRawButton(3)){
				setting=-20;
			}
			if(stick->GetRawButton(2))
				setting=-130/2;
			if(stick->GetRawButton(4))
				setting=90;
			if(encode->GetDistance() < setting-1){
				//move the arm down
				if((setting-1) - encode->GetDistance() < 2){
					speedMod = ((setting-1) - encode->GetDistance())/2;
				}
				liftA->Set(0.4 * speedMod);
				liftB->Set(-0.4 * speedMod);
			}
			if(encode->GetDistance() > setting+1){
				//move the arm up
				if(encode->GetDistance() - (setting+1) < 2){
					speedMod = (encode->GetDistance() - (setting+1))/2;
				}
				liftA->Set(-0.4 * speedMod);
				liftB->Set(0.4 * speedMod);
			}
			if(encode->GetDistance()<top&&encode->GetDistance()>bottom)
			{
				atTop=false;
				atBottom=false;
			}
		}
		if(false) //false to disable test PID controller
		{
			if(stick->GetRawAxis(2)>0.2)
				feederPID->SetSetpoint(feederPID->GetSetpoint()-0.75);
			if(stick->GetRawAxis(3)>0.2)
				feederPID->SetSetpoint(feederPID->GetSetpoint()+0.75);
			if(stick->GetRawButton(1))
				feederPID->SetSetpoint(-130);
			if(stick->GetRawButton(3))
				feederPID->SetSetpoint(-20);
			if(stick->GetRawButton(2))
				feederPID->SetSetpoint(-130/2);
			if(stick->GetRawButton(4))
				feederPID->SetSetpoint(-90);
			liftA->Set(feederPID->Get());
			liftB->Set(feederPID->Get());
		}
		if(outputTime->HasPeriodPassed(1)) // Only returns true once every second
		{
			if(doesEncoderWork) // Output current measured distance of the encoder
				std::cout<<encode->GetDistance()<<std::endl;
			if(atTop) // Print an error if the encoder has moved too far
				std::cout<<"The lifter arm has exceeded it's upper limit!"<<std::endl;
			if(atBottom)
				std::cout<<"The lifter arm has exceeded it's lower limit!"<<std::endl;
			std::cout<<"Top is: "<<top<<std::endl<<"Bottom is: "<<bottom<<std::endl;
			outputTime->Reset();
		}
		std::cout<<encode->GetDistance()<<":"<<setting<<std::endl;
		Wait(0.005);
	}
};
START_ROBOT_CLASS(PiBot2016)
