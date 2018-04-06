#include "vectorboolcomplete.hpp"

VectorBoolComplete::VectorBoolComplete(Function<VectorBool>& fn, unsigned int n) : Solver<VectorBool>(fn), m_n(n)
{
	
}

VectorBoolComplete::~VectorBoolComplete()
{
	
}

VectorBool VectorBoolComplete::solve()
{
	VectorBool s(m_n);
	for(unsigned int i(0); i < m_n; ++i)
	{
		s[i] = false;
	}
	s.setScore(Solver<VectorBool>::m_fn.evaluate(s));
	VectorBool best = s;

	bool goon = true;

	while(goon)
	{
		for(unsigned int i(0); i < m_n; ++i)
		{
			if(!s[i])
			{
				s[i] = true;
				break;
			}
			else
			{
				s[i] = false;
			}
		}
		s.setScore(Solver<VectorBool>::m_fn.evaluate(s));

		if(best < s)
		{
			best = s;
		}

		for(unsigned int i(0); i < m_n; ++i)
		{
			if(!s[i])
			{
				break;
			}
			else if(i == m_n-1 && s[i])
			{
				goon = false;
			}
		}
	}

	Solver<VectorBool>::iterate({best.getScore()});

	return best;
}


