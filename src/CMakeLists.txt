add_subdirectory(imgui)

set(GEOMETRYMODELINGKPI_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB GEOMETRYMODELINGKPI_SRC "${GEOMETRYMODELINGKPI_SOURCES_DIR}/*.cpp")

add_executable(GeometryModelingKPI ${GEOMETRYMODELINGKPI_SRC})
target_include_directories(GeometryModelingKPI PRIVATE ${GEOMETRYMODELINGKPI_PROJECT_INCLUDE_DIR})
target_link_libraries(GeometryModelingKPI SDL2 imgui)

