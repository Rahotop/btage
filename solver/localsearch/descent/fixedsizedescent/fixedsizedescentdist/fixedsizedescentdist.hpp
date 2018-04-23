#ifndef FIXEDSIZEDESCENTDIST_HPP_INCLUDED
#define FIXEDSIZEDESCENTDIST_HPP_INCLUDED

#include <vector>
#include "../fixedsizedescent.hpp"

template<class Indiv>
class FixedSizeDescentDist : public FixedSizeDescent<Indiv>
{
	public:

	FixedSizeDescentDist(Function<Indiv>& fn, GeneratorRO<Indiv>& gen) : FixedSizeDescent<Indiv>(fn, gen)
	{

	}

	~FixedSizeDescentDist()
	{

	}

	virtual Indiv solve()
	{
		std::vector<Indiv> prec;

		Indiv sol = Descent<Indiv>::m_gen.generate();
		sol.setScore(LocalSearch<Indiv>::m_fn.evaluate(sol));
		prec.push_back(sol);

		unsigned int n = Descent<Indiv>::m_gen.nearSize(sol);
		std::vector<unsigned int> near;
		for(unsigned int i(0); i < n; ++i)
			near.push_back(i);

		bool goon = true;

		while(goon)
		{
			goon = false;

			std::random_shuffle(near.begin(),near.end());

			for(unsigned int i(0); i < n; ++i)
			{
				Indiv tmp = Descent<Indiv>::m_gen.near(sol, near[i]);
				tmp.setScore(LocalSearch<Indiv>::m_fn.evaluate(tmp));

				if(tmp.getScore() > sol.getScore())
				{
					goon = true;
					sol = tmp;
					prec.push_back(sol);
					break;
				}
			}
		}

		for(unsigned int i(0); i < prec.size(); ++i)
		{
			Descent<Indiv>::iterate({-(float)prec[i].dist(prec.back())});
		}

		return sol;
	}

	private:

};

#endif