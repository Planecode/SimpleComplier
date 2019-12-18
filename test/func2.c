int func(int a)
{
	int out = 0;
	for(int i = 0;i < a; i++)
	{
		if((i % 2) == 0)
		{
			continue;
		}
		if(i >= 10)
		{
			break;
		}
		out++;
	}
	return out;
}
int main(){
    int a = 0;
    scan(&a);
    print(func(a));
    return 0;
}
