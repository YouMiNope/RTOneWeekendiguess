# CMakeList.txt: RayTracingWeekend 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#


# 将源代码添加到此项目的可执行文件。
add_executable (RayTracingWeekend "RayTracingWeekend.cpp" "RayTracingWeekend.h"  "vec3.h" "utils.h" "color.h" "sphere.h" "hitable_list.h" "interval.h" "camera.h" "window.h" "buffer.h" "material.h" )

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	target_link_libraries(RayTracingWeekend glfw3)
	target_link_libraries(RayTracingWeekend OpenGL32)
	message("Windows linked")
else()
	target_link_libraries(RayTracingWeekend glfw)
	target_link_libraries(RayTracingWeekend glew)
	target_link_libraries(RayTracingWeekend "-framework OpenGL")
	message("MacOS linked")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET RayTracingWeekend PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
