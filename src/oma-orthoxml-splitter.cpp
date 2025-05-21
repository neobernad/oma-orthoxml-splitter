//============================================================================
// Name        : oma-orthoxml-splitter.cpp
// Author      : José Antonio Bernabé Díaz
// Version     : 0.0
// Copyright   : Universidad de Murcia
// Description : Splits a OMA file formmated with orthoXML standard into several files
//============================================================================

#include <iostream>
#include "utils/transformer/transformer.hpp"
#include <omp.h>

using namespace std;

int main(int argc, char **argv) {
	double total_time = omp_get_wtime();

	xmlInitParser();
	LIBXML_TEST_VERSION

	#ifdef DEBUG
		std::cout << "Calling parseArgs()..." << std::endl;
	#endif

	double parse_args_time = omp_get_wtime();
	Parser parser;
	if (!parser.parseArgs(argc, argv)) {
		return 0;
	}

	#ifdef DEBUG
		std::cout << "parseArgs() Ok! - Time elapsed: " << (omp_get_wtime()-parse_args_time)
		 << " secs" << std::endl << std::endl;
		std::cout << "Calling parseOMAInput()..." << std::endl;
	#endif

	double parse_time = omp_get_wtime();
	if (!parser.parseOMAInput()) {
		return 0;
	}

	#ifdef DEBUG
		std::cout << "parseOMAInput() Ok! - Time elapsed: " << (omp_get_wtime()-parse_time)
		 << " secs" << std::endl << std::endl;
		std::cout << "Calling splitOMAInput()..." << std::endl;
	#endif

	double split_time = omp_get_wtime();
	Transformer transformer(&parser);
	if (!transformer.splitOMAInput()) {
		return 0;
	}

	#ifdef DEBUG
		std::cout << "splitOMAInput() Ok! - Time elapsed: " <<
				(omp_get_wtime()-split_time) << " secs" << std::endl << std::endl;
	#endif

	xmlCleanupParser();

	std::cout << "Total time elapsed: " << (omp_get_wtime()-total_time) << " secs" << std::endl;

	return EXIT_SUCCESS;
}
