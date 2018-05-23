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

	Solver(Function<Indiv>& fn) : m_save(), m_it(0), m_id(0), m_div(1), m_fn(fn)
	{

	}

	virtual ~Solver()
	{

	}

	virtual Indiv solve() = 0;

	void iterate(const std::vector<float>& v, unsigned int id = 0)
	{
		while(m_save.size() <= id)
		{
			m_save.push_back(std::vector<std::list<float>>());
			m_it.push_back(0);
		}

		for(unsigned int i(0); i < v.size(); ++i)
		{
			if(i == m_save[id].size())
			{
				m_save[id].push_back(std::list<float>());
				for(unsigned int j(0); j < m_it[id]; ++j)
					m_save[id][i].push_back(0.);
			}
			m_save[id][i].push_back(v[i]);
		}
		++m_it[id];
	}

	friend std::ostream& operator<<(std::ostream& o, Solver<Indiv>& s)
	{
		for(unsigned int i(0); i < s.m_it[s.m_id]; ++i)
		{
			o << i/s.m_div << " ";
			for(unsigned int j(0); j < s.m_save[s.m_id].size(); ++j)
			{
				o << s.m_save[s.m_id][j].front() << " ";
				s.m_save[s.m_id][j].pop_front();
			}
			o << std::endl;
		}
		s.m_id = 0;
		s.m_div = 1;

		return o;
	}

	template<class Indiv2>
	void addLine(const Solver<Indiv2>& s, unsigned int id = 0)
	{
		while(m_save.size() <= id)
		{
			m_save.push_back(std::vector<std::list<float>>());
			m_it.push_back(0);
		}

		if(m_it[id] < s.getIt())
		{
			m_it[id] = s.getIt();
			for(unsigned int i(0); i < m_save[id].size(); ++i)
			{
				while(m_save[id][i].size() < m_it[id])
				{
					m_save[id][i].push_back(m_save[id][i].back());
				}
			}
		}

		for(unsigned int i(0); i < s.getSave().size(); ++i)
		{
			m_save[id].push_back(s.getSave()[i]);
			if(m_it[id] > s.getIt())
			{
				while(m_save[id].back().size() < m_it[id])
				{
					m_save[id].back().push_back(m_save[id].back().back());
				}
			}
		}
	}

	void addConst(float c, unsigned int id = 0)
	{
		while(m_save.size() <= id)
		{
			m_save.push_back(std::vector<std::list<float>>());
			m_it.push_back(0);
		}

		if(!m_it[id])
			++m_it[id];
		m_save[id].push_back(std::list<float>());
		for(unsigned int i(0); i < m_it[id]; ++i)
			m_save[id].back().push_back(c);
	}

	void operator+=(Solver<Indiv>& s)
	{
		if(m_it[m_id] < s.m_it[s.m_id])
		{
			m_it[m_id] = s.m_it[s.m_id];
			for(unsigned int i(0); i < m_save[m_id].size(); ++i)
			{
				while(m_save[m_id][i].size() < m_it[m_id])
				{
					m_save[m_id][i].push_back(m_save[m_id][i].back());
				}
			}
		}

		for(unsigned int i(0); i < m_save[m_id].size(); ++i)
		{
			std::list<float>::iterator it = m_save[m_id][i].begin();
			std::list<float>::iterator it2 = s.m_save[s.m_id][i].begin();

			for(unsigned int j(0); j < m_it[m_id]; ++j)
			{
				if(j < s.m_it[s.m_id])
					*it += *it2;
				else
					*it += s.m_save[s.m_id][i].back();
				++it;
				++it2;
			}
		}

		m_id = 0;
	}

	void operator+=(float c)
	{
		for(unsigned int i(0); i < m_save[m_id].size(); ++i)
		{
			for(auto& x : m_save[m_id][i])
			{
				x += c;
			}
		}
		m_id = 0;
	}

	void operator/=(float div)
	{
		for(unsigned int i(0); i < m_save[m_id].size(); ++i)
		{
			for(auto& x : m_save[m_id][i])
			{
				x /= div;
			}
		}
		m_id = 0;
	}

	unsigned int getIt(unsigned int id = 0) const
	{
		return m_it[id];
	}

	unsigned int getWidth(unsigned int id = 0) const
	{
		return m_save[id].size();
	}

	const std::vector<std::list<float>>& getSave(unsigned int id = 0) const
	{
		return m_save[id];
	}

	Solver<Indiv>& operator()(unsigned int id)
	{
		m_id = id;
		return *this;
	}

	Solver<Indiv>& operator()(unsigned int id, unsigned int div)
	{
		m_id = id;
		m_div = div;
		return *this;
	}

	private:

	std::vector<std::vector<std::list<float>>> m_save;
	std::vector<unsigned int> m_it;
	unsigned int m_id;
	unsigned int m_div;

	protected:

	Function<Indiv>& m_fn;
};

#endif
