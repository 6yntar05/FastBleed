# FastBleed
Yet another lightweight CLIcker

###### In development...
###### Only for unix systems for now

## Building
### Dependencies (Optional):
- X11
  <br>and/or
- wayland-client
- wayland-protocols

### Building
```sh
git submodule init
git submodule update
mkdir build
cd build
cmake .. <options>
cmake --build ./ -j$(nproc)
```

### CMake options
- -DUSE_X11=ON/OFF              # Include X11 libraries and compile X11 implementation
- -DUSE_WAYLAND=ON/OFF          # Include Wayland libraries and compile Wayland implementation
- -DUSE_COLORS=ON/OFF           # Use colorful output
- 
## Usage
```sh
./FastBleed --help
```
