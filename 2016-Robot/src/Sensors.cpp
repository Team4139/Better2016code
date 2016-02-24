#include "WPILib.h"
#include "Math.h"
#include "AnalogInput.h"

struct Sensors_In
{
	bool resetGyro;
};
struct Sensors_Out
{
	float returnGyroAngle;
	float AccelX, AccelY, AccelZ;
	float sonarDistance; //distance measured from the ultrasonic sensor values
};
class Sensors
{
private:
	//you do this
	AnalogInput* sonic;
	Accelerometer *accel;

public:
	const double valueTocm = 0.125; //factor to convert sensor values to a distance in centimeter

	Sensors()
	{
		//you do this
		sonic = new AnalogInput(1); // 1 is temp value
		accel = new BuiltInAccelerometer(Accelerometer::kRange_4G);

	}
	Sensors_Out Run(Sensors_In input)
	{
		Sensors_Out out;
		//out.sonarDistance = (sonic.GetValue()) * valueTocm; //sensor returns a value from 0-4095 that is scaled to inches

		//std::cout<<out.sonarDistance <<std::endl;
		out.AccelX = accel->GetX();
		out.AccelY = accel->GetY();
		out.AccelZ = accel->GetZ();
		//SmartDashboard::PutNumber("accel x",out.AccelX);
		//SmartDashboard::PutNumber("accel y",out.AccelY);
		//SmartDashboard::PutNumber("accel z",out.AccelZ);

		// code here

		return out;
	}
};
