struct m{
int a;
int b;
};

int main(){
	struct m a;
	a.a = 2;
	a.b = 2;
	struct m b;
	b.a = a.a;
	b.b = b.a * a.b;
	print(a.b);
	print(b.b);
}