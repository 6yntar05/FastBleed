# FastBleed
Macro programmer

###### In development...
###### Only for unix systems for now

## Building
### Dependencies (Optional):
- boost::program_options
- hands (not necessarily straight)
- X11
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
- -DUSE_BOOST=ON/OFF            # For systems without c++ boost
- -DUSE_COLORS=ON/OFF           # Use colorful output
- -DUSE_GUI=ON/OFF              # Qt GUI

## Usage
```sh
./FastBleed --help
```
