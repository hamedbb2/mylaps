#include <rcs_server.hpp>
#include "gtest/gtest.h"

namespace {
    using namespace MyLaps;

    TEST(WinnerTest, WinnerNormal1) {
        RcsServer server;
        std::map<std::string, std::vector<std::string>> test_data = {
            {"1", {"12:00:00", "12:01:10", "12:02:12", "12:03:15", "12:04:28"}},
            {"2", {"12:00:01", "12:01:00", "12:02:10", "12:03:20", "12:04:20"}},
            {"3", {"12:00:01", "12:01:15", "12:02:17", "12:03:36", "12:04:46"}},
            {"4", {"12:00:02", "12:01:10", "12:02:12", "12:03:26", "12:04:45"}},
            {"5", {"12:00:02", "12:01:12", "12:02:16", "12:03:37", "12:04:33"}}
        };
        auto test = server._get_winner(test_data);
        EXPECT_EQ(test.first, "5");
        EXPECT_EQ(test.second, "12:03:37-12:04:33");
    }

    TEST(WinnerTest, WinnerNormal2) {
        RcsServer server;
        std::map<std::string, std::vector<std::string>> test_data = {
            {"1", {"12:00:00", "12:01:10", "12:02:12", "12:03:15", "12:04:28"}},
            {"2", {"12:00:01", "12:00:51", "12:02:10", "12:03:20", "12:04:20"}},
            {"3", {"12:00:01", "12:01:15", "12:02:17", "12:03:36", "12:04:46"}},
            {"4", {"12:00:02", "12:01:10", "12:02:12", "12:03:26", "12:04:45"}},
            {"5", {"12:00:02", "12:01:12", "12:02:16", "12:03:37", "12:04:33"}}
        };
        auto test = server._get_winner(test_data);
        EXPECT_EQ(test.first, "2");
        EXPECT_EQ(test.second, "12:00:01-12:00:51");
    }

    TEST(WinnerTest, WinnerEmpty) {
        RcsServer server;
        std::map<std::string, std::vector<std::string>> test_data = {};
        auto test = server._get_winner(test_data);
        EXPECT_EQ(test.first, "");
        EXPECT_EQ(test.second, "");
    }
}