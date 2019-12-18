int main() {
    int a, b = 1;
    a = b + 1; // pay attention to operation priority
       if (a != 0) {
        while (a < 10 && b <= 10) {
            a = a * b;
            b = b + 2;
            if (a == 2 || a >= 3) {
                a = a + 1;
            }
        }
        for (int i = 0; i < 1; i++) {
            i = b;
        }
    }
    int c;
    scan(&c);
    int d;
    d = a+c;
    print(d * 10/9);
    return 0;
}