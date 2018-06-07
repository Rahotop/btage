#include "functiontree.hpp"

//std::default_random_engine FunctionTree::s_gen = std::default_random_engine(rand());
//std::uniform_real_distribution<float> FunctionTree::s_dist = std::uniform_real_distribution<float>(0.,1.);

FunctionTree::FunctionTree() :
	Function<VectorBool>(),
	m_maxsize(0),
	m_op(nullptr),
	m_root(nullptr),
	m_child1(nullptr),
	m_child2(nullptr),
	m_isvarin(nullptr),
	m_preveval(nullptr),
	m_curreval(nullptr),
	m_scal(nullptr),
	m_bitindex(nullptr),
	m_isused(nullptr),
	m_n(0),
	s_gen(rand()),
	s_dist(0.,1.)
{
	
}

FunctionTree::FunctionTree(const FunctionTree& fn) :
	Function<VectorBool>(),
	m_maxsize(fn.m_maxsize),
	m_op(new unsigned int[fn.m_maxsize]),
	m_root(new unsigned int[fn.m_maxsize]),
	m_child1(new unsigned int[fn.m_maxsize]),
	m_child2(new unsigned int[fn.m_maxsize]),
	m_isvarin(new bool[fn.m_maxsize*fn.m_n]),
	m_preveval(new float[fn.m_maxsize]),
	m_curreval(new float[fn.m_maxsize]),
	m_scal(new float[fn.m_maxsize]),
	m_bitindex(new unsigned int[fn.m_maxsize]),
	m_isused(new bool[fn.m_maxsize]),
	m_n(fn.m_n),
	s_gen(rand()),
	s_dist(0.,1.)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}

	for(unsigned int i(0); i < m_maxsize*m_n; ++i)
	{
		m_isvarin[i] = false;
	}

	copy(fn,0);
	updateVarIn();
}

FunctionTree::FunctionTree(const FunctionTree& fn, unsigned int node) :
	Function<VectorBool>(),
	m_maxsize(fn.m_maxsize),
	m_op(new unsigned int[fn.m_maxsize]),
	m_root(new unsigned int[fn.m_maxsize]),
	m_child1(new unsigned int[fn.m_maxsize]),
	m_child2(new unsigned int[fn.m_maxsize]),
	m_isvarin(new bool[fn.m_maxsize*fn.m_n]),
	m_preveval(new float[fn.m_maxsize]),
	m_curreval(new float[fn.m_maxsize]),
	m_scal(new float[fn.m_maxsize]),
	m_bitindex(new unsigned int[fn.m_maxsize]),
	m_isused(new bool[fn.m_maxsize]),
	m_n(fn.m_n),
	s_gen(rand()),
	s_dist(0.,1.)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}

	for(unsigned int i(0); i < m_maxsize*m_n; ++i)
	{
		m_isvarin[i] = false;
	}

	copy(fn,node);
	updateVarIn();
}

FunctionTree::FunctionTree(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full) :
	Function<VectorBool>(),
	m_maxsize(maxsize),
	m_op(new unsigned int[maxsize]),
	m_root(new unsigned int[maxsize]),
	m_child1(new unsigned int[maxsize]),
	m_child2(new unsigned int[maxsize]),
	m_isvarin(new bool[maxsize*termset]),
	m_preveval(new float[maxsize]),
	m_curreval(new float[maxsize]),
	m_scal(new float[maxsize]),
	m_bitindex(new unsigned int[maxsize]),
	m_isused(new bool[m_maxsize]),
	m_n(termset),
	s_gen(rand()),
	s_dist(0.,1.)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}

	for(unsigned int i(0); i < m_maxsize*m_n; ++i)
	{
		m_isvarin[i] = false;
	}

	construct(depth,termset,full);
	updateVarIn();
}

FunctionTree::FunctionTree(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset, unsigned int termset) :
	Function<VectorBool>(),
	m_maxsize(calculSizePb(depths, fnset)),
	m_op(new unsigned int[m_maxsize]),
	m_root(new unsigned int[m_maxsize]),
	m_child1(new unsigned int[m_maxsize]),
	m_child2(new unsigned int[m_maxsize]),
	m_isvarin(new bool[m_maxsize*termset]),
	m_preveval(new float[m_maxsize]),
	m_curreval(new float[m_maxsize]),
	m_scal(new float[m_maxsize]),
	m_bitindex(new unsigned int[m_maxsize]),
	m_isused(new bool[m_maxsize]),
	m_n(termset),
	s_gen(rand()),
	s_dist(0.,1.)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}

	for(unsigned int i(0); i < m_maxsize*m_n; ++i)
	{
		m_isvarin[i] = false;
	}

	constructPb(depths,fnset,termset);
	updateVarIn();
}

FunctionTree::~FunctionTree()
{
	if(m_op)
		delete[] m_op;
	if(m_root)
		delete[] m_root;
	if(m_child1)
		delete[] m_child1;
	if(m_child2)
		delete[] m_child2;
	if(m_isvarin)
		delete[] m_isvarin;
	if(m_preveval)
		delete[] m_preveval;
	if(m_curreval)
		delete[] m_curreval;
	if(m_scal)
		delete[] m_scal;
	if(m_bitindex)
		delete[] m_bitindex;
	if(m_isused)
		delete[] m_isused;
}

FunctionTree& FunctionTree::operator=(const FunctionTree& f)
{
	if(m_maxsize != f.m_maxsize)
	{
		if(m_op)
			delete[] m_op;
		if(m_root)
			delete[] m_root;
		if(m_child1)
			delete[] m_child1;
		if(m_child2)
			delete[] m_child2;
		if(m_isvarin)
			delete[] m_isvarin;
		if(m_preveval)
			delete[] m_preveval;
		if(m_curreval)
			delete[] m_curreval;
		if(m_scal)
			delete[] m_scal;
		if(m_bitindex)
			delete[] m_bitindex;
		if(m_isused)
			delete[] m_isused;

		m_maxsize = f.m_maxsize;
		m_n = f.m_n;
		m_op = new unsigned int[m_maxsize];
		m_root = new unsigned int[m_maxsize];
		m_child1 = new unsigned int[m_maxsize];
		m_child2 = new unsigned int[m_maxsize];
		m_isvarin = new bool[m_maxsize*m_n];
		m_preveval = new float[m_maxsize];
		m_curreval = new float[m_maxsize];
		m_scal = new float[m_maxsize];
		m_bitindex = new unsigned int[m_maxsize];
		m_isused = new bool[m_maxsize];
	}

	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}

	for(unsigned int i(0); i < m_maxsize*m_n; ++i)
	{
		m_isvarin[i] = false;
	}

	copy(f,0);
	updateVarIn();

	return *this;
}

void FunctionTree::swap(FunctionTree& f)
{
	unsigned int tmpui = m_maxsize;
	m_maxsize = f.m_maxsize;
	f.m_maxsize = tmpui;

	unsigned int *tmpuip = m_op;
	m_op = f.m_op;
	f.m_op = tmpuip;

	tmpuip = m_root;
	m_root = f.m_root;
	f.m_root = tmpuip;

	tmpuip = m_child1;
	m_child1 = f.m_child1;
	f.m_child1 = tmpuip;

	tmpuip = m_child2;
	m_child2 = f.m_child2;
	f.m_child2 = tmpuip;

	bool *tmpbp = m_isvarin;
	m_isvarin = f.m_isvarin;
	f.m_isvarin = tmpbp;

	float *tmpfp = m_preveval;
	m_preveval = f.m_preveval;
	f.m_preveval = tmpfp;

	tmpfp = m_curreval;
	m_curreval = f.m_curreval;
	f.m_curreval = tmpfp;

	tmpfp = m_scal;
	m_scal = f.m_scal;
	f.m_scal = tmpfp;

	tmpuip = m_bitindex;
	m_bitindex = f.m_bitindex;
	f.m_bitindex = tmpuip;

	tmpbp = m_isused;
	m_isused = f.m_isused;
	f.m_isused = tmpbp;

	tmpui = m_n;
	m_n = f.m_n;
	f.m_n = tmpui;
}

unsigned int FunctionTree::newNode()
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		if(!m_isused[i])
		{
			m_isused[i] = true;
			return i;
		}
	}
	return 0;
}

void FunctionTree::erase(unsigned int node)
{
	if(m_op[node] > 1)
		erase(m_child1[node]);
	if(m_op[node] > 2)
		erase(m_child2[node]);

	m_isused[node] = false;
}

void FunctionTree::replace(unsigned int node, const FunctionTree& f)
{
	if(node)
	{
		unsigned int root = m_root[node];
		erase(node);

		if(m_child1[root] == node)
		{
			m_child1[root] = copy(f,0,root);
			updateVarIn(m_child1[root]);
		}
		else
		{
			m_child2[root] = copy(f,0,root);
			updateVarIn(m_child2[root]);
		}

		updateVarInBU(root);
	}
	else
	{
		erase(0);
		copy(f,0);

		updateVarIn();
	}
}

void FunctionTree::cut(unsigned int node)
{
	if(node)
	{
		unsigned int root = m_root[node];
		erase(node);
		if(m_child1[root] == node)
		{
			m_child1[root] = leaf(root);
			updateVarIn(m_child1[root]);
		}
		else
		{
			m_child2[root] = leaf(root);
			updateVarIn(m_child2[root]);
		}

		updateVarInBU(root);
	}
	else
	{
		erase(0);
		unsigned int tmp = newNode();

		m_op[tmp] = rand()%2;
		m_root[tmp] = 0;
		m_scal[tmp] = 0.;
		m_bitindex[tmp] = rand()%m_n;
		m_child1[tmp] = 0;
		m_child2[tmp] = 0;

		updateVarIn();
	}
}

void FunctionTree::mutate(unsigned int node)
{
	if(m_op[node] < 2)
	{
		m_op[node] = rand()%2;
		m_bitindex[node] = rand()%m_n;

		updateVarInBU(node);
	}
	else if(m_op[node] == 2)
	{
		m_scal[node] = s_dist(s_gen);
	}
	else
	{
		m_op[node] = 3+rand()%4;
	}
}

unsigned int FunctionTree::leaf(unsigned int root)
{
	unsigned int node = newNode();

	m_op[node] = rand()%2;
	m_root[node] = root;
	m_scal[node] = 0.;
	m_bitindex[node] = rand()%m_n;
	m_child1[node] = 0;
	m_child2[node] = 0;

	return node;
}

unsigned int FunctionTree::copy(const FunctionTree& f, unsigned int fnode, unsigned int root)
{
	unsigned int node = newNode();

	m_op[node] = f.m_op[fnode];
	m_root[node] = root;
	m_scal[node] = f.m_scal[fnode];
	m_bitindex[node] = f.m_bitindex[fnode];

	if(m_op[node] == 2)
	{
		unsigned int child = f.m_child1[fnode];/*
		while(f.m_op[child] == 2)
		{
			m_scal[node] *= f.m_scal[child];
			child = f.m_child1[child];
		}*/
		m_child1[node] = copy(f, child, node);
	}
	else if(m_op[node] > 2)
	{
		m_child1[node] = copy(f, f.m_child1[fnode], node);
		m_child2[node] = copy(f, f.m_child2[fnode], node);

		//m_op[node] = (isOPinSubTree(6,node)) ? 6 : m_op[node];
	}

	return node;
}

unsigned int FunctionTree::construct(unsigned int depth, unsigned int termset, bool full, unsigned int root)
{
	unsigned int node = newNode();

	if(!depth || rand()%(1+!full))	//Arity 0
	{
		m_op[node] = rand()%2;
		m_root[node] = root;
		m_scal[node] = 0.;
		m_bitindex[node] = rand()%termset;
		m_child1[node] = 0;
		m_child2[node] = 0;
	}
	else if(!(rand()%5))			//Arity 1
	{
		m_op[node] = 2;
		m_root[node] = root;
		m_scal[node] = s_dist(s_gen);
		m_bitindex[node] = 0;
		m_child1[node] = construct(depth-1, termset, full, node);
		m_child2[node] = 0;
	}
	else							//Arity 2
	{
		m_op[node] = 3+rand()%4;
		m_root[node] = root;
		m_scal[node] = 0.;
		m_bitindex[node] = 0;
		m_child1[node] = construct(depth-1, termset, full, node);
		m_child2[node] = construct(depth-1, termset, full, node);
	}

	return node;
}

unsigned int FunctionTree::constructPb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset, unsigned int termset, unsigned int root)
{
	unsigned int node = newNode();

	m_op[node] = 0;

	for(unsigned int i(0); i < depths.size(); ++i)
	{
		if(depths[i])
		{
			--depths[i];

			m_op[node] = fnset[i];
			m_root[node] = root;
			m_scal[node] = (fnset[i] == 2) ? s_dist(s_gen) : 0.;
			m_bitindex[node] = 0;

			m_child1[node] = constructPb(depths, fnset, termset, node);
			m_child2[node] = (m_op[i] > 2) ? constructPb(depths, fnset, termset, node) : 0;
		}
	}

	if(!m_op[node])
	{
		m_op[node] = rand()%2;
		m_root[node] = root;
		m_scal[node] = 0.;
		m_bitindex[node] = rand()%termset;
		m_child1[node] = 0;
		m_child2[node] = 0;
	}

	return node;
}

unsigned int FunctionTree::calculSizePb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset)
{
	unsigned int sum = 1;
	unsigned int prev = 1;

	for(unsigned int i(0); i < depths.size(); ++i)
	{
		for(unsigned int j(0); j < depths[i]; ++j)
		{
			prev *= 1+(fnset[i] > 2);
			sum += prev;
		}
	}

	return sum;
}

float FunctionTree::evaluate(VectorBool& s)
{
	return evaluate(s,0);
}

float FunctionTree::evaluate(VectorBool& s, unsigned int node)
{
	float tmp = 0;
	if(m_op[node] == 0)
	{
		tmp = s[m_bitindex[node]];
	}
	else if(m_op[node] == 1)
	{
		tmp = !s[m_bitindex[node]];
	}
	else if(m_op[node] == 2)
	{
		tmp = m_scal[node]*evaluate(s,m_child1[node]);
	}
	else if(m_op[node] == 3)
	{
		float c1 = evaluate(s,m_child1[node]);
		float c2 = evaluate(s,m_child2[node]);
		tmp = ((c1 == 0 || c1 == 1) && (c2 == 0 || c2 == 1)) ? c1 == c2 : c1 + c2;
		//tmp = evaluate(s,m_child1[node]) == evaluate(s,m_child2[node]);
	}
	else if(m_op[node] == 4)
	{
		float c1 = evaluate(s,m_child1[node]);
		float c2 = evaluate(s,m_child2[node]);
		tmp = ((c1 == 0 || c1 == 1) && (c2 == 0 || c2 == 1)) ? std::max(c1,c2) : c1 + c2;
		//tmp = std::max(evaluate(s,m_child1[node]),evaluate(s,m_child2[node]));
	}
	else if(m_op[node] == 5)
	{
		float c1 = evaluate(s,m_child1[node]);
		float c2 = evaluate(s,m_child2[node]);
		tmp = ((c1 == 0 || c1 == 1) && (c2 == 0 || c2 == 1)) ? std::min(c1,c2) : c1 + c2;
		//tmp = std::min(evaluate(s,m_child1[node]),evaluate(s,m_child2[node]));
	}
	else if(m_op[node] == 6)
	{
		tmp = evaluate(s,m_child1[node]) + evaluate(s,m_child2[node]);
	}

	m_curreval[node] = tmp;
	m_preveval[node] = tmp;

	return tmp;
}

unsigned int FunctionTree::size() const
{
	unsigned int tmp = 0;
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		tmp += m_isused[i];
	}
	return tmp;
}

unsigned int FunctionTree::getNode(unsigned int offset) const
{
	unsigned int tmp = 0;
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		if(m_isused[i])
		{
			if(tmp == offset)
			{
				return i;
			}
			tmp++;
		}
	}
	return 0;
}

bool FunctionTree::isLeaf(unsigned int node) const
{
	return m_op[node] < 2;
}

void FunctionTree::show(std::ostream& o, unsigned int depth, unsigned int node) const
{
	for(unsigned int i(0); i < depth; ++i)
	{
		o << "| ";
	}
	o << "-";
	if(m_op[node] == 0)
		o << "X" << m_bitindex[node];
	else if(m_op[node] == 1)
		o << "!X" << m_bitindex[node];
	else if(m_op[node] == 2)
		o << "Scal " << m_scal[node];
	else if(m_op[node] == 3)
		o << "Equal";
	else if(m_op[node] == 4)
		o << "Max";
	else if(m_op[node] == 5)
		o << "Min";
	else if(m_op[node] == 6)
		o << "Plus";
	o << " " << m_isused[node] << "," << node << std::endl;

	if(m_op[node] > 1)
		show(o, depth+1, m_child1[node]);
	if(m_op[node] > 2)
		show(o, depth+1, m_child2[node]);
}

void FunctionTree::showClauses(std::ostream& o, unsigned int depth, unsigned int node) const
{
	if(isOPinSubTree(6, node))
	{
		if(m_op[node] > 1)
			showClauses(o, depth, m_child1[node]);
		if(m_op[node] > 2)
			showClauses(o, depth, m_child2[node]);
	}
	else
	{
		for(unsigned int i(0); i < depth; ++i)
		{
			o << "| ";
		}
		o << "-";
		if(m_op[node] == 0)
			o << "X" << m_bitindex[node];
		else if(m_op[node] == 1)
			o << "!X" << m_bitindex[node];
		else if(m_op[node] == 2)
			o << "Scal " << m_scal[node];
		else if(m_op[node] == 3)
			o << "Equal";
		else if(m_op[node] == 4)
			o << "Max";
		else if(m_op[node] == 5)
			o << "Min";
		else if(m_op[node] == 6)
			o << "Plus";
		o << std::endl;

		if(m_op[node] > 1)
			showClauses(o, depth+1, m_child1[node]);
		if(m_op[node] > 2)
			showClauses(o, depth+1, m_child2[node]);
	}
}

void FunctionTree::updateVarIn(unsigned int node)
{
	if(m_op[node] < 2)
	{
		for(unsigned int i(0); i < m_n; ++i)
		{
			m_isvarin[i*m_maxsize+node] = false;
		}
		m_isvarin[m_bitindex[node]*m_maxsize+node] = true;
	}
	else if(m_op[node] < 3)
	{
		updateVarIn(m_child1[node]);
		for(unsigned int i(0); i < m_n; ++i)
		{
			m_isvarin[i*m_maxsize+node] = m_isvarin[i*m_maxsize+m_child1[node]];
		}
	}
	else
	{
		updateVarIn(m_child1[node]);
		updateVarIn(m_child2[node]);
		for(unsigned int i(0); i < m_n; ++i)
		{
			m_isvarin[i*m_maxsize+node] = m_isvarin[i*m_maxsize+m_child1[node]] || m_isvarin[i*m_maxsize+m_child2[node]];
		}
	}
}

void FunctionTree::updateVarInBU(unsigned int node)
{
	bool tmp = false;

	if(m_op[node] < 2)
	{
		for(unsigned int i(0); i < m_n; ++i)
		{
			m_isvarin[i*m_maxsize+node] = false;
		}
		m_isvarin[m_bitindex[node]*m_maxsize+node] = true;
		tmp = true;
	}
	else if(m_op[node] < 3)
	{
		for(unsigned int i(0); i < m_n; ++i)
		{
			if(m_isvarin[i*m_maxsize+node] != m_isvarin[i*m_maxsize+m_child1[node]])
			{
				m_isvarin[i*m_maxsize+node] = !m_isvarin[i*m_maxsize+node];
				tmp = true;
			}
		}
	}
	else
	{
		for(unsigned int i(0); i < m_n; ++i)
		{
			if(m_isvarin[i*m_maxsize+node] != (m_isvarin[i*m_maxsize+m_child1[node]] || m_isvarin[i*m_maxsize+m_child2[node]]))
			{
				m_isvarin[i*m_maxsize+node] = !m_isvarin[i*m_maxsize+node];
				tmp = true;
			}
		}
	}

	if(tmp && node)
	{
		updateVarInBU(m_root[node]);
	}
}

void FunctionTree::makeCurrentPrev(unsigned int bitChanged)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_preveval[i] = (m_isvarin[bitChanged*m_maxsize+i]) ? m_curreval[i] : m_preveval[i];
	}
}

float FunctionTree::evaluateInc(VectorBool& s, unsigned int bitChanged, unsigned int node)
{
	float tmp = 0;

	if(m_op[node] == 0)
	{
		tmp = s[m_bitindex[node]];
	}
	else if(m_op[node] == 1)
	{
		tmp = !s[m_bitindex[node]];
	}
	else if(m_op[node] == 2)
	{
		tmp = m_scal[node]*evaluateInc(s,bitChanged,m_child1[node]);
	}
	else if(m_op[node] == 3)
	{
		float c1 = (m_isvarin[bitChanged*m_maxsize+m_child1[node]]) ? evaluateInc(s,bitChanged,m_child1[node]) : m_preveval[m_child1[node]];
		float c2 = (m_isvarin[bitChanged*m_maxsize+m_child2[node]]) ? evaluateInc(s,bitChanged,m_child2[node]) : m_preveval[m_child2[node]];

		tmp = ((c1 == 0 || c1 == 1) && (c2 == 0 || c2 == 1)) ? c1 == c2 : c1 + c2;
		//tmp = c1 == c2;
	}
	else if(m_op[node] == 4)
	{
		float c1 = (m_isvarin[bitChanged*m_maxsize+m_child1[node]]) ? evaluateInc(s,bitChanged,m_child1[node]) : m_preveval[m_child1[node]];
		float c2 = (m_isvarin[bitChanged*m_maxsize+m_child2[node]]) ? evaluateInc(s,bitChanged,m_child2[node]) : m_preveval[m_child2[node]];

		tmp = ((c1 == 0 || c1 == 1) && (c2 == 0 || c2 == 1)) ? std::max(c1,c2) : c1 + c2;
		//tmp = std::max(c1, c2);
	}
	else if(m_op[node] == 5)
	{
		float c1 = (m_isvarin[bitChanged*m_maxsize+m_child1[node]]) ? evaluateInc(s,bitChanged,m_child1[node]) : m_preveval[m_child1[node]];
		float c2 = (m_isvarin[bitChanged*m_maxsize+m_child2[node]]) ? evaluateInc(s,bitChanged,m_child2[node]) : m_preveval[m_child2[node]];
		
		tmp = ((c1 == 0 || c1 == 1) && (c2 == 0 || c2 == 1)) ? std::min(c1,c2) : c1 + c2;
		//tmp = std::min(c1, c2);
	}
	else if(m_op[node] == 6)
	{
		float c1 = (m_isvarin[bitChanged*m_maxsize+m_child1[node]]) ? evaluateInc(s,bitChanged,m_child1[node]) : m_preveval[m_child1[node]];
		float c2 = (m_isvarin[bitChanged*m_maxsize+m_child2[node]]) ? evaluateInc(s,bitChanged,m_child2[node]) : m_preveval[m_child2[node]];
		
		tmp = c1 + c2;
	}

	m_curreval[node] = tmp;

	return tmp;
}

unsigned int FunctionTree::countOP(unsigned int op) const
{
	unsigned int tmp = 0;
	if(op < 3)
	{
		for(unsigned int i(0); i < m_maxsize; ++i)
		{
			tmp += m_isused[i] && (m_op[i] == op);
		}
	}
	else if(op > 5)
	{
		for(unsigned int i(0); i < m_maxsize; ++i)
		{
			tmp += m_isused[i] && (m_op[i] > 2) && isOPinSubTree(6,i);
		}
	}
	else
	{
		for(unsigned int i(0); i < m_maxsize; ++i)
		{
			tmp += m_isused[i] && (m_op[i] == op) && !isOPinSubTree(6,i);
		}
	}
	return tmp;
}

void FunctionTree::varLinks(std::ostream& o) const
{
	auto links = getLinks();

	unsigned int *tab = new unsigned int[m_n*m_n];
	for(unsigned int i(0); i < m_n*m_n; ++i) tab[i] = 0;

	for(unsigned int i(0); i < links.size(); ++i)
	{
		for(unsigned int j(0); j < links[i].size(); ++j)
		{
			for(unsigned int k(j+1); k < links[i].size(); ++k)
			{
				++tab[links[i][j]*m_n+links[i][k]];
				++tab[links[i][k]*m_n+links[i][j]];
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
	for(unsigned int i(0); i < m_n; ++i)
	{
		unsigned int tmp = 0;
		for(unsigned int j(0); j < m_maxsize; ++j)
		{
			tmp += (m_isused[j]) && (m_op[j] == 0) && (m_bitindex[j] == i);
		}
		o << std::setw(3) << tmp;
	}
	o << std::endl;
	for(unsigned int i(0); i < m_n; ++i)
	{
		unsigned int tmp = 0;
		for(unsigned int j(0); j < m_maxsize; ++j)
		{
			tmp += (m_isused[j]) && (m_op[j] == 1) && (m_bitindex[j] == i);
		}
		o << std::setw(3) << tmp;
	}
	o << std::endl;

	delete[] tab;
}

bool FunctionTree::isOPinSubTree(unsigned int op, unsigned int node) const
{
	if(m_op[node] == op)
		return true;
	else if(m_op[node] < 2)
		return false;
	else if(m_op[node] == 2)
		return isOPinSubTree(op, m_child1[node]);
	return isOPinSubTree(op, m_child1[node]) || isOPinSubTree(op, m_child2[node]);
}

std::vector<unsigned int> FunctionTree::getVar(unsigned int node) const
{
	std::vector<unsigned int> tmp;

	if(m_op[node] < 2)
	{
		tmp.push_back(m_bitindex[node]);
	}
	else if(m_op[node] == 2)
	{
		tmp = getVar(m_child1[node]);
	}
	else
	{
		tmp = getVar(m_child1[node]);

		std::vector<unsigned int> tmp2 = getVar(m_child2[node]);

		for(unsigned int i(0); i < tmp2.size(); ++i)
		{
			tmp.push_back(tmp2[i]);
		}
	}

	return tmp;
}

std::vector<std::vector<unsigned int>> FunctionTree::getLinks(unsigned int node) const
{
	std::vector<std::vector<unsigned int>> tmp;

	if(!isOPinSubTree(6, node))
	{
		tmp.push_back(getVar(node));
	}
	else if(m_op[node] == 2)
	{
		tmp = getLinks(m_child1[node]);
	}
	else
	{
		tmp = getLinks(m_child1[node]);

		std::vector<std::vector<unsigned int>> tmp2 = getLinks(m_child2[node]);

		for(unsigned int i(0); i < tmp2.size(); ++i)
		{
			tmp.push_back(tmp2[i]);
		}
	}

	return tmp;
}







