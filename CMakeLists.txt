cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(sensordealarme)


list(APPEND HEADERS
    "${PROJECT_BINARY_DIR}/include/generated"
    "${PROJECT_BINARY_DIR}/zephyr/include/generated"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/setup"
    "${PROJECT_SOURCE_DIR}/include/input"
    "${PROJECT_SOURCE_DIR}/include/output"
    "${PROJECT_SOURCE_DIR}/include/sensor_dht"
    )

list(APPEND SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.c"
    "${PROJECT_SOURCE_DIR}/src/input.c"
    "${PROJECT_SOURCE_DIR}/src/output.c"
    "${PROJECT_SOURCE_DIR}/src/sensor_dht.c"
)

include_directories(${HEADERS})
target_sources(app PRIVATE ${SOURCES})
