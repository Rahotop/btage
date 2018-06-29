#ifndef ONEMAX_HPP_INCLUDED
#define ONEMAX_HPP_INCLUDED

#include "../problem.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"
#include "../../../individual/indfunction/indfunction.hpp"
#include "../../../individual/indarray/indarray.hpp"
#include "../../../generator/generatorro/generatorroonemax/generatorroonemax.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescent.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescentinc/fixedsizedescentinc.hpp"

class OneMax : public Problem<VectorBool>, public Problem<IndFunction>, public Problem<IndArray>
{
	public:

	OneMax(unsigned int n);
	~OneMax();

	float evaluate(VectorBool& s);
	float evaluate(IndFunction& s);
	float evaluate(IndArray& s);

	unsigned long long getnbeval() const;
	void varLinks(std::ostream& o) const;

	private:

	unsigned int m_n;
	unsigned long long m_nbeval;
};

#endif