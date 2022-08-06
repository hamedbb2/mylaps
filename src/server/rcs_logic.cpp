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
    std::string RcsLogic::current_ts() {
        using namespace std::chrono;
        milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );
        return std::to_string(ms.count());
    }

    int RcsLogic::time_to_int(std::string lap_time) {
        lap_time.erase(std::remove(lap_time.begin(), lap_time.end(), ':'), lap_time.end());
        return std::stoi(lap_time);
    }

    std::string RcsLogic::set_results(json results) {
        const auto storing_ref = current_ts();
        std::thread([storing_ref, data = std::move(results)] {
            std::ofstream o("results/" + storing_ref + ".json"); 
            o << std::setw(4) << data << std::endl;
        }).detach();
        return storing_ref;
    }

    std::vector<std::string>  RcsLogic::get_results() {
        namespace fs = std::filesystem;
        std::string path = "results";
        std::vector<std::string> available;
        for (const auto & entry : fs::directory_iterator(path))
            available.emplace_back(entry.path().stem());
        return available;
    }

    std::map<std::string, std::vector<std::string>> RcsLogic::results_reader(const std::string& ref) {
        std::ifstream raw("results/" + ref + ".json");
        json j_reader;
        raw >> j_reader;
        return j_reader.get<std::map<std::string, std::vector<std::string>>>();
    }

    std::pair<std::string, std::string> RcsLogic::get_winner(const std::map<std::string, std::vector<std::string>>& results) {
        int best = std::numeric_limits<int>::max();
        std::string winner, winning_lap;
        for (auto& [car, laps]: results) {
            for (size_t i = 0; i < laps.size() - 1; i++) {
                auto start = laps[i];
                auto end = laps[i+1];
                auto lap_time = time_to_int(end) - time_to_int(start);
                if (lap_time < best) {
                    best = lap_time;
                    winner = car;
                    winning_lap = start + "-" + end;
                }
            }
        }
        return std::make_pair(winner, winning_lap);
    }

    std::pair<std::string, std::string> RcsLogic::get_winner(const std::string& ref) {
        auto results = results_reader(ref);
        return get_winner(std::move(results));
    }
};
