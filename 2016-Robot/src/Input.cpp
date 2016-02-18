//Written by Andrew Kuo
//glhf	
#include "WPILib.h"
#include "X360Controller.cpp"
#include "Sensors.cpp"

struct Input_In
{

};

struct Input_Out
{
	//Controller
	float returnX, returnY, returnRotation;
	int turboMode;
	int triggerPressed, buttonPressed;
	//Sensors
	float returnGyroAngle;
	float AccelX, AccelY, AccelZ;
	double sonarDistance;
	
};

class Input
{
private:
	X360Controller* controller;
	Sensors* sensors;
public:
	Input()
	{
		controller = new X360Controller();
		sensors = new Sensors();
	}
	Input_Out Run(Input_In input)
	{
		//Structs and stuff
		Input_Out output;
		
		X360Controller_In xbIn;
		X360Controller_out xbOut;
		Sensors_In sensIn;
		Sensors_Out sensOut;
		
		sensOut = sensors->Run(sensIn);
		xbOut = controller->Run(xbIn);
		
		//Retreving controller info
		output.returnX = xbOut.returnX;
		output.returnY = xbOut.returnY;
		output.returnRotation = xbOut.returnRotation;
		output.returnTurbo = xbOut.returnTurbo;
		output.returnTriggerPressed = xbOut.returnTriggerPressed;
		output.returnButtonPressed = xbOut.returnButtonPressed;
		
		//Retreving sensor info
		out.returnGyroAngle = sensOut.returnGyroAngle;
		out.AccelX = sensOut.AccelX;
		out.AccelY = sensOut.AccelY;
		out.AccelZ = sensOut.AccelZ;
		out.sonarDistance = sensOut.sonarDistance;
		
		return output;
	}
};
