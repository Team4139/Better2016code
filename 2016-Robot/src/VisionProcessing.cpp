#include "WPILib.h"
struct Vision_In
{
};
struct Vision_Out
{
	double cogX;
};
class VisionProcessing
{
private:

public:
	VisionProcessing()
{
		SmartDashboard::init();
}
	Vision_Out Run(Vision_In input)
	{
		Vision_Out output;
		output.cogX = SmartDashboard::GetNumber("COG_X", 0.0);
		return output;
	}

};
