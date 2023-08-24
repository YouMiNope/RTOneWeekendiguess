# 将源代码添加到此项目的可执行文件。

set(
	MAIN_SOURCE
	"RayTracingWeekend.cpp"
	"RayTracingWeekend.h"
	"vec3.h"
	"utils.h"
	"color.h"
	"sphere.h"
	"hitable_list.h"
	"interval.h"
	"camera.h"
	"window.h"
	"buffer.h"
	"material.h"
	"../imgui/backends/imgui_impl_glfw.cpp"
	"../imgui/backends/imgui_impl_glfw.h"
	"../imgui/backends/imgui_impl_opengl3.cpp"
	"../imgui/backends/imgui_impl_opengl3.h"
)
file (GLOB IMGUI_FILES "${PROJECT_SOURCE_DIR}/imgui/*.cpp" "${PROJECT_SOURCE_DIR}/imgui/*.h")
add_executable (main 
	${MAIN_SOURCE} 
	${IMGUI_FILES}
)

target_include_directories(main PRIVATE "${INCLUDE_DIR}")
target_link_directories(main PRIVATE "${LIBRARY_DIR}")
target_include_directories(main PRIVATE 
	"${PROJECT_SOURCE_DIR}/imgui"
	"${PROJECT_SOURCE_DIR}/imgui/backends"
)

if (WIN32)
	target_link_libraries(main glfw3)
	target_link_libraries(main OpenGL32)
else()
	target_link_libraries(main glfw)
	target_link_libraries(main glew)
	target_link_libraries(main "-framework OpenGL")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET main PROPERTY CXX_STANDARD 20)
endif()


