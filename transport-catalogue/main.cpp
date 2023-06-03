#include "json_reader.h"
#include "request_handler.h"

int main() {
    infocatalogue::TransportCatalogue catalogue;
    jReader json_doc(std::cin);

    json_doc.FillCatalogue(catalogue);

    const auto& stat_requests = json_doc.StatRequests();
    const auto& render_settings = json_doc.RenderSettings().AsMap();
    const auto& renderer = json_doc.FillRenderSettings(render_settings);

    RequestHandler rh(catalogue, renderer);
    json_doc.ProcessRequests(stat_requests, rh);
}
