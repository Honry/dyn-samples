# dyn-samples
[Dynamic Linking](https://emscripten.org/docs/compiling/Dynamic-Linking.html?highlight=dlopen#dynamic-linking ) samples

## Key Files / Functions Introduction

`calculate_primes.cc`: A WebAssembly side module that will be linked to the main module: `main.cc`. This file exports a `FindPrimes` function to be called in main module.

`main.cc`: A WebAssembly main module that can link to side module.

`emscripten_async_wget()`: A function to download the side module to Emscripten's file system.

`dlopen()`: A function to load side modules after the program is already running.

`dlsym()`: A function to get symbols exported by side modules.


## Build

**Side Module**: `$ emcc calculate_primes.cc -s SIDE_MODULE=1 -O1 -o calculate_primes.wasm`

**Main Module**: `$ emcc main.cc -s MAIN_MODULE=1 -o main.html`

## Reduce Code Size

Modules disable dead code elimination, use normal dead code elimination by building with SIDE_MODULE=2
- `emcc calculate_primes.cc -s SIDE_MODULE=2 -O1 -o calculate_primes.wasm`
- `emcc main.cc -s MAIN_MODULE=2 -s EXPORTED_FUNCTIONS=['_putchar','_main','_iprintf']  -o main.html`




## Demo

https://honry.github.io/dyn-samples/main.html