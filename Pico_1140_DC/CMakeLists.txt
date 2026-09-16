cmake_minimum_required(VERSION 3.12)

# The Pico 2 W is the primary Fuzzball-development target.  Override this at
# configure time (for example, -DPICO_BOARD=pico_w) when checking the RP2040
# boards.  The selection must happen before pico_sdk_init().
set(PICO_BOARD "pico2_w" CACHE STRING "Raspberry Pi Pico board target")

# Development uses the Maker Pi Pico's onboard SDIO socket.  An external
# Adafruit SDIO breakout can use the same GP10-15 wiring in a compact device.
# A future SPI profile will be added only after its physical pinout is chosen.
set(PICO_FUZZBALL_STORAGE_PROFILE "maker_pi_pico_sdio" CACHE STRING
    "Storage wiring profile: maker_pi_pico_sdio or external_sdio_gp10_15")
set_property(CACHE PICO_FUZZBALL_STORAGE_PROFILE PROPERTY STRINGS
    maker_pi_pico_sdio external_sdio_gp10_15)

if(PICO_FUZZBALL_STORAGE_PROFILE STREQUAL "maker_pi_pico_sdio")
    set(PICO_FUZZBALL_STORAGE_SOURCE storage/sdio_gp10_15.c)
elseif(PICO_FUZZBALL_STORAGE_PROFILE STREQUAL "external_sdio_gp10_15")
    set(PICO_FUZZBALL_STORAGE_SOURCE storage/sdio_gp10_15.c)
else()
    message(FATAL_ERROR "Unsupported PICO_FUZZBALL_STORAGE_PROFILE: ${PICO_FUZZBALL_STORAGE_PROFILE}")
endif()

include(pico_sdk_import.cmake)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(Pico_1140_DC C CXX ASM)
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER_FORCED)
set(CMAKE_CXX_COMPILER_FORCED)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
if (TARGET tinyusb_device)
    add_executable(Pico_1140 avr11.cxx disasm.cxx getline.cxx kb11.cxx kl11.cxx kt11.cxx kw11.cxx lp11.cxx pc11.cxx Pico_1140.cxx rk11.cxx rl11.cxx unibus.cxx dl11.cxx fp11.cxx ${PICO_FUZZBALL_STORAGE_SOURCE})

    # pull in common dependencies
    target_link_libraries(Pico_1140 pico_stdlib no-OS-FatFS-SD-SDIO-SPI-RPi-Pico)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(Pico_1140 1)
    pico_enable_stdio_uart(Pico_1140 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(Pico_1140)

    # add url via pico_set_program_url
#     example_auto_set_url(Pico_1140)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
# Pinned upstream SDIO/SPI FatFs snapshot.  The legacy SPI-only copy remains
# vendored for comparison while this port is validated on real hardware.
add_subdirectory(no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src)
add_compile_definitions(PICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE=1)
add_compile_definitions(PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=0)
