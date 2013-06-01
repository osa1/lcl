# lcl -- Lua Container Library

lcl is a project to use STL containers(and maybe some others in future) in Lua. You can dynamically link liblcl to your Lua interpreter, and start using it.

This is my first C/C++ project, it's written in 2 days, and it's written mostly for learning purposes. Source may be full of bad practices. Pull requests and issue reports are very welcome. See also TODO list below.

This library is only tested on my 64bit linux system, please tell me( @sinan1111111111 ) if you run this in other platforms, or send a pull request with required changes!

## Usage

Until I find a way to generate only one .so file, you need to generate two .so files, and load them from Lua.

See `lua_dynload.lua` file as an example. Output is in `lua_dynload.output` file.

## Installation

You should set three variables in `Makefile`:

* `LUA_INSTALL`: This should point to a folder that contains `include` and `lib` folders. Lua 5.2.2 headers should be in that `include` folder, and compiled files should be in `lib` folder.

  eg. If you set it `/usr/local`, then `/usr/local/include/` should contain Lua 5.2.2 headers, and `/usr/local/lib/` should contain compiled object files.
* `CPP`: C++ compiler
* `CC`: C compiler

After setting the variables, `make` should generate required `.so` files. Then you should load them like in `lua_dynload.lua` example(line 3 and 4).

## TODO

* Write a configure to set CC, CPP and LUA_INSTALL variables in Makefile.
* Find a way to generate only one `.so` file.
* Check what happens when read/pop from an empty container. Return a consistent value in that situations.
* Add more containers?
* Tests.
* Update README with short but illustrative code snippets.
