cmake_minimum_required(VERSION 3.20)

project(GiftCardEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/CommandParser.cpp
    src/FileRepository.cpp
    src/GiftCard.cpp
    src/RedemptionPlanner.cpp
    src/RiskAnalyzer.cpp
    src/Wallet.cpp
    src/GCApp.cpp
)

add_executable(cli ${SOURCES})