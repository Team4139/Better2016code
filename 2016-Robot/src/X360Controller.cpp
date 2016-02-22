// 2016 Warren
#include "WPILib.h"
#include "Math.h"

struct X360Controller_In{

};

struct X360Controller_Out{
	float leftX, leftY, rightX, rightY = 0; //Joystick X and Y for each stick
	float leftBumper, rightBumper = 0 //Left and Right bumpers (L2, R2)
	bool leftTrigger, rightTrigger = false //Left and Right Triggers (L1, R1)
	bool buttonX, buttonY, buttonA, buttonB = false //All four buttons
};

class X360Controller
{
private:
	Joystick *stick;
	const float DZ = 0.15f;

public:
	X360Controller(){
		stick = new Joystick(5);
	}

	// signFinder, used for deadzone
	float FindSign(float num)
	{
		if (num > 0)
			return 1;
		else if (num < 0)
			return -1;
		else
			return 0;
	}

	// Deadzone due to bad controllers
	float ApplyDZ(float axis, float deadzone)
	{
		return fabs(axis) < deadzone ? 0 : (fabs(axis) - deadzone) / (1	- deadzone) * FindSign(axis);
	}

	X360Controller_Out Run(X360Controller_In input)
	{
		X360Controller_Out output;
		//Left joystick
		output.rightX = ApplyDZ(stick->GetRawAxis(0), DZ);
		output.rightY = -ApplyDZ(stick->GetRawAxis(1), DZ);

		//Right joystick
		output.leftX = ApplyDZ(stick->GetRawAxis(4), DZ);
		output.leftY = -ApplyDZ(stick->GetRawAxis(5), DZ);
		
		//Rumpers (L2, R2)
		output.leftBumper = stick->GetRawAxis(2);
		output.rightBumper = stick->GetRawAxis(3);

		//Triggers (R1, L1)
		output.leftTrigger = stick->GetRawButton(5);
		output.rightTrigger = stick->GetRawButton(6);

		//Button controls
		output.buttonY = stick->GetRawButton(4);
		output.buttonX = stick->GetRawButton(3);
		output.buttonB = stick->GetRawButton(2);
		output.buttonA = stick->GetRawButton(1);

		return output;
	}
};
