add_library(stb stb.cpp)

target_include_directories(stb SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(stb
	INTERFACE SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}"
)