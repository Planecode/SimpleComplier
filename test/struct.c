int main(){
int a[6] = {2, 3, 4, 5, 6, 7};
int *c = a;
*(c+2) = 10;
print(a[2]);
}