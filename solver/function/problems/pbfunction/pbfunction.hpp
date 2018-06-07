#ifndef PBFUNCTION_HPP_INCLUDED
#define PBFUNCTION_HPP_INCLUDED

#include "../problem.hpp"
#include "../../functiontree/functiontree.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"
#include "../../../individual/indfunction/indfunction.hpp"
#include "../../../individual/indarray/indarray.hpp"
#include "../../../generator/generatorro/generatorroonemax/generatorroonemax.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescent.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescentinc/fixedsizedescentinc.hpp"

class PbFunction : public Problem<IndFunction>, public FunctionTree, public Problem<IndArray>
{
	public:

	PbFunction(unsigned int n, unsigned int depth1, unsigned int depth2);
	~PbFunction();

	float evaluate(IndFunction& s);
	float evaluate(VectorBool& s);
	float evaluate(IndArray& s);

	unsigned long long getnbeval() const;

	friend std::ostream& operator<<(std::ostream& o, const PbFunction& f);

	private:

	unsigned int m_n;
	unsigned long long m_nbeval;
};

#endif