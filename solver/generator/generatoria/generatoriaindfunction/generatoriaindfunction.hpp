#ifndef GENERATORIAINDFUNCTION_HPP_INCLUDED
#define GENERATORIAINDFUNCTION_HPP_INCLUDED

#include <sstream>
#include <string>
#include "../generatoria.hpp"
#include "../../../individual/indfunction/indfunction.hpp"

class GeneratorIAIndFunction : public GeneratorIA<IndFunction>
{
	public:

	GeneratorIAIndFunction(unsigned int n, unsigned int maxsize, unsigned int depthinit);
	~GeneratorIAIndFunction();

	IndFunction generate() const;
	IndFunction crossover(const std::vector<IndFunction>& pop) const;
	IndFunction mutation(const std::vector<IndFunction>& pop) const;

	IndFunction subtreeMutation(const std::vector<IndFunction>& pop) const;
	IndFunction pointMutation(const std::vector<IndFunction>& pop) const;

	std::ostream& show(std::ostream& o, IndFunction& ind);
	std::ostream& oneLine(std::ostream& o, IndFunction& ind);

	private:

	unsigned int m_n;
	unsigned int m_maxsize;
	unsigned int m_depthinit;
};

#endif