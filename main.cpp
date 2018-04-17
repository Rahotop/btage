#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "solver/all.hpp"
#include "opt.hpp"

#define VERSION 3

int main(int argc, char **argv)
{
	srand(time(nullptr));

	std::vector<opt> run = getopt(argc, argv);

	std::ofstream gnuplot("gnuplot.sh", std::ios::ate);

	for(unsigned int i(0); i < run.size(); ++i)
	{
		//PBTREE

		if(run[i].pbtree)
		{
			srand(run[i].seed);
			PbFunction pb(run[i].n, run[i].depthplus, run[i].depthmin);
			GeneratorIAIndFunction gen(run[i].n, run[i].maxsizetree, run[i].initdepth);
			Algogen<IndFunction> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);

			IndFunction s = algo.solve();

			if(run[i].graphalgogen)
			{
				srand(run[i].seed);
				Plot<IndFunction> graph(pb, run[i].out + "-algogen-graph.dat", run[i].out + "-gnuplot-algogen.sh");
				graph.add(algo, {"fitness","size","avsize","nbscal","nbequal","nbmax","nbmin","nbplus"});

				std::string settings;
				settings += "detph+ : "+std::to_string(run[i].depthplus)+" ";
				settings += "detphmin : "+std::to_string(run[i].depthmin)+" ";
				settings += "N : "+std::to_string(run[i].n)+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-algogen-graph.jpg", "fitness as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-algogen.sh" << std::endl;
			}

			if(run[i].graphmean)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(run[i].n);

				Plot<VectorBool> meanpb(pb);
				Plot<VectorBool> meansol(pb);

				meanpb.add(0., "F-obj");
				meansol.add(0., "F-ound");

				for(unsigned int j(0); j < run[i].meansize; ++j)
				{
					FixedSizeDescent<VectorBool> ls1(pb,genom);
					FixedSizeDescentComp<VectorBool> ls2(s.getFunction(), pb, genom);

					ls1.solve();
					ls2.solve();

					meanpb += ls1;
					meansol += ls2;
				}
				meanpb /= (float)run[i].meansize;
				meansol /= (float)run[i].meansize;

				FixedSizeILS<VectorBool> ls3(pb,genom,10000,15);
				VectorBool best = ls3.solve();



				Plot<VectorBool> graph(pb, run[i].out + "-mean-graph.dat", run[i].out + "-gnuplot-mean.sh");

				graph.add(meanpb);
				graph.add(meansol);
				graph.add(best.getScore(), "ILS");

				std::string settings;
				settings += "detph+ : "+std::to_string(run[i].depthplus)+" ";
				settings += "detphmin : "+std::to_string(run[i].depthmin)+" ";
				settings += "N : "+std::to_string(run[i].n)+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "meansize : "+std::to_string(run[i].meansize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-mean-graph.jpg", "mean fitness over " + std::to_string(run[i].meansize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-mean.sh" << std::endl;
			}

			if(run[i].graphagregate)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(run[i].n);

				Plot<VectorBool> graph(pb, run[i].out + "-agregate-graph.dat", run[i].out + "-gnuplot-agregate.sh");

				for(unsigned int j(0); j < run[i].agregatesize; ++j)
				{
					FixedSizeDescentComp<VectorBool> ls(s.getFunction(), pb, genom);

					ls.solve();

					graph.add(ls, {std::to_string(j)});
				}

				std::string settings;
				settings += "detph+ : "+std::to_string(run[i].depthplus)+" ";
				settings += "detphmin : "+std::to_string(run[i].depthmin)+" ";
				settings += "N : "+std::to_string(run[i].n)+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "agregatesize : "+std::to_string(run[i].agregatesize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-agregate-graph.jpg", "agretation of fitness of " + std::to_string(run[i].agregatesize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-agregate.sh" << std::endl;
			}

			if(run[i].Rgraphmean)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(run[i].n);

				Plot<VectorBool> meanpb(pb);
				Plot<VectorBool> meansol(pb);

				meanpb.add(0., "F-obj");
				meansol.add(0., "F-ound");

				for(unsigned int j(0); j < run[i].Rmeansize; ++j)
				{
					FixedSizeDescentComp<VectorBool> ls1(pb, s.getFunction(), genom);
					FixedSizeDescent<VectorBool> ls2(s.getFunction(), genom);

					ls1.solve();
					ls2.solve();

					meanpb += ls1;
					meansol += ls2;
				}
				meanpb /= (float)run[i].Rmeansize;
				meansol /= (float)run[i].Rmeansize;

				FixedSizeILS<VectorBool> ls3(s.getFunction(),genom,10000,run[i].n/5);
				VectorBool best = ls3.solve();



				Plot<VectorBool> graph(pb, run[i].out + "-Rmean-graph.dat", run[i].out + "-gnuplot-Rmean.sh");

				graph.add(meanpb);
				graph.add(meansol);
				graph.add(best.getScore(), "ILS");

				std::string settings;
				settings += "detph+ : "+std::to_string(run[i].depthplus)+" ";
				settings += "detphmin : "+std::to_string(run[i].depthmin)+" ";
				settings += "N : "+std::to_string(run[i].n)+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "Rmeansize : "+std::to_string(run[i].Rmeansize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-Rmean-graph.jpg", "mean fitness over " + std::to_string(run[i].Rmeansize) + " runs as a function of iterations in the F-ound space "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-Rmean.sh" << std::endl;
			}

			if(run[i].links)
			{
				std::ofstream out((run[i].out + "-links.dat").c_str());

				s.getFunction().varLinks(out);
				out << std::endl;
				s.getFunction().show(out);
			}

			if(run[i].correlation)
			{
				std::ofstream out1((run[i].out + "-correlation1.dat").c_str());
				std::ofstream out2((run[i].out + "-correlation2.dat").c_str());
				std::ofstream out3((run[i].out + "-correlation3.dat").c_str());
				std::ofstream plot((run[i].out + "-gnuplot-correlation.sh").c_str());

				plot << "set terminal jpeg size 2000,1000" << std::endl;
				plot << "set output \"" << run[i].out << "-correlation-graph.jpg" << "\"" << std::endl;
				plot << "set title \'Correlation between F-ound and F-obj\'" << std::endl;
				plot << "set xlabel \'F-ound\'" << std::endl;
				plot << "set ylabel \'F-obj\'" << std::endl;
				plot << "set key outside" << std::endl;
				plot << "plot \"" << run[i].out << "-correlation1.dat\" using 1:2 with points title \'random\',\\" << std::endl;
				plot << "\"" << run[i].out << "-correlation2.dat\" using 1:2 with points title \'LS on F-obj\',\\" << std::endl;
				plot << "\"" << run[i].out << "-correlation3.dat\" using 1:2 with points title \'LS on F-ound\'";

				GeneratorROOneMax genom(run[i].n);

				for(unsigned int i(0); i < 10000; ++i)
				{
					auto tmp = genom.generate();
					out1 << s.getFunction().evaluate(tmp) << " " << ((FunctionTree)pb).evaluate(tmp) << std::endl;
				}

				for(unsigned int i(0); i < 1000; ++i)
				{
					FixedSizeDescent<VectorBool> ls(pb, genom);
					auto tmp = ls.solve();

					out2 << s.getFunction().evaluate(tmp) << " " << ((FunctionTree)pb).evaluate(tmp) << std::endl;
				}

				for(unsigned int i(0); i < 1000; ++i)
				{
					FixedSizeDescent<VectorBool> ls(s.getFunction(), genom);
					auto tmp = ls.solve();

					out3 << s.getFunction().evaluate(tmp) << " " << ((FunctionTree)pb).evaluate(tmp) << std::endl;
				}

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-correlation.sh" << std::endl;
			}
		}

		//NK

		if(run[i].nk)
		{
			srand(run[i].seed);
			Nk pb(run[i].pbfile);
			GeneratorIAIndFunction gen(pb.getN(), run[i].maxsizetree, run[i].initdepth);
			Algogen<IndFunction> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);

			IndFunction s = algo.solve();

			if(run[i].graphalgogen)
			{
				srand(run[i].seed);
				Plot<IndFunction> graph(pb, run[i].out + "-algogen-graph.dat", run[i].out + "-gnuplot-algogen.sh");
				graph.add(algo, {"fitness","size","avsize","nbscal","nbequal","nbmax","nbmin","nbplus"});

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-algogen-graph.jpg", "fitness as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-algogen.sh" << std::endl;
			}

			if(run[i].graphmean)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(pb.getN());

				Plot<VectorBool> meanpb(pb);
				Plot<VectorBool> meansol(pb);

				meanpb.add(0., "F-obj");
				meansol.add(0., "F-ound");

				for(unsigned int j(0); j < run[i].meansize; ++j)
				{
					FixedSizeDescent<VectorBool> ls1(pb,genom);
					FixedSizeDescentComp<VectorBool> ls2(s.getFunction(), pb, genom);

					ls1.solve();
					ls2.solve();

					meanpb += ls1;
					meansol += ls2;
				}
				meanpb /= (float)run[i].meansize;
				meansol /= (float)run[i].meansize;

				FixedSizeILS<VectorBool> ls3(pb,genom,10000,15);
				VectorBool best = ls3.solve();



				Plot<VectorBool> graph(pb, run[i].out + "-mean-graph.dat", run[i].out + "-gnuplot-mean.sh");

				graph.add(meanpb);
				graph.add(meansol);
				graph.add(best.getScore(), "ILS");

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "meansize : "+std::to_string(run[i].meansize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-mean-graph.jpg", "mean fitness over " + std::to_string(run[i].meansize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-mean.sh" << std::endl;
			}

			if(run[i].graphagregate)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(pb.getN());

				Plot<VectorBool> graph(pb, run[i].out + "-agregate-graph.dat", run[i].out + "-gnuplot-agregate.sh");

				for(unsigned int j(0); j < run[i].agregatesize; ++j)
				{
					FixedSizeDescentComp<VectorBool> ls(s.getFunction(), pb, genom);

					ls.solve();

					graph.add(ls, {std::to_string(j)});
				}

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "agregatesize : "+std::to_string(run[i].agregatesize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-agregate-graph.jpg", "agretation of fitness of " + std::to_string(run[i].agregatesize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-agregate.sh" << std::endl;
			}

			if(run[i].Rgraphmean)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(pb.getN());

				Plot<VectorBool> meanpb(pb);
				Plot<VectorBool> meansol(pb);

				meanpb.add(0., "F-obj");
				meansol.add(0., "F-ound");

				for(unsigned int j(0); j < run[i].Rmeansize; ++j)
				{
					FixedSizeDescentComp<VectorBool> ls1(pb, s.getFunction(), genom);
					FixedSizeDescent<VectorBool> ls2(s.getFunction(), genom);

					ls1.solve();
					ls2.solve();

					meanpb += ls1;
					meansol += ls2;
				}
				meanpb /= (float)run[i].Rmeansize;
				meansol /= (float)run[i].Rmeansize;

				FixedSizeILS<VectorBool> ls3(s.getFunction(),genom,10000,15);
				VectorBool best = ls3.solve();



				Plot<VectorBool> graph(pb, run[i].out + "-Rmean-graph.dat", run[i].out + "-gnuplot-Rmean.sh");

				graph.add(meanpb);
				graph.add(meansol);
				graph.add(best.getScore(), "ILS");

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "Rmeansize : "+std::to_string(run[i].Rmeansize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-Rmean-graph.jpg", "mean fitness over " + std::to_string(run[i].Rmeansize) + " runs as a function of iterations in the F-ound space "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-Rmean.sh" << std::endl;
			}

			if(run[i].links)
			{
				std::ofstream out((run[i].out + "-links.dat").c_str());

				s.getFunction().varLinks(out);
				out << std::endl;
				s.getFunction().show(out);
			}

			if(run[i].correlation)
			{
				std::ofstream out1((run[i].out + "-correlation1.dat").c_str());
				std::ofstream out2((run[i].out + "-correlation2.dat").c_str());
				std::ofstream out3((run[i].out + "-correlation3.dat").c_str());
				std::ofstream plot((run[i].out + "-gnuplot-correlation.sh").c_str());

				plot << "set terminal jpeg size 2000,1000" << std::endl;
				plot << "set output \"" << run[i].out << "-correlation-graph.jpg" << "\"" << std::endl;
				plot << "set title \'Correlation between F-ound and F-obj\'" << std::endl;
				plot << "set xlabel \'F-ound\'" << std::endl;
				plot << "set ylabel \'F-obj\'" << std::endl;
				plot << "set key outside" << std::endl;
				plot << "plot \"" << run[i].out << "-correlation1.dat\" using 1:2 with points title \'random\',\\" << std::endl;
				plot << "\"" << run[i].out << "-correlation2.dat\" using 1:2 with points title \'LS on F-obj\',\\" << std::endl;
				plot << "\"" << run[i].out << "-correlation3.dat\" using 1:2 with points title \'LS on F-ound\'";

				GeneratorROOneMax genom(pb.getN());

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

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-correlation.sh" << std::endl;
			}
		}

		//MAXSAT

		if(run[i].maxsat)
		{
			srand(run[i].seed);
			MaxSat pb(run[i].pbfile);
			GeneratorIAIndFunction gen(pb.getN(), run[i].maxsizetree, run[i].initdepth);
			Algogen<IndFunction> algo(pb,gen,run[i].nbind,run[i].nbit,run[i].cross,run[i].copy,run[i].mutation,run[i].newop);

			IndFunction s = algo.solve();

			if(run[i].graphalgogen)
			{
				srand(run[i].seed);
				Plot<IndFunction> graph(pb, run[i].out + "-algogen-graph.dat", run[i].out + "-gnuplot-algogen.sh");
				graph.add(algo, {"fitness","size","avsize","nbscal","nbequal","nbmax","nbmin","nbplus"});

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-algogen-graph.jpg", "fitness as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-algogen.sh" << std::endl;
			}

			if(run[i].graphmean)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(pb.getN());

				Plot<VectorBool> meanpb(pb);
				Plot<VectorBool> meansol(pb);

				meanpb.add(0., "F-obj");
				meansol.add(0., "F-ound");

				for(unsigned int j(0); j < run[i].meansize; ++j)
				{
					FixedSizeDescent<VectorBool> ls1(pb,genom);
					FixedSizeDescentComp<VectorBool> ls2(s.getFunction(), pb, genom);

					ls1.solve();
					ls2.solve();

					meanpb += ls1;
					meansol += ls2;
				}
				meanpb /= (float)run[i].meansize;
				meansol /= (float)run[i].meansize;

				FixedSizeILS<VectorBool> ls3(pb,genom,10000,run[i].n/5);
				VectorBool best = ls3.solve();



				Plot<VectorBool> graph(pb, run[i].out + "-mean-graph.dat", run[i].out + "-gnuplot-mean.sh");

				graph.add(meanpb);
				graph.add(meansol);
				graph.add(best.getScore(), "ILS");

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "meansize : "+std::to_string(run[i].meansize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-mean-graph.jpg", "mean fitness over " + std::to_string(run[i].meansize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-mean.sh" << std::endl;
			}

			if(run[i].graphagregate)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(pb.getN());

				Plot<VectorBool> graph(pb, run[i].out + "-agregate-graph.dat", run[i].out + "-gnuplot-agregate.sh");

				for(unsigned int j(0); j < run[i].agregatesize; ++j)
				{
					FixedSizeDescentComp<VectorBool> ls(s.getFunction(), pb, genom);

					ls.solve();

					graph.add(ls, {std::to_string(j)});
				}

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "agregatesize : "+std::to_string(run[i].agregatesize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-agregate-graph.jpg", "agretation of fitness of " + std::to_string(run[i].agregatesize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-agregate.sh" << std::endl;
			}

			if(run[i].Rgraphmean)
			{
				srand(run[i].seed);
				GeneratorROOneMax genom(pb.getN());

				Plot<VectorBool> meanpb(pb);
				Plot<VectorBool> meansol(pb);

				meanpb.add(0., "F-obj");
				meansol.add(0., "F-ound");

				for(unsigned int j(0); j < run[i].Rmeansize; ++j)
				{
					FixedSizeDescentComp<VectorBool> ls1(pb, s.getFunction(), genom);
					FixedSizeDescent<VectorBool> ls2(s.getFunction(), genom);

					ls1.solve();
					ls2.solve();

					meanpb += ls1;
					meansol += ls2;
				}
				meanpb /= (float)run[i].Rmeansize;
				meansol /= (float)run[i].Rmeansize;

				FixedSizeILS<VectorBool> ls3(s.getFunction(),genom,10000,run[i].n/5);
				VectorBool best = ls3.solve();



				Plot<VectorBool> graph(pb, run[i].out + "-Rmean-graph.dat", run[i].out + "-gnuplot-Rmean.sh");

				graph.add(meanpb);
				graph.add(meansol);
				graph.add(best.getScore(), "ILS");

				std::string settings;
				settings += "file : "+run[i].pbfile+" ";
				settings += "N : "+std::to_string(pb.getN())+" ";
				settings += "maxsize : "+std::to_string(run[i].maxsizetree)+" ";
				settings += "initdepth : "+std::to_string(run[i].initdepth)+" ";
				settings += "nbind : "+std::to_string(run[i].nbind)+" ";
				settings += "cross : "+std::to_string(run[i].cross)+" ";
				settings += "copy : "+std::to_string(run[i].copy)+" ";
				settings += "mutation : "+std::to_string(run[i].mutation)+" ";
				settings += "newop : "+std::to_string(run[i].newop)+" ";
				settings += "Rmeansize : "+std::to_string(run[i].Rmeansize)+" ";
				settings += "seed : "+std::to_string(run[i].seed)+" ";
				settings += "version : "+std::to_string(VERSION);

				graph.save(run[i].out + "-Rmean-graph.jpg", "mean fitness over " + std::to_string(run[i].Rmeansize) + " runs as a function of iterations in the F-ound space "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-Rmean.sh" << std::endl;
			}

			if(run[i].links)
			{
				std::ofstream out((run[i].out + "-links.dat").c_str());

				s.getFunction().varLinks(out);
				out << std::endl;
				s.getFunction().show(out);
			}

			if(run[i].correlation)
			{
				std::ofstream out1((run[i].out + "-correlation1.dat").c_str());
				std::ofstream out2((run[i].out + "-correlation2.dat").c_str());
				std::ofstream out3((run[i].out + "-correlation3.dat").c_str());
				std::ofstream plot((run[i].out + "-gnuplot-correlation.sh").c_str());

				plot << "set terminal jpeg size 2000,1000" << std::endl;
				plot << "set output \"" << run[i].out << "-correlation-graph.jpg" << "\"" << std::endl;
				plot << "set title \'Correlation between F-ound and F-obj\'" << std::endl;
				plot << "set xlabel \'F-ound\'" << std::endl;
				plot << "set ylabel \'F-obj\'" << std::endl;
				plot << "set key outside" << std::endl;
				plot << "plot \"" << run[i].out << "-correlation1.dat\" using 1:2 with points title \'random\',\\" << std::endl;
				plot << "\"" << run[i].out << "-correlation2.dat\" using 1:2 with points title \'LS on F-obj\',\\" << std::endl;
				plot << "\"" << run[i].out << "-correlation3.dat\" using 1:2 with points title \'LS on F-ound\'";

				GeneratorROOneMax genom(pb.getN());

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

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-correlation.sh" << std::endl;
			}
		}
	}

	return 0;
}


