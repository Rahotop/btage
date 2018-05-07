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
	void crossover(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const;
	void mutation(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const;

	void subtreeMutation(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const;
	void pointMutation(const std::vector<IndFunction>& pop, std::vector<IndFunction>& newgen, unsigned int index) const;

	private:

	unsigned int m_n;
	unsigned int m_maxsize;
	unsigned int m_depthinit;
};

#endif