# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects

find_package(Qt4 REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include(${QT_USE_FILE})

set(minigui_SOURCES   mainwindow.cpp main.cpp)
set(minigui_HEADERS   mainwindow.h)
set(minigui_FORMS     mainwindow.ui)
set(minigui_RESOURCES res.qrc)


QT4_WRAP_UI(minigui_FORMS_HEADERS ${minigui_FORMS})
QT4_WRAP_CPP(minigui_HEADERS_MOC ${minigui_HEADERS})

QT4_ADD_RESOURCES(minigui_RESOURCES_RCC ${minigui_RESOURCES})


add_executable(minigui ${minigui_SOURCES} ${minigui_HEADERS_MOC} 
	${minigui_FORMS_HEADERS} 
	${minigui_RESOURCES_RCC})

target_link_libraries(minigui ${QT_LIBRARIES})


install(TARGETS minigui RUNTIME DESTINATION bin)
install(FILES init.sh DESTINATION /etc/rc.d/S99minigui
	PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)
