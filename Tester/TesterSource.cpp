#include <iostream>
using namespace std;
static int h = 0;
int FindOdd(int A)
{
	h++;
	if (A % 2 == 0)
	{
		A = A / 2;
		
		return FindOdd(A);
		
	}
	

}

void main()
{
	int A = 24;
	cout << FindOdd(A);
	cout << "\t" << h;
	getchar();
}