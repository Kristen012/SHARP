cmake_minimum_required(VERSION 3.8)

# Define the project name of these source code
project(test-dut)

# Define the C++ standard -std=c++17
set(CMAKE_CXX_STANDARD 17)

# Add optimization & warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Set clock period = 10 ns
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D CLOCK_PERIOD=25.0")

# Set NATIVE_SYSTEMC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D NATIVE_SYSTEMC")

# Define the executable name and its source code
file(GLOB SRC "*.cpp")
add_executable(test-dut ${SRC})

# Define the used libraries of the executable, Equal to -l flags of g++
target_link_libraries(test-dut systemc)

# Define custom target to run and compare output
add_custom_target(run_and_check ALL
  COMMAND ${CMAKE_COMMAND} -E echo "===== Running test-dut ====="
  COMMAND ./test-dut
  COMMAND ${CMAKE_COMMAND} -E echo "===== Comparing output ====="
  COMMAND diff response.dat ../data/golden.dat || echo "Output differs from golden!"
  DEPENDS test-dut
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
