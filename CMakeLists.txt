cmake_minimum_required(VERSION 3.23)
project(openglproj)

set(OpenGlLinkers -ldl -lglfw) # setting all the Glut libraries as one variable.
################################################

add_executable(openglproj main.cpp glad.c shader.h)
target_link_libraries(openglproj ${OpenGlLinkers}) # linking opengl libraries to the project