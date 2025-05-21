/*
 * parser.hpp
 *
 *  Created on: 3 nov. 2017
 *      Author: Neo
 */

#ifndef UTILS_PARSER_HPP_
#define UTILS_PARSER_HPP_

#include <libxml/xpath.h>
#include <vector>
#include "../argManager/argManager.hpp"
#include "../../models/xmlNodeSpecies.hpp"
#include "../../models/xmlNodeOrthoGroups.hpp"

enum EXTRACT { ALL_DATA = 1, ONLY_NS_PROP = 2 };

class Parser {

	private:
		//Attributes
		ArgManager argManager;
		std::vector<xmlNodeOrthoGroups> orthoGroupsNodes;
		unsigned int numOrthoGroupsNodes;
		std::vector<xmlNodeSpecies> speciesNodes;
		unsigned int numSpeciesNodes;
		xmlNodePtr orthoXMLNode;
		//Functions
		void usage(const char *name);
		bool existsParameterInArgs(int argc, char **argv, const std::string& param);
		std::string getParameterInArgs(int argc, char **argv, const std::string& param);
		xmlXPathObjectPtr findNodes(const xmlChar* xpathExpr, xmlXPathContextPtr xpathCtx);
		void registerDocumentNSToCtx(xmlDocPtr input_doc, xmlXPathContextPtr ctx);
		int readOrthologGroups(xmlXPathContextPtr globalCtx);
		int readSpecies(xmlXPathContextPtr globalCtx);
		int checkNodeIntegrity(xmlNodePtr node, const std::string& node_name);
		void errorNoClause(xmlNodePtr node, const std::string& expected_str);
		unsigned long getFirstGeneId(xmlNodePtr species);
		void sortSpeciesVector();
	public:
		Parser();
		int dirExists(const char *path);
		ArgManager& getArgManager();
		std::string getInputFile();
		std::string getOutputDir();
		int getNumPartitions();
		int getNumThreads() const;
		bool isParallel() const;
		int parseArgs(int argc, char **argv);
		int parseOMAInput();
		void resetNS(xmlNodePtr node, xmlChar* href);
		xmlNodePtr getOrthoXMLNode();
		std::vector<xmlNodeOrthoGroups>& getOrthoGroupsNodes();
		unsigned int getNumOrthoGroupsNodes() const;
		std::vector<xmlNodeSpecies>& getSpeciesNodes();
		unsigned int getNumSpeciesNodes() const;
};


#endif /* UTILS_PARSER_HPP_ */
