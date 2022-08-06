#ifndef RCS_SERVER_H
#define RCS_SERVER_H

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>
#include <string>
#include <rcs_logic.hpp>

namespace MyLaps {
using namespace Pistache;
using namespace nlohmann;

class RcsServer {
    public:
        explicit RcsServer(Address addr);
        void init(size_t thr);
        void start();

    private:
        void setup_routes();

        void set_results(const Rest::Request& request, Http::ResponseWriter response);
        void get_results(const Rest::Request& request, Http::ResponseWriter response);
        void get_winner(const Rest::Request& request, Http::ResponseWriter response);

    private:
        std::shared_ptr<Http::Endpoint> _http_endpoint;
        Rest::Router _router;
        RcsLogic _logic;
};
}

#endif