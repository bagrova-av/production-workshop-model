cmake_minimum_required(VERSION 3.14)
project(WorkshopModelApp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_LIST
    main.cpp
    Parsing/InputParser.cpp
    Simulation/Simulator.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_include_directories (${PROJECT_NAME} PRIVATE
    DomainEntities/
    Parsing/
    Simulation/
)