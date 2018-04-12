#ifndef NK_HPP_INCLUDED
#define NK_HPP_INCLUDED

#include <string>
#include <fstream>
#include <vector>
#include "../problem.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"
#include "../../../individual/indfunction/indfunction.hpp"
#include "../../../generator/generatorro/generatorroonemax/generatorroonemax.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescent.hpp"
#include "../../../localsearch/descent/fixedsizedescent/fixedsizedescentinc/fixedsizedescentinc.hpp"

class Nk : public Problem<VectorBool>, public Problem<IndFunction>
{
	public:

	Nk(const std::string& path);
	~Nk();

	float evaluate(VectorBool& s);
	float evaluate(IndFunction& s);

	unsigned int getN() const;

	private:

	unsigned int m_n;
	unsigned int m_k1;
	unsigned int m_2k1;

	float *m_mat;
	unsigned int *m_var;
};

#endif