#include "WPILib.h"
//created by Dexin Zhou
struct Loader_In
{
	//float movement;
};
struct Loader_Out
{
bool loaddirection;
};
class Loader
{
	Victor m_loader;
	Victor m_loaderb;
	Joystick m_stick;

private:

public:
	int loadport=1;
	int loadportb=2;
	int stickport=1;

	double loadspeed=0.5;
	int direction=0;
	Loader()
{
		m_loader(loadport),
		m_loaderb(loadportb),
		m_stick(stickport);

		//You can sort this out
}

	Loader_Out Run(Loader_In input)
	{
		Loader_Out output;
		if (m_stick.GetRawButton(2)==1){
			direction=1;
		}
		else if(m_stick.GetButton(2)==1){
			direction=-1;
		}

		m_loader.Set(loadspeed*direction);
		output.loaddirection=direction;
		//SmartDashboard::PutNumber("load direction",output.loaddirection);
		// Code here

		return output;
	}
};
