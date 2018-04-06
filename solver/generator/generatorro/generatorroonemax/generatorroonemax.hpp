#ifndef GENERATORROONEMAX_HPP_INCLUDED
#define GENERATORROONEMAX_HPP_INCLUDED

#include <cstdlib>
#include "../generatorro.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"

class GeneratorROOneMax : public GeneratorRO<VectorBool>
{
	public:

	GeneratorROOneMax(unsigned int n) : GeneratorRO<VectorBool>(), m_n(n)
	{

	}

	~GeneratorROOneMax()
	{

	}

	VectorBool generate() const
	{
		VectorBool tmp(m_n);
		for(unsigned int i(0); i < m_n; ++i)
		{
			tmp[i] = rand()%2;
		}
		return tmp;
	}

	VectorBool near(const VectorBool& ind, unsigned int i) const
	{
		VectorBool tmp(ind);
		tmp[i] = !tmp[i];
		return tmp;
	}

	unsigned int nearSize(const VectorBool& ind) const
	{
		ind.getScore();
		return m_n;
	}

	private:

	unsigned int m_n;
};

#endif