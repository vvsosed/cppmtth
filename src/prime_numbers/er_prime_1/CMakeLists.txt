configure_file(config.h.in generated/config.h @ONLY)

add_executable(er_prime_1 main.cpp)
target_include_directories(er_prime_1 PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
