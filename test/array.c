int g[10][20];
int main(){
	g[1][9] = 3;
    int c =3;
	int b[5] = {1, 2, 3};
	print(b[1]);
	print(b[3]);
    int a[3][4];
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			a[i][j] = i * j;
		}
	}
   print(a[2][3]);
   print(g[1][9]);
    return 0;
}
