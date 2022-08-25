# FastBleed
In development...
Only for unix systems for now

## Building
### Dependencies
- boost::program_options
- hands (not necessarily straight)
###### Optional:
- X11
- wayland-client

### Building
```sh
mkdir build
cd build
cmake ..
cmake --build ./ -j$(nproc)
```

## Usage
```sh
./FastBleed --help
```