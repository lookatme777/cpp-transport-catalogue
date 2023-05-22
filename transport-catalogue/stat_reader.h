#pragma once
#include "transport_catalogue.h"
#include <iostream>
/*stat_reader.h, stat_reader.cpp — чтение запросов на вывод и сам вывод;*/
namespace out {
	void Transport(infocatalogueclass::TransportCatalogue&, std::istream&);
	void Bus(infocatalogueclass::TransportCatalogue&, const std::string&, std::ostream&);
	void Stop(infocatalogueclass::TransportCatalogue&, const std::string&, std::ostream&);
}
