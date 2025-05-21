/*
 * xmlNodeSpecies.cpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#include "xmlNodeSpecies.hpp"
#include <iostream>
#include <libxml/parser.h>
#include <libxml/xpathInternals.h>


xmlNodeSpecies::xmlNodeSpecies() {
	gene_set_size = 0;
};

xmlNodeSpecies::~xmlNodeSpecies() {};

unsigned int xmlNodeSpecies::getGeneSetSize() const {
		return gene_set_size;
	}

bool operator==(const xmlNodeSpecies& lhs, const xmlNodeSpecies& rhs) {
	if (lhs.getGeneSetSize() != rhs.getGeneSetSize()) {
		return false;
	}

	hash_map rhsMap = rhs.getGeneSet();
	for (auto itLhs : lhs.getGeneSet()) {
		hash_map::const_iterator iter = rhsMap.find(itLhs.first);
		if (iter == rhsMap.end())
			return false;
	}
	return true;

}

//@Override
void xmlNodeSpecies::setNode(xmlNodePtr node) {
	xmlNodeCustom::setNode(node);
}

//@Override
int xmlNodeSpecies::importNodeInfo() {
	if (!checkNodeIntegrity(this->node, "species"))
		return 0;

	xmlNodePtr database = xmlFirstElementChild(this->node);
	if(!checkNodeIntegrity(database, "database"))
		return 0;

	xmlNodePtr genes = xmlFirstElementChild(database);
	if (!checkNodeIntegrity(genes, "genes"))
		return 0;

	xmlNodePtr nodeGene = genes->children;
	while (nodeGene) {
		if (nodeGene->type == XML_ELEMENT_NODE && (!xmlStrcmp(nodeGene->name, BAD_CAST "gene"))) {
			xmlChar *id = xmlGetProp(nodeGene, BAD_CAST "id");
			if (!id) {
				std::cerr << "Error: wrong formatted input. <gene> node in line " << xmlGetLineNo(nodeGene) << " has no 'id' property" << std::endl;
				return 0;
			}
			unsigned long gene_id = std::stoul((char*) id);
			gene_set[gene_id] = gene_id;
			gene_set_size++;
		}
		nodeGene = nodeGene->next;
	}

	return 1;
}

const hash_map& xmlNodeSpecies::getGeneSet() const {
	return gene_set;
}

unsigned long xmlNodeSpecies::getGene(unsigned long position) {
	hash_map::const_iterator iter = gene_set.find(position);
	if (iter == gene_set.end())
		return 0;
	else
		return iter->second;
}


