file(GLOB MAIN_SOURCE 
	"${PROJECT_SOURCE_DIR}/RayTracingWeekend/*.cpp" 
	"${PROJECT_SOURCE_DIR}/RayTracingWeekend/*.h"
)

file (GLOB IMGUI_FILES 
	"${PROJECT_SOURCE_DIR}/imgui/*.cpp" 
	"${PROJECT_SOURCE_DIR}/imgui/*.h"
	"../imgui/backends/imgui_impl_glfw.cpp"
	"../imgui/backends/imgui_impl_glfw.h"
	"../imgui/backends/imgui_impl_opengl3.cpp"
	"../imgui/backends/imgui_impl_opengl3.h"
)

add_executable (main 
	${MAIN_SOURCE} 
)

add_library(imgui
	${IMGUI_FILES}
)

target_link_libraries (main imgui)

target_link_directories(main PRIVATE "${LIBRARY_DIR}")
target_include_directories(main PRIVATE "${INCLUDE_DIR}")
target_include_directories(imgui PUBLIC 
	"${INCLUDE_DIR}"
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


