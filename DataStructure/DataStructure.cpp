#include <iostream>
int iqueue[10];
int ifront;
int iindex;

void init()
{
	ifront = 0;
	iindex = ifront;
}
bool PushQueue(int itemp)
{
	if ((iindex + 1) % 10 == ifront)
	{
		printf("\n xxxxxxxxxoverxxxxxxxxxx");
		return false;
	}
	iqueue[iindex] = itemp;
	iindex = iindex++ % 10;
	return true;
}
int PopQueue(int itemp)
{
	if (iindex == ifront)
	{
		printf("\n xxxxxxxxxxemptyxxxxxxxxxxxx");
		return -1;
	}
	int i = iqueue[ifront];
	ifront = ++ifront % 10;
	return i;
}
int main()
{

	return 0;
}
