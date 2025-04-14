# Just assigments for university	

This Code uses the [Botan](https://botan.randombit.net/) cryptography library in C++ for implementing cryptographic operations.

Since Botan does not provide official precompiled binaries, the library is integrated differently depending on the platform.

### Windows
- Botan was manually compiled from source using **MinGW**.
- The CMake configuration currently expects the compiled Botan library to be located at the hardcoded path: "C:/botan"   --> (so on the cdrive)

> If you are building on Windows, ensure your Botan build is located at this path, or adjust the `CMakeLists.txt` accordingly. Any legit compiler that Botan supports should work 

### Linux

- On Linux, the **precompiled Botan binaries from the Arch Linux repository** were used:

```bash
sudo pacman -S botan
```
If you're using a different Linux distribution, install the appropriate Botan version using your package manager or build it from source.

 Then just build the Projects individually in the "Praktikum x" folders 

```bash
mkdir build
cd build
cmake ..
make
```

