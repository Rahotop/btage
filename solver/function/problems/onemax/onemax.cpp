#include "onemax.hpp"

OneMax::OneMax(unsigned int n) : m_n(n)
{

}

OneMax::~OneMax()
{

}

float OneMax::evaluate(VectorBool& s)
{
	float tmp = 0.;
	for(unsigned int i(0); i < s.size(); ++i)
		tmp += s[i];
	return tmp;
}

float OneMax::evaluate(IndFunction& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionTree fn = s.getFunction();
	FixedSizeDescent<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i(0); i < 10; ++i)
	{
		VectorBool v = d.solve(); 
		tmp += evaluate(v);
	}
	tmp /= 10.;
	return tmp;
}


