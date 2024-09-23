cmake_minimum_required(VERSION 3.5)

project(polt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED Sql PrintSupport Concurrent)

set(qcustomplot_src
    qcustomplot-source/qcustomplot.h
    qcustomplot-source/qcustomplot.cpp
    )


add_executable(polt
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    showwidget.h
    showwidget.cpp
    wdatabase.h
    wdatabase.cpp
   ${qcustomplot_src}
    )

target_link_libraries(polt PRIVATE
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Concurrent
    Qt5::Sql
    )
