#!/bin/bash

cd "./windmill/res/shader"
for file in $(find ~+ -type f -path "*.vert" -or -path "*.frag");
do
    glslangValidator -V $file -o $file".spv"
done;
