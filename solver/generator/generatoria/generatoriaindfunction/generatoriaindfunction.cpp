#include "generatoriaindfunction.hpp"

GeneratorIAIndFunction::GeneratorIAIndFunction(unsigned int n, unsigned int maxsize, unsigned int depthinit) : m_n(n), m_maxsize(maxsize), m_depthinit(depthinit)
{

}

GeneratorIAIndFunction::~GeneratorIAIndFunction()
{

}

IndFunction GeneratorIAIndFunction::generate() const
{
	IndFunction tmp(m_maxsize,m_depthinit,m_n,rand()%2);
	return tmp;
}

IndFunction GeneratorIAIndFunction::generatenew() const
{
	IndFunction tmp(m_maxsize,m_depthinit,m_n,rand()%2);
	return tmp;
}

void GeneratorIAIndFunction::crossover(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const
{
	IndFunction tmp1(pop[rand()%pop.size()]);
	IndFunction tmp2(pop[rand()%pop.size()]);


	unsigned int choice = rand()% 10;
	unsigned int node = 0;
	if(tmp2.getFunction().size() != 1)
	{
		do{
			node = tmp2.getFunction().getNode(rand()%tmp2.getFunction().size());
		}while(tmp2.getFunction().isLeaf(node) != !choice);
	}

	FunctionTree toadd(tmp2.getFunction(), node);


	while(tmp1.getFunction().size() + toadd.size() > m_maxsize)
	{
		tmp1.getFunction().cut(tmp1.getFunction().getNode(rand()%tmp1.getFunction().size()));
	}


	tmp1.getFunction().replace(tmp1.getFunction().getNode(rand()%tmp1.getFunction().size()), toadd);
	tmp1.resetScore();

	newgen[index].swap(tmp1);
}

void GeneratorIAIndFunction::mutation(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const
{
	if(rand()%1)
		return subtreeMutation(pop, newgen, index);
	else
		return pointMutation(pop, newgen, index);
}

void GeneratorIAIndFunction::subtreeMutation(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const
{
	IndFunction tmp(pop[rand()%pop.size()]);

	IndFunction toadd = generate();

	while(tmp.getFunction().size() + toadd.getFunction().size() > m_maxsize)
	{
		tmp.getFunction().cut(tmp.getFunction().getNode(rand()%tmp.getFunction().size()));
	}

	tmp.getFunction().replace(tmp.getFunction().getNode(rand()%tmp.getFunction().size()), toadd.getFunction());
	tmp.resetScore();

	newgen[index].swap(tmp);
}

void GeneratorIAIndFunction::pointMutation(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const
{
	IndFunction tmp(pop[rand()%pop.size()]);

	tmp.getFunction().mutate(tmp.getFunction().getNode(rand()%tmp.getFunction().size()));
	tmp.resetScore();

	newgen[index].swap(tmp);
}





