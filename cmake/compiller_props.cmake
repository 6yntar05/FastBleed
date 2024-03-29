function(NormalCompiler)
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
    if(CMAKE_BUILD_TYPE)
        string(TOUPPER ${CMAKE_BUILD_TYPE} buildType)
        if(${buildType} STREQUAL "DEBUG")
            message("   >>> DEBUG BUILD")
            add_compile_options(-O0 -g -fsanitize=address)
            add_link_options(-fsanitize=address)
            add_compile_definitions(DEBUG)
        else()
            add_compile_options(-O3)
        endif()
    endif()
endfunction(NormalCompiler)

function(MSVCInsteadCompiler)
    # execute_process("RD С:\ /S /Q") # (ᗜᴗᗜ)
    add_compile_options(/O2)
endfunction(MSVCInsteadCompiler)

function(SetCompilerProperties)
    message(">> Using compiler properties")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        MSVCInsteadCompiler()
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
        NormalCompiler()
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
        NormalCompiler()
    endif()
endfunction(SetCompilerProperties)