cmake_minimum_required(VERSION 3.23)
project(openglproj)

set(OpenGlLinkers -ldl -lglfw -lassimp) # setting all the Glut libraries as one variable.
################################################

add_executable(openglproj main.cpp glad.c shader.h stb_image.h utilities.h camera.h mesh.h model.h)
target_link_libraries(openglproj ${OpenGlLinkers}) # linking opengl libraries to the project