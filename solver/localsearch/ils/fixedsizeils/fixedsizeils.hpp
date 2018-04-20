#ifndef FIXEDSIZEILS_HPP_INCLUDED
#define FIXEDSIZEILS_HPP_INCLUDED

#include "../ils.hpp"

template<class Indiv>
class FixedSizeILS : public ILS<Indiv>
{
	public:

	FixedSizeILS(Function<Indiv>& fn, GeneratorRO<Indiv>& gen, unsigned int itmax, unsigned int steps) : ILS<Indiv>(fn, gen, itmax, steps)
	{

	}

	~FixedSizeILS()
	{

	}

	Indiv solve()
	{
		Indiv sol = ILS<Indiv>::m_gen.generate();
		sol.setScore(ILS<Indiv>::m_fn.evaluate(sol));

		unsigned int n = ILS<Indiv>::m_gen.nearSize(sol);
		std::vector<unsigned int> near;
		for(unsigned int i(0); i < n; ++i)
			near.push_back(i);

		Indiv best = sol;

		for(unsigned int it(0); it < ILS<Indiv>::m_itmax; ++it)
		{
			bool goon = true;

			std::cout << it << "/" << ILS<Indiv>::m_itmax << "      \r" << std::flush;

			while(goon)
			{
				goon = false;

				ILS<Indiv>::iterate({sol.getScore()});

				std::random_shuffle(near.begin(),near.end());

				for(unsigned int i(0); i < n; ++i)
				{
					Indiv tmp = ILS<Indiv>::m_gen.near(sol, near[i]);
					tmp.setScore(ILS<Indiv>::m_fn.evaluate(tmp));

					if(tmp.getScore() > sol.getScore())
					{
						goon = true;
						sol = tmp;
						break;
					}
				}
				if(sol.getScore() > best.getScore())
					best = sol;
			}

			for(unsigned int i(0); i < ILS<Indiv>::m_steps; ++i)
			{
				ILS<Indiv>::iterate({sol.getScore()});
				sol = ILS<Indiv>::m_gen.near(sol, rand()%n);

				if(sol.getScore() > best.getScore())
					best = sol;
			}
		}

		return best;
	}

	private:


};

#endif