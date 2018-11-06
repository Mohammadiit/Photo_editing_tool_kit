#include<iostream>
#include<string.h>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
#include<fstream>
#include<bitset>


using namespace std;

int truncate(int value);
void write(ofstream &oFile,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);

void posterization(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("posterization.bmp", ios::binary | ios::out);
	
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
		for(int i=0;i<height;++i)
		{
			for(int j=0;j<width;++j)
			{
				int value[8];
				for(int k=0;k<8;++k)
				{
					value[k]=(((k+1)*32-1)+((k+1)*32-15)+1)/2;
				}
				int x1=(unsigned char)duplicateImageData[i][j][0];
				int x2=(unsigned char)duplicateImageData[i][j][1];
				int x3=(unsigned char)duplicateImageData[i][j][2];
				
				x1=x1/32;
				x2=x2/32;
				x3=x3/32;
				
				duplicateImageData[i][j][0]=(unsigned char)truncate(value[x1]);
				duplicateImageData[i][j][1]=(unsigned char)truncate(value[x2]);
				duplicateImageData[i][j][2]=(unsigned char)truncate(value[x3]);
			}
		}
		for(int i=0;i<height;++i)     
		{
			for(int j=0;j<width;++j)
			{
				oFile.write(duplicateImageData[i][j],depth);
			}
		}
		
		oFile.close();
	}
	
	else cout <<"can't open bmp1"<<endl;
}
