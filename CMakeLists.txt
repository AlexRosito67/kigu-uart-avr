cmake_minimum_required(VERSION 3.16)
project(kigu-uart-avr VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(kigu-uart-avr ${SOURCES})

if(NOT DEFINED KIGU_UART_AVR_VERSION)
    set(KIGU_UART_AVR_VERSION "${PROJECT_VERSION}")
endif()

target_compile_definitions(kigu-uart-avr PRIVATE
    KIGU_UART_AVR_VERSION="${KIGU_UART_AVR_VERSION}"
)

if(MSVC)
    target_compile_options(kigu-uart-avr PRIVATE /W4 /O2)
else()
    target_compile_options(kigu-uart-avr PRIVATE -Wall -Wextra -O2)
endif()

install(TARGETS kigu-uart-avr DESTINATION bin)
