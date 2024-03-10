cmake_minimum_required(VERSION 3.23)
set (CMAKE_CXX_STANDARD 11)
project(Project7)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

set(SOURCES
        src/mesh.cpp
        src/model.cpp
        src/shader.cpp
        src/texture.cpp
        src/utils.cpp
        src/framebuffer.cpp
        src/application.cpp
        )

set(INCLUDES
        include/
        libs/glad/include/
        libs/glfw/include/
        libs/cyCode/
        libs/stb/
        )

set(LIBS
        glfw
        glad
        )

set(DEFINITIONS
        -DCY_NO_INTRIN_H
        )

set(RESOURCE_OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}
        )

if (WIN32)

        set(LIBS ${LIBS} opengl32.lib)
        set(RESOURCE_OUTPUT ${RESOURCE_OUTPUT}/Debug)

elseif (APPLE)

        set(LIBS ${LIBS} "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
        set(DEFINITIONS ${DEFINITIONS} -DGL_SILENCE_DEPRECATION)

endif()

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
add_definitions(${DEFINITIONS})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${RESOURCE_OUTPUT})