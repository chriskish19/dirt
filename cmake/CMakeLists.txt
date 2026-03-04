# 3.18 min version required
cmake_minimum_required(VERSION 3.18)

# set the name of the project
set(dt_core "dt-core")

# set build type
set(build_type "terminal")

# create the main project
project(${dt_core})

# using c++ 20 required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# check compiler version
set(MSVC_compiler_id "MSVC")
set(CLANG_compiler_id "Clang")
set(GCC_compiler_id "GNU")
set(is_using_MSVC_compiler OFF)
set(is_using_CLANG_compiler OFF)
set(is_using_GCC_compiler OFF)
string(COMPARE EQUAL ${CMAKE_CXX_COMPILER_ID} ${MSVC_compiler_id} is_using_MSVC_compiler)
string(COMPARE EQUAL ${CMAKE_CXX_COMPILER_ID} ${CLANG_compiler_id} is_using_CLANG_compiler)
string(COMPARE EQUAL ${CMAKE_CXX_COMPILER_ID} ${GCC_compiler_id} is_using_GCC_compiler)

# error on wrong compiler
if(NOT (is_using_MSVC_compiler OR is_using_CLANG_compiler OR is_using_GCC_compiler))
    message(FATAL_ERROR "Compiler not supported, You are using: ${CMAKE_CXX_COMPILER_ID}")
endif()


#############################
# dt core exe
#############################

# get core source and header files
file(GLOB_RECURSE dt_core_sf "../projects/dt.core/*.cpp") # dt_core_sf (core source files)
file(GLOB_RECURSE dt_core_hf "../projects/dt.core/*.hpp") # dt_core_sf (core header files)

# core exe
set(is_terminal_build OFF)
set(is_gui_build OFF)
set(is_gui_terminal_build OFF)


string(COMPARE EQUAL ${build_type} "terminal" is_terminal_build)
if(is_terminal_build)
    add_executable(${dt_core} ${dt_core_sf} ${dt_core_hf})
    target_compile_definitions(${dt_core} PRIVATE
        UNICODE
        _UNICODE
        TERMINAL_BUILD=1
    )
endif()

string(COMPARE EQUAL ${build_type} "win32-gui" is_gui_build)
if(is_gui_build)
    add_executable(${dt_core} WIN32 ${dt_core_sf} ${dt_core_hf})

    # define UNICODE
    target_compile_definitions(${dt_core} PRIVATE
            UNICODE
            _UNICODE
            WIN32_GUI_BUILD=1
        )
endif()

string(COMPARE EQUAL ${build_type} "win32-gui-terminal" is_gui_terminal_build)
if(is_gui_terminal_build)
    add_executable(${dt_core} ${dt_core_sf} ${dt_core_hf})

    # define UNICODE
    target_compile_definitions(${dt_core} PRIVATE
            UNICODE
            _UNICODE
            WIN32_GUI_AND_TERMINAL=1
        )

    # Link against essential Windows libraries
    target_link_libraries(${dt_core} PRIVATE
        kernel32.lib
        user32.lib      # Needed for MessageBox, window creation, message pump
        gdi32.lib       # If you use any GDI functions (most GUI apps do)
        shell32.lib     # Optional but common (e.g., ShellExecute)
        advapi32.lib    # For some registry/thread functions
        comdlg32.lib    # If you use common dialogs (OpenFile, etc.)
    )

    target_compile_options(${dt_core} PRIVATE /utf-8)

endif()


string(COMPARE EQUAL ${build_type} "system-test" is_system_test)
if(is_system_test)
    add_executable(${dt_core} ${dt_core_sf} ${dt_core_hf})

    # define UNICODE
    target_compile_definitions(${dt_core} PRIVATE
            UNICODE
            _UNICODE
            SYSTEM_TEST=1
        )

    # Link against essential Windows libraries
    target_link_libraries(${dt_core} PRIVATE
        kernel32.lib
        user32.lib      # Needed for MessageBox, window creation, message pump
        gdi32.lib       # If you use any GDI functions (most GUI apps do)
        shell32.lib     # Optional but common (e.g., ShellExecute)
        advapi32.lib    # For some registry/thread functions
        comdlg32.lib    # If you use common dialogs (OpenFile, etc.)
    )

    target_compile_options(${dt_core} PRIVATE /utf-8)

endif()


# core header file names
# creates macros for each hpp file include path
# very handy for name changes and file moves
foreach(HEADER_FILE ${dt_core_hf})
    get_filename_component(FILE_NAME ${HEADER_FILE} NAME_WE)
    set(FILE_NAME_HPP "CORE_${FILE_NAME}_hpp")
    set(HEADER_FILE_PATH "\"${HEADER_FILE}\"")
    target_compile_definitions(${dt_core} PRIVATE -D${FILE_NAME_HPP}=${HEADER_FILE_PATH})
    
    # for file name changes in the project
    # their is a file called names.hpp that manages all file name changes in the project
    if(${FILE_NAME} STREQUAL "names")
        target_compile_definitions(${dt_core} PRIVATE -DCORE_NAMES_INCLUDE=${FILE_NAME_HPP})
    endif()
endforeach()

# assets converted to macro names that hold the file paths
if(is_gui_build OR is_gui_terminal_build OR is_system_test)
    # assets
    file(GLOB dt_buttons "../assets/*.bmp")
    foreach(BUTTON ${dt_buttons})
        get_filename_component(FILE_NAME ${BUTTON} NAME_WE)
        string(TOUPPER "${FILE_NAME}" FILE_NAME_UPPER)
        set(MACRO_BUTTON_NAME "DT_${FILE_NAME_UPPER}_BUTTON_PATH")
        set(BUTTON_FILE_PATH "L\"${BUTTON}\"")
        target_compile_definitions(${dt_core} PRIVATE -D${MACRO_BUTTON_NAME}=${BUTTON_FILE_PATH})
    endforeach()

    # get all the icon files
    file(GLOB dt_icons "../assets/*.ico")

    # icons converted to macro names that hold the file paths
    # we have a function in c++ that handles setting icons so no need to prepend for unicode paths
    foreach(ICON ${dt_icons})
        get_filename_component(FILE_NAME ${ICON} NAME_WE)
        string(TOUPPER "${FILE_NAME}" FILE_NAME_UPPER)
        set(MACRO_ICON_NAME "DT_${FILE_NAME_UPPER}_ICON_PATH")
        set(ICON_FILE_PATH "\"${ICON}\"")
        target_compile_definitions(${dt_core} PRIVATE -D${MACRO_ICON_NAME}=${ICON_FILE_PATH})
    endforeach()
endif()




#############################
# End of dt core exe
#############################
