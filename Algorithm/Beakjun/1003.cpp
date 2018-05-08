//boj 1003 �Ǻ���ġ �Լ�

/*

- program thinking

�̹��� for���� ���� Ǭ��.

0�� 1��°�� ���� �����ϰ�, �� �� �迭�� �����ϴ� ������ DP �˰��� ������ �Ѵ�.

�޸����̼�...����

Bottom-Up
*/

#include<iostream>

using namespace std;

int testcase;
int zero[41];
int one[41];

void fibo_count(int a)
{
	if (zero[a] || one[a]) return;

	for (int i = 2; i <= a; i++)
	{
		zero[i] = zero[i - 1] + zero[i - 2];
		one[i] = one[i - 1] + one[i - 2];
	}
}

int main()
{
	ios_base::sync_with_stdio(false);

	int testnum;
	cin >> testnum;

	//ó�� �ʱ�ȭ
	zero[0] = 1;
	one[1] = 1;

	for (int i = 1; i <= testnum; i++)
	{
		cin >> testcase;
		fibo_count(testcase);
		cout << zero[testcase] << " " << one[testcase]<<endl;
	}

	return 0;
}