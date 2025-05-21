/*
 * xmlNodeSpecies.hpp
 *
 *  Created on: 6 nov. 2017
 *      Author: Neo
 */

#ifndef MODELS_XMLNODESPECIES_HPP_
#define MODELS_XMLNODESPECIES_HPP_

#include "xmlNodeCustom.hpp"
#include <unordered_map>

typedef std::unordered_map< unsigned int, unsigned int > hash_map;

class xmlNodeSpecies : public xmlNodeCustom {

	private:
		hash_map gene_set;
		unsigned int gene_set_size;
	public:
		xmlNodeSpecies();
		~xmlNodeSpecies();
		int importNodeInfo(); //@Override
		void setNode(xmlNodePtr node); //@Override
		const hash_map& getGeneSet() const;
		unsigned int getGeneSetSize() const;
		unsigned long getGene(unsigned long position);
};

typedef xmlNodeSpecies* xmlNodeSpeciesPtr;

bool operator==(const xmlNodeSpecies& lhs, const xmlNodeSpecies& rhs);


#endif /* MODELS_XMLNODESPECIES_HPP_ */
