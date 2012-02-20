cmake_minimum_required(VERSION 2.6.0)

project(TcpHub C)

option(ARCHLINUX "Install init script for ArchLinux" OFF)

option(DYNAMIC "Run control serveur on port 7751" ON)

file(GLOB SRC_FILES src/*.c)

include_directories("include")

add_executable(tcphub ${SRC_FILES})

target_link_libraries(tcphub rt)

#if (CMAKE_COMPILER_IS_GNUCC)
	set_target_properties(tcphub PROPERTIES COMPILE_FLAGS -std=gnu99)
#endif (CMAKE_COMPILER_IS_GNUCC)

if (DYNAMIC)
	set_target_properties(tcphub PROPERTIES COMPILE_DEFINITIONS DYNAMIC)
endif (DYNAMIC)

install(TARGETS tcphub RUNTIME DESTINATION bin)

if (ARCHLINUX)
	install(FILES tcphub
		DESTINATION /etc/rc.d
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
endif (ARCHLINUX)
