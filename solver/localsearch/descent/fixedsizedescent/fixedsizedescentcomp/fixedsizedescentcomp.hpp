#ifndef FIXEDSIZEDESCENTCOMP_HPP_INCLUDED
#define FIXEDSIZEDESCENTCOMP_HPP_INCLUDED

#include "../fixedsizedescent.hpp"

template<class Indiv>
class FixedSizeDescentComp : public FixedSizeDescent<Indiv>
{
	public:

	FixedSizeDescentComp(Function<Indiv>& fn, Function<Indiv>& comp, GeneratorRO<Indiv>& gen) : FixedSizeDescent<Indiv>(fn, gen), m_comp(comp)
	{

	}

	~FixedSizeDescentComp()
	{

	}

	virtual Indiv solve()
	{
		Indiv sol = FixedSizeDescent<Indiv>::m_gen.generate();
		sol.setScore(LocalSearch<Indiv>::m_fn.evaluate(sol));

		unsigned int n = FixedSizeDescent<Indiv>::m_gen.nearSize(sol);
		std::vector<unsigned int> near;
		for(unsigned int i(0); i < n; ++i)
			near.push_back(i);

		bool goon = true;

		while(goon)
		{
			goon = false;

			FixedSizeDescent<Indiv>::iterate({m_comp.evaluate(sol)});
			std::random_shuffle(near.begin(),near.end());

			for(unsigned int i(0); i < n; ++i)
			{
				Indiv tmp = FixedSizeDescent<Indiv>::m_gen.near(sol, near[i]);
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

	Function<Indiv>& m_comp;
};

#endif