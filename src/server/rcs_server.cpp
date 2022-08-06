#include "rcs_server.hpp"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <limits>

namespace MyLaps
{
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

    void RcsServer::setup_routes() {
        using namespace Rest;
        Routes::Post(_router, "/results", Routes::bind(&RcsServer::set_results, this));
        Routes::Get(_router, "/results", Routes::bind(&RcsServer::get_results, this));
        Routes::Get(_router, "/winner/:ref", Routes::bind(&RcsServer::get_winner, this));
    }

    void RcsServer::set_results(const Rest::Request& request, Http::ResponseWriter response) {
        const auto results = json::parse(request.body());
        json res = {{"reference", _logic.set_results(std::move(results))}};
        response.send(Http::Code::Ok, res.dump());
    }

    void RcsServer::get_results([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response) {
        json res = {{"available", _logic.get_results()}};
        response.send(Http::Code::Ok, res.dump());
    }

    void RcsServer::get_winner(const Rest::Request& request, Http::ResponseWriter response) {
        const auto ref = request.param(":ref").as<std::string>();
        const auto winner = _logic.get_winner(ref);
        json res = {{"winner", winner.first}, {"winning_lap", winner.second}};
        response.send(Http::Code::Ok, res.dump());
    }
};
