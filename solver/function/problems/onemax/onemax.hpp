#ifndef ONEMAX_HPP_INCLUDED
#define ONEMAX_HPP_INCLUDED

#include "../problem.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"
#include "../../../individual/indfunction/indfunction.hpp"
#include "../../../generator/generatorro/generatorroonemax/generatorroonemax.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescent.hpp"

class OneMax : public Problem<VectorBool>, public Problem<IndFunction>
{
	public:

	OneMax(unsigned int n);
	~OneMax();

	float evaluate(VectorBool& s);
	float evaluate(IndFunction& s);

	private:

	unsigned int m_n;
};

#endif