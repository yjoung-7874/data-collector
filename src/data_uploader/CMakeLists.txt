cmake_minimum_required(VERSION 3.15)

project(data_uploader)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(REALSENSE_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/realsense")
# set(MONGODB_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/mongodb")
set(UPLOADER_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/uploader")

set(data_uploader_srcs
  "main.cpp"
  "${UPLOADER_DIR}/image_saver.hpp"
  #  "${UPLOADER_DIR}/image_uploader.hpp"
  #  "${MONGODB_DIR}/mongo_utility.hpp"
  #  "${MONGODB_DIR}/mongo_wrapper.hpp"
  "${REALSENSE_DIR}/rs_wrapper.hpp"
)

foreach(_target data_uploader)
  add_executable(${_target} ${${_target}_srcs})
#  target_link_libraries(${_target} PRIVATE ${_ABSL} ${_REALSENSE} ${_MONGOCXX} ${_CPPZMQ} ${_OPENCV} ${_MONGOC})
  target_link_libraries(${_target} PRIVATE ${_ABSL} ${_REALSENSE} ${_OPENCV})
endforeach()

install(
  TARGETS data_uploader
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# install(
#   DIRECTORY server/preset
#   DESTINATION bin
# )
