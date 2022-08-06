#include "rcs_server.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <thread>

namespace MyLaps
{
    using namespace nlohmann;
    RcsServer::RcsServer(Address addr)
        : _http_endpoint(std::make_shared<Http::Endpoint>(addr)) { }

    void RcsServer::init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
                        .threads(static_cast<int>(thr));
        _http_endpoint->init(opts);
        setup_routes();
    }

    void RcsServer::start() {
        _http_endpoint->setHandler(_router.handler());
        _http_endpoint->serve();
    }

    std::string RcsServer::current_ts() {
        using namespace std::chrono;
        milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );
        return std::to_string(ms.count());
    }

    void RcsServer::setup_routes() {
        using namespace Rest;
        Routes::Post(_router, "/results", Routes::bind(&RcsServer::set_results, this));
        Routes::Get(_router, "/results", Routes::bind(&RcsServer::get_results, this));
        Routes::Get(_router, "/winner", Routes::bind(&RcsServer::get_winner, this));
    }

    void RcsServer::set_results([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response) {
        const auto results = json::parse(request.body());
        const auto storing_ref = current_ts();
        std::thread([storing_ref, data = std::move(results)] { //in-mem-copy to disk-copy tradeoff
            std::ofstream o("results/" + storing_ref + ".json"); 
            o << std::setw(4) << data << std::endl;
        }).detach();
        json res = {{"reference", storing_ref}};
        response.send(Http::Code::Ok, res.dump());
    }

    void RcsServer::get_results([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "2");
    }

    void RcsServer::get_winner([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "3");
    }
};
