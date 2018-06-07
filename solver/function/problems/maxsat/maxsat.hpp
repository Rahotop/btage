#ifndef MAXSAT_HPP_INCLUDED
#define MAXSAT_HPP_INCLUDED

#include <sstream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include "../problem.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"
#include "../../../individual/indfunction/indfunction.hpp"
#include "../../../individual/indarray/indarray.hpp"
#include "../../../generator/generatorro/generatorroonemax/generatorroonemax.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescent.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescentinc/fixedsizedescentinc.hpp"

class MaxSat : public Problem<VectorBool>, public Problem<IndFunction>, public Problem<IndArray>
{
	public:

	MaxSat(const std::string& path);
	~MaxSat();

	float evaluate(VectorBool& s);
	float evaluate(IndFunction& s);
	float evaluate(IndArray& s);

	unsigned long long getnbeval() const;

	unsigned int getN() const;
	void varLinks(std::ostream& o) const;

	private:

	unsigned int m_n;
	unsigned int m_nbclauses;
	std::vector<int> *m_clauses;
	unsigned long long m_nbeval;
};

#endif