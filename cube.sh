#!/usr/bin/env sh
cd CUBE
cc -g -o cube -l GLEW -l OpenGL -l glfw -l m main.c
./cube
