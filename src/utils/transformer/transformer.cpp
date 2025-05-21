/*
 * Transformer.cpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#include "transformer.hpp"
#include <sys/stat.h>
#include <libxml/parser.h>
#include <libxml/xpathInternals.h>
#include <omp.h>

Transformer::Transformer() {
	parser = NULL;
};

Transformer::Transformer(Parser *parser) {
	this->parser = parser;
	this->initOrthoXMLFiles();
};


bool Transformer::existFile (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

void Transformer::initOrthoXMLFiles() {
	this->orthoXMLFiles.clear();
	for (int i = 0; i < this->parser->getNumPartitions(); i++) {
		this->orthoXMLFiles.push_back(OrthoXMLFileData());
	}
}

void Transformer::setParser(Parser *parser) {
	this->parser = parser;
	this->initOrthoXMLFiles();
}

Parser* Transformer::getParser() {
	return this->parser;
}

int Transformer::checkSplitting(int numPartitions, int numGroups, int groupsPerFile) {
	if (groupsPerFile == 0) {
		std::cout << "Error: The number of 'orthologGroup' clauses (" << numGroups << ") in file "
				<< this->parser->getInputFile() << " cannot be split into " << numPartitions
				<< " files" << std::endl;
		return 0;
	}
	return 1;
}


int Transformer::splitOMAInput() {
	int numPartitions = this->parser->getNumPartitions();
	int numGroups = this->parser->getNumOrthoGroupsNodes();
	int groupsPerFile = numGroups/numPartitions; //Num. groups per file
	if (!this->checkSplitting(numPartitions, numGroups, groupsPerFile)) {
		return 0;
	}

	#ifdef DEBUG
		std::cout << "Groups per file: " << groupsPerFile <<  std::endl;
	#endif

	if (!this->searchGenesFromGroups(numPartitions, numGroups,
			groupsPerFile, this->parser->getNumThreads()))
		return 0;

	return 1;
}

int Transformer::searchGenesFromGroups(int numPartitions, int numGroups, int groupsPerFile, int numThreads) {
	int numFile = 0, pos = 0, offset = 0;
	int remainingElements = numGroups%numPartitions;
	std::vector<xmlNodeOrthoGroups>& orthoGroupsNodes = this->parser->getOrthoGroupsNodes();
	std::vector<xmlNodeSpecies>& speciesNodes = this->parser->getSpeciesNodes();
	#pragma omp parallel for private(numFile, pos, offset) \
	shared(numPartitions, numGroups, groupsPerFile, remainingElements, orthoGroupsNodes, speciesNodes) \
	num_threads(numThreads)
	for (numFile = 0; numFile < numPartitions; numFile++) {
		offset = (numFile*groupsPerFile);
		for (register int group = 0; group < groupsPerFile; group++) {
			pos = offset + group;
			if (!this->matchGroupWithSpecies(numFile, orthoGroupsNodes, speciesNodes, pos))
				exit(0); //return 0 if no parallel region
		}
		//if remainingElements and lastFile -> !dump
		if ((numFile != numPartitions-1) || (remainingElements == 0 && numFile == numPartitions-1)) {
			if (!this->dumpData(numFile)) {
				exit(0); //return 0 if no parallel region
			}
		}
	}

	//Iterate the last groups which didn't fit in the last chunk
	//This happens if numGroups/numPartitions is odd
	if (remainingElements) {
		numFile = numPartitions-1; //Last file
		offset = (numFile*groupsPerFile)+groupsPerFile; //New offset -> Jump to the last position
		for (int chunk = 0; chunk < remainingElements; chunk++) {
			pos = offset + chunk;
			this->matchGroupWithSpecies(numFile, orthoGroupsNodes, speciesNodes, pos);
		}
		if (!this->dumpData(numFile)) {
			return 0;
		}
	}

	#ifdef DEBUG
		for (unsigned int i = 0; i < this->orthoXMLFiles.size(); i++) {
			unsigned int sizeG = this->orthoXMLFiles[i].getNumOrthoGroupsNodes();
			unsigned int sizeS = this->orthoXMLFiles[i].getNumSpeciesNodes();
				std::cout << "File " << i << " has "
					<< sizeG << " groups with "
					<< sizeS << " species" << std::endl;
		}
	#endif

	return 1;
}

int Transformer::matchGroupWithSpecies(int numFile, std::vector<xmlNodeOrthoGroups>& orthoGroupsNodes,
		std::vector<xmlNodeSpecies>& speciesNodes, int groupPos) {
	std::vector<unsigned long>& ids = orthoGroupsNodes[groupPos].getIds();
	OrthoXMLFileData& currentFile = this->orthoXMLFiles[numFile];
	for(unsigned long  g_gene_id : ids) { //For each id of geneRef in this group 'groupPos'
		unsigned int numTries = 0;
		for(xmlNodeSpecies& species : speciesNodes) {
			unsigned long queried = species.getGene(g_gene_id);
			if (queried == 0) { numTries++; }
			else { // Found the gene 'g_gene_id' from group 'groupPos' in the xmlNodeSpecies 'species'
				currentFile.addSpeciesNode(species);
			}
		}
		//As many tries as species are? The gene_id in the orthoGroup doesn't exist -> ERROR
		if (numTries == this->parser->getNumSpeciesNodes()) {
			std::cout << "Error: geneRef = '" << g_gene_id << "' does not exist in any species. " <<
					"'orthologGroup' node line: " << xmlGetLineNo(orthoGroupsNodes[groupPos].getNode()) << std::endl;
			return 0;
		}
	}

	currentFile.addOrthoGroupsNode(orthoGroupsNodes[groupPos]);
	return 1;
}

int Transformer::createHierarchyToFile(xmlDocPtr *doc, int numFile) {
	OrthoXMLFileData orthoXMLFileData = this->orthoXMLFiles[numFile];
	xmlNodePtr orthoXMLNode = xmlCopyNode(this->parser->getOrthoXMLNode(), EXTRACT::ALL_DATA);
	// Create the document
	*doc = xmlNewDoc(BAD_CAST "1.0");
	xmlDocSetRootElement(*doc, orthoXMLNode);
	xmlNodePtr parent_node = orthoXMLNode;
	for (unsigned int i = 0; i < orthoXMLFileData.getNumSpeciesNodes(); i++) {
		xmlNodeSpeciesPtr speciesNode = orthoXMLFileData.getSpeciesNodes()[i];
		xmlAddChild(parent_node, xmlCopyNode(speciesNode->getNode(), EXTRACT::ALL_DATA));
	}
	xmlNodePtr groups = xmlNewChild(parent_node, NULL, BAD_CAST "groups", NULL);
	for (unsigned int i = 0; i < orthoXMLFileData.getNumOrthoGroupsNodes(); i++) {
		xmlNodeOrthoGroups orthoNode = orthoXMLFileData.getOrthoGroupsNodes()[i];
		xmlAddChild(groups, xmlCopyNode(orthoNode.getNode(), EXTRACT::ALL_DATA));
	}

	return 1;
}

void Transformer::freeHierarchy(xmlDocPtr *doc, int numFile) {
	xmlNodePtr root_node = xmlDocGetRootElement(*doc);
	xmlUnlinkNode(root_node);
	xmlNodePtr child = root_node->children;
	while (child) {
		xmlUnlinkNode(child);
		xmlFreeNodeList(child); //Frees siblings and children
		child = child->next;
	}
	xmlFreeNodeList(root_node);
	xmlFreeDoc(*doc);
}


void Transformer::freeData() {
	xmlFreeNode(this->parser->getOrthoXMLNode());
	for (int numFile = 0; numFile < this->parser->getNumPartitions(); numFile++) {
		OrthoXMLFileData orthoXMLFileData = this->orthoXMLFiles[numFile];
		for (unsigned int i = 0; i < orthoXMLFileData.getNumSpeciesNodes(); i++) {
			//xmlNodeSpecies speciesNode = orthoXMLFileData.getSpeciesNodes()[i];
			//xmlFreeNode(speciesNode.getNode());

		}
		for (unsigned int i = 0; i < orthoXMLFileData.getNumOrthoGroupsNodes(); i++) {
			//xmlNodeOrthoGroups orthoNode = orthoXMLFileData.getOrthoGroupsNodes()[i];
			//xmlFreeNode(orthoNode.getNode());
		}
	}
}

int Transformer::dumpData(int numFile) {
	//for (int numFile = 0; numFile < this->parser->getNumPartitions(); numFile++) {
	xmlDocPtr doc = NULL;
	this->createHierarchyToFile(&doc, numFile);
	std::string outputPrefix = "result_";
	std::string fileName = this->parser->getInputFile();
	size_t lastindex = fileName.find_last_of(".");
	std::string intput_file_no_ext = fileName.substr(0, lastindex); //Removing extension
	lastindex = fileName.find_last_of("/");
	std::string input_file_no_ext_trimmed = intput_file_no_ext.substr(lastindex+1); // Removing trailing path

	std::string pathOutputFile = this->parser->getOutputDir()+outputPrefix+
			input_file_no_ext_trimmed+"_"+std::to_string(numFile)+".orthoXML";

	#pragma omp critical
	{
		std::cout << "Dumping file " << numFile << " to " << pathOutputFile << std::endl;
	}

	xmlKeepBlanksDefault(0);
	xmlSaveFormatFileEnc(pathOutputFile.c_str(), doc, "UTF-8", 1);
	OrthoXMLFileData orthoXMLFileData = this->orthoXMLFiles[numFile];
	orthoXMLFileData.freeXmlOrthoGroupsNodes();
	this->freeHierarchy(&doc, numFile);
	//}

	return 1;
}
