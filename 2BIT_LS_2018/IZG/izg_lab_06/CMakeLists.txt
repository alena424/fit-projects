cmake_minimum_required(VERSION 3.7.0)

PROJECT(Izg2017Lab6)

set(APPLICATION_NAME izg2017Lab6)

if(MSVC)
	set(USE_PREBUILD_LIB_PACKAGE ON CACHE BOOL "Use prebuild lib package.")
	if(USE_PREBUILD_LIB_PACKAGE)
		set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/win64/cmake CACHE STRING "" FORCE)
		file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/win64/bin/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
		file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/win64/bin/SDL2d.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	endif() 
endif() 

find_package(SDL2          REQUIRED)

set(STUDENT_SOURCES
  student/student.c
  student/linearAlgebra.c
  student/main.c
  student/camera.c
  student/bunny.c
  student/mouseCamera.c
  student/program.c
  )

set(STUDENT_INCLUDES
  student/student.h
  student/camera.h
  student/bunny.h
  student/mouseCamera.h
  student/program.h
  )

set(EXAMPLE_SOURCES
  examples/quadExample.c
  )

set(EXAMPLE_HEADERS
  examples/quadExample.h
  )

set(OPENGL_INCLUDES
  opengl/opengl.h
  )

set(OPENGL_SOURCES
  opengl/opengl.c
  )

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

IF(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic)
ENDIF()


add_executable(${APPLICATION_NAME} ${OPENGL_SOURCES} ${OPENGL_INCLUDES} ${STUDENT_SOURCES} ${STUDENT_INCLUDES} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS})

target_link_libraries(${APPLICATION_NAME} SDL2::SDL2 SDL2::SDL2main)
#if(MSVC)
#  target_link_libraries(${APPLICATION_NAME} SDL2::SDL2 SDL2::SDL2main)
#else()
#  target_link_libraries(${APPLICATION_NAME} SDL2::SDL2 SDL2::SDL2main m)
#endif()

target_include_directories(${APPLICATION_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
