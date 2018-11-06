#include<iostream>
#include<stdio.h>
#include<sstream>

using namespace std;

int getNum()
{
	string s1;
	int num;
	while(1)
	{
		getline(cin,s1);
		stringstream ss(s1);
		if(ss>>num && !(ss>>s1))
		{
			return num;
		}
		else
		{
			cout<<"wrong input .Please try again "<<endl;
		}	
	}
}

int main()
{
	int i,k;
	string s1;
	getline(cin,s1);
	for(int j=0;j<5;++j)
	{
		i=getNum();
		cout<<i<<endl;
	}
}
