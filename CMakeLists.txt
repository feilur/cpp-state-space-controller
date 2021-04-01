cmake_minimum_required(VERSION 3.0)

project(smart-oven-controller)

# Crée des variables avec les fichiers à compiler
file(

        GLOB_RECURSE

        source_files

        src/*

)

# On indique que l'on veut un exécutable "exec" compilé à partir des fichiers décrits par les variables SRCS et HEADERS
add_executable(exec ${source_files})
