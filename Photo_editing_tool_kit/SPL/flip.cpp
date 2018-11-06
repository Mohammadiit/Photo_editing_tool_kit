#include<iostream>
#include<string.h>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
#include<fstream>
#include<bitset>


using namespace std;

void write(ofstream &oFile,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);

void flip(int &height,int &width,int &depth ,int &restPartHeader,char ch,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("flipped.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
		
		extern char ***imageData;
		
		write(oFile,restPartHeader,sizeHeaderPart,sizeHeaderInfo);
	
		char duplicateImageData[height][width][depth];
		for(int i=0;i<height;++i)
		{
			for(int j=0;j<width;++j)
			{
				for(int k=0;k<depth;++k)
				{
					duplicateImageData[i][j][k]=imageData[i][j][k];
				}
			}
		}
		if(ch=='v')
		{
			for(int i=height-1;i>=0;--i)
			{
				for(int j=0;j<width;++j)
				{
					oFile.write(duplicateImageData[i][j],depth);
				}
			}
			oFile.close();
		}
		else if(ch=='h')
		{
			for(int i=0;i<height;++i)					//horizontally flipped
			{
				for(int j=width-1;j>=0;--j)
				{
					oFile.write(duplicateImageData[i][j],depth);
				}
			}
			oFile.close();
		}
		else
		{
			cout<<"you enter wrong detection"<<endl;
			return;
		}
		
	}
	else
	{
		cout<<"file not open"<<endl;
	}
}





