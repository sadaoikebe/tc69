cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(tc69 C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(tc69
        main.c
        usb_descriptors.c
        )


# pull in common dependencies
target_link_libraries(tc69 pico_stdlib tinyusb_device tinyusb_board)
include_directories(tc69 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${PICO_SDK_PATH})

pico_set_program_name(tc69 "tc69 keyboard") # as "foo" would be the default
pico_set_program_description(tc69 "TC69 NICOLA oyayubi shift")
pico_set_program_version(tc69 "0.1")
pico_set_program_url(tc69 "https://github.com/sadaoikebe/tc69")

# create map/bin/hex file etc.
pico_add_extra_outputs(tc69)

add_custom_target(flash
    picotool load build/tc69.uf2
    DEPENDS build/tc69.uf2
)
