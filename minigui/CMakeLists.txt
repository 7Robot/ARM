# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects

FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

INCLUDE(${QT_USE_FILE})

SET(minigui_SOURCES   mainwindow.cpp main.cpp)
SET(minigui_HEADERS   mainwindow.h)
SET(minigui_FORMS     mainwindow.ui)
SET(minigui_RESOURCES res.qrc)


QT4_WRAP_UI(minigui_FORMS_HEADERS ${minigui_FORMS})
QT4_WRAP_CPP(minigui_HEADERS_MOC ${minigui_HEADERS})

QT4_ADD_RESOURCES(minigui_RESOURCES_RCC ${minigui_RESOURCES})


ADD_EXECUTABLE(minigui ${minigui_SOURCES} ${minigui_HEADERS_MOC} 
	${minigui_FORMS_HEADERS} 
	${minigui_RESOURCES_RCC})

TARGET_LINK_LIBRARIES(minigui ${QT_LIBRARIES})
