#pragma once
#include "transport_catalogue.h"
#include <iostream>
/*stat_reader.h, stat_reader.cpp — чтение запросов на вывод и сам вывод;*/
namespace out {
	void Transport(infocatalogueclass::TransportCatalogue& catalogue);
	void Bus(infocatalogueclass::TransportCatalogue&, std::string);
	void Stop(infocatalogueclass::TransportCatalogue&, std::string);
}
