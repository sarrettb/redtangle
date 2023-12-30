cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(REDTANGLE)

# Build Dependencies 
# SDL and SDL_image should be added as git submodules, so that they can be built from source via add_subdirectory 
set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
add_subdirectory("${LIBS_DIR}/SDL")
add_subdirectory("${LIBS_DIR}/SDL_image")

# Include Directories for SDL, SDL_Image, and ImGui
set(SDL_INCLUDE_DIR "${LIBS_DIR}/SDL/include")
set(SDL_IMAGE_INCLUDE_DIR "${LIBS_DIR}/SDL_image/include")
set(IMGUI_INCLUDE_DIR "${LIBS_DIR}/imgui/backends" "${LIBS_DIR}/imgui")
message(STATUS IMGUI_INCLUDE_DIR: ${IMGUI_INCLUDE_DIR})

# Imgui does not use CMake so build source files into a static library 
file(GLOB IMGUI_HEADERS "${LIBS_DIR}/imgui/*.h")
file(GLOB IMGUI_SOURCES "${LIBS_DIR}/imgui/*.cpp"
                        "${LIBS_DIR}/imgui/backends/imgui_impl_sdl2.cpp"
                        "${LIBS_DIR}/imgui/backends/imgui_impl_sdlrenderer2.cpp"
    )
add_library(imgui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})
include_directories(imgui ${IMGUI_INCLUDE_DIR} ${SDL_INCLUDE_DIR})

# Utility library for parsing commandline which is used by the server
set(BUTILITY_DIR "${PROJECT_SOURCE_DIR}/../butility")
add_subdirectory(${BUTILITY_DIR} ${CMAKE_BINARY_DIR}/butility)

# gRPC generated sources based on proto file 
set(REDTANGLE_GRPC_INCLUDES "${PROJECT_SOURCE_DIR}/include/grpc")
set(GENERATED_DIR ${REDTANGLE_GRPC_INCLUDES}/generated)
set(REDTANGLE_PROTO_SOURCE "${GENERATED_DIR}/redtangle.pb.cc")
set(REDTANGLE_PROTO_HEADER "${GENERATED_DIR}/redtangle.pb.h")
set(REDTANGLE_GRPC_HEADER "${GENERATED_DIR}/redtangle.grpc.pb.cc")
set(REDTANGLE_GRPC_SOURCE "${GENERATED_DIR}/redtangle.grpc.pb.h")
file(MAKE_DIRECTORY ${GENERATED_DIR})
get_filename_component(REDTANGLE_PROTO "${REDTANGLE_GRPC_INCLUDES}/redtangle.proto" ABSOLUTE)
get_filename_component(REDTANGLE_PROTO_PATH "${REDTANGLE_PROTO}" PATH)

# Required for gRPC
find_package(Threads REQUIRED)

# Windows is much more difficult to build from source
# So it needs to be installed locally on the system via vcpkg
# CMake will find these packages of the install when CMAKE_TOOLCHAIN_FILE is specified from terminal 
# See https://sanoj.in/2020/05/07/working-with-grpc-in-windows.html for instructions 
if (WIN32)
    message(STATUS "Using gRPC local installation.")
    if(MSVC)
        add_definitions(-D_WIN32_WINNT=0x600)
    endif()
    # Protobuf
    set(protobuf_MODULE_COMPATIBLE TRUE)
    find_package(Protobuf CONFIG REQUIRED)
    message(STATUS "Using protobuf ${protobuf_VERSION}")
    if(CMAKE_CROSSCOMPILING)
        find_program(_PROTOBUF_PROTOC protoc)
    else()
        set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc> CACHE INTERNAL "")
    endif()
    # gRPC
    find_package(gRPC CONFIG REQUIRED)
    set(_GRPC_GRPCPP gRPC::grpc++)
    message(STATUS "Using gRPC ${gRPC_VERSION}")
    if(CMAKE_CROSSCOMPILING)
        find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
    else()
        set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin> CACHE INTERNAL "")
    endif()
    set(GRPC_LIBS gRPC::grpc++
                  gRPC::grpc++_reflection
                  gRPC::gpr 
                  gRPC::grpc 
                  gRPC::grpc++ 
                  protobuf::libprotoc 
                  protobuf::libprotobuf 
                  protobuf::libprotobuf-lite
        )
    # /ENTRY:mainCRTStartup keeps the same "main" function instead of requiring "WinMain")
    
# If building for Linux (tested on Ubuntu) 
# Then gRPC just needs to be added as a gitsubmodule via git submodule update --init --recursive
# gRPC will be built from source without requiring a local install 
else()
    set(ABSL_ENABLE_INSTALL ON)
    add_subdirectory("${LIBS_DIR}/grpc")
    message(STATUS "Using gRPC via add_subdirectory.")
    # After using add_subdirectory, we can now use the grpc targets directly from this build.
    if(CMAKE_CROSSCOMPILING)
        find_program(_PROTOBUF_PROTOC protoc CACHE INTERNAL "")
    else()
        set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc> CACHE INTERNAL "")
    endif()
    if(CMAKE_CROSSCOMPILING)
        find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin CACHE INTERNAL "")
    else()
        set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin> CACHE INTERNAL "")
    endif()
    list(APPEND GRPC_LIBS grpc++_reflection)
    list(APPEND GRPC_LIBS grpc++)
    list(APPEND GRPC_LIBS libprotobuf)
    set(GPRC_INCLUDE "${PROJECT_SOURCE_DIR}/libs/grpc/include")
endif()

# Run the gRPC protoc compiler from CMake
add_custom_command(
    OUTPUT "${REDTANGLE_PROTO_SOURCE}" "${REDTANGLE_PROTO_HEADER}" "${REDTANGLE_GRPC_SOURCE}" "${REDTANGLE_GRPC_HEADER}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${GENERATED_DIR}"
        --cpp_out "${GENERATED_DIR}"
        -I "${REDTANGLE_PROTO_PATH}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${REDTANGLE_PROTO}"
    DEPENDS "${REDTANGLE_PROTO}")

# Create static library for generated gRPC source 
add_library(grpc_redtangle STATIC
            ${REDTANGLE_GRPC_SOURCE}
            ${REDTANGLE_PROTO_SOURCE}
            ${REDTANGLE_GRPC_HEADER}
            ${REDTANGLE_PROTO_HEADER}
          )
include_directories(grpc_redtangle ${_PROTOBUF_INCLUDE_DIRS})
target_link_libraries(grpc_redtangle ${GRPC_LIBS})

# Build Client Application 
set(REDTANGLE_INCLUDES "${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE UI_SOURCES "${PROJECT_SOURCE_DIR}/src/ui/*.cpp")
file(GLOB_RECURSE GAME_SOURCES "${PROJECT_SOURCE_DIR}/src/game/*.cpp")
list(APPEND REDTANGLE_APP_SOURCES ${UI_SOURCES})
list(APPEND REDTANGLE_APP_SOURCES ${GAME_SOURCES})
list(APPEND REDTANGLE_APP_SOURCES "${PROJECT_SOURCE_DIR}/src/apps/redtangle_app.cpp")
list(APPEND REDTANGLE_APP_SOURCES "${PROJECT_SOURCE_DIR}/src/apps/redtangle_builder.cpp")
list(APPEND REDTANGLE_APP_SOURCES "${PROJECT_SOURCE_DIR}/src/grpc/redtangle_client.cpp")
message(STATUS REDTANGLE_APP_SOURCES ${REDTANGLE_APP_SOURCES})
add_executable(redtangle ${REDTANGLE_APP_SOURCES} ) 
include_directories(redtangle ${REDTANGLE_INCLUDES} 
                              ${REDTANGLE_GRPC_INCLUDES}
                              ${SDL_INCLUDE_DIR}
                              ${SDL_IMAGE_INCLUDE_DIR}
                              ${IMGUI_INCLUDE_DIR} 
                   )
target_link_libraries(redtangle SDL2main 
                                SDL2-static
                                SDL2_image
                                imgui
                                grpc_redtangle
                      )
set_target_properties(
    redtangle
    PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
                LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

# Build Server Application 
list(APPEND REDTANGLE_SERVER_SOURCES "${PROJECT_SOURCE_DIR}/src/game/redtangle_game.cpp")
list(APPEND REDTANGLE_SERVER_SOURCES "${PROJECT_SOURCE_DIR}/src/game/empty_piece.cpp")
list(APPEND REDTANGLE_SERVER_SOURCES "${PROJECT_SOURCE_DIR}/src/game/redtangle_piece.cpp")
list(APPEND REDTANGLE_SERVER_SOURCES "${PROJECT_SOURCE_DIR}/src/apps/redtangle_server_app.cpp")
list(APPEND REDTANGLE_SERVER_SOURCES "${PROJECT_SOURCE_DIR}/src/grpc/redtangle_server.cpp")
add_executable(redtangle_server ${REDTANGLE_SERVER_SOURCES} ) 
include_directories(redtangle_server ${REDTANGLE_INCLUDES} 
                                     ${BUTILITY_DIR}
                   )
target_link_libraries(redtangle_server grpc_redtangle butility)