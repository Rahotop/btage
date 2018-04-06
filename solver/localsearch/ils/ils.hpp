#ifndef ILS_HPP_INCLUDED
#define ILS_HPP_INCLUDED

#include "../localsearch.hpp"

template<class Indiv>
class ILS : public LocalSearch<Indiv>
{
	public:

	ILS(Function<Indiv>& fn, GeneratorRO<Indiv>& gen, unsigned int itmax, unsigned int steps) : LocalSearch<Indiv>(fn, gen), m_itmax(itmax), m_steps(steps)
	{

	}

	~ILS()
	{

	}

	Indiv solve()
	{
		Indiv sol = LocalSearch<Indiv>::m_gen.generate();
		sol.setScore(LocalSearch<Indiv>::m_fn.evaluate(sol));

		Indiv best = sol;

		for(unsigned int it(0); it < m_itmax; ++it)
		{
			bool goon = true;

			while(goon)
			{
				goon = false;

				LocalSearch<Indiv>::iterate({sol.getScore()});

				unsigned int n = LocalSearch<Indiv>::m_gen.nearSize(sol);
				std::vector<unsigned int> near;
				for(unsigned int i(0); i < n; ++i)
					near.push_back(i);
				std::random_shuffle(near.begin(),near.end());

				for(unsigned int i(0); i < n; ++i)
				{
					Indiv tmp = LocalSearch<Indiv>::m_gen.near(sol, near[i]);
					tmp.setScore(LocalSearch<Indiv>::m_fn.evaluate(tmp));

					if(tmp.getScore() > sol.getScore())
					{
						goon = true;
						sol = tmp;
						break;
					}
				}
			}

			if(sol.getScore() > best.getScore())
				best = sol;

			for(unsigned int i(0); i < m_steps; ++i)
			{
				LocalSearch<Indiv>::iterate({sol.getScore()});
				sol = LocalSearch<Indiv>::m_gen.near(sol, rand()%LocalSearch<Indiv>::m_gen.nearSize(sol));
			}
		}

		return best;
	}

	protected:

	unsigned int m_itmax;
	unsigned int m_steps;
};

#endif