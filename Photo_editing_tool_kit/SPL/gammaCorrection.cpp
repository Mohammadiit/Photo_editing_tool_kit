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

void gammaCorrection(int &height,int &width,int &depth ,int &restPartHeader,double gammaValue,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("gammaAdjusted.bmp", ios::binary | ios::out);
	
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
			
				double gammaCorrection = 1 / gammaValue;
				duplicateImageData[i][j][0]= 255 * pow((x1   / 255) ,gammaCorrection);
				if(depth==3)
				{
					double x2=(unsigned char)duplicateImageData[i][j][1];
					double x3=(unsigned char)duplicateImageData[i][j][2];
					duplicateImageData[i][j][1]= 255 * pow((x2   / 255) , gammaCorrection);
					duplicateImageData[i][j][2]= 255 * pow((x3   / 255) , gammaCorrection);
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
