cmake_minimum_required(VERSION 3.20)

project(GiftCardEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# ── CLI ──────────────────────────────────────────────────────────────
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

# ── External libraries ───────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    httplib
    URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.15.3.zip
)
FetchContent_MakeAvailable(httplib)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.zip
)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(nlohmann_json)

# ── Tests ─────────────────────────────────────────────────────────────
set(TEST_SOURCES
    src/GiftCard.cpp
    src/Wallet.cpp
    src/FileRepository.cpp
    src/RedemptionPlanner.cpp
    src/RiskAnalyzer.cpp
    tests/test_gift_card.cpp
    tests/test_wallet.cpp
    tests/test_file_repository.cpp
    tests/test_redemption_planner.cpp
    tests/test_risk_analyzer.cpp
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests gtest gtest_main)
target_include_directories(run_tests PRIVATE include)

enable_testing()
add_test(NAME all_tests COMMAND run_tests)

# ── Server ────────────────────────────────────────────────────────────
set(SERVER_SOURCES
    src/server_main.cpp
    src/GiftCard.cpp
    src/Wallet.cpp
    src/FileRepository.cpp
    src/RedemptionPlanner.cpp
    src/RiskAnalyzer.cpp
)

add_executable(server ${SERVER_SOURCES})
target_link_libraries(server httplib::httplib nlohmann_json::nlohmann_json)
target_include_directories(server PRIVATE include)
