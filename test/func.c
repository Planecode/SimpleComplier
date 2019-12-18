int factor(int a)
{
    if(a == 1)
        return 1;
    return factor(a - 1) * a;
}
int main(){
    int a = 4;
    scan(&a);
    print(factor(a));
    return 0;
}
