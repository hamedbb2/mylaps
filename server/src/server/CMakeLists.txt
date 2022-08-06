add_library(RCSServer STATIC
    rcs_server.cpp
    rcs_logic.cpp
)

target_include_directories(RCSServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(RCSServer PUBLIC
        pistache_static
        nlohmann_json::nlohmann_json
)