// 2016 Warren
#include "WPILib.h"
#include "Math.h"

struct X360Controller_In{

};

struct X360Controller_Out{
	float returnX, returnY, returnRotation = 0;
	bool returnTurbo, returnRightTrigger = false;
	int returnButtonPressed = 0; //Returns a int corresponding to current button
								 //Returns 0 if no buttons are being pressed
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
		output.returnX = ApplyDZ(stick->GetRawAxis(0), DZ);
		output.returnY = -ApplyDZ(stick->GetRawAxis(1), DZ);

		//Rotation
		output.returnRotation += stick->GetRawAxis(2); //Left Bumper I think
		output.returnRotation += -stick->GetRawAxis(3); //Right Bumper I think

		//Turbo mode
		output.returnTurbo = stick->GetRawButton(5); //Left trigger

		//Button controls - Only one button per cycle. Check if more
		//buttons are necessary.
		if(stick->GetRawButton(1)){ //Button A
			output.returnButtonPressed = 1;
		} else if(stick->GetRawButton(2)){ //Button B
			output.returnButtonPressed = 2;
		} else if(stick->GetRawButton(4)){ //Button Y
			output.returnButtonPressed = 4;
		} else if(stick->GetRawButton(3)){ //Button X
			output.returnButtonPressed = 3;
		}

		//Just right trigger as both bumpers handle rotation, and left
		//trigger handles turbo mode on or off
		if(stick->GetRawButton(6)){
			output.returnRightTrigger = true;
		}

		return output;
	}
};
