#Copyright 2017 Husky Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

find_package(Threads)
find_package(GTest)

if(GTEST_FOUND)
  enable_testing()
  file(GLOB_RECURSE UNIT_TEST_SOURCE ${PROJECT_SOURCE_DIR}/test/*_unittest.cc)
  if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
    list(REMOVE_ITEM UNIT_TEST_SOURCE ${FILTER_SOURCE})
  endif()
  include_directories(${GTEST_INCLUDE_DIR})
  add_executable(${PROJECT_NAME}_unit_tests ${UNIT_TEST_SOURCE} unittest_main.cc)
  set_property(TARGET ${PROJECT_NAME}_unit_tests PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PRIVATE_RUNTIME_DIR})
  # FIXME Below is a workaround to make the linking work
  target_link_libraries(${PROJECT_NAME}_unit_tests ${GTEST_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${ZMQ_LIBRARY} data-static engine-static network-static gflags ${glog_LIBRARY} ${CMAKE_DL_LIBS})
  add_test(AllTestsIn${PROJECT_NAME}UnitTests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_unit_tests)
else()
  message(WARNING "Google Test not found")
endif()
