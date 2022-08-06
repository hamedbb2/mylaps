#include "rcs_server.hpp"


namespace MyLaps
{
    RcsServer::RcsServer(Address addr)
        : _http_endpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void RcsServer::init(size_t thr = 2)
    {
        auto opts = Http::Endpoint::options()
                        .threads(static_cast<int>(thr));
        _http_endpoint->init(opts);
        setup_routes();
    }

    void RcsServer::start()
    {
        _http_endpoint->setHandler(_router.handler());
        _http_endpoint->serve();
    }

    void RcsServer::setup_routes()
    {
        using namespace Rest;
        Routes::Post(_router, "/results", Routes::bind(&RcsServer::set_results, this));
        Routes::Get(_router, "/results", Routes::bind(&RcsServer::get_results, this));
        Routes::Get(_router, "/winner", Routes::bind(&RcsServer::get_winner, this));
    }

    void RcsServer::set_results([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response)
    {
        response.send(Http::Code::Ok, "1");
    }

    void RcsServer::get_results([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response)
    {
        response.send(Http::Code::Ok, "2");
    }

    void RcsServer::get_winner([[maybe_unused]] const Rest::Request& request, Http::ResponseWriter response)
    {
        response.send(Http::Code::Ok, "3");
    }
};
