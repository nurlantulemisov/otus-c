cmake_minimum_required(VERSION 3.10)

  project(DirViewer LANGUAGES C)

#Поиск пакета GTK4
    find_package(PkgConfig REQUIRED) pkg_check_modules(GTK4 REQUIRED gtk4)

#Указываем путь к заголовочным файлам GTK4
      include_directories(${GTK4_INCLUDE_DIRS})

# #Указываем путь к библиотекам GTK4
	link_directories(${GTK4_LIBRARY_DIRS})

	  set(CMAKE_C_STANDARD 11) set(CMAKE_C_STANDARD_REQUIRED True)
	    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
		"${CMAKE_CURRENT_SOURCE_DIR}/bin")

	      add_compile_options(-Wall - Wextra - Wpedanti)

		add_executable(viewer src / main.c)

#Линковка с библиотекой GTK4
		  target_link_libraries(viewer ${GTK4_LIBRARIES})
