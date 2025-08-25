cmake_minimum_required(VERSION 3.20) # modern enough for VS2022
project(Raytracer LANGUAGES CXX)

# require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# source files
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/source-files/*.cpp"
)

# add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Copy input and renders directories next to the executable after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/input" $<TARGET_FILE_DIR:${PROJECT_NAME}>/input
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/renders" $<TARGET_FILE_DIR:${PROJECT_NAME}>/renders
)

# point CMake to your SFML installation
set(SFML_DIR "C:/Users/Pranav/Libraries/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

# link SFML
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)