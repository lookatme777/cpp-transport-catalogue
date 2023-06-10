#include "json_reader.h"
#include "request_handler.h"

int main() {

    infocatalogue::TransportCatalogue catalogue;
    JSONReader doc(std::cin);

    json_doc.FillCatalogue(catalogue);

    const auto& stat_requests = doc.StatRequests();
    const auto& render_settings = doc.RenderSettings().AsMap();
    const auto& renderer = doc.FillRenderSettings(render_settings);

    RequestHandler rh(catalogue, renderer);
    rh.ProcessRequests(stat_requests);
}
