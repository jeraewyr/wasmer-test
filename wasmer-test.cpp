// wasmer-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS 1
//#define WASMER_EMSCRIPTEN_ENABLED 1

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "libs/wasmer/include/wasmer.hh"
#pragma comment(lib, "wasmer_c_api.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Userenv.lib")


#define WASM_I32 wasmer_value_tag::WASM_I32
#define WASM_I64 wasmer_value_tag::WASM_I64
#define WASM_F32 wasmer_value_tag::WASM_F32
#define WASM_F64 wasmer_value_tag::WASM_F64

#define WASMER_OK wasmer_result_t::WASMER_OK
#define WASM_FUNCTION wasmer_import_export_kind::WASM_FUNCTION

inline wasmer_value_t wvalue(int i, wasmer_value_tag tag = WASM_I32)
{
    wasmer_value_t r;
    r.tag = tag;
    if (WASM_I32 == tag) {
        r.value.I32 = i;
    }
    else {
        r.value.I64 = i;
    }
    return r;
}

inline wasmer_value_t wvalue(float v, wasmer_value_tag tag = WASM_F32)
{
    wasmer_value_t r;
    r.tag = tag;
    if (WASM_F32 == tag) {
        r.value.F32 = v;
    }
    else {
        r.value.F64 = v;
    }
    return r;
}

void test_exports(wasmer_instance_t* instance)
{
     // Get the exports.
     wasmer_exports_t *exports = NULL;
     wasmer_instance_exports(instance, &exports);
    
     // Get the number of exports.
     int exports_length = wasmer_exports_len(exports);
     printf("Number of exports: %d\n", exports_length);
    
     for (int i = 0; i < exports_length; i++)
     {
         // Read the first export.
         wasmer_export_t* export0 = wasmer_exports_get(exports, i);

         // Get the kind of the export.
         wasmer_import_export_kind export_kind = wasmer_export_kind(export0);

         // Read the export name.
         wasmer_byte_array name_bytes = wasmer_export_name(export0);

         std::string name((char*)name_bytes.bytes, name_bytes.bytes_len);
         printf("Export %d, kind=%d, name=%s\n", i, export_kind, name.c_str());

         //assert(name_bytes.bytes_len == sizeof("sum") - 1);
         //assert(memcmp(name_bytes.bytes, "sum", sizeof("sum") - 1) == 0);
     }
    
     // Destroy the exports.
     wasmer_exports_destroy(exports);
}


inline bool test_call(wasmer_instance_t* instance, const char* func, const wasmer_value_t& i1, const wasmer_value_t& i2, wasmer_value_t& o1)
{
    wasmer_value_t inputs[] = { i1,i2 };
    wasmer_value_t outputs[] = { o1 };
    auto result = wasmer_instance_call(instance, func, inputs, 2, outputs, 1);
    if (result == WASMER_OK) {
        o1 = outputs[0];
        return true;
    }
    else {
        return false;
    }
}

void test_func_calls(wasmer_instance_t* instance)
{
    wasmer_value_t result = wvalue(0);
    auto r = test_call(instance, "add", wvalue(-22), wvalue(2), result);
    assert(r == true);
    assert(result.tag == WASM_I32);
    assert(result.value.I32 == -20);
}

void test_start()
{
     // 1. Read a WebAssembly module from a file.
     //FILE* file = fopen("sum.wasm", "r");
     //FILE *file = fopen("sum-O1.wasm", "r");
     FILE* file = fopen("fibo-wasi-O1.wasm", "r");
     fseek(file, 0, SEEK_END);
     long bytes_length = ftell(file);
     uint8_t *bytes = (uint8_t*) malloc(bytes_length);
     fseek(file, 0, SEEK_SET);
     fread(bytes, 1, bytes_length, file);
     fclose(file);
    
     // 2. Declare the imports (here, none).
     wasmer_import_t imports[10] = {};
    
     // 3. Instantiate the WebAssembly module.
     wasmer_instance_t *instance = NULL;
     wasmer_result_t result = wasmer_instantiate(&instance, bytes, bytes_length, imports, 0);
    
     // 4. Check for errors.
     if (result != WASMER_OK) {
         int error_length = wasmer_last_error_length();
         char *error = (char*) malloc(error_length);
         wasmer_last_error_message(error, error_length);
         printf("%s", error);
         // Do something with `error`…
         return;
     }
    
     test_exports(instance);
     test_func_calls(instance);

     // 5. Free the memory!
     wasmer_instance_destroy(instance);
}


int main()
{
    test_start();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
