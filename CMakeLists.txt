# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(ConversoresAD C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ConversoresAD ConversoresAD.c lib/ssd1306.c)

pico_set_program_name(ConversoresAD "ConversoresAD")
pico_set_program_version(ConversoresAD "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ConversoresAD 0)
pico_enable_stdio_usb(ConversoresAD 1)

# Add the standard library to the build
target_link_libraries(ConversoresAD
    pico_stdlib          # Biblioteca padrão do SDK
    hardware_i2c         # Suporte ao I2C
    hardware_pwm         # Suporte ao PWM
    hardware_adc         # Suporte ao ADC
)
# Add the standard include files to the build
target_include_directories(ConversoresAD PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/lib 
)

pico_add_extra_outputs(ConversoresAD)

