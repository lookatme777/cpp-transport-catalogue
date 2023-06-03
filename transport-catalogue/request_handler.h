#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "json_reader.h"

#include <sstream>

class RequestHandler {
public:
    RequestHandler(const infocatalogue::TransportCatalogue& catalogue, const renderer::MapRenderer& renderer): catalogue_(catalogue), renderer_(renderer){
    }

    std::optional<domain::RouteInforamtion> GetBusStat(const std::string_view) const;
    const std::set<std::string> GetBusesByStop(std::string_view) const;
    bool GetBusNumber(const std::string_view) const;
    bool GetStopName(const std::string_view) const;

    void ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const;
    const json::Node PrintStop(const json::Dict&, RequestHandler&) const;
    const json::Node PrintRoute(const json::Dict&, RequestHandler&) const;
    const json::Node PrintMap(const json::Dict&, RequestHandler&) const;

    svg::Document RenderMap() const;

private:
    const infocatalogue::TransportCatalogue& catalogue_;
    const renderer::MapRenderer& renderer_;
};
