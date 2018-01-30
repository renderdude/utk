#include "../samplers/SamplerNiederreiter.hpp"
#include "../parameters/ParamParser_getopt.hpp"
#include "../io/fileIO.hpp"
#include <chrono>

#include "runSampler.hpp"

using namespace utk;

typedef uint T;
#define D 2
typedef Point<D, T> P;
typedef SamplerNiederreiter S;

int main(int argc, char** argv)
{
	ParamParser_getopt parser;
	S sampler;
	
	int basis = 3;
	//PARSE PARAM
	initParserSampler(&parser);
	parser.addShortOption('b', &basis, 1, assignInt, displayInt, (char*)"<uint=3>\tBasis", (char*)"Basis");
	
	//PARSE PARAM
	initParserSampler(&parser);
	//PARSING
	parser.parse(argc, argv);
	
	if(!dealParamParserSampler(&parser))
		return 0;
	
	sampler.setBasis(basis);
	
	PointsetWriter<D, T, P> writer;
	writer.open(param_output.c_str());
	while(param_nbrealisations>0)
	{
		//SAMPLE
		Pointset<D, T, P> pts;
		
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		if(!sampler.generateSamples<D, T, P>(pts, param_nbsamples))
			return 1;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		if(param_verbose)
			std::cout << std::fixed << std::setprecision(5) << "Generated " << pts.size() << " samples in " << time_span.count() << " secs" << std::endl;
		
		//WRITE
		writer.writePointset(pts);
		param_nbrealisations--;
	}
	writer.close();
	
	return 0;
}