cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RayTracingWeekend")

add_subdirectory ("RayTracingWeekend")

set(INCLUDE_DIR "$ENV{INCLUDE_DIR}" CACHE PATH "Custom Include Directory")
set(LIBRARY_DIR "$ENV{LIBRARY_DIR}" CACHE PATH "Custom Library Directory")