/*
 * OrthoXMLFile.cpp
 *
 *  Created on: 10 nov. 2017
 *      Author: Neo
 */

#include "OrthoXMLFileData.hpp"

OrthoXMLFileData::OrthoXMLFileData() {
	this->numOrthoGroupsNodes = 0;
	this->numSpeciesNodes = 0;
};

OrthoXMLFileData::~OrthoXMLFileData() {};

void OrthoXMLFileData::freeXmlOrthoGroupsNodes() {
	for (unsigned int i = 0; i < this->orthoGroupsNodes.size(); i++) {
		xmlNodeOrthoGroups& orthoNode = this->orthoGroupsNodes[i];
		xmlFreeNode(orthoNode.getNode());
	}
}

void OrthoXMLFileData::freeXmlSpeciesNodes() {
	for (unsigned int i = 0; i < this->speciesNodes.size(); i++) {
		xmlNodeSpeciesPtr speciesNode = this->speciesNodes[i];
		xmlFreeNode(speciesNode->getNode());
	}
}

std::vector<xmlNodeOrthoGroups>& OrthoXMLFileData::getOrthoGroupsNodes() {
	return this->orthoGroupsNodes;
}

unsigned int OrthoXMLFileData::getNumOrthoGroupsNodes() const {
	return this->numOrthoGroupsNodes;
}

std::vector<xmlNodeSpeciesPtr>& OrthoXMLFileData::getSpeciesNodes() {
	return this->speciesNodes;
}

unsigned int OrthoXMLFileData::getNumSpeciesNodes() const {
		return this->numSpeciesNodes;
}

void OrthoXMLFileData::addOrthoGroupsNode(xmlNodeOrthoGroups &orthoGroupNode) {
	this->orthoGroupsNodes.push_back(orthoGroupNode);
	this->numOrthoGroupsNodes++;
}

// This method does not add repeated xmlNodeSpecies
void OrthoXMLFileData::addSpeciesNode(xmlNodeSpecies &speciesNode) {
	for (unsigned int i = 0; i < numSpeciesNodes; i++) {
		xmlNodeSpeciesPtr currentSpeciesNode = speciesNodes[i];
		if (*currentSpeciesNode == speciesNode) {
			return;
		}
	}
	this->speciesNodes.push_back(&speciesNode);
	this->numSpeciesNodes++;
}
