/**
 * emcc sum.cc -s WASM=1 -s EXPORTED_FUNCTIONS=["_add"] -o sum.js && wasmer inspect sum.wasm
 * emcc sum.cc -O1 -s WASM=1 -s EXPORTED_FUNCTIONS=["_add"] -o sum.js && wasmer inspect sum.wasm
 */

// #include <stdlib.h>
// #include <stdio.h>
// #include <memory.h>
// #include <emscripten/emscripten.h>

#ifdef __cplusplus
extern "C" {
#endif

// EMSCRIPTEN_KEEPALIVE 
int add(int value, int value2) {
    return value + value2;
}

#ifdef __cplusplus
}
#endif