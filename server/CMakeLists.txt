add_executable(RCSServer
    rcs_server.cpp
    main.cpp
)

target_link_libraries(RCSServer PRIVATE
        pistache_static
)