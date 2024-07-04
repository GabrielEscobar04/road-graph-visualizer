cmake_minimum_required(VERSION 3.10)
project(graph-visualizer)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GLFW subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw)

# Find source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add GLAD source
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

# Create necessary directories
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/build/shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/build/data
)

# Copy config.txt to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/config.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/build/config.txt
)

# Copy shader files to build directory
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/src/shaders/*")
foreach(SHADER_FILE ${SHADER_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SHADER_FILE}
        ${CMAKE_CURRENT_SOURCE_DIR}/build/shaders/
    )
endforeach()

# Copy data files to build directory
file(GLOB DATA_FILES "${CMAKE_SOURCE_DIR}/data/*")
foreach(DATA_FILE ${DATA_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${DATA_FILE}
        ${CMAKE_CURRENT_SOURCE_DIR}/build/data/
    )
endforeach()

# Custom targets
add_custom_target(run
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
    COMMENT "Running the executable"
)