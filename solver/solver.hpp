#ifndef SOLVER_HPP_INCLUDED
#define SOLVER_HPP_INCLUDED

#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include "function/function.hpp"

template<class Indiv>
class Solver
{
	public:

	Solver(Function<Indiv>& fn) : m_save(), m_it(0), m_fn(fn)
	{

	}

	virtual ~Solver()
	{

	}

	virtual Indiv solve() = 0;

	void iterate(const std::vector<float>& v)
	{
		for(unsigned int i(0); i < v.size(); ++i)
		{
			if(i == m_save.size())
			{
				m_save.push_back(std::list<float>());
				for(unsigned int j(0); j < m_it; ++i)
					m_save[i].push_back(0.);
			}
			m_save[i].push_back(v[i]);
		}
		++m_it;
	}

	friend std::ostream& operator<<(std::ostream& o, Solver<Indiv>& s)
	{
		for(unsigned int i(0); i < s.m_it; ++i)
		{
			o << i << " ";
			for(unsigned int j(0); j < s.m_save.size(); ++j)
			{
				o << s.m_save[j].front() << " ";
				s.m_save[j].pop_front();
			}
			o << std::endl;
		}

		return o;
	}

	template<class Indiv2>
	void addLine(const Solver<Indiv2>& s)
	{
		if(m_it < s.getIt())
		{
			m_it = s.getIt();
			for(unsigned int i(0); i < m_save.size(); ++i)
			{
				while(m_save[i].size() < m_it)
				{
					m_save[i].push_back(m_save[i].back());
				}
			}
		}

		for(unsigned int i(0); i < s.getSave().size(); ++i)
		{
			m_save.push_back(s.getSave()[i]);
			if(m_it > s.getIt())
			{
				while(m_save.back().size() < m_it)
				{
					m_save.back().push_back(m_save.back().back());
				}
			}
		}
	}

	void addConst(float c)
	{
		if(!m_it)
			++m_it;
		m_save.push_back(std::list<float>());
		for(unsigned int i(0); i < m_it; ++i)
			m_save.back().push_back(c);
	}

	void operator+=(Solver<Indiv>& s)
	{
		if(m_it < s.m_it)
		{
			m_it = s.m_it;
			for(unsigned int i(0); i < m_save.size(); ++i)
			{
				while(m_save[i].size() < m_it)
				{
					m_save[i].push_back(m_save[i].back());
				}
			}
		}

		for(unsigned int i(0); i < m_save.size(); ++i)
		{
			std::list<float>::iterator it = m_save[i].begin();
			std::list<float>::iterator it2 = s.m_save[i].begin();

			for(unsigned int j(0); j < m_it; ++j)
			{
				if(j < s.m_it)
					*it += *it2;
				else
					*it += s.m_save[i].back();
				++it;
				++it2;
			}
		}
	}

	void operator+=(float c)
	{
		for(unsigned int i(0); i < m_save.size(); ++i)
		{
			for(auto& x : m_save[i])
			{
				x += c;
			}
		}
	}

	void operator/=(float div)
	{
		for(unsigned int i(0); i < m_save.size(); ++i)
		{
			for(auto& x : m_save[i])
			{
				x /= div;
			}
		}
	}

	unsigned int getIt() const
	{
		return m_it;
	}

	const std::vector<std::list<float>>& getSave() const
	{
		return m_save;
	}

	private:

	std::vector<std::list<float>> m_save;
	unsigned int m_it;

	protected:

	Function<Indiv>& m_fn;
};

#endif
