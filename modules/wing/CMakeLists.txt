ivw_module(Wing)

set(HEADER_FILES
    include/KTH/wing/datastructures/feature.h
    include/KTH/wing/processors/wingfeatureselector.h
    include/KTH/wing/processors/wingmeshreader.h
    include/KTH/wing/wingmodule.h
    include/KTH/wing/wingmoduledefine.h
)
ivw_group("Header Files" ${HEADER_FILES})

set(SOURCE_FILES
    src/datastructures/feature.cpp
    src/processors/wingfeatureselector.cpp
    src/processors/wingmeshreader.cpp
    src/wingmodule.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

set(SHADER_FILES
	# Add shaders
)
ivw_group("Shader Files" ${SHADER_FILES})

set(TEST_FILES
    tests/unittests/wing-unittest-main.cpp
)
ivw_add_unittest(${TEST_FILES})

ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

target_include_directories(inviwo-module-wing PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../eigenutils/ext/eigengitlab>
        )

# Add shader directory to install package
#ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)
