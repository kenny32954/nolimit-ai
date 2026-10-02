# Configure with [fxsdk build-cg], which provides the CG50 toolchain.
cmake_minimum_required(VERSION 3.15)
project(qbai_cg50 LANGUAGES C ASM)

include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Generate both CG50 menu icons from stdlib-only Python.
execute_process(
  COMMAND ${Python3_EXECUTABLE}
          ${CMAKE_CURRENT_SOURCE_DIR}/tools/make_icon.py
          ${CMAKE_CURRENT_SOURCE_DIR}/src/icon-uns.png
  RESULT_VARIABLE ICON_UNS_RESULT
)
execute_process(
  COMMAND ${Python3_EXECUTABLE}
          ${CMAKE_CURRENT_SOURCE_DIR}/tools/make_icon.py
          ${CMAKE_CURRENT_SOURCE_DIR}/src/icon-sel.png
          --selected
  RESULT_VARIABLE ICON_SEL_RESULT
)

if(NOT ICON_UNS_RESULT EQUAL 0 OR NOT ICON_SEL_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to generate CG50 icons")
endif()

add_executable(qbai_cg50
  src/main.c
  src/diagnostics.c
  src/editor.c
  src/picker.c
  src/reference.c
  src/base64.c
  src/transport.c
  src/serial_syscalls.S
)

target_compile_options(qbai_cg50 PRIVATE -Wall -Wextra -Os)
target_link_libraries(qbai_cg50 Gint::Gint m)

generate_g3a(TARGET qbai_cg50 OUTPUT "QBAI.g3a"
  NAME "QBAI"
  ICONS src/icon-uns.png src/icon-sel.png
)
