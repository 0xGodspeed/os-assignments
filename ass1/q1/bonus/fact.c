#include <stdio.h>

int fact(int n) {
    int ans = 1;
    for (int i = 1; i<=n; i++) {
        ans = ans * i;
    }
    return ans;
}

int main() {
    printf("factorial of 4: %d", fact(4));
}