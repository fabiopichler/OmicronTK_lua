cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME OmicronTK_lua)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(WIN32)
    set(THIRD_PARTY_PATH ${CMAKE_SOURCE_DIR}/../third_party/prefix-win64)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(THIRD_PARTY_PATH ${CMAKE_SOURCE_DIR}/../third_party/prefix-wasm)
else()
    set(THIRD_PARTY_PATH ${CMAKE_SOURCE_DIR}/../third_party/prefix-linux64)
endif()

set(THIRD_PARTY_INCLUDE_PATH ${THIRD_PARTY_PATH}/include)
set(THIRD_PARTY_LIB_PATH ${THIRD_PARTY_PATH}/lib)

if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/win64/debug)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/win64)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/win64/debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/win64)
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/linux64/debug)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/linux64)

    string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,-rpath,\'$ORIGIN\',-z,origin")
    string(APPEND CMAKE_SHARED_LINKER_FLAGS " -Wl,-rpath,\'$ORIGIN\',-z,origin -Wl,--no-undefined")
endif()

option(BUILD_LUA "Build Lua" ON)
option(USE_CUSTOM_LUA "Use a custom Lua build" OFF)
option(USE_LUAJIT "Use LuaJIT" ON)

if(BUILD_LUA)
    set(LUA_INCLUDE_DIR ${THIRD_PARTY_INCLUDE_PATH})
    set(LUA_DIRECTORIES ${THIRD_PARTY_LIB_PATH})

    set(LUA_LIBRARIES luajit-5.1)
elseif(USE_CUSTOM_LUA)
    set(LUA_INCLUDE_DIR "" CACHE STRING "Lua include directory")
    set(LUA_LIBRARIES "" CACHE STRING "Lua library")
else()
    if(USE_LUAJIT)
        find_package(LuaJIT 2.1 REQUIRED)
    else()
        find_package(Lua 2.1 REQUIRED)
    endif()
endif()

include_directories(include ${LUA_INCLUDE_DIR})

set(SRC_FILES
    include/OmicronTK/lua/global.h
    include/OmicronTK/lua/defines.hpp
    include/OmicronTK/lua/Lua.hpp
    include/OmicronTK/lua/Value.hpp
    include/OmicronTK/lua/version.h
    src/helpers.cpp
    include/OmicronTK/lua/helpers.hpp
    src/Value.cpp
    src/Lua.cpp
    src/CallbackInfo.cpp
    include/OmicronTK/lua/CallbackInfo.hpp
    src/NativeClass.cpp
    include/OmicronTK/lua/NativeClass.hpp
    include/OmicronTK/lua/Callback.hpp
    src/StackValue.cpp
    include/OmicronTK/lua/StackValue.hpp
    include/OmicronTK/lua/alternative_api/util/ObjectUtil.hpp
    include/OmicronTK/lua/alternative_api/Object.hpp
    include/OmicronTK/lua/alternative_api/Functional.hpp
    include/OmicronTK/lua/Class.hpp
    src/Class.cpp
    include/OmicronTK/lua/Object.hpp
    src/Object.cpp
    include/OmicronTK/lua/Function.hpp
    src/Function.cpp
    include/OmicronTK/lua/Persistent.hpp
    src/Persistent.cpp)

if(WIN32)
    #set(SRC_FILES ${SRC_FILES} rc/lua.rc)
endif()

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE OTKLUA_BUILD_CORE_LIB)
target_link_directories(${PROJECT_NAME} PRIVATE ${LUA_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()
