#ifndef FIXEDSIZEDESCENT_HPP_INCLUDED
#define FIXEDSIZEDESCENT_HPP_INCLUDED

#include "../descent.hpp"

template<class Indiv>
class FixedSizeDescent : public Descent<Indiv>
{
	public:

	FixedSizeDescent(Function<Indiv>& fn, GeneratorRO<Indiv>& gen) : Descent<Indiv>(fn, gen)
	{

	}

	~FixedSizeDescent()
	{

	}

	virtual Indiv solve()
	{
		Indiv sol = Descent<Indiv>::m_gen.generate();
		sol.setScore(LocalSearch<Indiv>::m_fn.evaluate(sol));

		unsigned int n = Descent<Indiv>::m_gen.nearSize(sol);
		std::vector<unsigned int> near;
		for(unsigned int i(0); i < n; ++i)
			near.push_back(i);

		bool goon = true;

		while(goon)
		{
			goon = false;

			Descent<Indiv>::iterate({sol.getScore()});
			std::random_shuffle(near.begin(),near.end());

			for(unsigned int i(0); i < n; ++i)
			{
				Indiv tmp = Descent<Indiv>::m_gen.near(sol, near[i]);
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