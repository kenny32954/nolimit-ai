cmake_minimum_required(VERSION 3.15)
project(qbai_cg50)

include(GenerateG3A)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Generate the icon from stdlib-only Python so the repository stays text-only.
execute_process(
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/make_icon.py
          ${CMAKE_CURRENT_SOURCE_DIR}/src/icon.png
  RESULT_VARIABLE ICON_RESULT
)
if(NOT ICON_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to generate CG50 icon")
endif()

fxconv_declare_assets(src/icon.png WITH_METADATA)

add_executable(qbai_cg50
  src/main.c
  src/editor.c
  src/base64.c
  src/transport.c
  src/serial_syscalls.S
)

target_compile_options(qbai_cg50 PRIVATE -Wall -Wextra -Os)
target_link_libraries(qbai_cg50 Gint::Gint m)

generate_g3a(qbai_cg50 OUTPUT "QBAI.g3a"
  NAME "QBAI"
  ICON src/icon.png
)
