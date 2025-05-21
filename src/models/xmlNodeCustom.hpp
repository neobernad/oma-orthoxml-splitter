/*
 * xmlNodeCustom.hpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#ifndef MODELS_XMLNODECUSTOM_HPP_
#define MODELS_XMLNODECUSTOM_HPP_

#include <libxml/xpath.h>
#include <string>

class xmlNodeCustom {

	protected:
		xmlNodePtr node;
	public:
		xmlNodeCustom();
		xmlNodeCustom(xmlNodePtr node);
		virtual ~xmlNodeCustom();
		xmlNodePtr getNode();
		virtual void setNode(xmlNodePtr node);
		virtual void freeNode();
		virtual int importNodeInfo() = 0; // Pure virtual
		int checkNodeIntegrity(xmlNodePtr node, const std::string& node_name);
		void errorNoClause(xmlNodePtr node, const std::string& expected_str);
};

#endif /* MODELS_XMLNODECUSTOM_HPP_ */
