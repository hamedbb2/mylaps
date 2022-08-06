#ifndef RCS_SERVER_H
#define RCS_SERVER_H

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>
#include <string>

namespace MyLaps {
using namespace Pistache;
using namespace nlohmann;

class RcsServer {
    public:
        explicit RcsServer() {};
        explicit RcsServer(Address addr);
        void init(size_t thr);
        void start();

        //For test
        std::pair<std::string, std::string> _get_winner(const std::map<std::string, std::vector<std::string>>& results);

    private:
        //Helpers
        void setup_routes();
        std::string current_ts();
        int time_to_int(std::string lap_time);
        std::map<std::string, std::vector<std::string>> results_reader(const std::string& ref);

        //Routes
        void set_results(const Rest::Request& request, Http::ResponseWriter response);
        void get_results(const Rest::Request& request, Http::ResponseWriter response);
        void get_winner(const Rest::Request& request, Http::ResponseWriter response);

        //Logics (should move to a separate class in production code)
        std::string _set_results(json results);
        std::vector<std::string> _get_results();
        std::pair<std::string, std::string> _get_winner(const std::string& ref);

    private:
        std::shared_ptr<Http::Endpoint> _http_endpoint;
        Rest::Router _router;
};
}

#endif