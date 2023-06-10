#include "request_handler.h"
#include "json_builder.h"

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

bool RequestHandler::IsBusNumber(const std::string_view bus_number) const {
    return catalogue_.FindBusRoute(bus_number);
}

bool RequestHandler::IsStopName(const std::string_view stop_name) const {
    return catalogue_.FindBusStop(stop_name);
}

svg::Document RequestHandler::RenderMap() const {
    return renderer_.SVG(catalogue_.GetSortedBuses());
}

void RequestHandler::ProcessRequests(const json::Node& stat_requests) const {
    json::Array result;
    for (auto& request : stat_requests.AsArray()) {
        const auto& request_map = request.AsMap();
        const auto& type = request_map.at("type").AsString();
        if (type == "Stop") result.push_back(PrintStop(request_map).AsMap());
        if (type == "Bus") result.push_back(PrintRoute(request_map).AsMap());
        if (type == "Map") result.push_back(PrintMap(request_map).AsMap());
    }

    json::Print(json::Document{ result }, std::cout);
}


const json::Node RequestHandler::PrintRoute(const json::Dict& request_map) const {
    json::Node result;
    const std::string& route_number = request_map.at("name").AsString();
    const int id = request_map.at("id").AsInt();

    if (!IsBusNumber(route_number)) {
        result = json::Builder{}
            .StartDict()
            .Key("request_id").Value(id)
            .Key("error_message").Value("not found")
            .EndDict()
            .Build();
    }
    else {
        const auto& route_info = GetBusStat(route_number);
        result = json::Builder{}
            .StartDict()
            .Key("request_id").Value(id)
            .Key("curvature").Value(route_info->curvature)
            .Key("route_length").Value(route_info->route_length)
            .Key("stop_count").Value(static_cast<int>(route_info->stops_count))
            .Key("unique_stop_count").Value(static_cast<int>(route_info->unique_stops_count))
            .EndDict()
            .Build();
    }
    return result;
}

const json::Node RequestHandler::PrintStop(const json::Dict& request_map) const {
    json::Node result;
    const std::string& stop_name = request_map.at("name").AsString();
    const int id = request_map.at("id").AsInt();

    if (!IsStopName(stop_name)) {
        result = json::Builder{}
            .StartDict()
            .Key("request_id").Value(id)
            .Key("error_message").Value("not found")
            .EndDict()
            .Build();
    }
    else {
        json::Array buses;
        for (const auto& bus : GetBusesByStop(stop_name)) {
            buses.push_back(bus);
        }
        result = json::Builder{}
            .StartDict()
            .Key("request_id").Value(id)
            .Key("buses").Value(buses)
            .EndDict()
            .Build();
    }
    return result;
}

const json::Node RequestHandler::PrintMap(const json::Dict& request_map) const {
    json::Node result;
    const int id = request_map.at("id").AsInt();
    std::ostringstream strm;
    svg::Document map = RenderMap();
    map.Render(strm);

    result = json::Builder{}
        .StartDict()
        .Key("request_id").Value(id)
        .Key("map").Value(strm.str())
        .EndDict()
        .Build();

    return result;
}
