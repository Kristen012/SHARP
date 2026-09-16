cmake_minimum_required(VERSION 3.8)

# Define the project name of these source code
project (gaussian_laplacian)

# Define the C++ standard -std=c++17
# This is required to match the systemc installed with Ubuntu
set (CMAKE_CXX_STANDARD 17)

# Add optimization & warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Set clock period = 10 ns
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D CLOCK_PERIOD=10.0")

# Set NATIVE_SYSTEMC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D NATIVE_SYSTEMC")

# Define the executable name and its source code
file(GLOB SRC "*.cpp")
add_executable(gaussian_laplacian ${SRC})

# Define the used libraries of the executable, Equal to -l flags of g++
# In here, the libraries are given in apsolute path. hence no needs for -L
# flag
target_link_libraries(gaussian_laplacian systemc)

# Add test sobel filter command
set(OUT_FILE1 "response_1.bmp")
add_custom_command(OUTPUT ${OUT_FILE1}
	COMMAND ${CMAKE_COMMAND} -E echo "===== Running LoGFilter on picture1 ====="
	COMMAND ./gaussian_laplacian ../data/input_picture1.bmp ./${OUT_FILE1}
	DEPENDS gaussian_laplacian
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
set(OUT_FILE2 "response_2.bmp")
add_custom_command(OUTPUT ${OUT_FILE2}
	COMMAND ${CMAKE_COMMAND} -E echo "===== Running LoGFilter on picture2 ====="
	COMMAND ./gaussian_laplacian ../data/input_picture2.bmp ./${OUT_FILE2}
	DEPENDS gaussian_laplacian
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(run
	DEPENDS ${OUT_FILE1} ${OUT_FILE2}
)

# Compare output & golden
add_custom_target(check
	COMMAND ${CMAKE_COMMAND} -E echo "===== Comparing ${OUT_FILE1} with golden_1.bmp ====="
	COMMAND diff -s ${CMAKE_BINARY_DIR}/${OUT_FILE1} ${CMAKE_SOURCE_DIR}/data/golden_1.bmp
	COMMAND ${CMAKE_COMMAND} -E echo "===== Comparing ${OUT_FILE2} with golden_2.bmp ====="
	COMMAND diff -s ${CMAKE_BINARY_DIR}/${OUT_FILE2} ${CMAKE_SOURCE_DIR}/data/golden_2.bmp
	DEPENDS run
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
