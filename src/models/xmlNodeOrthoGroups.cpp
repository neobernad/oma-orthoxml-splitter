/*
 * xmlNodeOrthoGroups.cpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#include "xmlNodeOrthoGroups.hpp"

xmlNodeOrthoGroups::xmlNodeOrthoGroups():xmlNodeCustom() {};

xmlNodeOrthoGroups::~xmlNodeOrthoGroups() {};

//@Override
void xmlNodeOrthoGroups::setNode(xmlNodePtr node) {
	xmlNodeCustom::setNode(node);
}

//orthologGroup nodes might contain nested orthologGroup or
//paralogGroup tags, hence the recursive search to keep searching over more nodes
int xmlNodeOrthoGroups::recursiveSearch(xmlNodePtr node) {
	if (!node)
		return 1; //Just go backwards if node == null
	int childResult = 1;
	while (node->next) {
		if (node->type == XML_ELEMENT_NODE && (!xmlStrcmp(node->name, BAD_CAST "geneRef"))) {
			xmlChar *id = xmlGetProp(node, BAD_CAST "id");
			if (!id) {
				std::cerr << "Error: wrong formatted input. <geneRef> node in line " << xmlGetLineNo(node) << " has no 'id' property" << std::endl;
				childResult = 0;
			}
			unsigned long geneId = std::stoul((char*) id);
			this->ids.push_back(geneId);
		}
		childResult = this->recursiveSearch(node->children); //Keep searching in their children nodes
		node = node->next;
	}
	return childResult;
}

//@Override --> Parent class calls importNodeInfo()
int xmlNodeOrthoGroups::importNodeInfo() {
	if (!checkNodeIntegrity(this->node, "orthologGroup"))
		return 0;
	int a = recursiveSearch(this->node->children);
	return a;
}

std::vector<unsigned long>& xmlNodeOrthoGroups::getIds() { return this->ids; }
