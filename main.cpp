#include <iostream>
#include "kfifo.h"
#define SIZE 31
int main() {
    kfifo_t *fifo = kfifo_alloc(1024, nullptr);
    char p[SIZE]="tttbdewujfbwafba.wfbguaefif";
    char o[SIZE]={0};
    while(1) {
        kfifo_put(fifo, (unsigned char *) p, sizeof(p));
        kfifo_get(fifo, (unsigned char *) o, sizeof(o));
        std::cout << "Hello, World!" << o << std::endl;
    }
    return 0;
}
