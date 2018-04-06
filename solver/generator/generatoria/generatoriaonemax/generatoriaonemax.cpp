#include "generatoriaonemax.hpp"

GeneratorIAOneMax::GeneratorIAOneMax(unsigned int n) : GeneratorIA<VectorBool>(), m_n(n)
{

}

GeneratorIAOneMax::~GeneratorIAOneMax()
{

}

VectorBool GeneratorIAOneMax::generate() const
{
	VectorBool tmp(m_n);
	for(unsigned int i(0); i < m_n; ++i)
	{
		tmp[i] = rand()%2;
	}
	return tmp;
}

VectorBool GeneratorIAOneMax::crossover(const std::vector<VectorBool>& pop) const
{
	unsigned int s1 = rand()%pop.size();
	unsigned int s2 = rand()%pop.size();

	VectorBool tmp(m_n);
	for(unsigned int i(0); i < m_n; ++i)
	{
		tmp[i] = (rand()%2) ? pop[s1][i] : pop[s2][i];
	}
	return tmp;
}

VectorBool GeneratorIAOneMax::mutation(const std::vector<VectorBool>& pop) const
{
	unsigned int s = rand()%pop.size();

	VectorBool tmp(pop[s]);
	unsigned int mut = rand()%m_n;
	tmp[mut] = !tmp[mut];

	return tmp;
}





