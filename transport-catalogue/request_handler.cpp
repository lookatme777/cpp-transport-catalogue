#include "request_handler.h"

std::optional<domain::RouteInforamtion> RequestHandler::GetBusStat(const std::string_view bus_number) const {
    domain::RouteInforamtion bus_stat{};
    const domain::Bus* bus = catalogue_.FindBusRoute(bus_number);

    if (!bus) {
        throw std::invalid_argument("bus not found");
    } 
    if (bus->is_circle) bus_stat.stops_count = bus->stops.size();
    else bus_stat.stops_count = bus->stops.size() * 2 - 1;

    int route_length = 0;
    double geographic_length = 0.0;

    for (size_t i = 0; i < bus->stops.size() - 1; ++i) {
        auto from = bus->stops[i];
        auto to = bus->stops[i + 1];
        if (bus->is_circle) {
            route_length += catalogue_.GetStopDistance(from, to);
            geographic_length += geo::ComputeDistance(from->coordinates,
                to->coordinates);
        }
        else {
            route_length += catalogue_.GetStopDistance(from, to) + catalogue_.GetStopDistance(to, from);
            geographic_length += geo::ComputeDistance(from->coordinates,
                to->coordinates) * 2;
        }
    }

    bus_stat.unique_stops_count = catalogue_.UniqueStopsCount(bus_number);
    bus_stat.route_length = route_length;
    bus_stat.curvature = route_length / geographic_length;

    return bus_stat;
}

const std::set<std::string> RequestHandler::GetBusesByStop(std::string_view stop_name) const {
    return catalogue_.FindBusStop(stop_name)->buses_by_stop;
}

bool RequestHandler::GetBusNumber(const std::string_view bus_number) const {
    return catalogue_.FindBusRoute(bus_number);
}

bool RequestHandler::GetStopName(const std::string_view stop_name) const {
    return catalogue_.FindBusStop(stop_name);
}

svg::Document RequestHandler::RenderMap() const {
    return renderer_.SVG(catalogue_.GetSortedBuses());
}

void RequestHandler::ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const {
    json::Array result;
    for (auto& request : stat_requests.AsArray()) {
        const auto& request_map = request.AsMap();
        const auto& type = request_map.at("type").AsString();
        if (type == "Stop") {
            result.push_back(RequestHandler::PrintStop(request_map, rh).AsMap());
        }

        if (type == "Bus") {
            result.push_back(PrintRoute(request_map, rh).AsMap());
        }

        if (type == "Map") {
            result.push_back(PrintMap(request_map, rh).AsMap());
        }

    }
    json::Print(json::Document{ result }, std::cout);
}

const json::Node RequestHandler::PrintRoute(const json::Dict& request_map, RequestHandler& rh) const {
    json::Dict result;
    const std::string& route_number = request_map.at("name").AsString();
    result["request_id"] = request_map.at("id").AsInt();
    if (!rh.GetBusNumber(route_number)) {
        result["error_message"] = json::Node{ static_cast<std::string>("not found") };
    }
    else {
        result["curvature"] = rh.GetBusStat(route_number)->curvature;
        result["route_length"] = rh.GetBusStat(route_number)->route_length;
        result["stop_count"] = static_cast<int>(rh.GetBusStat(route_number)->stops_count);
        result["unique_stop_count"] = static_cast<int>(rh.GetBusStat(route_number)->unique_stops_count);
    }

    return json::Node{ result };
}



const json::Node RequestHandler::PrintStop(const json::Dict& request_, RequestHandler& rh) const {
    json::Dict result;
    const std::string& stop_name = request_.at("name").AsString();
    result["request_id"] = request_.at("id").AsInt();
    if (!rh.GetStopName(stop_name)) {
        result["error_message"] = json::Node{ static_cast<std::string>("not found") };
    }
    else {
        json::Array buses;
        for (auto& bus : rh.GetBusesByStop(stop_name)) {
            buses.push_back(bus);
        }
        result["buses"] = buses;
    }

    return json::Node{ result };
}

const json::Node RequestHandler::PrintMap(const json::Dict& request_, RequestHandler& rh) const {
    json::Dict result;
    result["request_id"] = request_.at("id").AsInt();
    std::ostringstream strm;
    svg::Document map = rh.RenderMap();
    map.Render(strm);
    result["map"] = strm.str();

    return json::Node{ result };
}
