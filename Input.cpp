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
	float returnAccelX, returnAccelY, returnAccelZ;
	float returnDistance;
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
		output.returnGyroAngle = sensOut.returnGyroAngle;
		output.returnAccelX = sensOut.returnAccelX;
		output.returnAccelY = sensOut.returnAccelY;
		output.returnAccelZ = sensOut.returnAccelZ;
		output.returnDistance = sensOut.returnDistance;
		
		return output;
	}
};
