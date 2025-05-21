/*
 * OrthoXMLFile.hpp
 *
 *  Created on: 10 nov. 2017
 *      Author: Neo
 */

#ifndef UTILS_ORTHOXMLFILEDATA_ORTHOXMLFILEDATA_HPP_
#define UTILS_ORTHOXMLFILEDATA_ORTHOXMLFILEDATA_HPP_

#include <libxml/xpath.h>
#include <vector>
#include "../../models/xmlNodeSpecies.hpp"
#include "../../models/xmlNodeOrthoGroups.hpp"

class OrthoXMLFileData {

	private:
		std::vector<xmlNodeSpeciesPtr> speciesNodes;
		unsigned int numSpeciesNodes;
		std::vector<xmlNodeOrthoGroups> orthoGroupsNodes;
		unsigned int numOrthoGroupsNodes;
	public:
		OrthoXMLFileData();
		~OrthoXMLFileData();
		void freeXmlOrthoGroupsNodes();
		void freeXmlSpeciesNodes();
		void addOrthoGroupsNode(xmlNodeOrthoGroups &orthoGroupNode);
		std::vector<xmlNodeOrthoGroups>& getOrthoGroupsNodes();
		unsigned int getNumOrthoGroupsNodes() const;
		void addSpeciesNode(xmlNodeSpecies &speciesNode);
		std::vector<xmlNodeSpeciesPtr>& getSpeciesNodes();
		unsigned int getNumSpeciesNodes() const;
};




#endif /* UTILS_ORTHOXMLFILEDATA_ORTHOXMLFILEDATA_HPP_ */
