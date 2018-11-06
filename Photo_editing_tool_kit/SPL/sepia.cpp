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

void sepia(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("sepia.bmp", ios::binary | ios::out);
	
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
				
				double tb=0.393*x1+.769*x2+.189*x3;
				double tg=0.349*x1+.686*x2+.168*x3;
				double tr=0.272*x1+.534*x2+.131*x3;
				
				tr=truncate(tr);
				tg=truncate(tg);
				tb=truncate(tb);
				
				duplicateImageData[i][j][0]=tr;
				duplicateImageData[i][j][1]=tg;
				duplicateImageData[i][j][2]=tb;
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
