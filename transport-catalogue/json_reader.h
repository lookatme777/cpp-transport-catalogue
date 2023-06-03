#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "request_handler.h"

#include <iostream>

class jReader {
public:
    jReader(std::istream& input): input_(json::Load(input)){
    }

    const json::Node& RenderSettings() const;
    const json::Node& BaseRequests() const;
    const json::Node& StatRequests() const;
    void ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const;
    void FillCatalogue(infocatalogue::TransportCatalogue&);
    renderer::MapRenderer FillRenderSettings(const json::Dict& request_map) const;
    const json::Node PrintStop(const json::Dict&, RequestHandler&) const;
    const json::Node PrintRoute(const json::Dict&, RequestHandler&) const;
    const json::Node PrintMap(const json::Dict&, RequestHandler&) const;

private:
    json::Document input_;
    json::Node null_ = nullptr;

    std::tuple<std::string_view, geo::Coordinates, std::map<std::string_view, int>> FillStop(const json::Dict&) const;
    void FillStopDistances(infocatalogue::TransportCatalogue&) const;
    std::tuple<std::string_view, std::vector<const domain::Stop*>, bool> FillRoute(const json::Dict&, infocatalogue::TransportCatalogue&) const;
};
