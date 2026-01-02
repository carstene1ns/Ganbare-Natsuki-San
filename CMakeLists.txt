cmake_minimum_required(VERSION 3.18)

project(Ganbare_Natsuki_San)

set(CMAKE_C_STANDARD 99)

add_executable(gannatsu
	src/act.c
	src/act.h
	src/dconv.c
	src/dconv.h
	src/define.h
	src/ending.c
	src/ending.h
	src/extern.h
	src/function.c
	src/function.h
	src/general.h
	src/init.c
	src/logo.c
	src/logo.h
	src/option.c
	src/option.h
	src/paths.h
	src/ram.c
	src/refresh.c
	src/refresh.h
	src/scene.c
	src/scene.h
	src/title.c
	src/title.h
	src/util_snd.c
	src/util_snd.h)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

target_include_directories(gannatsu PRIVATE ${SDL_INCLUDE_DIRS} ${SDL_MIXER_INCLUDE_DIRS})
target_link_libraries(gannatsu PRIVATE ${SDL_LIBRARIES} ${SDL_MIXER_LIBRARIES})
