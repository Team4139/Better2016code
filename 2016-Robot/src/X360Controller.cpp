// Look andrew it exists
#include "WPILib.h"
#include "Math.h"

struct X360Controller_In
{

};
struct X360Controller_Out
{
	// Wheels
	float returnX, returnY, returnRotation;
	int turboMode; // 1: Slow, 2:Normal, 3: Turbo
	// Shooter

	// Lifter

	// Other
};

class X360Controller
{
private:
	Joystick* stick;
public:
	X360Controller()
	{
		stick = new Joystick(5);
	}
	X360Controller_Out Run(X360Controller_In input)
	{
		X360Controller_Out output;

		// Code here

		return output;
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
};
