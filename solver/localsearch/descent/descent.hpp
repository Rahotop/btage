#ifndef DESCENT_HPP_INCLUDED
#define DESCENT_HPP_INCLUDED

#include "../localsearch.hpp"

template<class Indiv>
class Descent : public LocalSearch<Indiv>
{
	public:

	Descent(Function<Indiv>& fn, GeneratorRO<Indiv>& gen) : LocalSearch<Indiv>(fn, gen)
	{

	}

	~Descent()
	{

	}

	virtual Indiv solve()
	{
		Indiv sol = LocalSearch<Indiv>::m_gen.generate();
		sol.setScore(LocalSearch<Indiv>::m_fn.evaluate(sol));

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

		return sol;
	}

	private:


};

#endif