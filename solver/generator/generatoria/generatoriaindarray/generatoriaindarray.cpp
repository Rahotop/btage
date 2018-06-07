#include "generatoriaindarray.hpp"

GeneratorIAIndArray::GeneratorIAIndArray(unsigned int maxWidth, unsigned int maxDepth, unsigned int initWidth, unsigned int initDepth, unsigned int termset, Function<VectorBool>& pb) :
	m_maxWidth(maxWidth),
	m_maxDepth(maxDepth),
	m_initWidth(initWidth),
	m_initDepth(initDepth),
	m_n(termset),
	m_pb(pb)
{

}

GeneratorIAIndArray::~GeneratorIAIndArray()
{

}

IndArray GeneratorIAIndArray::generate() const
{/*
	if(rand()%2)
	{
		FunctionArray f(m_maxWidth,m_maxDepth,0,m_initDepth,m_n,{1,7,9,11,13});

		GeneratorROOneMax genom(m_n);
		FixedSizeDescent<VectorBool> ls(m_pb,genom);

		VectorBool s1 = ls.solve();
		VectorBool s2 = ls.solve();

		for(unsigned int i(0); i < m_n; ++i)
		{
			//if(s1[i] == s2[i])
			if(rand()%4)
			{
				f.add(i, !s1[i], (float)((rand()%10)+1)/10.);
			}
			else
			{
				for(unsigned int j(i+1); j < m_n; ++j)
				{
					if(s1[j] != s2[j])
					{
						f.add((float)((rand()%10)+1)/10., 9, s1[i] != s1[j], i, 0, j, 0);
						break;
					}
				}
			}
		}
		return IndArray(f);
	}*/
	return generatenew();
}

IndArray GeneratorIAIndArray::generatenew() const
{
	return IndArray(m_maxWidth,m_maxDepth, m_initWidth, m_initDepth, m_n, {1, 7, 9, 11, 13});
}

void GeneratorIAIndArray::crossover(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const
{/*
	if(rand()%2)
	{*/
		crossuni(pop,newgen,index);/*
	}
	else
	{
		crossrand(pop,newgen,index);
	}*/
}

void GeneratorIAIndArray::crossuni(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const
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

void GeneratorIAIndArray::crossrand(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const
{
	unsigned int p1 = rand()%pop.size();
	unsigned int p2 = rand()%pop.size();

	FunctionArray f(m_maxWidth,m_maxDepth,0,m_initDepth,m_n,{1,7,9,11,13});

	for(unsigned int i(0); i < pop[p1].getFunction().width() && i < pop[p2].getFunction().width(); ++i)
	{
		if(rand()%2)
			f.add(pop[p1], rand()%pop[p1].width());
		else
			f.add(pop[p2], rand()%pop[p2].width());
	}
	for(unsigned int i(0); i < pop[p1].getFunction().width(); ++i)
	{
		f.add(pop[p1], rand()%pop[p1].width());
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




















