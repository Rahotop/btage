#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "solver/all.hpp"
#include "opt.hpp"

int main(int argc, char **argv)
{
	srand(time(nullptr));

	std::vector<opt> run = getopt(argc, argv);

	std::ofstream gnuplot("gnuplot.sh");

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
				graph.add(algo, {"fitness"});

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
				settings += "seed : "+std::to_string(run[i].seed);

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

				FixedSizeILS<VectorBool> ls3(pb,genom,10000,run[i].n/5);
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
				settings += "seed : "+std::to_string(run[i].seed);

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
				settings += "seed : "+std::to_string(run[i].seed);

			graph.save(run[i].out + "-agregate-graph.jpg", "agretation of fitness of " + std::to_string(run[i].agregatesize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-agregate.sh" << std::endl;
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
				graph.add(algo, {"fitness"});

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
				settings += "seed : "+std::to_string(run[i].seed);

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
				settings += "seed : "+std::to_string(run[i].seed);

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
				settings += "seed : "+std::to_string(run[i].seed);

			graph.save(run[i].out + "-agregate-graph.jpg", "agretation of fitness of " + std::to_string(run[i].agregatesize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-agregate.sh" << std::endl;
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
				graph.add(algo, {"fitness"});

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
				settings += "seed : "+std::to_string(run[i].seed);

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
				settings += "seed : "+std::to_string(run[i].seed);

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
				settings += "seed : "+std::to_string(run[i].seed);

			graph.save(run[i].out + "-agregate-graph.jpg", "agretation of fitness of " + std::to_string(run[i].agregatesize) + " runs as a function of iterations "+settings);

				gnuplot << "gnuplot " << run[i].out << "-gnuplot-agregate.sh" << std::endl;
			}
		}
	}

	return 0;
}


