#-----------------------------------------
# CMakeLists Skeleton 2020/12/10
#-----------------------------------------
cmake_minimum_required(VERSION 3.11.4)
project(quicky_utils LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_STANDARD 17)
set(ENABLE_CUDA_CODE off)

set(MY_SOURCE_FILES
    include/ansi_colors.h
    include/common.h
    include/ext_int.h
    include/ext_uint.h
    include/fract.h
    include/multi_thread_signal_handler.h
    include/multi_thread_signal_handler_listener_if.h
    include/password_input.h
    include/quicky_bitfield.h
    include/quicky_files.h
    include/quicky_test.h
    include/quicky_utils.h
    include/quicky_C_io.h
    include/safe_int.h
    include/safe_types.h
    include/safe_type_exception.h
    include/safe_uint.h
    include/signal_handler.h
    include/signal_handler_listener_if.h
    include/type_string.h
    src/quicky_test.cpp
    src/signal_handler.cpp
    src/type_string.cpp
   )

# List header directories in project
set(MY_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
   )

#Create dependancy list
set(DEPENDANCY_LIST "")
LIST(APPEND DEPENDANCY_LIST "quicky_exception")

#------------------------------
#- Generic part
#------------------------------
set(GENERATED_FILES "")
set(DEPENDANCY_OBJECTS "")
set(LINKED_LIBRARIES "-lpthread")
#iterate on dependancies
foreach(DEPENDANCY_ITEM IN ITEMS ${DEPENDANCY_LIST})
    message("${PROJECT_NAME} need dependancy ${DEPENDANCY_ITEM}")
    set(IS_IMPORTED FALSE)
    #Get information from dependancies if not already done
    if(NOT TARGET ${DEPENDANCY_ITEM})
        set(SUB_DEPENDANCY_OBJECTS "")
        set(SUB_LINKED_LIBRARIES "")
        add_subdirectory($ENV{QUICKY_REPOSITORY}/${DEPENDANCY_ITEM} ${CMAKE_CURRENT_BINARY_DIR}/${DEPENDANCY_ITEM})
        get_target_property(IS_IMPORTED ${DEPENDANCY_ITEM} IMPORTED)
        if(NOT ${IS_IMPORTED})
            list(APPEND DEPENDANCY_OBJECTS "$<TARGET_OBJECTS:${DEPENDANCY_ITEM}>" ${SUB_DEPENDANCY_OBJECTS})
        else()
            list(APPEND LINKED_LIBRARIES ${DEPENDANCY_ITEM})
        endif()
        list(APPEND LINKED_LIBRARIES ${SUB_LINKED_LIBRARIES})
    endif()

    # Complete include directories with directories coming from dependancy because of transitivity issues with Object libraries
    if(NOT ${IS_IMPORTED})
        set(MY_INCLUDE_DIRECTORIES ${MY_INCLUDE_DIRECTORIES} $<TARGET_PROPERTY:${DEPENDANCY_ITEM},INCLUDE_DIRECTORIES>)
    else(NOT ${IS_IMPORTED})
        set(MY_INCLUDE_DIRECTORIES ${MY_INCLUDE_DIRECTORIES} $<TARGET_PROPERTY:${DEPENDANCY_ITEM},INTERFACE_INCLUDE_DIRECTORIES>)
    endif()

endforeach(DEPENDANCY_ITEM)


#Prepare targets
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(IS_DIRECTORY ${HAS_PARENT})
    message("Declare library ${PROJECT_NAME}")
    add_library(${PROJECT_NAME} OBJECT ${MY_SOURCE_FILES} ${GENERATED_FILES})
    set(SUB_DEPENDANCY_OBJECTS ${DEPENDANCY_OBJECTS} PARENT_SCOPE)
    set(SUB_LINKED_LIBRARIES ${LINKED_LIBRARIES} PARENT_SCOPE)
else()
    #set(CMAKE_VERBOSE_MAKEFILE ON)
    set(MY_SOURCE_FILES
        include/test_fract.h
        src/test_ansi_colors.cpp
        src/test_ext_types.cpp
        src/test_multi_thread_signal_handler.cpp
        src/test_quicky_bitfield.cpp
        src/test_safe_types.cpp
        src/test_type_string.cpp
        ${MY_SOURCE_FILES}
        )
    add_executable(${PROJECT_NAME} ${MY_SOURCE_FILES} ${DEPENDANCY_OBJECTS} src/main_${PROJECT_NAME}.cpp)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D$<UPPER_CASE:${PROJECT_NAME}>_SELF_TEST)
    target_link_libraries(${PROJECT_NAME} ${LINKED_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -ansi $<$<CONFIG:Debug>:-O0> ${MY_CPP_FLAGS})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${MY_INCLUDE_DIRECTORIES})

foreach(DEPENDANCY_ITEM IN ITEMS ${DEPENDANCY_LIST})
    add_dependencies(${PROJECT_NAME} ${DEPENDANCY_ITEM})
endforeach(DEPENDANCY_ITEM)

#Force use of -std=c++11 instead of -std=gnu++11
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

#EOF
