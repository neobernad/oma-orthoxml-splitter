/*
 * Transformer.hpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#ifndef UTILS_TRANSFORMER_TRANSFORMER_HPP_
#define UTILS_TRANSFORMER_TRANSFORMER_HPP_

#include "../orthoXMLFileData/OrthoXMLFileData.hpp"
#include "../parser/parser.hpp"

class Transformer {

	private:
		Parser *parser;
		std::vector<OrthoXMLFileData> orthoXMLFiles;
		bool existFile (const std::string& name);
		void initOrthoXMLFiles();
		int searchGenesFromGroups(int numPartitions, int numGroups, int groupsPerFile, int numThreads);
		int matchGroupWithSpecies(int numFile, std::vector<xmlNodeOrthoGroups>& orthoGroupsNodes,
				std::vector<xmlNodeSpecies>& speciesNodes, int groupPos);
	public:
		Transformer();
		Transformer(Parser *parser);
		void setParser(Parser *parser);
		Parser* getParser();
		int checkSplitting(int numPartitions, int numGroups, int groupsPerFile);
		int splitOMAInput();
		int createHierarchyToFile(xmlDocPtr *doc, int numFile);
		void freeHierarchy(xmlDocPtr *doc, int numFile);
		void freeData();
		int dumpData(int numFile);


};



#endif /* UTILS_TRANSFORMER_TRANSFORMER_HPP_ */
