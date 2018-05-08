//boj 1003 피보나치 함수

/*

- program thinking

이번에 for문을 통해 푼다.

0과 1번째는 직접 저장하고, 그 뒤 배열에 저장하는 식으로 DP 알고리즘 구현을 한다.

메모리제이션...구현

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

	//처음 초기화
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