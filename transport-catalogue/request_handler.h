#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"

#include <sstream>

class RequestHandler {
public:
    RequestHandler(const infocatalogue::TransportCatalogue& catalogue, const renderer::MapRenderer& renderer): catalogue_(catalogue), renderer_(renderer){
    }

    std::optional<domain::RouteInforamtion> GetBusStat(const std::string_view) const;
    const std::set<std::string> GetBusesByStop(std::string_view) const;
    bool BusNumber(const std::string_view) const;
    bool StopName(const std::string_view) const;

    svg::Document RenderMap() const;

private:
    const infocatalogue::TransportCatalogue& catalogue_;
    const renderer::MapRenderer& renderer_;
};
