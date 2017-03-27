# Environment

## Mac

* cmake


## Windows

* install MSYS2 - http://www.msys2.org/

```bash
pacman -Syu
pacman -Su

For 32-bits, run pacman -S mingw-w64-i686-toolchain
For 64 bits, run pacman -S mingw-w64-x86_64-toolchain

pacman -S make
pacman -S cmake
```




# Compiling

# Toolchain

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=thrird_party/msys-toolchain.cmake .

```

# Mode

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake -DCMAKE_BUILD_TYPE=Debug ..

```

