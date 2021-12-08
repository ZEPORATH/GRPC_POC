cmake_minimum_required(VERSION 3.5)

project(gRPC_POC LANGUAGES CXX)
include(common.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_filename_component(sapi_proto "statusapis.proto" ABSOLUTE)
get_filename_component(sapi_proto_path "${sapi_proto}" PATH)

set(sapi_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/statusapis.pb.cc")
set(sapi_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/statusapis.pb.h")
set(sapi_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/statusapis.grpc.pb.cc")
set(sapi_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/statusapis.grpc.pb.h")

add_custom_command(
    OUTPUT "${sapi_proto_srcs}" "${sapi_proto_hdrs}" "${sapi_grpc_srcs}" "${sapi_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${sapi_proto_path}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${sapi_proto}"
    DEPENDS "${sapi_proto}"
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(sapi_grpc_proto
    ${sapi_grpc_srcs}
    ${sapi_grpc_hdrs}
    ${sapi_proto_srcs}
    ${sapi_proto_hdrs})
target_link_libraries(sapi_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

#add_executable(gRPC_POC main.cpp)
add_executable(gRPC_client statusclient.cpp)
add_executable(gRPC_server statusserver.cpp)
#target_link_libraries(gRPC_POC sapi_grpc_proto ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(gRPC_client sapi_grpc_proto ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(gRPC_server sapi_grpc_proto ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
