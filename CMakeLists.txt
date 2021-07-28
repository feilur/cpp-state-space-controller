cmake_minimum_required(VERSION 3.0)

project(state-space-controller)

file(

        GLOB_RECURSE

        source_files

        src/*

)

add_executable(exec ${source_files})
