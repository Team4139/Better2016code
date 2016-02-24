#include "WPILib.h"
#include "Input.cpp"
#include "Output.cpp"
#include "VisionProcessing.cpp"

class PiBot2016: public IterativeRobot
{
public:
	//
private:
	Output* output;
	Input* input;
	Timer* timer;
	int autonomousMethod, autonomousStage;
	void RobotInit()
	{
		output = new Output();
		input  = new Input();
		timer  = new Timer();
	}


	void AutonomousInit()
	{
		timer->Stop();
		timer->Reset();
		autonomousMethod = 1; // Number corresponds to starting gate
		autonomousStage  = 0; // 0: Not Started, 1: Moving Foreward, 2: Spinning, 3: Shooting
	}

	void AutonomousPeriodic()
	{
		Input_In aInputIn;
		Input_Out aInputOut;
		Output_In aOutputIn;
		Output_Out aOutputOut;

		aInputOut = input->Run(aInputIn);
	}

	void TeleopInit()
	{
		timer->Stop();
		timer->Reset();
	}

	void TeleopPeriodic()
	{
		Input_In inputIn;
		Input_Out inputOut;
		Output_In outputIn;
		Output_Out outputOut;

		Input_Out = input->Run(inputIn);

		outputIn.xMovement=0;
		outputIn.yMovement=inputOut.leftY;
		outputIn.rotation=inputOut.rightX;
		outputIn.gyroAngle=0;
		outputIn.spin=false;
		if(inputOut.buttonY){
			outputIn.spin=!outputIn.spin;
			Wait(0.5);
		}
		while(inputOut.buttonX)
			outputIn.feederMovement=-1;
		while(inputOut.buttonB)
			outputIn.feederMovement=1;
		outputIn.turboMode=0;
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(PiBot2016)
