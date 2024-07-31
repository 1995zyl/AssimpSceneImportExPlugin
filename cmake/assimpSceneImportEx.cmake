macro(find_qt_package)
    if(NOT EXISTS ${QT_SDK_DIR})
        message(STATUS "QT_SDK_DIR not found, try to read from environment variable: QTDIR")
        set(QT_SDK_DIR "$ENV{QTDIR}" CACHE STRING "QT_SDK_DIR read from environment variable: QTDIR" FORCE)
    endif()
    
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_SDK_DIR})
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core 3DCore 3DRender 3DExtras 3DAnimation REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core 3DCore 3DRender 3DExtras 3DAnimation REQUIRED)
    
    set(Qt_VERSION ${Qt${QT_VERSION_MAJOR}Core_VERSION})
    set(CMAKE_GLOBAL_AUTOGEN_TARGET OFF)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    add_definitions(-DQT_SDK_DIR="${QT_SDK_DIR}")
endmacro()
