/*
 * xmlNodeCustom.cpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#include "xmlNodeCustom.hpp"
#include <iostream>
#include <libxml/parser.h>
#include <libxml/xpathInternals.h>

xmlNodeCustom::xmlNodeCustom() {
	this->node = NULL;
}

xmlNodeCustom::~xmlNodeCustom() {};

xmlNodePtr xmlNodeCustom::getNode() {
	return this->node;
}

void xmlNodeCustom::setNode(xmlNodePtr node) {
	this->node = node;
	if (!importNodeInfo()) {
		std::cerr << "Exit " << EXIT_FAILURE << std::endl;
		exit(EXIT_FAILURE);
	}
}

void xmlNodeCustom::freeNode() {
	if (node != NULL) {
		xmlUnlinkNode(this->node);
		xmlFreeNode(this->node);
	}
}

int xmlNodeCustom::checkNodeIntegrity(xmlNodePtr node, const std::string& node_name) {
	// xmlStrcmp: equals (0), different (1)
	if (xmlStrcmp(node->name, BAD_CAST node_name.c_str())) {
		this->errorNoClause(node, node_name);
		return 0;
	}
	if (xmlChildElementCount(node) == 0) {
		this->errorNoClause(node, "");
		return 0;
	}
	return 1;
}

void xmlNodeCustom::errorNoClause(xmlNodePtr node, const std::string& expected_str) {
	if (!expected_str.empty())
		std::cerr << "Error: wrong formatted input. Expetected '" << expected_str << "' but found '" << node->name << "'"<< std::endl;
	else
		std::cerr << "Error: wrong formatted input. No children nodes in node '" << node->name << "' line: " << xmlGetLineNo(node)<< std::endl;
}
