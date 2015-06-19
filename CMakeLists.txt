cmake_minimum_required(VERSION 3.2)
project(Computer_Architecture_Lab4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lboost_serialization -lboost_unit_test_framework -lboost_program_options -lboost_system")

# unit_test_framework   - for tests
# system                - for boost.asio
FIND_PACKAGE(Boost COMPONENTS serialization unit_test_framework system REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})


set(SOURCE_FILES server/server.cpp)
add_executable(Server_FORK ${SOURCE_FILES} server/server.cpp task/task.hpp serialization/serialization.hpp)

set(SOURCE_FILES server/server.cpp)
add_executable(Server_PTHREAD ${SOURCE_FILES} server/server.cpp task/task.hpp serialization/serialization.hpp)

set(SOURCE_FILES server/server_asio/server.cpp)
add_executable(Server_ASIO ${SOURCE_FILES} server/server_asio/server.cpp task/task.hpp serialization/serialization.hpp)

set(SOURCE_FILES client/client.cpp)
add_executable(Client ${SOURCE_FILES} client/client.cpp serialization/serialization.hpp)

set(SOURCE_FILES tests/tests.cpp)
add_executable(Tests ${SOURCE_FILES} tests/tests.cpp task/task.hpp)


TARGET_LINK_LIBRARIES(Server_FORK ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(Server_PTHREAD ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(Server_ASIO ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(Client ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(Tests ${Boost_LIBRARIES})
