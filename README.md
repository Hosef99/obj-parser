# obj-parser
A lightweight, simple `.obj` file parser.

> [!WARNING]
> This is an underdeveloped project, and it is not meant to be used in serious applications. Use at your own risks.

## Features
### Why should you use this?
- Lightweight
- Easy to use
- Made with love ❤️

### Why should you NOT use this?
- does not support any type of faces other than triangular faces
- ~~currently only supports face formats of `v/vt/vn`~~
- not efficient
- no texture maps
- small bugs here and there

## Usage
Clone this repository into your project by running `git clone https://github.com/Hosef99/obj-parser.git`.

### To output C equivalent vertices in an array of floats

Use the compile command `g++ sandbox/c_output.cpp src/parser.cpp -o obj-parser -Iinclude` to compile the command, this will create an executable at your directory called `obj-parser`.

The usage of `obj-parser` is as simple as you breath, just do `obj-parser [obj file] [output file (optional)]`. If no output file is provided, the program will put the output in a `output/out.c` file.

The arrays that are outputted are currently in the form of:
```c
const float modelmaterial1[] = {
    x, y, z, u, v, nx, ny, nz,
    ...
};

const float modelmaterial2[] = {
    x, y, z, u, v, nx, ny, nz,
    ...
};

...
```
where `x`, `y`, and `z` are the positions of the vertex, `u` and `v` are the texture coordinates of the vertex, and `nx`, `ny`, `nz` are the normals of the vertex.


### To use the models directly in your project

If you are using CMake, add `add_subdirectory([path-to-obj-parser])`, and link the library by adding the line `target_link_libraries([your project] obj-parser)` in your `CMakeLists.txt` file.

On Code::Blocks, the easiest way to link the library is to add the `.cpp` files in `obj-parser/src` into your project's `src` folder, and add all the `.h` files in `obj-parser/include` into your project's `include` folder. This would recompile the library every time, thus compile time will be slightly slower. It is still recommended to learn and use CMake.

It is also recommended to read `include/parser.h` to get a feel how the structures are like.

TODO: draw a diagram to illustrate the relationships of the structure.

The `OP::parse` function receives a string path to the `.obj` file and returns a `Model` (Note that the `.mtl` file should be in the same parent directory as the `.obj` file). This `Model` will contain most data that the user needs. Please do look into the `sandbox/main.cpp` file as it provides a nice example that shows how to use this library to draw shapes in `/assets` using OpenGL 1.1 and GLUT.

> [!IMPORTANT]
> When exporting models from Blender or any other modelling software, it is **REQUIRED** to check the triangulated mesh option. As of now, `obj-parser` does not support any other types of mesh other than triangles.

## Bugs
If there are any bugs found when using this library, submit an issue and I'll get to workin as soon as possible!

## Contributing
Reach out to me personally or submit a pull request on an existing issue.

## Acknowledgements
- BenXilam for... educating me on what "opaque" means. Thanks, I guess...
