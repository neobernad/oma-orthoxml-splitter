/*
 * parser.cpp
 *
 *  Created on: 3 nov. 2017
 *      Author: Neo
 */

#include "parser.hpp"
#include <iostream>
#include <libxml/parser.h>
//#include <libxml/xpath.h> //Included in parser.hpp
#include <libxml/xpathInternals.h>
#include <algorithm> //sort
#include <sys/stat.h>
#include <omp.h>

const std::string XPATH_ORTHOXML = "/orthoXML";
const std::string XPATH_ORTH_GROUPS = XPATH_ORTHOXML + "/groups/orthologGroup";
const std::string XPATH_SPECIES = XPATH_ORTHOXML + "/species";

Parser::Parser() {
	this->numOrthoGroupsNodes = 0;
	this->numSpeciesNodes = 0;
	this->orthoXMLNode = NULL;
}

void Parser::usage(const char *name) {
    if (name != NULL) {
    	std::cout << "Usage: " << name << " -f input.orthoXML -n number_files [-o output_name] [-p]" << std::endl;
    	std::cout << "\t-f: An input file formatted in OrthoXML standard" << std::endl;
    	std::cout << "\t-n: Number of files to split the original input" << std::endl;
    	std::cout << "\t-o (optional): Output directory, default is 'output'" << std::endl;
    	std::cout << "\t-p (optional): Parallel execution" << std::endl;
    }
}

//Checks whether the string param is contained in the arguments
//i.e: test.exe -all -> existsParameterInArgs(argc,argv, "-all") - returns true
bool Parser::existsParameterInArgs(int argc, char **argv, const std::string& param) {
	for (int i = 1; i < argc; i++) {
		if (param == argv[i]) {
			return true;
		}
	}
	return false;
}

//Returns the value associated to the parameter "param".
//i.e: -f file.txt, "param" is "-f", and this funtion returns its value "file.txt"
std::string Parser::getParameterInArgs(int argc, char **argv, const std::string& param) {
	for (int i = 1; i < argc; i++) {
		if (param == argv[i]) { // Parameter found (i.e: -f, -i...)
			if (i + 1 < argc) { // Making sure we aren't at the end of argv!
				return argv[i + 1];
			}
		}
	}
	return std::string(); //Parameter not found :-(, empty string
}

int Parser::dirExists(const char *path) {
    struct stat info;

    if(stat( path, &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}

//Returns a reference that can alter argManager
ArgManager& Parser::getArgManager() {
	return argManager;
}

std::string Parser::getInputFile() {
	return argManager.getInputFile();
}

std::string Parser::getOutputDir() {
	return argManager.getOutputDir();
}

int Parser::getNumPartitions() {
	return argManager.getNumPartitions();
}

int Parser::getNumThreads() const {
	return argManager.getNumThreads();
}

bool Parser::isParallel() const {
	return argManager.isParallel();
}

int Parser::parseArgs(int argc, char **argv) {
	if (argc == 1) {
		std::cout << "Error: wrong number of arguments.\n" << std::endl;
		usage(argv[0]);
		return 0;
	}

	// INPUT DATA (-f)
	std::string inputFile = getParameterInArgs(argc, argv, "-f");
	if (inputFile.empty()) {
		std::cout << "Error: specify a valid input file" << std::endl;
		usage(argv[0]);
		return 0;
	}

	argManager.setInputFile(inputFile);

	// OUTPUT FILE (-o)
	std::string outputDir;
	if (!existsParameterInArgs(argc, argv, "-o")) {
		outputDir = "output/";
	} else {
		outputDir = getParameterInArgs(argc, argv, "-o");
		if (outputDir.empty()) {
			std::cout << "Error: specify a valid output file" << std::endl;
			usage(argv[0]);
			return 0;
		}
	}

	if (!dirExists(outputDir.c_str())) {
		const int dir_err = mkdir(outputDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (dir_err == -1) {
			std::cerr << "Error creating directory \'" + outputDir + "\'." << std::endl ;
			exit(1);
		}
	} else {
		std::cout << "There is already a directory called \'" + outputDir + "\'" << std::endl;
		std::cout << "Please, rename it or specify another output directory with '-o' parameter." << std::endl ;
		//exit(1);
	}

	argManager.setOutputDir(outputDir);

	// Number of files to be split (-n)
	std::string numPartitions_str = getParameterInArgs(argc, argv, "-n");
	if (numPartitions_str.empty()) {
		std::cout << "Error: specify a valid number of partitions to split the data" << std::endl;
		usage(argv[0]);
		return 0;
	}
	int numPartitions = std::stoi(numPartitions_str);
	if (numPartitions <= 1) {
		std::cout << "Error: specify a positive number (>1) of partitions" << std::endl;
		return 0;
	}

	argManager.setNumPartitions(numPartitions);
	// Parallel execution (-p)
	int numThreads = 1;
	std::string numThreads_str = getParameterInArgs(argc, argv, "-p");
	if (numThreads_str.empty()) {
		if (existsParameterInArgs(argc, argv, "-p")) {
			//Are there more threads than files to be split?
			#pragma omp parallel
			{
				#pragma omp single
				{
					numThreads = omp_get_num_threads();
					if (numThreads > numPartitions) {
						numThreads = numPartitions;
						std::cout << "Warning: There are more threads than files to be split,"
								" setting thread count to: " << numPartitions << std::endl;
					}
				}
			}
		}
	} else {
		int numThreadsUsr = std::stoi(numThreads_str);
		if (numThreadsUsr <= 1) { // Num. of threads less than 1?
			std::cout << "Error: specify a positive number (>1) of threads" << std::endl;
			return 0;
		} else { //Are there more threads than files to be split? Repeated code... :-(
			#pragma omp parallel
			{
				#pragma omp single
				{
					numThreads = omp_get_num_threads();
					if (numThreads > numPartitions) {
						if (numThreadsUsr < numPartitions)
							numThreads = numThreadsUsr;
						else
							numThreads = numPartitions;
						std::cout << "Warning: There are more threads than files to be split,"
							" setting thread count to: " << numPartitions << std::endl;
					} else if ((numThreadsUsr < numThreads) && (numThreadsUsr < numPartitions)) { //Avoiding more threads than the CPU actually has
						numThreads = numThreadsUsr;
						std::cout << "Warning: There are more threads than your CPU holds,"
							" setting thread count to: " << numThreads << std::endl;
					} else {
						std::cout << "Warning: There are more threads than your CPU can bear,"
							" setting thread count to: " << numThreads << std::endl;
					}
				}
			}
		}
	}

	argManager.setNumThreads(numThreads);

	std::cout << "Input file: " << argManager.getInputFile() << std::endl;
	std::cout << "Number of partitions: " << argManager.getNumPartitions() << std::endl;
	std::cout << "Parallel execution?: ";
	if (argManager.isParallel())
		std::cout << "Yes - Thread count: " << argManager.getNumThreads() << std::endl;
	else
		std::cout << "No" << std::endl;

	return 1;
}

std::vector<xmlNodeOrthoGroups>& Parser::getOrthoGroupsNodes() {
	return this->orthoGroupsNodes;
}

unsigned int Parser::getNumOrthoGroupsNodes() const {
		return this->numOrthoGroupsNodes;
}

std::vector<xmlNodeSpecies>& Parser::getSpeciesNodes() {
	return this->speciesNodes;
}

unsigned int Parser::getNumSpeciesNodes() const {
		return this->numSpeciesNodes;
}

xmlXPathObjectPtr Parser::findNodes(const xmlChar* xpathExpr, xmlXPathContextPtr xpathCtx) {
    xmlXPathObjectPtr xpathObj;
	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
	if(xpathObj == NULL || !(xpathObj->nodesetval && xpathObj->nodesetval->nodeTab)) {
		std::cerr << "Warning: unable to evaluate XPath expression " << xpathExpr << std::endl;
		return NULL;
	}
	return xpathObj; //Remember to free xpathObj after its usage
}

//Resets the namespace of the current node, this occurs when the
//default namespace of the file doesn't have a prefix.
void Parser::resetNS(xmlNodePtr node, xmlChar* href) {
	if (node->children) {
		xmlNodePtr child = node->children;
		while (child) {
			resetNS(child, href);
			child = child->next;
		}
	}
	xmlNsPtr ns = node->ns;
	while (ns) {
		// Pick the right nodes with href!
		if (!xmlStrcmp(ns->href, href)) {
			xmlSetNs(node, NULL);
		}
		ns = ns->next;
	}
}

void Parser::registerDocumentNSToCtx(xmlDocPtr input_doc, xmlXPathContextPtr ctx) {
	xmlNodePtr root = xmlDocGetRootElement(input_doc);
	if (root == NULL) {
		std::cerr << "Error: empty document registerInputNsToCtx()" << input_doc->URL << std::endl;
		return;
	}
	xmlNsPtr ns = root->ns;
	while (ns != NULL) {
		std::string prefix, href;
		// Namespaces with no prefix (==NULL) will cause errors in Xpath evaluations.
		// Therefore, remove them with resetNS()
		if (ns->prefix == NULL) {
			href = (char*) ns->href;
			xmlSetNs(root, NULL);
			xmlNodePtr node = root->children;
			std::cout << "Info: The file \"" << input_doc->URL << "\" has no prefix for its default namespace! ("
					<< href << ")" << std::endl;
			while (node) {
				resetNS(node, BAD_CAST href.c_str());
				node = node->next;
			}
		} else {
			prefix = (char*) ns->prefix;
			href = (char*) ns->href;
			xmlXPathRegisterNs(ctx, BAD_CAST prefix.c_str(), BAD_CAST href.c_str());
		}
		ns = ns->next;
	}
}

int Parser::parseOMAInput() {
	xmlDocPtr input_doc;

	/* Load XML document */
	input_doc = xmlReadFile(argManager.getInputFile().c_str(), NULL, XML_PARSE_BIG_LINES);
	//input_doc = xmlParseFile(argManager.getInputFile().c_str());
	if (input_doc == NULL) {
		std::cout << "Error: unable to parse file \"" << argManager.getInputFile() << std::endl;
		return 0;
	}

	xmlXPathContextPtr globalCtx = xmlXPathNewContext(input_doc);
	if(globalCtx == NULL) {
		std::cerr << "Error: unable to create new XPath context in " << input_doc->URL << std::endl;
		xmlXPathFreeContext(globalCtx);
		xmlFreeDoc(input_doc);
		return 0;
	}

	//Register the default namespaces of the input file into the context that will be used to make queries
	this->registerDocumentNSToCtx(input_doc, globalCtx);

	//Retrieve the orthoXML root node, it's needed for dumping the correct output
	xmlXPathObjectPtr xpathQuery = this->findNodes((xmlChar*) XPATH_ORTHOXML.c_str(), globalCtx);
	if (xpathQuery == NULL || xpathQuery->nodesetval->nodeTab == NULL) {
		std::cerr << "Error: unable to find '" << XPATH_ORTHOXML << "' tag in file " << globalCtx->doc->URL << std::endl;
		return 0;
	}

	int numNodes = xpathQuery->nodesetval->nodeNr;
	if (numNodes != 1) {
		std::cerr << "Error: Multiple " << XPATH_ORTHOXML << " found in " << globalCtx->doc->URL << std::endl;
		return 0;
	}
	xmlNodePtr *orthoXMLNodeArray = xpathQuery->nodesetval->nodeTab;
	this->orthoXMLNode = xmlCopyNode(orthoXMLNodeArray[0], EXTRACT::ONLY_NS_PROP);

	xmlXPathSetContextNode(xmlDocGetRootElement(input_doc), globalCtx);
	xmlXPathFreeObject(xpathQuery);
	if (!this->readOrthologGroups(globalCtx)) {
		return 0;
	}


	//Point to the root node again for the next XPath queries
	xmlXPathSetContextNode(xmlDocGetRootElement(input_doc), globalCtx);

	if (!this->readSpecies(globalCtx)) {
		return 0;
	}

	xmlXPathFreeContext(globalCtx);
	xmlFreeDoc(input_doc);
	return 1;
}

xmlNodePtr Parser::getOrthoXMLNode() {
	return this->orthoXMLNode;
}

/*void Parser::sortSpeciesVector() {
	std::sort( speciesNodes.begin(), speciesNodes.end(), []( const xmlNodeSpecies& lhs, const xmlNodeSpecies& rhs ) {
	   return lhs.getFirstGene() < rhs.getFirstGene();
	});
}*/

int Parser::readSpecies(xmlXPathContextPtr globalCtx) {
	xmlXPathObjectPtr xpathQuery = this->findNodes((xmlChar*) XPATH_SPECIES.c_str(), globalCtx);
	if (xpathQuery == NULL) {
		std::cerr << "Error: unable to find 'species' tags in file " << globalCtx->doc->URL << std::endl;
		return 0;
	}

	int numNodes = xpathQuery->nodesetval->nodeNr;
	xmlNodePtr *nodeArray = xpathQuery->nodesetval->nodeTab;


	//Ordered sorting pls
	for (int i = 0; i < numNodes; i++) {
		xmlNodeSpecies species_node;
		species_node.setNode(xmlCopyNode(nodeArray[i], EXTRACT::ALL_DATA));
		speciesNodes.push_back(species_node);
		numSpeciesNodes++;
	}

	std::cout << "Number of species: " << numSpeciesNodes << std::endl;

	//this->sortSpeciesVector(); //Now we use a map not a vector

	xmlXPathFreeObject(xpathQuery); //Clear xpathQuery obj, but doesn't free the nodes in nodesetval, which is good

	return 1;
}

int Parser::readOrthologGroups(xmlXPathContextPtr globalCtx) {
	xmlXPathObjectPtr xpathQuery = this->findNodes((xmlChar*) XPATH_ORTH_GROUPS.c_str(), globalCtx);
	if (xpathQuery == NULL) {
		std::cerr << "Error: unable to find 'orthologGroup' tags in file " << globalCtx->doc->URL << std::endl;
		return 0;
	}

	int numNodes = xpathQuery->nodesetval->nodeNr;
	xmlNodePtr *nodeArray = xpathQuery->nodesetval->nodeTab;

	for (int i = 0; i < numNodes; i++) {
		xmlNodeOrthoGroups orthoGroup;
		orthoGroup.setNode(xmlCopyNode(nodeArray[i], EXTRACT::ALL_DATA));
		orthoGroupsNodes.push_back(orthoGroup);
		numOrthoGroupsNodes++;
	}

	std::cout << "Number of orthologGroup: " << numOrthoGroupsNodes << std::endl;

	/*for (int i = 0; i < numNodes; i++) {
		std::cout << i << std::endl;
		std::vector<unsigned long> &vector = orthoGroupsNodes[i].getIds();
		for (int j = 0; j < vector.size(); j++) {
			std::cout << "\t geneRef= " <<  vector[j] << std::endl;
		}
		std::cout << "------" << std::endl;
	}*/

	xmlXPathFreeObject(xpathQuery); //Clear xpathQuery obj, but doesn't free the nodes in nodesetval, which is good

	return 1;
}


