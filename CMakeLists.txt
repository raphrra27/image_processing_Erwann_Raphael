cmake_minimum_required(VERSION 3.30)
project(image_processing_Erwann_Raphael C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(image_processing_Erwann_Raphael
    bmp8.h
    bmp_8.c
    main.c)
