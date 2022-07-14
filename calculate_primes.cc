#include <cstdlib>
#include <cstdio>
#include <emscripten.h>
#include <emscripten/val.h>
#include <math.h>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif

int IsPrime(int value) {
  emscripten::val console = emscripten::val::global("console");
  emscripten::val ml = emscripten::val::global("navigator")["ml"];
  emscripten::val context_options = emscripten::val::object();
  context_options.set("devicePreference", emscripten::val("gpu"));
  context_options.set("powerPreference", emscripten::val("default"));
  console.call<void>("log", ml);
  emscripten::val wnn_context = ml.call<emscripten::val>("createContext", context_options);
  console.call<void>("log", wnn_context);
  if (value == 2) { return 1; }
  if (value <= 1 || value % 2 == 0) { return 0; }

  for (int i = 3; (i*i) <= value; i += 2 ) {
    if (value % i == 0) { return 0; }
  }
  return 1;
}

// Two options to make functions visible to the JavaScript code:
// - Include the EMSCRIPTEN_KEEPALIVE declaration with the function.
// - Include the function names in the command line's EXPORTED_FUNCTIONS array when compiling the module
EMSCRIPTEN_KEEPALIVE
void FindPrimes(int start, int end) {
  printf("Prime numbers between %d and %d:\n", start, end);

  for (int i = start; i <= end; i += 2) {
    if(IsPrime(i)) {
      printf("%d ", i);
    }
  }
  printf("\n");
}

#ifdef __cplusplus
}
#endif


//$ emcc calculate_primes.cc -s SIDE_MODULE=1 -O1 --bind -o calculate_primes.wasm
// modules disable dead code elimination, use normal dead code elimination by building with SIDE_MODULE=2
// emcc calculate_primes.cc -s SIDE_MODULE=2 -O1 --bind -o calculate_primes.wasm