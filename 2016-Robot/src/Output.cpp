#include "WPILib.h"
#include "Wheels.cpp"
#include "Lifter.cpp"
#include "Loader.cpp"

struct Output_In
{
	//Wheels
	float xMovement, yMovement, rotation, gyroAngle;
	int turboMode;
	//Lifter
	float zMovement;
	//Loader
	float movement;

};
struct Output_Out
{
	//Nothing?
};
class Output
{
private:
	Wheels* wheels;
	Lifter* lifter;
	Loader* loader;
public:
	Output()
	{
		wheels = new Wheels();
		lifter = new Lifter();
		loader = new Loader();
	}
	Output_Out Run(Output_In input)
	{
		Output_Out output;

		Wheels_In wIn;
		Wheels_Out wOut;
		Lifter_In lfIn;
		Lifter_Out lfOut;
		Loader_In lIn;
		Loader_Out lOut;

		wIn.xMovement = input.xMovement;
		wIn.yMovement = input.yMovement;
		wIn.rotation = input.rotation;
		wIn.gyroAngle = input.gyroAngle;
		wIn.TurboMode = input.turboMode;

		wOut = wheels->Run(wIn);

		lfIn.zMovement = input.zMovement;

		lfOut = lifter->Run(lfIn);

		lIn.movement = input.movement;

		lOut = loader->Run(lIn);

		return output;
	}
};
