int wrongplus(int a)
{
	if(a == 1)
		return 1;
    return wrongplus(a - 1) * a;
}
int main(){
    int a = 4;
	input(&a);
    print(wrongplus(a));
    return 0;
}
