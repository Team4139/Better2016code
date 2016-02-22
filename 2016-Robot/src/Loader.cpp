#include "WPILib.h"
//created by Dexin Zhou
struct Loader_In
{
};
struct Loader_Out
{
int loaddirection;
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
	int stickloadinbutton=1;
	int stickloadoutbutton=2;
	double loadspeed=0.5;
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
		if (m_stick.GetRawButton(stickloadinbutton)==1){
			output.loaddirection=1;
		}
		else if(m_stick.GetRawButton(stickloadoutbutton)==1){
			output.loaddirection=-1;
		}
		else{
			output.loaddirection=0;
		}

		m_loader.Set(loadspeed*output.loaddirection);
		//SmartDashboard::PutNumber("load direction",output.loaddirection);
		// Code here

		return output;
	}
};
