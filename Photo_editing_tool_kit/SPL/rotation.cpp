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
void duplicateHeaderWrite(ofstream &oFile,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);
		
void rotate1(ofstream &oFile,int &height,int &width,int &depth ,int &restPartHeader,int &degree,int sizeHeaderPart,int sizeHeaderInfo)
{
	extern char ***imageData;
	
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
	
	duplicateHeaderWrite(oFile,restPartHeader,sizeHeaderPart,sizeHeaderInfo);
	
	if(degree==90)
	{
		for(int i=width-1;i>=0;--i)     //rotate 90degree
		{
			for(int j=0;j<height;++j)
			{
				oFile.write(duplicateImageData[j][i],depth);
			}
		}
		
		oFile.close();
	}
	else if(degree==270)
	{
		for(int i=0;i<width;++i)     //rotate 270degree
		{
			for(int j=height-1;j>=0;--j)
			{
				oFile.write(duplicateImageData[j][i],depth);
			}
		}
		
		oFile.close();
	}
}
		
void rotate(int &height,int &width,int &depth ,int &restPartHeader,int degree,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("rotated.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
		
		extern char ***imageData;
		
		if(degree==90||degree==270)
		{
			rotate1(oFile,height,width,depth,restPartHeader,degree,sizeHeaderPart,sizeHeaderInfo);
		}
		else if(degree==180||degree==360)
		{
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
			
			write(oFile,restPartHeader,sizeHeaderPart,sizeHeaderInfo);
			
			if(degree==180)
			{
				for(int i=height-1;i>=0;--i)
				{
					for(int j=0;j<width;++j)
					{
						oFile.write(duplicateImageData[i][j],depth);
					}
				}
			}
			else if(degree==360)
			{
				for(int i=0;i<height;++i)     
				{
					for(int j=0;j<width;++j)
					{
						oFile.write(duplicateImageData[i][j],depth);
					}
				}
			}
			else
			{
				cout<<"Wrong input "<<endl;
				return;
			}
		
			oFile.close();
		}
		else 
		{
			cout<<"You enter wrong input"<<endl;
		}
	}
	else 
	{
		cout<<"You enter wrong input"<<endl;
	}
}








