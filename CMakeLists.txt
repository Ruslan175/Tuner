cmake_minimum_required(VERSION 3.0)
project( Tuner )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall")

find_package(PkgConfig REQUIRED)
pkg_check_modules( RtAudio REQUIRED IMPORTED_TARGET rtaudio )
find_package(nlohmann_json 3.7.0 REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtkmm-4.0)

add_executable(${PROJECT_NAME} main.cpp CMicrophone.cpp CSound.cpp CNote.cpp CConfig.cpp CMainWindow.cpp CPostman.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC PkgConfig::RtAudio nlohmann_json::nlohmann_json PkgConfig::GTK4)

