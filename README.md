# lcl -- Lua Container Library

lcl is a project to use STL containers(and maybe some others in future) in Lua. You can dynamically link liblcl to your Lua interpreter, and start using it.

This is my first C/C++ project, and it's written mostly for learning purposes. Source may be full of bad practices. Pull requests and issue reports are very welcome.

This library is only tested on my 64bit linux system, please tell me( @sinan1111111111 ) if you run this in other platforms, or send a pull request with required changes!

It's tested with Lua 5.2.2, Lua 5.1.5, LuaJIT 2.0.2 and Love2D compiled with LuaJIT 2.0.2 (see `tests/` folder)

## Usage

You need to load `liblcl.so` file with `package.loadlib` Lua function. See `tests/dynload.lua` file as an example. Expected output is in `tests/dynload.output` file.

## Installation

You should set three variables in `Makefile`:

* `LUA_INSTALL`: This should point to a folder that contains `include` and `lib` folders. Lua 5.2.2 headers should be in that `include` folder, and compiled files should be in `lib` folder.

  eg. If you set it `/usr/local`, then `/usr/local/include/` should contain Lua 5.2.2 headers, and `/usr/local/lib/` should contain compiled object files.
* `CPP`: C++ compiler
* `CC`: C compiler

After setting the variables, `make` should generate required `.so` file(`liblcl.so`). Then you should load `liblcl.so` like in `tests/dynload.lua` example(line 3).
