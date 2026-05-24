cmake_minimum_required(VERSION 3.16)

project(app VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP_ICON_RESOURCE ${CMAKE_SOURCE_DIR}/resources/resource.rc)
#set(app_icon...

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui Multimedia MultimediaWidgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui Multimedia MultimediaWidgets LinguistTools)

add_executable(${PROJECT_NAME} WIN32
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
  ${CMAKE_SOURCE_DIR}/src/mainwindow.h
  ${CMAKE_SOURCE_DIR}/src/converter.h
  ${CMAKE_SOURCE_DIR}/src/converter.cpp
  ${CMAKE_SOURCE_DIR}/src/utils.h
  ${CMAKE_SOURCE_DIR}/src/utils.cpp

  ${CMAKE_SOURCE_DIR}/src/ui/slicedbtn.h
  ${CMAKE_SOURCE_DIR}/src/ui/slicedbtn.cpp
  ${CMAKE_SOURCE_DIR}/src/ui/traplabel.h
  ${CMAKE_SOURCE_DIR}/src/ui/traplabel.cpp

  ${CMAKE_SOURCE_DIR}/src/ui/roundbtn.h
  ${CMAKE_SOURCE_DIR}/src/ui/roundbtn.cpp
  ${CMAKE_SOURCE_DIR}/src/ui/skipbtn.h
  ${CMAKE_SOURCE_DIR}/src/ui/skipbtn.cpp
  ${CMAKE_SOURCE_DIR}/src/ui/playstopbtn.h
  ${CMAKE_SOURCE_DIR}/src/ui/playstopbtn.cpp
  ${CMAKE_SOURCE_DIR}/src/ui/line.h
  ${CMAKE_SOURCE_DIR}/src/ui/line.cpp
  ${CMAKE_SOURCE_DIR}/src/ui/showhidebtn.h
  ${CMAKE_SOURCE_DIR}/src/ui/showhidebtn.cpp

  ${CMAKE_SOURCE_DIR}/src/ui/sliderstyle.h
  ${CMAKE_SOURCE_DIR}/src/ui/sliderstyle.cpp

  ${CMAKE_SOURCE_DIR}/src/serverrequester.h
  ${CMAKE_SOURCE_DIR}/src/serverrequester.cpp

  ${CMAKE_SOURCE_DIR}/src/sessionsmanager.h
  ${CMAKE_SOURCE_DIR}/src/sessionsmanager.cpp

  ${CMAKE_SOURCE_DIR}/src/appdata.h

  ${CMAKE_SOURCE_DIR}/include/llhttp/llhttp.c
  ${CMAKE_SOURCE_DIR}/include/llhttp/api.c
  ${CMAKE_SOURCE_DIR}/include/llhttp/http.c

  ${CMAKE_SOURCE_DIR}/resources/resources.qrc
  ${APP_ICON_RESOURCE}
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

find_library(AVFORMAT avformat PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)
find_library(AVCODEC avcodec PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)
find_library(AVUTIL avutil PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)
find_library(SWSCALE swscale PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)
find_library(SWRESAMPLE swresample PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)
find_library(UV uv PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)

target_link_libraries(${PROJECT_NAME}
  Qt6::Widgets
  Qt6::Gui
  Qt6::Multimedia
  Qt6::MultimediaWidgets
  ${AVFORMAT}
  ${AVCODEC}
  ${AVUTIL}
  ${SWSCALE}
  ${SWRESAMPLE}
  ${UV}
)

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall
  -Wextra
  -pedantic
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


#function(cpy_dll TARGET_NAME DLL_NAME)
#  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#      ${DLL_NAME}
#      $<TARGET_FILE_DIR:${TARGET_NAME}>
#  )
#endfunction()
#
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/libstdc++-6.dll)
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/libgcc_s_seh-1.dll)
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/libwinpthread-1.dll)
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/Qt6Widgets.dll)
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/Qt6Gui.dll)
#
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/avcodec-61.dll)
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/avformat-61.dll)
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/avutil-59.dll)
#cpy_dll(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/libuv-1.dll)
