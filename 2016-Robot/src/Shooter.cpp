#include "WPILib.h"
//created by Dexin Zhou
struct Shooter_In
{
};
struct Shooter_Out
{
bool shooting;
};
class Shooter
{
	Victor m_shoot;
	Joystick m_stick;
private:

public:
		int shootport=1;
		int stickport=1;
		int stickshootbutton=1;
		double shootamount=1;
		Shooter():
m_shoot(shootport),

m_stick(stickport)
{
		// Stuff
}

	Shooter_Out Run(Shooter_In input)
	{
		Shooter_Out output;
	if (m_stick.GetRawButton(stickshootbutton)==1){
		m_shoot.Set(shootamount);
		output.shooting=1;
		Wait(3);//    Wait seconds
		m_shoot.Set(0);
		output.shooting=0;
	}

		//SmartDashboard::PutNumber("shooting",output.shooting);
		// Code here

		return output;
	}
};




