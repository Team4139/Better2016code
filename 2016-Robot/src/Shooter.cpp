#include "WPILib.h"
//created by Dexin Zhou
struct Shooter_In
{
	bool buttonA;
};
struct Shooter_Out
{
};
class Shooter
{
	Victor m_shoot;
private:

public:
		int shootport=8;
		double shootspeed=1;
		Shooter():
m_shoot(shootport)

{
		// Stuff
}

	Shooter_Out Run(Shooter_In input)
	{
		Shooter_Out output;
if(input.buttonA==1){
		m_shoot.Set(shootspeed);
		Wait(3);//    Wait seconds
		if(input.buttonA==1){
		m_shoot.Set(0);
		}
	}

		// Code here

		return output;
	}
};




