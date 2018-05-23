#include "generatoriaindarray.hpp"

GeneratorIAIndArray::GeneratorIAIndArray(unsigned int maxWidth, unsigned int maxDepth, unsigned int initWidth, unsigned int initDepth, unsigned int termset) :
	m_maxWidth(maxWidth),
	m_maxDepth(maxDepth),
	m_initWidth(initWidth),
	m_initDepth(initDepth),
	m_n(termset)
{

}

GeneratorIAIndArray::~GeneratorIAIndArray()
{

}

IndArray GeneratorIAIndArray::generate() const
{
	return IndArray(m_maxWidth,m_maxDepth, m_initWidth, m_initDepth, m_n, {1, 7, 9, 11, 13});
}

void GeneratorIAIndArray::crossover(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const
{
	unsigned int p1 = rand()%pop.size();
	unsigned int p2 = rand()%pop.size();

	FunctionArray f(m_maxWidth,m_maxDepth,0,m_initDepth,m_n,{1,7,9,11,13});

	for(unsigned int i(0); i < pop[p1].getFunction().width() && i < pop[p2].getFunction().width(); ++i)
	{
		if(rand()%2)
			f.add(pop[p1], i);
		else
			f.add(pop[p2], i);
	}
	for(unsigned int i(0); i < pop[p1].getFunction().width(); ++i)
	{
		f.add(pop[p1], i);
	}
	newgen[index] = IndArray(f);
	newgen[index].simplification();
}

void GeneratorIAIndArray::mutation(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const
{
	newgen[index] = pop[rand()%pop.size()];
	if(!(rand()%4))
	{
		newgen[index].addRandom();
		//newgen[index].simplification();
	}
	else if(newgen[index].width())
	{
		if(!(rand()%3))
		{
			newgen[index].reconstruct(rand()%newgen[index].width());
			//newgen[index].simplification();
		}
		else if(!(rand()%2))
		{
			newgen[index].erase(rand()%newgen[index].width());
		}
		else
		{
			newgen[index].mutate(rand()%newgen[index].width());
			//newgen[index].simplification();
		}
	}
	else
	{
		newgen[index].addRandom();
	}
}




















