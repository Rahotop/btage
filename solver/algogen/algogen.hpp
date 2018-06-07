#ifndef ALGOGEN_HPP_INCLUDED
#define ALGOGEN_HPP_INCLUDED

#include <iostream>
#include <random>
#include <algorithm>
#include "../solver.hpp"
#include "../generator/generatoria/generatoria.hpp"
#include "../individual/indarray/indarray.hpp"

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
		std::vector<Indiv> newgen(m_nbInd);
		for(unsigned int i(0); i < m_nbInd; ++i)
		{
			pop.push_back(m_gen.generate());
			float tmp = Solver<Indiv>::m_fn.evaluate(pop.back());
			pop.back().setScore(tmp);
		}
		std::sort(pop.begin(),pop.end());

		Indiv best = pop.back();
		Solver<Indiv>::iterate({best.getScore(), (float)best.getFunction().size(), 0., 0., 0., 0., 0., 0.});


		unsigned int copy = m_nbInd-1;
		std::default_random_engine generator(rand());
		std::uniform_real_distribution<float> distribution(0.0,1.0);
		for(unsigned int it(0); it < m_itMax; ++it)
		{
			std::cout << it << "/" << m_itMax << "    \r" << std::flush;
			copy = m_nbInd-1;

			unsigned int avsize = 0;
			for(unsigned int i = 0; i < m_nbInd; ++i)
			{
				float tmp = distribution(generator);
				if(tmp < m_pcross)
				{
					m_gen.crossover(pop, newgen, i);
				}
				else if(tmp < m_pcopy)
				{
					newgen[i] = pop[copy--];
				}
				else if(tmp < m_pmut)
				{
					m_gen.mutation(pop, newgen, i);
				}
				else
				{
					newgen[i] = m_gen.generatenew();
				}

				newgen[i].setScore(Solver<Indiv>::m_fn.evaluate(newgen[i]));

				avsize += newgen[i].getFunction().size();
			}
			std::sort(newgen.begin(),newgen.end());

			pop.swap(newgen);
			Solver<Indiv>::iterate({pop.back().getScore(), (float)pop.back().getFunction().size(), (float)avsize/(float)m_nbInd,
									(float)pop.back().getFunction().countOP(2),
									(float)pop.back().getFunction().countOP(3),
									(float)pop.back().getFunction().countOP(4),
									(float)pop.back().getFunction().countOP(5),
									(float)pop.back().getFunction().countOP(6)});

			for(unsigned int i(0); i < m_nbInd; ++i)
			{
				Solver<Indiv>::iterate({(float)i,
										(float)pop[i].getFunction().countOP(2),
										(float)pop[i].getFunction().countOP(3),
										(float)pop[i].getFunction().countOP(4),
										(float)pop[i].getFunction().countOP(5),
										(float)pop[i].getFunction().countOP(6),
										(float)pop[i].getFunction().size(),
										pop[i].getScore()}, 1);
			}

			best.setScore(Solver<Indiv>::m_fn.evaluate(best));
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