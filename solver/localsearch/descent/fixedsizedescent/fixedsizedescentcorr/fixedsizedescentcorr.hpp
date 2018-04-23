#ifndef FIXEDSIZEDESCENTCORR_HPP_INCLUDED
#define FIXEDSIZEDESCENTCORR_HPP_INCLUDED

#include "../fixedsizedescent.hpp"

template<class Indiv>
class FixedSizeDescentCorr : public FixedSizeDescent<Indiv>
{
	public:

	FixedSizeDescentCorr(Function<Indiv>& fn, Function<Indiv>& x, Function<Indiv>& y, GeneratorRO<Indiv>& gen) : FixedSizeDescent<Indiv>(fn, gen), m_x(x), m_y(y)
	{

	}

	~FixedSizeDescentCorr()
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

			Descent<Indiv>::iterate({m_x.evaluate(sol),m_y.evaluate(sol)});
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

	Function<Indiv>& m_x;
	Function<Indiv>& m_y;
};

#endif