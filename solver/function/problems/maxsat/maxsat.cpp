#include "maxsat.hpp"

MaxSat::MaxSat(const std::string& path) : m_n(0), m_nbclauses(0), m_clauses(nullptr)
{
	std::ifstream in(path.c_str());

	while(true)
	{
		std::string tmp;
		in >> tmp;
		if(tmp == "c")
		{
			char buffer[256];
			in.getline(buffer,256);
		}
		else if(tmp == "p")
		{
			in >> tmp >> m_n >> m_nbclauses;
			break;
		}
	}

	m_clauses = new std::vector<int>[m_nbclauses];

	for(unsigned int i(0); i < m_nbclauses; ++i)
	{
		m_clauses[i] = std::vector<int>();
		while(true)
		{
			int tmp;
			in >> tmp;
			if(tmp)
			{
				m_clauses[i].push_back(tmp);
			}
			else
			{
				break;
			}
		}
	}
}

MaxSat::~MaxSat()
{
	delete[] m_clauses;
}

float MaxSat::evaluate(VectorBool& s)
{
	unsigned int sum = 0;
	for(unsigned int i(0); i < m_nbclauses; ++i)
	{
		bool sat = false;
		for(unsigned int j(0); j < m_clauses[i].size() && !sat; ++j)
		{
			int tmp = m_clauses[i][j];
			sat = (tmp > 0) ? s[tmp-1] : !s[abs(tmp)-1];
		}
		sum += sat;
	}
	return (float)sum;
}

float MaxSat::evaluate(IndFunction& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionTree fn(s.getFunction());
	FixedSizeDescentInc<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i(0); i < 10; ++i)
	{
		VectorBool v = d.solve(); 
		tmp += evaluate(v);
	}
	tmp /= 10.;
	return tmp;
}

unsigned int MaxSat::getN() const
{
	return m_n;
}

