add_executable(TestServer
        ../main.cpp
        rcs_server_test.cpp
)

target_compile_definitions(TestServer PUBLIC UNIT_TESTS)
target_link_libraries(TestServer PUBLIC
        gtest
        gmock
        RCSServer
)

include(GoogleTest)
gtest_discover_tests(TestServer)