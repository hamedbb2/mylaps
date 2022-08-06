#include "server/rcs_server.hpp"

using namespace Pistache;

int main(int argc, char* argv[])
{
    Port port(9080);

    int thr = hardware_concurrency(); //Not the best idea but for the sake of simplicity

    if (argc >= 2)
    {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3)
            thr = std::stoi(argv[2]);
    }

    Address addr(Ipv4::any(), port);
    
    std::cout << "Running server on " << thr << " threads" << std::endl;
    MyLaps::RcsServer rcs_server(addr);
    rcs_server.init(thr);
    rcs_server.start();
}