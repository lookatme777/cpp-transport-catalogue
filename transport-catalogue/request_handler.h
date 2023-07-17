#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "transport_router.h"

#include <sstream>

class RequestHandler {
public:
    RequestHandler(const infocatalogue::TransportCatalogue& catalogue, const renderer::MapRenderer& renderer, const transport_router::Router& router): catalogue_(catalogue), renderer_(renderer), router_(router)
    {}

    std::optional<domain::RouteInforamtion> GetBusStat(const std::string_view bus_number) const;
    const std::set<std::string> GetBusesByStop(std::string_view stop_name) const;
    bool IsBusNumber(const std::string_view bus_number) const;
    bool IsStopName(const std::string_view stop_name) const;
    const std::optional<graph::Router<double>::RouteInfo> GetOptimalRoute(const std::string_view stop_from, const std::string_view stop_to) const;
    const graph::DirectedWeightedGraph<double>& GetRouterGraph() const;

    void ProcessRequests(const json::Node& stat_requests) const;

    const json::Node PrintRoute(const json::Dict& request_map) const;
    const json::Node PrintStop(const json::Dict& request_map) const;
    const json::Node PrintMap(const json::Dict& request_map) const;
    const json::Node PrintRouting(const json::Dict& request_map) const;

    svg::Document RenderMap() const;

private:
    const infocatalogue::TransportCatalogue& catalogue_;
    const renderer::MapRenderer& renderer_;
    const transport_router::Router& router_;
};
