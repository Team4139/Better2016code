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
	float leftX, leftY, rightX, rightY;
	float leftTrigger, rightTrigger;
	bool leftBumper, rightBumper;
	bool buttonX, buttonY, buttonA, buttonB;
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
		output.rightX = xbOut.rightX;
		output.rightY = xbOut.rightY;
		output.leftX = xbOut.leftX;
		output.leftY = xbOut.leftY;
		output.leftTrigger = xbOut.leftTrigger;
		output.rightTrigger = xbOut.rightTrigger;
		output.leftBumper = xbOut.leftBumper;
		output.rightBumper = xbOut.rightBumper;
		output.buttonY = xbOut.buttonY;
		output.buttonX = xbOut.buttonX;
		output.buttonB = xbOut.buttonB;
		output.buttonA = xbOut.buttonA;
		
		//Retreving sensor info
		output.returnGyroAngle = sensOut.returnGyroAngle;
		output.AccelX = sensOut.AccelX;
		output.AccelY = sensOut.AccelY;
		output.AccelZ = sensOut.AccelZ;
		output.sonarDistance = sensOut.sonarDistance;
		
		return output;
	}
};
