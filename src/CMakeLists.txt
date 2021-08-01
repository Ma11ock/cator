# Separate main.cpp from everything else for testing purposes.
set(BINARY_FINAL ${CMAKE_PROJECT_NAME})
set(BINARY ${BINARY_FINAL}_lib)
enable_language(C)
enable_language(CXX)
set(CMAKE_C_VERSION 99)
set(CMAKE_CXX_VERSION 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0075 NEW)


# Set the source files.
set(SRC_SOURCES
  main.cpp
  main.hpp

  saveBuf.hpp
  saveBuf.cpp
  )



set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(
  ${BINARY}
  ${SRC_SOURCES}
  )


# Link everything else to main.cpp
# The main executable
add_executable(
  ${BINARY_FINAL}
  app.cpp
  app.hpp
  )

# Dependencies (WX)

set(wxWidgets_USE_DEBUG ON)
set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_FIND_COMPONENTS base core net xml html adv)

target_include_directories(${BINARY} PUBLIC
  "/usr/lib/wx/include/gtk3-unicode-3.0"
  "/usr/include/wx-3.0")

target_compile_definitions(${BINARY} PUBLIC
  _FILE_OFFSET_BITS=64
  WXUSINGDLL=1
  __WXGTK__=1)

target_link_libraries(${BINARY} PUBLIC
  wx_gtk3u_xrc-3.0
  wx_gtk3u_webview-3.0
  wx_gtk3u_html-3.0
  wx_gtk3u_qa-3.0
  wx_gtk3u_adv-3.0
  wx_gtk3u_core-3.0
  wx_baseu_xml-3.0
  wx_baseu_net-3.0
  wx_baseu-3.0)

# Libm
target_link_libraries(${BINARY} PUBLIC m)

target_link_libraries(
  ${BINARY}
  PUBLIC
  ${CMAKE_REQUIRED_LIBRARIES})


set_target_properties(${BINARY} PROPERTIES LINK_FLAGS_RELEASE -s)
target_compile_features(${BINARY} PUBLIC cxx_std_17)
target_compile_features(${BINARY} PUBLIC c_std_99)

target_link_libraries(${BINARY_FINAL} PUBLIC ${BINARY})

add_custom_target(run
  COMMAND ${BINARY_FINAL}
  DEPENDS ${BINARY_FINAL}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})

install(TARGETS ${BINARY}

  DESTINATION bin/
  PERMISSIONS
  OWNER_READ
  OWNER_WRITE
  OWNER_EXECUTE
  GROUP_READ
  GROUP_EXECUTE
  WORLD_READ
  WORLD_EXECUTE
  )

# For the install script
