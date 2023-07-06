# FastBleed
Yet another lightweight CLIcker

###### Only for X11/UInput systems for now
###### Handling only mouse for now (Uses additional mouse buttons by default)

## Building
### Dependencies (Optional):
- For X11: X11
- For UInput: linux, libinput, libudev

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
- `-DUSE_UINPUT=ON/OFF`           # Include compile uinput implementation
- `-DUSE_X11=ON/OFF`              # Include X11 libraries and compile X11 implementation
- `-DUSE_COLORS=ON/OFF`           # Use colorful output

## Usage
```sh
./FastBleed --help
```
