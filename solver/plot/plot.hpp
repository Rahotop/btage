#ifndef PLOT_HPP_INCLUDED
#define PLOT_HPP_INCLUDED

#include <fstream>
#include <string>
#include <vector>
#include "../solver.hpp"

template<class Indiv>
class Plot : public Solver<Indiv>
{
	public:

	Plot(Function<Indiv>& fn) : Solver<Indiv>(fn), m_data(), m_script(), m_names(), m_end(), m_path()
	{

	}

	Plot(Function<Indiv>& fn, const std::string& pathdata, const std::string& pathscript) : Solver<Indiv>(fn), m_data(pathdata.c_str()), m_script(pathscript.c_str()), m_names(), m_end(), m_path(pathdata)
	{

	}

	~Plot()
	{

	}

	Indiv solve()
	{
		return Indiv();
	}

	template<class Indiv2>
	void add(const Solver<Indiv2>& s, const std::vector<std::string>& names, const std::vector<std::string>& end)
	{
		Solver<Indiv>::addLine(s);
		for(unsigned int i(0); i < names.size(); ++i)
		{
			m_names.push_back(names[i]);
		}
		for(unsigned int i(0); i < end.size(); ++i)
		{
			m_end.push_back(end[i]);
		}
	}

	template<class Indiv2>
	void add(const Plot<Indiv2>& s)
	{
		Solver<Indiv>::addLine(s);
		for(unsigned int i(0); i < s.m_names.size(); ++i)
		{
			m_names.push_back(s.m_names[i]);
		}
		for(unsigned int i(0); i < s.m_end.size(); ++i)
		{
			m_end.push_back(s.m_end[i]);
		}
	}

	void add(float c, const std::string& name, const std::string& end)
	{
		Solver<Indiv>::addConst(c);
		m_names.push_back(name);
		m_end.push_back(end);
	}

	void save(const std::string& output, const std::string& title)
	{
		m_data << *this;
		//operator<<(m_data,*this);

		m_script << "set terminal jpeg size 2000,1000" << std::endl;
		m_script << "set output \"" << output << "\"" << std::endl;
		m_script << "set title \'" << title << "\'" << std::endl;
		m_script << "set xlabel \'iteration\'" << std::endl;
		m_script << "set ylabel \'fitness\'" << std::endl;
		m_script << "set key outside" << std::endl;
		m_script << "plot ";
		for(unsigned int i(0); i < m_names.size(); ++i)
		{
			m_script << "\"" << m_path << "\" using 1:" << 2+i << " with lines " << m_end[i] << " title \'" << m_names[i] << "\'";
			if(i < m_names.size()-1)
				m_script << ",\\";
			m_script << std::endl;
		}
	}

	void save()
	{
		m_data << *this;
	}

	private:

	std::ofstream m_data;
	std::ofstream m_script;

	std::vector<std::string> m_names;
	std::vector<std::string> m_end;
	std::string m_path;
};

#endif