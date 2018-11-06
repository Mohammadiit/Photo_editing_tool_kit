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

void contrast(int &height,int &width,int &depth ,int &restPartHeader,int contrastValue,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("contrastAdjusted.bmp", ios::binary | ios::out);
	
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
				
				double factor = (259.0 * (contrastValue + 255.0)) / (255.0 * (259.0 - contrastValue));
				
				duplicateImageData[i][j][0]=truncate((int)(factor * (x1   - 128) + 128));
				if(depth==3)
				{
					double x2=(unsigned char)duplicateImageData[i][j][1];
					double x3=(unsigned char)duplicateImageData[i][j][2];
					duplicateImageData[i][j][1]=truncate((int)(factor * (x2   - 128) + 128));
					duplicateImageData[i][j][2]=truncate((int)(factor * (x3   - 128) + 128));
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
