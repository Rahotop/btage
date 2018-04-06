#ifndef ALGOGEN_HPP_INCLUDED
#define ALGOGEN_HPP_INCLUDED

#include <iostream>
#include <random>
#include <algorithm>
#include "../solver.hpp"
#include "../generator/generatoria/generatoria.hpp"

template<class Indiv>
class Algogen : public Solver<Indiv>
{
	public:

	Algogen(Function<Indiv>& fn, GeneratorIA<Indiv>& gen, unsigned int nbInd, unsigned int itMax, float pcross, float pcopy, float pmut, float paa) :
		Solver<Indiv>(fn), m_gen(gen), m_nbInd(nbInd), m_itMax(itMax), m_pcross(pcross), m_pcopy(pcross+pcopy), m_pmut(pcross+pcopy+pmut), m_paa(pcross+pcopy+pmut+paa)
	{

	}

	~Algogen()
	{

	}

	Indiv solve()
	{
		std::vector<Indiv> pop;
		for(unsigned int i(0); i < m_nbInd; ++i)
		{
			pop.push_back(m_gen.generate());
			float tmp = Solver<Indiv>::m_fn.evaluate(pop.back());
			pop.back().setScore(tmp);
		}
		std::sort(pop.begin(),pop.end());

		Indiv best = pop.back();
		Solver<Indiv>::iterate({best.getScore()});


		unsigned int copy = m_nbInd-1;
		std::default_random_engine generator(rand());
		std::uniform_real_distribution<float> distribution(0.0,1.0);
		for(unsigned int it(0); it < m_itMax; ++it)
		{
			std::cout << it << "/" << m_itMax << "    \r" << std::flush;
			copy = m_nbInd-1;
			std::vector<Indiv> newgen;

			for(unsigned int i(0); i < m_nbInd; ++i)
			{
				float tmp = distribution(generator);
				if(tmp < m_pcross)
				{
					newgen.push_back(m_gen.crossover(pop));
				}
				else if(tmp < m_pcopy)
				{
					newgen.push_back(pop[copy]);
					--copy;
				}
				else if(tmp < m_pmut)
				{
					newgen.push_back(m_gen.mutation(pop));
				}
				else
				{
					newgen.push_back(m_gen.generate());
				}
			}
			#pragma omp parallel for
			for(unsigned int i = 0; i < m_nbInd; ++i)
			{
				newgen[i].setScore(Solver<Indiv>::m_fn.evaluate(newgen[i]));
			}
			std::sort(newgen.begin(),newgen.end());

			pop.swap(newgen);
			Solver<Indiv>::iterate({pop.back().getScore()});
			if(best < pop.back())
				best = pop.back();
		}

		return best;
	}

	private:

	GeneratorIA<Indiv>& m_gen;
	unsigned int m_nbInd;
	unsigned int m_itMax;
	float m_pcross;
	float m_pcopy;
	float m_pmut;
	float m_paa;//architecture altering
};

#endif