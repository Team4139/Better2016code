#include "WPILib.h"
#include "Math.h"
struct Sensors_In
{
	bool resetGyro;
};
struct Sensors_Out
{
	float returnGyroAngle;
	float returnAccelX, returnAccelY, returnAccelz;
	float returnDistance;
};
class Sensors
{
private:
	//you do this
public:
	Sensors()
	{
		//you do this
	}
	Sensors_Out Run(Sensors_In input)
	{
		Sensors_Out output;

		// code here

		return output;
	}
};
