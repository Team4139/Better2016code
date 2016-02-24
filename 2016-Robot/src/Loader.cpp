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
	Victor m_loaderc;
	Joystick m_stick;

private:

public:
	uint32_t loadport=0;
	uint32_t loadportb=7;
	uint32_t loadportc=9;
	uint32_t stickport=5;
	int stickloadinbutton=1;
	int stickloadoutbutton=2;
	double loadspeed=0.1;
	Loader()
{
		m_loader(0),
		m_loaderb(7),
		m_loaderc(9),
		m_stick(5);
		m_loader.SetInverted(1);//moter are reverted
		m_loaderc.SetInverted(1);
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
		m_loaderb.Set(loadspeed*output.loaddirection);
		m_loaderc.Set(loadspeed*output.loaddirection);

		//SmartDashboard::PutNumber("load direction",output.loaddirection);
		// Code here

		return output;
	}
};
