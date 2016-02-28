#include "WPILib.h"
//created by Dexin Zhou
struct Lifter_In
{
	float leftTrigger, rightTrigger;
};
struct Lifter_Out
{
};
class Lifter
{
	Victor m_lifta;
	Victor m_liftb;
private:

public:
		int liftporta=5;
		int liftportb=6;

	Lifter():
m_lifta(liftporta),
m_liftb(liftportb)
		{
		// Stuff
}
	Lifter_Out Run(Lifter_In input)
	{
		Lifter_Out output;
		if(input.leftTrigger>0.2)
		{
			m_lifta.Set(0.25*input.leftTrigger);
			m_liftb.Set(-0.25*input.leftTrigger);
		}
		if(input.rightTrigger>0.2)
		{
			m_lifta.Set(-0.25*input.rightTrigger);
			m_liftb.Set(0.25*input.rightTrigger);
		}
		if(input.rightTrigger<0.2 && input.leftTrigger<0.2)
		{
			m_lifta.Set(0);
			m_liftb.Set(0);
		}
		// Code here

		return output;
	}
};
