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
- currently only supports face formats of `v/vt/vn`
- not efficient
- no texture maps
- small bugs here and there

## Usage
Clone this repository into your project by running `git clone https://github.com/Hosef99/obj-parser.git`.

If you are using CMake, simply do `add_subdirectory([path-to-obj-parser])`, and link the library by adding the line `target_link_libraries([your project] obj-parser)` in your `CMakeLists.txt` file.

On Code::Blocks, the easiest way to link the library is to add the `.cpp` files in `obj-parser/src` into your project's `src` folder, and add all the `.h` files in `obj-parser/include` into your project's `include` folder. This would recompile the library every time, thus compile time will be slightly slower. It is still recommended to learn and use CMake.

It is also recommended to read `include/parser.h` to get a feel how the structures are like.

TODO: draw a diagram to illustrate the relationships of the structure.

The `OP::parse` function receives a string path to the `.obj` file and returns a `Model` (Note that the `.mtl` file should be in the same parent directory as the `.obj` file). This `Model` will contain most data that the user needs. Please do look into the `sandbox/main.cpp` file as it provides a nice example that shows how to use this library to draw shapes in `/assets` using OpenGL 1.1 and GLUT.

## Bugs
If there are any bugs found when using this library, submit an issue and I'll get to workin as soon as possible!

## Contributing
Reach out to me personally or submit a pull request on an existing issue.

## Acknowledgements
- BenXilam for... educating me on what "opaque" means. Thanks, I guess...
