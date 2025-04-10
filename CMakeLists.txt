cmake_minimum_required(VERSION 3.16)

# 指定生成器为Ninja或MinGW Makefiles，根据你的环境选择合适的生成器
set(CMAKE_GENERATOR "Ninja" CACHE STRING "Generator used by CMake")

project(ImageManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Concurrent)

message("opencv_path:${OpenCV_DIR}")
# 查找 OpenCV 包（指向 MinGW 构建目录）
find_package(OpenCV REQUIRED PATHS "D:/OpenCV-MinGW-Build-OpenCV-4.5.5-x64" NO_DEFAULT_PATH)


# 如果 OpenCV 没有找到，可以手动指定路径（可选）
if(NOT OpenCV_FOUND)
    set(OpenCV_DIR "D:/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
    find_package(OpenCV REQUIRED)
endif()

# 打印 OpenCV 信息（调试用）
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

set(TS_FILES ImageManager_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ImagePreview.cpp
        ImagePreview.h
        imagepreview.ui
        resources/resources.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ImageManager
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(ImageManager SHARED
                ${PROJECT_SOURCES}
        )
    else()
        add_executable(ImageManager
                ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 链接 Qt 库
target_link_libraries(ImageManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

# 包含 OpenCV 头文件路径
target_include_directories(ImageManager PRIVATE ${OpenCV_INCLUDE_DIRS})

# 链接 OpenCV 库
target_link_libraries(ImageManager PRIVATE ${OpenCV_LIBS})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ImageManager)
endif()
set_target_properties(ImageManager PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ImageManager
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ImageManager)
endif()