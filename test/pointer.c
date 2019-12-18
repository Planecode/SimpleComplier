int main(){
    int a = 55;
    int *p;
	p = &a;
    print(*p);
	
	int t[3] = {1, 2, 3};
	p = t;
	*(p+1) = 4;
	print(t[1]);
    return 0;
}
