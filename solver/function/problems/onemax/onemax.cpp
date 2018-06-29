#include "onemax.hpp"

OneMax::OneMax(unsigned int n) : m_n(n), m_nbeval(0)
{

}

OneMax::~OneMax()
{

}

float OneMax::evaluate(VectorBool& s)
{
	++m_nbeval;
	float tmp = 0.;
	for(unsigned int i(0); i < s.size(); ++i)
		tmp += s[i];
	return tmp;
}

float OneMax::evaluate(IndFunction& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionTree fn = s.getFunction();
	FixedSizeDescentInc<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i(0); i < 10; ++i)
	{
		VectorBool v = d.solve(); 
		tmp += evaluate(v);
	}
	tmp /= 10.;
	return tmp;
}

float OneMax::evaluate(IndArray& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionArray fn = s.getFunction();
	FixedSizeDescentInc<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i(0); i < 10; ++i)
	{
		VectorBool v = d.solve(); 
		tmp += evaluate(v);
	}
	tmp /= 10.;
	return tmp;
}

unsigned long long OneMax::getnbeval() const
{
	return m_nbeval;
}

void OneMax::varLinks(std::ostream& o) const
{
	o << "";
}

