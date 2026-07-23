# CERPEC

CERPEC is a personal project that aims to recreate popular data structures for the C programming language. These are divided based on their length into finite and 'infinite' (adjustable) structures.

## Prerequisite
C compiler that supports at least the **C99 standard**, for example:
- [GNU Compiler Collection (gcc)](https://gcc.gnu.org),
- [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/vs/features/cplusplus/),
- [Clang](https://clang.llvm.org)

[CMake](https://cmake.org/download/) for proper build process.

## Download

- Either save it as a ZIP, then unzip it in your project
- or clone it locally:

```bash
# example: 
git clone --recursive https://github.com/TheGAzed/cerpec.git
```

## Initialize

CMake code snipped showing how to add and link **cerpec** as a library:

```cmake
set(CERPEC_TEST OFF)
add_subdirectory([path-to-folder]/cerpec)
# ...
target_link_libraries([target-name] PRIVATE cerpec)
```

## **List of Implemented Data Structures:**

### [SEQUENCE](source/sequence)
- STACK
- QUEUE
- DEQUE

### [LIST](source/list)
- STRAIGHT LIST
- CIRCULAR LIST
- DOUBLE LIST

### [SET](source/set)
- HASH SET
- BITWISE SET
- RED-BLACK SET

### [TREE](source/tree)
- BINARY SEARCH TREE
- ALV TREE
- RED-BLACK TREE

### [MISC](source/misc)
- BINARY HEAP
- HASH MAP
