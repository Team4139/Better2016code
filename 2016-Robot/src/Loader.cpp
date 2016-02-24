#include "WPILib.h"
//created by Dexin Zhou
struct Loader_In
{
	float feederMovement;
};
struct Loader_Out
{
};
class Loader
{
	Talon m_loader;
	Talon m_loaderb;
	Talon m_loaderc;

private:

public:
	uint32_t loadport=0;
	uint32_t loadportb=7;
	uint32_t loadportc=9;
	double loadspeed=0.1;
/*	Loader()
{
		m_loader(0),
		m_loaderb(7),
		m_loaderc(9),
		m_loader.SetInverted(1);//moter are reverted
		m_loaderc.SetInverted(1);
		//You can sort this out
}
*/

	Loader_Out Run(Loader_In input)
	{
		Loader_Out output;
		m_loader.Set(loadspeed*input.feederMovement);
		m_loaderb.Set(-loadspeed*input.feederMovement);
		m_loaderc.Set(loadspeed*input.feederMovement);

		//SmartDashboard::PutNumber("load direction",output.loaddirection);
		// Code here

		return output;
	}
};
