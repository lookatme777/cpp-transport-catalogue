#pragma once

#include "geo.h"
#include "domain.h"

#include <iostream>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <optional>
#include <unordered_set>
#include <set>
#include <map>


namespace infocatalogue{
    class TransportCatalogue {
    public:
        void AddBusRoute(std::string_view, const std::vector<const domain::Stop*>, bool);
        void AddBusStop(std::string_view, const geo::Coordinates);
        const domain::Bus* FindBusRoute(std::string_view) const;
        const domain::Stop* FindBusStop(std::string_view) const;
        size_t UniqueStopsCount(std::string_view) const;
        void SetStopDistance(const domain::Stop*, const domain::Stop*, const int);
        int GetStopDistance(const domain::Stop*, const domain::Stop*) const;
        const std::map<std::string_view, const domain::Bus*> GetSortedBuses() const;

    private:
        std::deque<domain::Bus> buses_;
        std::deque<domain::Stop> stops_;
        std::unordered_map<std::string_view, const domain::Bus*> finderbus_;
        std::unordered_map<std::string_view, const domain::Stop*> finderstop_;
        std::unordered_map<std::pair<const domain::Stop*, const domain::Stop*>, int, domain::Hasher> stop_distances_;
    };
}


