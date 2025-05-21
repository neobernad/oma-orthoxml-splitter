#include "argManager.hpp"

ArgManager::ArgManager()
{
  numPartitions = 1;
  numThreads = 1;
}

//Returns a const reference, but it cannot change the state of inputFile
const std::string& ArgManager::getInputFile() const {
	return inputFile;
}

void ArgManager::setInputFile(const std::string& _inputFile) {
	inputFile = _inputFile;
}

const std::string& ArgManager::getOutputDir() const {
	return outputDir;
}

void ArgManager::setOutputDir(const std::string& outputDir) {
	this->outputDir = outputDir;
}


int ArgManager::getNumPartitions() const {
	return numPartitions;
}

void ArgManager::setNumPartitions(int _numPartitions) {
	numPartitions = _numPartitions;
}

void ArgManager::setNumThreads(int numThreads) {
	this->numThreads = numThreads;
}

int ArgManager::getNumThreads() const {
	return numThreads;
}

bool ArgManager::isParallel() const {
	return numThreads > 1;
}
