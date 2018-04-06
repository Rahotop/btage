g++ -Wall -Wextra -pedantic -std=c++11 -ggdb\
	*.cpp \
	solver/*.cpp \
	solver/algogen/*.cpp \
	solver/plot/*.cpp \
	solver/complete/vectorboolcomplete/*.cpp \
	solver/function/*.cpp \
	solver/function/functiontree/*.cpp \
	solver/function/problems/*.cpp \
	solver/function/problems/onemax/*.cpp \
	solver/function/problems/pbfunction/*.cpp \
	solver/function/problems/nk/*.cpp \
	solver/function/problems/maxsat/*.cpp \
	solver/generator/*.cpp \
	solver/generator/generatorro/*.cpp \
	solver/generator/generatorro/generatorroonemax/*.cpp \
	solver/generator/generatoria/*.cpp \
	solver/generator/generatoria/generatoriaonemax/*.cpp \
	solver/generator/generatoria/generatoriaindfunction/*.cpp \
	solver/individual/*.cpp \
	solver/individual/vectorbool/*.cpp \
	solver/individual/indfunction/*.cpp \
	solver/localsearch/*.cpp \
	solver/localsearch/descent/*.cpp \
	solver/localsearch/descent/fixedsizedescent/*.cpp \
	solver/localsearch/ils/*.cpp \
	solver/localsearch/ils/fixedsizeils/*.cpp