/*
 * FRC Team 4139 "Easy as Pi"
 * Written by Zack Mudd for the 2015-2016 Season
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
		//  Initializing motorcontrollers (the parameters are the port numbers)
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
		sonic      = new AnalogInput(1); // 1 is a temporary value: the sonar was never functional
		robot->SetExpiration(0.1);
		encode->SetDistancePerPulse(1);
		shooterTime = new Timer();
		feedTime    = new Timer();
		button1 = new DigitalInput(6);
		button2 = new DigitalInput(5);
		encode->Reset(); // Start off the robot at 0 time
	}
	void TestInit()
	{
		// Reset the timer for TestPeriodic()
		time->Stop();
		time->Reset();
		time->Start();
	}
	void TestPeriodic()
	{
		if(time->HasPeriodPassed(0.5)) // Display output for a few controls and sensors
		{
			time->Reset(); // Reset the timer so that output will print more than once
			std::cout<<"Sonar: "<<sonic->GetVoltage()*0.125<<std::endl; // Prints the current distance measured by the sonar in linear MRUs (non-functional)
		}
	}
	void AutonomousInit()
	{
		// Reset the timer for AutonomousPeriodic()
		time->Stop();
		time->Reset();
		time->Start();
		autoStage=0; // Set the autonomous to stage 1
		encode->Reset(); // "Zeroing" the encoder
		feederPID->SetSetpoint(0); // Set the lifter to horizontal (for PID system)
	}
	void AutonomousPeriodic()
	{
		double speedMod=1;
		setting=0; // Set the lifter to horizontal (for parkinson's system)
		cogX=SmartDashboard::GetNumber("COG_X", 0.0); // Get the X coordinate of the tracked object from the computer vision system (nonfunctional)
		// Autonomous code (No vision support)
			if(encode->GetDistance() < setting-1){ // If the arm is above the current setting
				//move the arm down
				if((setting-1) - encode->GetDistance() < 2){
					speedMod = ((setting-1) - encode->GetDistance())/2;
				}
				liftA->Set(0.4 * speedMod);
				liftB->Set(-0.4 * speedMod);
			}
			if(encode->GetDistance() > setting+1){ // If the arm is below the current setting
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
				autoStage++; // Move on to the next stage
				break;
				case 1:
				robot->TankDrive(1,1); // drive forward at 80% power
				if(time->HasPeriodPassed(7)){ // Wait for 7 seconds
				robot->TankDrive(0.0,0.0); // Stop the robot
				autoStage++; // Moves stage counter beyond range of switch, ending autonomous
				setting=-20; // I think -20 is vertical, so it sets the arm to be veritcal
			}
			break;
		}
		Wait(0.005);
	}
	void TeleopInit()
	{
		shoot=false;          // Whether or not the shooter wheel should be spinning
		doesEncoderWork=true; // Default is false because I'm assuming it doesn't
		atTop=false;          // True if the lifter arm has exceeded it's upper limit
		atBottom=false;       // True if the lifter arm has exceeded it's lower limit
		outputTime->Reset();  // Resets the timer for the printing section
		outputTime->Start();  // Starts the timer for the printing section
		feedTime->Reset();    // Resets the timer for the intakes
		feedTime->Start();    // Starts the timer for the intakes
		shooterTime->Reset(); // Resets the timer for the shooter wheel
		shooterTime->Start(); // Starts the timer for the shooter wheel
		setting=-20;          // Sets the lifter to vertical, from slightly back at robotinit
	}
	void TeleopPeriodic()
	{
		// ArcadeDrive: Axis 1 (Left stick up/down) is forward/back - Axis 4 (right stick left/right) is spin
		robot->ArcadeDrive(-stick->GetRawAxis(1), -stick->GetRawAxis(4), false); // Move robot based on input from the joysticks (negative because they built the robot backwards
		if(stick->GetRawButton(7)){ 		// Calibrates the bottom to current when 'START' button is pressed
			bottom=encode->GetDistance();
		}
		if(stick->GetRawButton(8)){ 		// Calibrates the top to current when 'BACK' button is pressed
			top=encode->GetDistance();
		}
		// feed: current state of feeder. 0: Not moving. 1: Taking ball in. 2: Taking ball out
		int feed = 0; 						// Initialize the feeder to not move
		if(pad->GetRawButton(3)){ 				// Button 3: The "out" button on the custom controller
			feed=2; 					// Set the feeder to push the ball out
		}
		if(pad->GetRawButton(4)){ 				// Button 4: The "in" button on the custom controller
			feed=1; 					// Set the feeder to pull the ball in
		}
		if(!pad->GetRawButton(3)&&!pad->GetRawButton(4)){ 	// If neither feeder buttons are pushed
			feed=0; 					// Set the feeder to not move
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
		if(pad->GetRawButton(5))        // If the shooter switch is set to on
			shooter->Set(-0.60); 	// Set the shooter to run at 60% power 
		if(!pad->GetRawButton(5)) 	// If the shooter switch is set to off
			shooter->Set(0);	// Set the shooter to off
		if(!doesEncoderWork) 		// Runs if the encoder does not work
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
		// Kyle's Parkinsons Code
		liftA->Set(0);
		liftB->Set(0);
		int speedMod = 1;
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
		if(stick->GetRawButton(1)) // Maps A,B,X,Y buttons on controller to different presets for the feeder arm
			setting=-130;
		if(stick->GetRawButton(3))
			setting=-20;
		if(stick->GetRawButton(2))
			setting=-130/2;
		if(stick->GetRawButton(4))
			setting=90;
		if(encode->GetDistance() < setting-1){ // If the arm is above where it should be
			//move the arm down
			if((setting-1) - encode->GetDistance() < 2){
				speedMod = ((setting-1) - encode->GetDistance())/2; // Some maths to figure out approximatly how fast the arm should move to not go full seizure mode
			}
			liftA->Set(0.4 * speedMod);
			liftB->Set(-0.4 * speedMod);
		}
		if(encode->GetDistance() > setting+1){ // If the arm is below where it should be
			//move the arm up
			if(encode->GetDistance() - (setting+1) < 2){
				speedMod = (encode->GetDistance() - (setting+1))/2; // Some maths to figure out approximatly how fast the arm should move to not go full seizure mode
			}
			liftA->Set(-0.4 * speedMod);
			liftB->Set(0.4 * speedMod);
		}
		if(encode->GetDistance()<top&&encode->GetDistance()>bottom)
		{
			atTop=false;
			atBottom=false;
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
