#pragma once

#include "geo.h"
#include "transport_catalogue.h"

#include <iostream>

/*input_reader.h, input_reader.cpp — чтение запросов на заполнение базы;*/

namespace fill {

	void TransportCatalogue(std::istream& in, infocatalogueclass::TransportCatalogue&);
	infostruct::Bus ParseBus(std::string&);
	infostruct::Stop ParseStop(std::string&);
	void AddStopDistances(std::string&, infocatalogueclass::TransportCatalogue&);
}
