#include "WPILib.h"
//created by Dexin Zhou
struct Lifter_In
{
	//float zMovement;

};
struct Lifter_Out
{
double liftamount;
};
class Lifter
{
	Victor m_lift;
	Victor m_liftb;
	Joystick m_stick;
private:

public:
		int liftport=1;
		int liftportb=2;
		int stickport=1;

	Lifter():
m_lift(liftport),
m_liftb(liftportb),

m_stick(stickport)
{
		// Stuff
}
	Lifter_Out Run(Lifter_In input)
	{
		Lifter_Out output;
		output.liftamount=m_stick.GetY();

		m_lift.Set(output.liftamount);
		m_liftb.Set(output.liftamount);
		//SmartDashboard::PutNumber("lift",output.liftamount);
		// Code here

		return output;
	}
};
