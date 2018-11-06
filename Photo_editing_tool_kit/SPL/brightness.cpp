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

void brightness(int &height,int &width,int &depth ,int &restPartHeader,int brightnessChange,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("brightnessAdjusted.bmp", ios::binary | ios::out);
	
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
				double x1=(unsigned char)duplicateImageData[i][j][0];
				
				duplicateImageData[i][j][0]=truncate(x1+brightnessChange);
				if(depth==3)
				{
					double x2=(unsigned char)duplicateImageData[i][j][1];
					double x3=(unsigned char)duplicateImageData[i][j][2];
					duplicateImageData[i][j][1]=truncate(x2+brightnessChange);
					duplicateImageData[i][j][2]=truncate(x3+brightnessChange);
				}
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

