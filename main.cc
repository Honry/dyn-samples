#include <cstdlib>
#include <cstdio>

#ifdef __EMSCRIPTEN__
  #include <dlfcn.h>
  #include <emscripten.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// function pointer signature for the FindPrimes function defined in side module
typedef void(*FindPrimes)(int, int);

void CalculatePrimes(const char* file_name) {
  // Use the dlopen function to open the side module, passing
  // in two parameter values:
  // The file name to open
  // An integer indicating the mode: RTLD_NOW
  // RTLD_NOW value is asking dlopen for the relocations to happen when the file is loaded.
  void* handle = dlopen(file_name, RTLD_NOW);
  if (handle == NULL) { return; }

  // Use dlsym to get 'FindPrimes' function pointer
  FindPrimes find_primes = (FindPrimes)dlsym(handle, "FindPrimes");
  if (find_primes == NULL) { return; }

  find_primes(3, 100);
  // release it when you've finished with a linked module
  dlclose(handle);
}

void DownloadErrorHandle(const char* file_name) {
  printf("Unable to download wasm file: %s \n", file_name);
}

int main() {
  // download the side module to Emscripten's file system.
  // This call is asynchronous and will call a callback function
  // which you will specify once the download is complete.
  emscripten_async_wget("calculate_primes.wasm", // file to download
    "calculate_primes.wasm", // Name to give to the file in Emscripten's file system
    CalculatePrimes, // Callback function on success
    DownloadErrorHandle); // Callback function on error

  return 0;
}

#ifdef __cplusplus
}
#endif


// emcc main.cc -s MAIN_MODULE=1 --bind -o main.html
// emcc main.cc -s MAIN_MODULE=2 --bind -s EXPORTED_FUNCTIONS=['_putchar','_main','_iprintf'] -s RUNTIME_LINKED_LIBS=[calculate_primes.wasm] -o main.html