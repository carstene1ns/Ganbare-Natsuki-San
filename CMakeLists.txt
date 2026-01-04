cmake_minimum_required(VERSION 3.18...4.0)

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
	src/main.c
	src/logo.c
	src/logo.h
	src/option.c
	src/option.h
	src/paths.h
	src/scene.c
	src/scene.h
	src/title.c
	src/title.h
	src/util_snd.c
	src/util_snd.h)

target_compile_options(gannatsu PRIVATE -Wall -Wunused)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
target_link_libraries(gannatsu PRIVATE SDL2::SDL2 SDL2_mixer::SDL2_mixer)
