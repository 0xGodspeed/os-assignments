#include <stdio.h>

int a = 0, b = 1, c = 1;
void fib(int n) {
    int a = 0;
    int b = 1;
    int c;
    if (n == 1) printf("0");
    if (n == 2) printf("0 1");
    printf("0 1 ");
    for (int i = 0; i < n-2; i++) {
        c = a + b; 
        printf("%d ", c);
        a = b;
        b = c;
    }
}

int main() {
    fib(16);
    printf("\n");
}