message(STATUS "After compilation, the resulting binary and reports can be found in ${CMAKE_CURRENT_BINARY_DIR}")

# compile object files
add_library("neuron_${STEP}" OBJECT "neuron.cpp")
add_library("main_${STEP}" OBJECT "main.cpp")

# link the resulting executable
add_executable("${PROJECT_NAME}_${STEP}" $<TARGET_OBJECTS:main_${STEP}> $<TARGET_OBJECTS:neuron_${STEP}>)

# edit executable name
set_property(TARGET "${PROJECT_NAME}_${STEP}" PROPERTY OUTPUT_NAME "ANN")

# fix optimization reports location
if (CMAKE_CXX_COMPILER_ID MATCHES "Intel" AND NOT WITH_PAPI)
    if (WIN32)
    	target_compile_options("neuron_${STEP}" PRIVATE "/Qopt-report-file:${CMAKE_CURRENT_BINARY_DIR}\neuron.cpp.optrpt")
	target_compile_options("main_${STEP}" PRIVATE "/Qopt-report-file:${CMAKE_CURRENT_BINARY_DIR}\main.cpp.optrpt")
    else () # UNIX
        target_compile_options("neuron_${STEP}" PRIVATE "-qopt-report-file:${CMAKE_CURRENT_BINARY_DIR}/neuron.cpp.optrpt")
	target_compile_options("main_${STEP}" PRIVATE "-qopt-report-file:${CMAKE_CURRENT_BINARY_DIR}/main.cpp.optrpt")
    endif ()
endif ()
