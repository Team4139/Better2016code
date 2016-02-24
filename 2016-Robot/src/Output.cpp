#include "WPILib.h"
#include "Wheels.cpp"
#include "Lifter.cpp"
#include "Loader.cpp"
#include "Shooter.cpp"

struct Output_In
{
	//Wheels
	float xMovement, yMovement, rotation, gyroAngle;
	int turboMode;
	//Feeder
	float feederMovement;
	//shooter
	bool spin;


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
	Shooter* shooter;
public:
	Output()
	{
		wheels = new Wheels();
		lifter = new Lifter();
		loader = new Loader();
		shooter = new Shooter();
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
		Shooter_Out shOut;
		Shooter_In shIn;

		wIn.yMovement = input.yMovement;
		wIn.rotation = input.rotation;
		wIn.turboMode = input.turboMode;

		wOut = wheels->Run(wIn);


		lfOut = lifter->Run(lfIn);


		lOut = loader->Run(lIn);

		shOut = shooter->Run(shIn);

		return output;
	}
};
