#include "algogen.hpp"

template<>
IndArray Algogen<IndArray>::solve()
{
	std::vector<IndArray> pop;
	std::vector<IndArray> newgen(m_nbInd);
	for(unsigned int i(0); i < m_nbInd; ++i)
	{
		pop.push_back(m_gen.generate());
		float tmp = Solver<IndArray>::m_fn.evaluate(pop.back());
		pop.back().setScore(tmp);
	}
	std::sort(pop.begin(),pop.end());

	IndArray best = pop.back();
	Solver<IndArray>::iterate({best.getScore(), (float)best.getFunction().size(), 0., 0., 0., 0., 0., 0.});


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

			newgen[i].setScore(Solver<IndArray>::m_fn.evaluate(newgen[i]));

			avsize += newgen[i].getFunction().size();
		}
		std::sort(newgen.begin(),newgen.end());

		pop.swap(newgen);
		Solver<IndArray>::iterate({pop.back().getScore(), (float)pop.back().getFunction().size(), (float)avsize/(float)m_nbInd,
								(float)pop.back().getFunction().countOP(1),
								(float)pop.back().getFunction().countOP(7),
								(float)pop.back().getFunction().countOP(9),
								(float)pop.back().getFunction().countOP(11),
								(float)pop.back().getFunction().countOP(13)});

		for(unsigned int i(0); i < m_nbInd; ++i)
		{
			Solver<IndArray>::iterate({(float)i,
									(float)pop[i].getFunction().countOP(1),
									(float)pop[i].getFunction().countOP(7),
									(float)pop[i].getFunction().countOP(9),
									(float)pop[i].getFunction().countOP(11),
									(float)pop[i].getFunction().countOP(13),
									(float)pop[i].getFunction().size(),
									pop[i].getScore()}, 1);
		}

		//std::vector<float> diversity;
		float max = 0.;
		float min = 1.;
		float ave = 0.;
		unsigned int count = 0;
		for(unsigned int i(0); i < m_nbInd; ++i)
		{
			for(unsigned int j(i+1); j < m_nbInd; ++j)
			{
				//diversity.push_back(std::min(pop[i] == pop[j], pop[j] == pop[i]));
				float tmp = std::min(pop[i] == pop[j], pop[j] == pop[i]);
				max = std::max(max,tmp);
				min = std::min(min,tmp);
				ave += tmp;
				++count;
			}
		}
		Solver<IndArray>::iterate({max,min,ave/(float)count}, 2);

		best.setScore(Solver<IndArray>::m_fn.evaluate(best));
		if(best < pop.back())
			best = pop.back();
	}

	return best;
}