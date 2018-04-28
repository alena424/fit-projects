cmake_minimum_required(VERSION 3.1.0)

PROJECT(IzgProjekt2017)

set(APPLICATION_NAME izgProjekt2017)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

if(MSVC)
	set(USE_PREBUILD_LIB_PACKAGE ON CACHE BOOL "Use prebuild lib package.")
	if(USE_PREBUILD_LIB_PACKAGE)
		set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/win64/cmake CACHE STRING "" FORCE)
		file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/win64/bin/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
		file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/win64/bin/SDL2d.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	endif() 
endif() 

find_package(SDL2          REQUIRED)

set(GPU_SOURCES
  gpu/gpu.cpp
  )

set(GPU_INCLUDES
  )

set(STUDENT_SOURCES
  student/student_cpu.c
  student/student_pipeline.c
  student/student_shader.c
  student/linearAlgebra.c
  student/main.c
  student/camera.c
  student/bunny.c
  student/mouseCamera.c
  student/swapBuffers.c
  student/globals.c
  )

set(STUDENT_INCLUDES
  student/student_cpu.h
  student/student_pipeline.h
  student/student_shader.h
  student/gpu.h
  student/uniforms.h
  student/buffer.h
  student/vertexPuller.h
  student/program.h
  student/camera.h
  student/bunny.h
  student/mouseCamera.h
  student/swapBuffers.h
  student/globals.h
  )

set(TESTS_SOURCES
  tests/conformanceTests.cpp
  tests/performanceTest.cpp
  )

set(TESTS_INCLUDES
  tests/conformanceTests.h
  tests/performanceTest.h
  )

set(EXAMPLE_SOURCES
  examples/triangleExample.c
  )

set(EXAMPLE_HEADERS
  examples/triangleExample.h
  )

set(3RDPARTY_SOURCES
  )

set(3RDPARTY_INCLUDES
  3rdParty/catch.hpp
  )

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

IF(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-pedantic)
  add_compile_options(-Wconversion)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-pedantic)
  add_compile_options(-Wconversion)
ENDIF()


add_executable(${APPLICATION_NAME} ${GPU_SOURCES} ${GPU_INCLUDES} ${STUDENT_SOURCES} ${STUDENT_INCLUDES} ${TESTS_SOURCES} ${TESTS_INCLUDES} ${3RDPARTY_SOURCES} ${3RDPARTY_INCLUDES} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS})
target_link_libraries(${APPLICATION_NAME} SDL2)
target_include_directories(${APPLICATION_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
