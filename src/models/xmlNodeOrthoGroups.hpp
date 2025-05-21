/*
 * xmlNodeOrthoGroups.hpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#ifndef MODELS_XMLNODEORTHOGROUPS_HPP_
#define MODELS_XMLNODEORTHOGROUPS_HPP_

#include "xmlNodeCustom.hpp"
#include <iostream>
#include <vector>

class xmlNodeOrthoGroups : public xmlNodeCustom {

	private:
		std::vector<unsigned long> ids;
	public:
		xmlNodeOrthoGroups();
		~xmlNodeOrthoGroups();
		int recursiveSearch(xmlNodePtr node);
		int importNodeInfo(); //@Override
		void setNode(xmlNodePtr node); //@Override
		std::vector<unsigned long>& getIds();

};

typedef xmlNodeOrthoGroups* xmlNodeOrthoGroupsPtr;

#endif /* MODELS_XMLNODEORTHOGROUPS_HPP_ */
