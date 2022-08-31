# FastBleed
Macro programmer

###### In development...
###### Only for unix systems for now

## Building
### Dependencies
- boost::program_options
- hands (not necessarily straight)
###### Optional:
- X11
- wayland-client
- wayland-protocols

### Building
```sh
mkdir build
cd build
cmake .. <options>
cmake --build ./ -j$(nproc)
```

### CMake options
- -DUSE_X11=ON/OFF              # Include X11 libraries and compile X11 implementation
- -DUSE_WAYLAND=ON/OFF          # Include Wayland libraries and compile Wayland implementation
- -DUSE_COLORS=ON/OFF           # Use colorful output

## Usage
```sh
./FastBleed --help
```
