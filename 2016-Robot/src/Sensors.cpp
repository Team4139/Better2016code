#include "WPILib.h"
#include "Math.h"

struct Sensors_In
{
	bool resetGyro;
};
struct Sensors_Out
{
	float returnGyroAngle;
	float AccelX, AccelY, AccelZ;
	double sonarDistance; //distance measured from the ultrasonic sensor values
};
class Sensors
{
private:
	//you do this
	AnalogInput ultrasonic;
	Accelerometer *accel;

public:
	const double valueTocm = 0.125; //factor to convert sensor values to a distance in centimeter

	Sensors()
	{
		//you do this
		uint32_t sonicchannel=ultrasonic.GetChannel();
		ultrasonic(sonicchannel);
		accel = new BuiltInAccelerometer(Accelerometer::kRange_4G);

	}
	Sensors_Out Run(Sensors_In input)
	{
		Sensors_Out out;
		out.sonarDistance = (ultrasonic.GetValue()) * valueTocm; //sensor returns a value from 0-4095 that is scaled to inches

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
