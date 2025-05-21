/*
 * argManager.hpp
 *
 *  Created on: 3 nov. 2017
 *      Author: Neo
 */

#ifndef UTILS_ARGMANAGER_ARGMANAGER_HPP_
#define UTILS_ARGMANAGER_ARGMANAGER_HPP_

#include <string>

class ArgManager {

	private:
		std::string inputFile;
		std::string outputDir;
		int numPartitions;
		int numThreads;
	public:
		ArgManager();
		const std::string& getInputFile() const;
		void setInputFile(const std::string& inputFile);
		const std::string& getOutputDir() const;
		void setOutputDir(const std::string& outputDir);
		int getNumPartitions() const;
		void setNumPartitions(int _numPartitions);
		void setNumThreads(int numThreads);
		int getNumThreads() const;
		bool isParallel() const;
};



#endif /* UTILS_ARGMANAGER_ARGMANAGER_HPP_ */
