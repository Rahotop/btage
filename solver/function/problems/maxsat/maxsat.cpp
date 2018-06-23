#include "maxsat.hpp"

MaxSat::MaxSat(const std::string& path) : m_n(0), m_nbclauses(0), m_clauses(nullptr), m_nbeval(0)
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
	++m_nbeval;
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
	FunctionTree& fn = s.getFunction();
	FixedSizeDescentInc<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i = 0; i < 10; ++i)
	{
		VectorBool v = d.solve();
		tmp += evaluate(v);
	}
	tmp /= 10.;
	return tmp;
}

float MaxSat::evaluate(IndArray& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionArray& fn = s.getFunction();
	FixedSizeDescentInc<VectorBool> d(fn, gen);

//	std::vector<VectorBool> nbsol;

	float tmp = 0.;
	for(unsigned int i = 0; i < 30; ++i)
	{
		VectorBool v = d.solve();
		tmp += evaluate(v);
/*
		bool in = false;
		for(unsigned int i(0); i < nbsol.size(); ++i)
		{
			in = (in || v == nbsol[i]) ? true : false;
		}
		if(!in)
		{
			nbsol.push_back(v);
		}*/
	}
	tmp /= 30.;
	return tmp;//*0.4+(nbsol.size()/10.)*0.6;
}

unsigned long long MaxSat::getnbeval() const
{
	return m_nbeval;
}

unsigned int MaxSat::getN() const
{
	return m_n;
}

void MaxSat::varLinks(std::ostream& o) const
{
	unsigned int *tab = new unsigned int[m_n*m_n];
	for(unsigned int i(0); i < m_n*m_n; ++i) tab[i] = 0;

	for(unsigned int i(0); i < m_nbclauses; ++i)
	{
		for(unsigned int j(0); j < m_clauses[i].size(); ++j)
		{
			for(unsigned int k(j+1); k < m_clauses[i].size(); ++k)
			{
				++tab[(abs(m_clauses[i][j])-1)*m_n+(abs(m_clauses[i][k])-1)];
				++tab[(abs(m_clauses[i][k])-1)*m_n+(abs(m_clauses[i][j])-1)];			
			}
		}
	}
	
	for(unsigned int i(0); i < m_n; ++i)
	{
		for(unsigned int j(0); j < m_n; ++j)
		{
			if(tab[i*m_n+j])
				o << std::setw(3) << tab[i*m_n+j];
			else if(i == j)
				o << "  |";
			else
				o << "  .";
		}
		o << std::endl;
	}

	delete[] tab;
}



