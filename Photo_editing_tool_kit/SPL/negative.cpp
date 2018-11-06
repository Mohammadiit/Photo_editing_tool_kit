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

void negative(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("negative.bmp", ios::binary | ios::out);
	
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
				double x2=(unsigned char)duplicateImageData[i][j][1];
				double x3=(unsigned char)duplicateImageData[i][j][2];
				
				duplicateImageData[i][j][0]=255-x1;
				duplicateImageData[i][j][1]=255-x2;
				duplicateImageData[i][j][2]=255-x3;
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
	else
	{
		cout<<"file not open"<<endl;
	}
}







