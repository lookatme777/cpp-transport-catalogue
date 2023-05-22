#include "stat_reader.h"

#include <iomanip>

void out::Transport(infocatalogueclass::TransportCatalogue& catalogue, std::istream& in) {
    size_t count;
    std::cin >> count;
    for (size_t i = 0; i < count; ++i) {
        std::string keyword, inf;
        std::cin >> keyword;
        std::getline(std::cin, inf);
        if (keyword == "Bus") {
            out::Bus(catalogue, inf, std::cout);
        }
        if (keyword == "Stop") {
            out::Stop(catalogue, inf, std::cout);
        }
    }
}

void out::Bus(infocatalogueclass::TransportCatalogue& ctlg, const std::string& inf, std::ostream& out) {
    std::string number = inf.substr(1, inf.npos);
    if (ctlg.FindBusRoute(number)) {
        out << "Bus " << number << ": " << ctlg.BusRouteInformation(number).stops_count << " stops on route, "
            << ctlg.BusRouteInformation(number).unique_stops_count << " unique stops, " << ctlg.BusRouteInformation(number).route_length << " route length, " << ctlg.BusRouteInformation(number).curvature << " curvature\n";
    }
    else {
        out << "Bus " << number << ": not found\n";
    }
}


void out::Stop(infocatalogueclass::TransportCatalogue& ctlg, const std::string& inf, std::ostream& out) {
    std::string stop_name = inf.substr(1, inf.npos);
    if (ctlg.FindBusStop(stop_name)) {
        out << "Stop " << stop_name << ": ";
        std::set<std::string> buses = ctlg.BusToStop(stop_name);
        if (!buses.empty()) {
            out << "buses ";
            for (const auto& bus : buses) {
                std::cout << bus << " ";
            }
            out << "\n";
        }
        else {
            out << "no buses\n";
        }
    }
    else {
        out << "Stop " << stop_name << ": not found\n";
    }
}
