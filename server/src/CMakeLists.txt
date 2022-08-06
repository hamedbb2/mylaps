add_executable(RestServer
    main.cpp
)

target_link_libraries(RestServer PRIVATE
    RCSServer
)

add_subdirectory(server)
