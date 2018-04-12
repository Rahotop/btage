#ifndef FIXEDSIZEDESCENTINC_HPP_INCLUDED
#define FIXEDSIZEDESCENTINC_HPP_INCLUDED

#include "../fixedsizedescent.hpp"

template<class Indiv>
class FixedSizeDescentInc : public FixedSizeDescent<Indiv>
{
	public:

	FixedSizeDescentInc(Function<Indiv>& fn, GeneratorRO<Indiv>& gen) : FixedSizeDescent<Indiv>(fn, gen)
	{

	}

	~FixedSizeDescentInc()
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
			//LocalSearch<Indiv>::m_fn.evaluate(sol);

			Descent<Indiv>::iterate({sol.getScore()});
			std::random_shuffle(near.begin(),near.end());

			for(unsigned int i(0); i < n; ++i)
			{
				Indiv tmp = Descent<Indiv>::m_gen.near(sol, near[i]);
				tmp.setScore(LocalSearch<Indiv>::m_fn.evaluateInc(tmp,near[i]));

				if(tmp.getScore() > sol.getScore())
				{
					goon = true;
					sol = tmp;
					LocalSearch<Indiv>::m_fn.makeCurrentPrev(near[i]);
					break;
				}
			}
		}

		return sol;
	}

	private:


};

#endif



