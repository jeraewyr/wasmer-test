/**
 * em++ -O1 fibo.cpp -o fibo.wasm --no-entry -s EXPORTED_FUNCTIONS=["_fibo"] && wasmer inspect fibo.wasm
 */


#include <stack>

extern "C"
uint8_t* fibo(int max) {
    std::stack<uint8_t> stack;

    uint8_t p = 1;
    uint8_t q = 1;
    stack.push(p);
    stack.push(q);

    while (true) {
        uint8_t r = p + q;
        if (r > max) break;
        stack.push(r);
        p = q;
        q = r;
    }

    int count = stack.size();
    stack.push(count);

    uint8_t* result = new uint8_t[stack.size()];
    int i = 0;
    while (stack.size() > 0) {
        result[i++] = stack.top();
        stack.pop();
    }
    return result;
}