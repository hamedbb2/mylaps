#include "rcs_server.hpp"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <filesystem>
#include <vector>

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

    std::string RcsServer::current_ts() {
        using namespace std::chrono;
        milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );
        return std::to_string(ms.count());
    }

    std::string RcsServer::_set_results(json results) {
        const auto storing_ref = current_ts();
        std::thread([storing_ref, data = std::move(results)] {
            std::ofstream o("results/" + storing_ref + ".json"); 
            o << std::setw(4) << data << std::endl;
        }).detach();
        return storing_ref;
    }

    std::vector<std::string>  RcsServer::_get_results() {
        namespace fs = std::filesystem;
        std::string path = "results";
        std::vector<std::string> available;
        for (const auto & entry : fs::directory_iterator(path))
            available.emplace_back(entry.path().stem());
        return available;
    }

    std::pair<std::string, std::string> RcsServer::_get_winner(std::string ref) {
        std::ifstream raw("results/" + ref + ".json");
        json reader;
        raw >> reader;
        for (auto& [car, laps]: reader.get<std::map<std::string, std::vector<std::string>>>()) {
            return std::make_pair(car, laps[0]);
        }
        return std::make_pair("", "");
    }

    void RcsServer::set_results(const Rest::Request& request, Http::ResponseWriter response) {
        const auto results = json::parse(request.body());
        json res = {{"reference", _set_results(std::move(results))}};
        response.send(Http::Code::Ok, res.dump());
    }

    void RcsServer::get_results([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response) {
        json res = {{"available", _get_results()}};
        response.send(Http::Code::Ok, res.dump());
    }

    void RcsServer::get_winner([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response) {
        const auto ref = request.param(":ref").as<std::string>();
        const auto winner = _get_winner(ref);
        json res = {{"winner", winner.first}, {"lap", winner.second}};
        response.send(Http::Code::Ok, res.dump());
    }
};
