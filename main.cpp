#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "solver/all.hpp"
#include "opt.hpp"

#define VERSION 6

std::string getSettings(opt run);
void plot3d(std::string path, opt run);
void plot3d(opt run, std::string path);
template<class Indiv, class PB>
void graphalgogen(std::ofstream& gnuplot, opt run, Algogen<Indiv>& algo, PB& pb);
template<class Indiv, class PB>
void graphmean(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s);
template<class Indiv, class PB>
void graphagregate(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s);
template<class Indiv, class PB>
void Rgraphmean(std::ofstream& o, opt run, PB& pb, Indiv& s);
template<class Indiv, class PB>
void links(opt run, PB& pb, Indiv& s);
template<class Indiv, class PB>
void correlation(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s);
template<class Indiv, class PB>
void freq(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s);
template<class Indiv, class PB>
void calculate(std::ofstream& gnuplot, opt run, Algogen<Indiv>& algo, PB& pb);

int main(int argc, char **argv)
{
	srand(time(nullptr));

	std::vector<opt> run = getopt(argc, argv);

	std::ofstream gnuplot("gnuplot.sh", std::ios::app);

	for(unsigned int i(0); i < run.size(); ++i)
	{
		if(run[i].execute)
		{
			if(run[i].functionarray)
			{
				//PBTREE

				if(run[i].pbtree)
				{
					srand(run[i].seed);
					PbFunction pb(run[i].n, run[i].depthplus, run[i].depthmin);
					GeneratorIAIndArray gen(run[i].maxsizetree, run[i].maxdepth, run[i].initwidth, run[i].initdepth, run[i].n, pb);
					Algogen<IndArray> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);

					calculate<IndArray,PbFunction>(gnuplot, run[i], algo, pb);
				}

				//NK

				else if(run[i].nk)
				{
					srand(run[i].seed);
					Nk pb(run[i].pbfile);
					GeneratorIAIndArray gen(run[i].maxsizetree, run[i].maxdepth, run[i].initwidth, run[i].initdepth, pb.getN(), pb);
					Algogen<IndArray> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);
					run[i].n = pb.getN();

					calculate<IndArray,Nk>(gnuplot, run[i], algo, pb);
				}

				//MAXSAT

				else if(run[i].maxsat)
				{
					srand(run[i].seed);
					MaxSat pb(run[i].pbfile);
					GeneratorIAIndArray gen(run[i].maxsizetree, run[i].maxdepth, run[i].initwidth, run[i].initdepth, pb.getN(), pb);
					Algogen<IndArray> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);
					run[i].n = pb.getN();

					calculate<IndArray,MaxSat>(gnuplot, run[i], algo, pb);
				}
			}
			else
			{
				//PBTREE

				if(run[i].pbtree)
				{
					srand(run[i].seed);
					PbFunction pb(run[i].n, run[i].depthplus, run[i].depthmin);
					GeneratorIAIndFunction gen(run[i].n, run[i].maxsizetree, run[i].initdepth);
					Algogen<IndFunction> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);

					calculate<IndFunction,PbFunction>(gnuplot, run[i], algo, pb);
				}

				//NK

				else if(run[i].nk)
				{
					srand(run[i].seed);
					Nk pb(run[i].pbfile);
					GeneratorIAIndFunction gen(pb.getN(), run[i].maxsizetree, run[i].initdepth);
					Algogen<IndFunction> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);
					run[i].n = pb.getN();

					calculate<IndFunction,Nk>(gnuplot, run[i], algo, pb);
				}

				//MAXSAT

				else if(run[i].maxsat)
				{
					srand(run[i].seed);
					MaxSat pb(run[i].pbfile);
					GeneratorIAIndFunction gen(pb.getN(), run[i].maxsizetree, run[i].initdepth);
					Algogen<IndFunction> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);
					run[i].n = pb.getN();

					calculate<IndFunction,MaxSat>(gnuplot, run[i], algo, pb);
				}
			}
		}
	}

	return 0;
}






























































std::string getSettings(opt run)
{
	std::string settings;
	settings += "file : "+run.pbfile+" ";
	settings += "N : "+std::to_string(run.n)+" ";
	settings += "maxsize : "+std::to_string(run.maxsizetree)+" ";
	settings += "initdepth : "+std::to_string(run.initdepth)+" ";
	settings += "nbind : "+std::to_string(run.nbind)+" ";
	settings += "cross : "+std::to_string(run.cross)+" ";
	settings += "copy : "+std::to_string(run.copy)+" ";
	settings += "mutation : "+std::to_string(run.mutation)+" ";
	settings += "newop : "+std::to_string(run.newop)+" ";
	settings += "seed : "+std::to_string(run.seed)+" ";
	settings += "version : "+std::to_string(VERSION);

	return settings;
}

void plot3d(std::string path, opt run)
{
	std::ofstream plot(path);

	plot << "set terminal jpeg size 2000,1000" << std::endl;
	plot << "set title \'Ratios of operators in population\'" << std::endl;
	plot << "set xlabel \'population\'" << std::endl;
	plot << "set ylabel \'nbOp\'" << std::endl;
	plot << "set key outside" << std::endl;
	plot << "set output \"" << run.out << "-0ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:8 with lines title 'Size'" << std::endl;
	plot << "set output \"" << run.out << "-1ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:3 with lines title 'Scal'" << std::endl;
	plot << "set output \"" << run.out << "-2ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:4 with lines title 'Equal'" << std::endl;
	plot << "set output \"" << run.out << "-3ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:5 with lines title 'Max'" << std::endl;
	plot << "set output \"" << run.out << "-4ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:6 with lines title 'Min'" << std::endl;
	plot << "set output \"" << run.out << "-5ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:7 with lines title 'Plus'" << std::endl;
	plot << "set output \"" << run.out << "-6ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:9 with lines title 'Fitness'" << std::endl;
}

void plot3d(opt run, std::string path)
{
	std::ofstream plot(path);

	plot << "set terminal jpeg size 2000,1000" << std::endl;
	plot << "set title \'Ratios of operators in population\'" << std::endl;
	plot << "set xlabel \'population\'" << std::endl;
	plot << "set ylabel \'nbOp\'" << std::endl;
	plot << "set key outside" << std::endl;
	plot << "set output \"" << run.out << "-0ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:8 with lines title 'Size'" << std::endl;
	plot << "set output \"" << run.out << "-1ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:3 with lines title 'AND'" << std::endl;
	plot << "set output \"" << run.out << "-2ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:4 with lines title 'OR'" << std::endl;
	plot << "set output \"" << run.out << "-3ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:5 with lines title 'EQUAL'" << std::endl;
	plot << "set output \"" << run.out << "-4ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:6 with lines title '<-'" << std::endl;
	plot << "set output \"" << run.out << "-5ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:7 with lines title '->'" << std::endl;
	plot << "set output \"" << run.out << "-6ratios-graph.jpg" << "\"" << std::endl;
	plot << "plot for [i=0:" << run.nbit << ":" << run.nbit/10-1 << "] \"" << run.out << "-ratios.dat\" every ::i*" << run.nbind << "::(i+1)*" << run.nbind << "-1 using 2:9 with lines title 'Fitness'" << std::endl;
}

template<class Indiv, class PB>
void graphalgogen(std::ofstream& gnuplot, opt run, Algogen<Indiv>& algo, PB& pb)
{
	srand(run.seed);
	Plot<Indiv> graph(pb, run.out + "-algogen-graph.dat", run.out + "-gnuplot-algogen.sh");
	graph.add(algo, {"fitness","size","avsize","nbscal","nbequal","nbmax","nbmin","nbplus"}, {"","axes x1y2","axes x1y2","axes x1y2","axes x1y2","axes x1y2","axes x1y2","axes x1y2"});

	graph.save(run.out + "-algogen-graph.jpg", "fitness as a function of iterations "+getSettings(run)+" nbeval: "+std::to_string(pb.getnbeval()));

	gnuplot << "gnuplot " << run.out << "-gnuplot-algogen.sh" << std::endl;

	std::ofstream ratio(run.out + "-ratios.dat");
	ratio << algo(1, run.nbind);

	plot3d(run.out + "-gnuplot-ratios.sh", run);
	gnuplot << "gnuplot " << run.out << "-gnuplot-ratios.sh" << std::endl;
}

template<class Indiv, class PB>
void graphalgogen(std::ofstream& gnuplot, opt run, Algogen<IndArray>& algo, PB& pb)
{
	srand(run.seed);
	Plot<IndArray> graph(pb, run.out + "-algogen-graph.dat", run.out + "-gnuplot-algogen.sh");
	graph.add(algo, {"fitness","size","avsize","AND","OR","EQUAL","<-","->"}, {"","axes x1y2","axes x1y2","axes x1y2","axes x1y2","axes x1y2","axes x1y2","axes x1y2"});

	graph.save(run.out + "-algogen-graph.jpg", "fitness as a function of iterations "+getSettings(run)+" nbeval: "+std::to_string(pb.getnbeval()));

	gnuplot << "gnuplot " << run.out << "-gnuplot-algogen.sh" << std::endl;

	std::ofstream ratio(run.out + "-ratios.dat");
	ratio << algo(1, run.nbind);

	plot3d(run, run.out + "-gnuplot-ratios.sh");
	gnuplot << "gnuplot " << run.out << "-gnuplot-ratios.sh" << std::endl;

	if(run.diversity)
	{
		std::ofstream diversity(run.out + "-diversity.dat");
		unsigned int width = algo.getWidth(2);
		diversity << algo(2);

		std::ofstream diversityPlot(run.out + "-gnuplot-diversity.sh");

		diversityPlot << "set terminal jpeg size 2000,1000" << std::endl;
		diversityPlot << "set output \"" << run.out << "-diversity.jpg\"" << std::endl;
		diversityPlot << "set title \'diversity\'" << std::endl;
		diversityPlot << "set xlabel \'iteration\'" << std::endl;
		diversityPlot << "set ylabel \'alike\'" << std::endl;
		diversityPlot << "unset key" << std::endl;
		diversityPlot << "plot ";
		for(unsigned int i(0); i < width; ++i)
		{
			diversityPlot << "\"" << run.out << "-diversity.dat\" using 1:" << 2+i << " with lines lt rgb \"gray\"";
			if(i < width-1)
				diversityPlot << ",\\";
			diversityPlot << std::endl;
		}

		gnuplot << "gnuplot " << run.out << "-gnuplot-diversity.sh" << std::endl;
	}
}

template<class Indiv, class PB>
void graphmean(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s)
{
	srand(run.seed);
	GeneratorROOneMax genom(run.n);

	Plot<VectorBool> meanpb(pb);
	Plot<VectorBool> meansol(pb);

	meanpb.add(0., "F-obj", "");
	meansol.add(0., "F-ound", "");

	for(unsigned int j(0); j < run.meansize; ++j)
	{
		FixedSizeDescent<VectorBool> ls1(pb,genom);
		FixedSizeDescentComp<VectorBool> ls2(s.getFunction(), pb, genom);

		ls1.solve();
		ls2.solve();

		meanpb += ls1;
		meansol += ls2;
	}
	meanpb /= (float)run.meansize;
	meansol /= (float)run.meansize;

	FixedSizeILS<VectorBool> ls3(pb,genom,1000,15);
	VectorBool best = ls3.solve();

	Plot<VectorBool> graph(pb, run.out + "-mean-graph.dat", run.out + "-gnuplot-mean.sh");

	graph.add(meanpb);
	graph.add(meansol);
	graph.add(best.getScore(), "ILS", "");

	graph.save(run.out + "-mean-graph.jpg", "mean fitness over " + std::to_string(run.meansize) + " runs as a function of iterations "+getSettings(run));

	gnuplot << "gnuplot " << run.out << "-gnuplot-mean.sh" << std::endl;
}

template<class Indiv, class PB>
void graphagregate(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s)
{
	srand(run.seed);
	GeneratorROOneMax genom(run.n);

	Plot<VectorBool> graph(pb, run.out + "-agregate-graph.dat", run.out + "-gnuplot-agregate.sh");
	Plot<VectorBool> mean(pb);

	mean.add(0., "mean", "ls 1");

	for(unsigned int j(0); j < run.agregatesize; ++j)
	{
		FixedSizeDescentComp<VectorBool> ls(s.getFunction(), pb, genom);

		ls.solve();

		mean += ls;
		graph.add(ls, {std::to_string(j)}, {"ls 2"});
	}
	mean /= run.agregatesize;
	graph.add(mean);

	graph.save(run.out + "-agregate-graph.jpg", "agretation of fitness of " + std::to_string(run.agregatesize) + " runs as a function of iterations "+getSettings(run));

	gnuplot << "gnuplot " << run.out << "-gnuplot-agregate.sh" << std::endl;
}

template<class Indiv, class PB>
void Rgraphmean(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s)
{
	srand(run.seed);
	GeneratorROOneMax genom(run.n);

	Plot<VectorBool> meanpb(pb);
	Plot<VectorBool> meansol(pb);

	meanpb.add(0., "F-obj", "");
	meansol.add(0., "F-ound", "");

	for(unsigned int j(0); j < run.Rmeansize; ++j)
	{
		FixedSizeDescentComp<VectorBool> ls1(pb, s.getFunction(), genom);
		FixedSizeDescent<VectorBool> ls2(s.getFunction(), genom);

		ls1.solve();
		ls2.solve();

		meanpb += ls1;
		meansol += ls2;
	}
	meanpb /= (float)run.Rmeansize;
	meansol /= (float)run.Rmeansize;

	FixedSizeILS<VectorBool> ls3(s.getFunction(),genom,1000,15);
	VectorBool best = ls3.solve();



	Plot<VectorBool> graph(pb, run.out + "-Rmean-graph.dat", run.out + "-gnuplot-Rmean.sh");

	graph.add(meanpb);
	graph.add(meansol);
	graph.add(best.getScore(), "ILS", "");

	graph.save(run.out + "-Rmean-graph.jpg", "mean fitness over " + std::to_string(run.Rmeansize) + " runs as a function of iterations in the F-ound space "+getSettings(run));

	gnuplot << "gnuplot " << run.out << "-gnuplot-Rmean.sh" << std::endl;
}

template<class Indiv, class PB>
void links(opt run, PB& pb, Indiv& s)
{
	std::ofstream out((run.out + "-links.dat").c_str());

	s.getFunction().varLinks(out);
	out << std::endl;
	pb.varLinks(out);
	out << std::endl;
	s.getFunction().show(out);
	out << std::endl;
	s.getFunction().showClauses(out);
}

template<class Indiv, class PB>
void correlation(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s)
{
	std::ofstream out1((run.out + "-correlation1.dat").c_str());
	std::ofstream out2((run.out + "-correlation2.dat").c_str());
	std::ofstream out3((run.out + "-correlation3.dat").c_str());
	std::ofstream plot((run.out + "-gnuplot-correlation.sh").c_str());

	plot << "set terminal jpeg size 2000,1000" << std::endl;
	plot << "set output \"" << run.out << "-correlation-graph.jpg" << "\"" << std::endl;
	plot << "set title \'Correlation between F-ound and F-obj\'" << std::endl;
	plot << "set xlabel \'F-ound\'" << std::endl;
	plot << "set ylabel \'F-obj\'" << std::endl;
	plot << "set key outside" << std::endl;
	plot << "plot \"" << run.out << "-correlation1.dat\" using 1:2 with points title \'random\',\\" << std::endl;
	plot << "\"" << run.out << "-correlation2.dat\" using 1:2 with points title \'LS on F-obj\',\\" << std::endl;
	plot << "\"" << run.out << "-correlation3.dat\" using 1:2 with points title \'LS on F-ound\'";

	GeneratorROOneMax genom(run.n);

	for(unsigned int i(0); i < 10000; ++i)
	{
		auto tmp = genom.generate();
		out1 << s.getFunction().evaluate(tmp) << " " << pb.evaluate(tmp) << std::endl;
	}

	for(unsigned int i(0); i < 1000; ++i)
	{
		FixedSizeDescent<VectorBool> ls(pb, genom);
		auto tmp = ls.solve();

		out2 << s.getFunction().evaluate(tmp) << " " << pb.evaluate(tmp) << std::endl;
	}

	for(unsigned int i(0); i < 1000; ++i)
	{
		FixedSizeDescent<VectorBool> ls(s.getFunction(), genom);
		auto tmp = ls.solve();

		out3 << s.getFunction().evaluate(tmp) << " " << pb.evaluate(tmp) << std::endl;
	}

	gnuplot << "gnuplot " << run.out << "-gnuplot-correlation.sh" << std::endl;



	Plot<VectorBool> graph(pb, run.out + "-correlation-paths.dat", "");

	for(unsigned int i(0); i < 10; ++i)
	{
		FixedSizeDescentCorr<VectorBool> ls(s.getFunction(),s.getFunction(),pb,genom);

		ls.solve();

		graph.add(ls, {"x","y"}, {"",""});
	}
	for(unsigned int i(0); i < 10; ++i)
	{
		FixedSizeDescentCorr<VectorBool> ls(pb,s.getFunction(),pb,genom);

		ls.solve();

		graph.add(ls, {"x","y"}, {"",""});
	}

	graph.save();

	std::ofstream plotcorr((run.out + "-gnuplot-correlation-paths.sh").c_str(), std::ios_base::trunc);

	plotcorr << "set terminal jpeg size 2000,1000" << std::endl;
	plotcorr << "set output \"" << run.out << "-correlation-paths-graph.jpg" << "\"" << std::endl;
	plotcorr << "set title \'Correlation between F-ound and F-obj with paths\'" << std::endl;
	plotcorr << "set xlabel \'F-ound\'" << std::endl;
	plotcorr << "set ylabel \'F-obj\'" << std::endl;
	plotcorr << "set key outside" << std::endl;
	plotcorr << "plot for [i=1:10] \"" << run.out << "-correlation-paths.dat\" u (column(2*i)):(column(2*i+1)) with lines ls 1 title 'F-ound',\\";
	plotcorr << std::endl;
	plotcorr << "for [i=11:20] \"" << run.out << "-correlation-paths.dat\" u (column(2*i)):(column(2*i+1)) with lines ls 2 title 'F-obj'";

	gnuplot << "gnuplot " << run.out << "-gnuplot-correlation-paths.sh" << std::endl;



	Plot<VectorBool> dist(pb, run.out + "-distances.dat", run.out + "-gnuplot-distances.sh");

	for(unsigned int i(0); i < 10; ++i)
	{
		FixedSizeDescentDist<VectorBool> ls(s.getFunction(),genom);

		ls.solve();

		dist.add(ls, {std::to_string(i)}, {""});
	}

	dist.save(run.out + "-distances-graph.jpg", "Distances");

	gnuplot << "gnuplot " << run.out << "-gnuplot-distances.sh" << std::endl;
}

template<class Indiv, class PB>
void freq(std::ofstream& gnuplot, opt run, PB& pb, Indiv& s)
{
	std::vector<VectorBool> maxPb;
	std::vector<unsigned int> freqPb;

	GeneratorROOneMax genom(run.n);

	for(unsigned int i(0); i < 100; ++i)
	{
		FixedSizeDescent<VectorBool> ls(pb, genom);
		VectorBool tmp = ls.solve();

		bool add = true;
		for(unsigned int i(0); i < maxPb.size(); ++i)
		{
			if(tmp == maxPb[i])
			{
				++freqPb[i];
				add = false;
			}
		}
		if(add)
		{
			maxPb.push_back(tmp);
			freqPb.push_back(1);
		}
	}


	std::vector<VectorBool> maxS;
	std::vector<unsigned int> freqS;
	std::vector<bool> isOptimum;

	for(unsigned int i(0); i < 100; ++i)
	{
		FixedSizeDescent<VectorBool> ls(s, genom);
		VectorBool tmp = ls.solve();

		bool add = true;
		for(unsigned int i(0); i < maxS.size(); ++i)
		{
			if(tmp == maxS[i])
			{
				++freqS[i];
				add = false;
			}
		}
		if(add)
		{
			maxS.push_back(tmp);
			freqS.push_back(1);

			Descent<VectorBool> ls(pb,genom);
			isOptimum.push_back(ls.isOptimum(tmp));
		}
	}


	std::ofstream out(run.out + "-freq.dat");
	out << std::setw(35) << "Fonction objectif" << "          " << std::setw(55) << "Fonction trouvee" << std::endl;
	out << std::setw(15) << "Fitness" << "     " << std::setw(15) << "Frequence" << "          ";
	out << std::setw(15) << "Fitness" << "     " << std::setw(15) << "Frequence" << "     " << std::setw(15) << "Opt loc f_obj" << std::endl;

	float meanPb = 0.;
	float meanS = 0.;

	for(unsigned int i(0); i < maxPb.size() || i < maxS.size(); ++i)
	{
		if(i < maxPb.size())
		{
			out << std::setw(15) << maxPb[i].getScore() << "     " << std::setw(15) << freqPb[i] << "          ";
			meanPb += maxPb[i].getScore() * freqPb[i];
		}
		else
		{
			out << std::string(' ', 45);
		}
		if(i < maxS.size())
		{
			float tmp = pb.evaluate(maxS[i]);
			out << std::setw(15) << tmp << "     " << std::setw(15) << freqS[i] << "     " << std::setw(15) << isOptimum[i];
			meanS += tmp * freqS[i];
		}
		out << std::endl;
	}

	out << std::setw(35) << "Fitness moyenne : " + std::to_string(meanPb/100.) << "          " << std::setw(55) << "Fitness moyenne : " + std::to_string(meanS/100.) << std::endl;

	std::ofstream out2(run.out + "-freq-hamming.dat");

	for(unsigned int i(0); i < maxS.size(); ++i)
	{
		for(unsigned int j(0); j < maxS.size(); ++j)
		{
			unsigned int tmp = maxS[i].dist(maxS[j]);
			unsigned int count = 0;
			for(unsigned int k(0); k < maxS.size(); ++k)
			{
				count += (tmp == maxS[i].dist(maxS[k]));
			}
			out2 << "0 0" << std::endl << tmp << " " << count << std::endl;
		}
	}

	std::ofstream plot(run.out + "-gnuplot-freq.sh");
	plot << "set terminal jpeg size 2000,1000" << std::endl;
	plot << "set output \"" << run.out << "-freq-graph.jpg\"" << std::endl;
	plot << "set title 'Frequence des maxima locaux en fonction de leur distances relatives'" << std::endl;
	plot << "set xlabel 'Distance de Hamming'" << std::endl;
	plot << "set ylabel 'Frequence'" << std::endl;
	plot << "set nokey" << std::endl;
	plot << "plot \"" << run.out << "-freq-hamming.dat\" using 1:2 with lines" << std::endl;

	gnuplot << "gnuplot " << run.out << "-gnuplot-freq.sh" << std::endl;
}

template<class Indiv, class PB>
void calculate(std::ofstream& gnuplot, opt run, Algogen<Indiv>& algo, PB& pb)
{
	Indiv s = algo.solve();

	if(run.graphalgogen)
	{
		graphalgogen<Indiv,PB>(gnuplot, run, algo, pb);
	}

	if(run.graphmean)
	{
		graphmean<Indiv,PB>(gnuplot, run, pb, s);
	}

	if(run.graphagregate)
	{
		graphagregate<Indiv,PB>(gnuplot, run, pb, s);
	}

	if(run.Rgraphmean)
	{
		Rgraphmean<Indiv,PB>(gnuplot, run, pb, s);
	}

	if(run.links)
	{
		links(run, pb, s);
	}

	if(run.correlation)
	{
		correlation<Indiv,PB>(gnuplot, run, pb, s);
	}

	if(run.freq)
	{
		freq<Indiv,PB>(gnuplot, run, pb, s);
	}
}


