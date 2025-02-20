
if(NOT NRF5_SDK_PATH)
    set(NRF5_SDK_PATH "${PROJECT_SOURCE_DIR}/lib/nrf5_sdk" CACHE PATH "Path to NRF5 SDK")
endif()
message("Using NRF5 SDK directory: ${NRF5_SDK_PATH}")
add_library(esb-home-fw)

target_sources(esb-home-fw PRIVATE
    driver/esb.c
    protocol/esb_protocol.c
    commands/esb_commands.c
    commands/esb_cmd_def_common.c
)

target_include_directories(esb-home-fw PRIVATE
    ../
    ${NRF5_SDK_PATH}/modules/nrfx/mdk
    ${NRF5_SDK_PATH}/components/toolchain/cmsis/include
    ${NRF5_SDK_PATH}/components/toolchain/cmsis/include
    ${NRF5_SDK_PATH}/components/proprietary_rf/esb
    ${NRF5_SDK_PATH}/components/libraries/util
    ${NRF5_SDK_PATH}/components/libraries/queue
    ${NRF5_SDK_PATH}/components/libraries/experimental_section_vars
    ${NRF5_SDK_PATH}/components/libraries/log
    ${NRF5_SDK_PATH}/components/drivers_nrf/nrf_soc_nosd
    ${NRF5_SDK_PATH}/config/nrf52840/config
)


target_compile_definitions(esb-home-fw PUBLIC NRF52840_XXAA)
