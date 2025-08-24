# typer

## Dependencies
This project use SDL3 and SDL3_ttf for fonts.

The required packages can be installed using:

```bash
sudo apt install -y build-essential cmake libsdl3-dev libsdl3-ttf-dev
```

## Build

```bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```