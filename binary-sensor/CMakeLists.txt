
add_library(esb-home-fw-binary-sensor)

target_sources(esb-home-fw-binary-sensor PRIVATE
    binary_sensor.c
    binary_sensor_esb_cmd_def.c
)

target_include_directories(esb-home-fw-binary-sensor PUBLIC
    ../
)

target_link_libraries(esb-home-fw-binary-sensor PUBLIC esb-home-fw)
