#ifndef PBFUNCTION_HPP_INCLUDED
#define PBFUNCTION_HPP_INCLUDED

#include "../problem.hpp"
#include "../../functiontree/functiontree.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"
#include "../../../individual/indfunction/indfunction.hpp"
#include "../../../generator/generatorro/generatorroonemax/generatorroonemax.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescent.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescentinc/fixedsizedescentinc.hpp"

class PbFunction : public Problem<IndFunction>, public FunctionTree
{
	public:

	PbFunction(unsigned int n, unsigned int depth1, unsigned int depth2);
	~PbFunction();

	float evaluate(IndFunction& s);
	float evaluate(VectorBool& s);

	friend std::ostream& operator<<(std::ostream& o, const PbFunction& f);

	private:

	unsigned int m_n;
};

#endif