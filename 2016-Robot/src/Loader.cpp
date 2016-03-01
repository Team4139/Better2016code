#include "WPILib.h"
//created by Dexin Zhou
struct Loader_In
{
bool buttonA, buttonB, buttonX;
};
struct Loader_Out
{
};
class Loader
{
	Talon* m_loadera;
	Talon* m_loaderb;
	Talon* m_loaderc;

private:

public:
	double loadspeed=0.4;
	Loader()
{
		m_loadera(0),
		m_loaderb(7),
		m_loaderc(9),
		m_loadera->SetInverted(1);//moter are reverted
		m_loaderc->SetInverted(1);
		//You can sort this out
}

	Loader_Out Run(Loader_In input)
	{
		Loader_Out output;
		int feeddirection;
		if(input.buttonA==1){
			feeddirection=1;
		}
		else if(input.buttonB==1){
			feeddirection=-1;
		}
		else if(input.buttonX==1){
			feeddirection=0;
		}
		m_loadera->Set(loadspeed*feeddirection);
		m_loaderb->Set(loadspeed*feeddirection);
		m_loaderc->Set(loadspeed*feeddirection);
		Wait(0.05);
		// Code here

		return output;
	}
};
