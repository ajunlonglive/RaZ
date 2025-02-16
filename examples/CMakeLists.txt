project(RaZ_Examples)

# Defining an interface library to hold common settings for all examples
add_library(RaZ_Examples INTERFACE)
target_compile_features(RaZ_Examples INTERFACE cxx_std_17)
target_link_libraries(RaZ_Examples INTERFACE RaZ)
target_sources(RaZ_Examples INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/DemoUtils.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/DemoUtils.hpp")

include(CompilerFlags)
add_compiler_flags(RaZ_Examples INTERFACE)

# Disabling some warnings, which are intendedly present
if (RAZ_COMPILER_GCC)
    set(
        RAZ_EXAMPLES_DISABLED_WARNINGS

        -Wno-unused-variable
    )
elseif (RAZ_COMPILER_CLANG)
    set(
        RAZ_EXAMPLES_DISABLED_WARNINGS

        -Wno-header-hygiene
        -Wno-unused-variable
    )
elseif (RAZ_COMPILER_MSVC)
    set(
        RAZ_EXAMPLES_DISABLED_WARNINGS

        # Warnings triggered by MSVC's standard library
        /wd4189 # Local variable is initalized but not referenced (unused variable)
        /wd5220 # Non-static volatile member doesn't imply non-trivial move/copy ctor/operator=
    )
endif ()

target_compile_options(RaZ_Examples INTERFACE ${RAZ_EXAMPLES_DISABLED_WARNINGS})

add_executable(RaZ_BloomDemo bloomDemo.cpp)
target_link_libraries(RaZ_BloomDemo RaZ_Examples)

if (NOT RAZ_USE_EMSCRIPTEN)
    add_executable(RaZ_ComputeDemo computeDemo.cpp)
    target_link_libraries(RaZ_ComputeDemo RaZ_Examples)
endif ()

add_executable(RaZ_DeferredDemo deferredDemo.cpp)
target_link_libraries(RaZ_DeferredDemo RaZ_Examples)

add_executable(RaZ_FullDemo fullDemo.cpp)
target_link_libraries(RaZ_FullDemo RaZ_Examples)

add_executable(RaZ_MinDemo minDemo.cpp)
target_link_libraries(RaZ_MinDemo RaZ_Examples)

add_executable(RaZ_PhysicsDemo physicsDemo.cpp)
target_link_libraries(RaZ_PhysicsDemo RaZ_Examples)

add_executable(RaZ_ShowcaseDemo showcaseDemo.cpp)
target_link_libraries(RaZ_ShowcaseDemo RaZ_Examples)

add_executable(RaZ_SoundDemo soundDemo.cpp)
target_link_libraries(RaZ_SoundDemo RaZ_Examples)

if (NOT RAZ_USE_EMSCRIPTEN)
    add_executable(RaZ_TessellationDemo tessellationDemo.cpp)
    target_link_libraries(RaZ_TessellationDemo RaZ_Examples)
endif ()

if (RAZ_USE_EMSCRIPTEN)
    set_target_properties(RaZ_BloomDemo    PROPERTIES SUFFIX ".html")
    set_target_properties(RaZ_DeferredDemo PROPERTIES SUFFIX ".html")
    set_target_properties(RaZ_FullDemo     PROPERTIES SUFFIX ".html")
    set_target_properties(RaZ_MinDemo      PROPERTIES SUFFIX ".html")
    set_target_properties(RaZ_PhysicsDemo  PROPERTIES SUFFIX ".html")
    set_target_properties(RaZ_ShowcaseDemo PROPERTIES SUFFIX ".html")
    set_target_properties(RaZ_SoundDemo    PROPERTIES SUFFIX ".html")

    include(EmscriptenAssets.cmake)
endif ()

# If FBX SDK used with MSVC, copy the DLL into the examples/ folder to link it properly
if (MSVC AND RAZ_USE_FBX AND FBX_FOUND)
    file(COPY "${FBX_DLL}" DESTINATION "${CMAKE_BINARY_DIR}/examples/")
endif ()
