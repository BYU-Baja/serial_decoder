cmake_minimum_required(VERSION 3.10)
project(serial_decoder VERSION 1.0.1)

find_package (Threads REQUIRED)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-pointer-arith -Wno-unused-variable -Wno-unused-parameter)

file(GLOB serial_decoder_source
    "src/*.c"
    "include/*.h"
)

message("source_files: ${serial_decoder_source}")

add_executable(serial_decoder ${serial_decoder_source})

target_include_directories(serial_decoder PRIVATE include/)
target_link_libraries(serial_decoder PUBLIC /usr/local/lib/libmqttc.a ${CMAKE_THREAD_LIBS_INIT})
