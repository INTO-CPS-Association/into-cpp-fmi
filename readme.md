# INTO-CPP-FMI

This is a INTO-CPS C++11 library for FMI it provides a class capable of loading and interacting with FMUs.

## Usage

In your cmake file addd this project as a subfolder:

```bash
git submodule add <repo-url>
```

and in the CMakeList file add:

```
target_link_libraries (<my-target-name> libintocppfmi)
```

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

# Toolchain on Windows using MSYS mingw from MSYS shell

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=thrird_party/msys-toolchain.cmake .

```

# Mode

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake -DCMAKE_BUILD_TYPE=Debug ..

```

