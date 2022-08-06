#ifndef RCS_LOGIC_H
#define RCS_LOGIC_H

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>
#include <string>

namespace MyLaps {
using namespace nlohmann;

class RcsLogic {
    public:
        explicit RcsLogic() {};
        std::pair<std::string, std::string> get_winner(const std::map<std::string, std::vector<std::string>>& results);
        std::string set_results(json results);
        std::vector<std::string> get_results();
        std::pair<std::string, std::string> get_winner(const std::string& ref);
    
    private:
        std::string current_ts();
        int time_to_int(std::string lap_time);
        std::map<std::string, std::vector<std::string>> results_reader(const std::string& ref);
};
}

#endif