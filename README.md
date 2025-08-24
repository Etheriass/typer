# typer

## Dependencies
This project use SDL2 and SDL2_ttf for fonts.

The required packages can be installed using:

```bash
sudo apt install -y build-essential cmake libsdl2-dev libsdl2-ttf-dev
```

## Build

```bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```