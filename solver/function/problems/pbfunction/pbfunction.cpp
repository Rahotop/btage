#include "pbfunction.hpp"

PbFunction::PbFunction(unsigned int n, unsigned int depth1, unsigned int depth2) : Problem<IndFunction>(), FunctionTree({depth1,1,depth2},{6,2,5},n), m_n(n)
{

}

PbFunction::~PbFunction()
{

}

float PbFunction::evaluate(IndFunction& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionTree fn(s.getFunction());
	FixedSizeDescentInc<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i(0); i < 10; ++i)
	{
		VectorBool v = d.solve(); 
		tmp += FunctionTree::evaluate(v);
	}
	tmp /= 10.;
	return tmp;
}

float PbFunction::evaluate(IndArray& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionArray fn(s.getFunction());
	FixedSizeDescentInc<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i(0); i < 10; ++i)
	{
		VectorBool v = d.solve(); 
		tmp += FunctionTree::evaluate(v);
	}
	tmp /= 10.;
	return tmp;
}

float PbFunction::evaluate(VectorBool& s)
{
	return FunctionTree::evaluate(s);
}


std::ostream& operator<<(std::ostream& o, const PbFunction& f)
{
	f.show(o);
	return o;
}





