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

void erosion(int &height,int &width,int &depth ,int &restPartHeader,int eroseTime,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("erosion.bmp", ios::binary | ios::out);
	
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
		char paddingArray[height+2][width+2];
		for(int i=0;i<(width+2);++i)
		{
			paddingArray[0][i]=255;
			paddingArray[height+1][i]=255;
		}
		for(int i=0;i<(height+2);++i)
		{
			paddingArray[i][0]=255;
			paddingArray[i][width+1]=255;
		}
		for(int time=0;time<eroseTime;++time)
		{
			
			int k=0,l=0;
			for(int i=1;i<(height+1);++i)
			{
				l=0;
				for(int j=1;j<(width+1);++j)
				{
					paddingArray[i][j]=(unsigned char)duplicateImageData[k][l][0];
					
					++l;
				
				}
				++k;
			}
		
			for(int k=0;k<height;++k)
			{
				for(int l=0;l<width;++l)
				{
					double min1=255;
					for(int i=k;i<(k+3);++i)
					{
						for(int j=l;j<(l+3);++j)
						{
							double x1=(unsigned char)paddingArray[i][j];
							min1=min(min1,x1);
						}
					}
					duplicateImageData[k][l][0]=min1;
					
				}
			}
			if(depth==3)
			{
				int k=0,l=0;
				for(int i=1;i<(height+1);++i)
				{
					l=0;
					for(int j=1;j<(width+1);++j)
					{
						paddingArray[i][j]=(unsigned char)duplicateImageData[k][l][1];
						
						++l;
				
					}
					++k;
				}
		
				for(int k=0;k<height;++k)
				{
					for(int l=0;l<width;++l)
					{
						double min1=255;
						for(int i=k;i<(k+3);++i)
						{
							for(int j=l;j<(l+3);++j)
							{
								double x1=(unsigned char)paddingArray[i][j];
								min1=min(min1,x1);
							}
						}
						duplicateImageData[k][l][1]=min1;
					}
				}
				k=0,l=0;
				for(int i=1;i<(height+1);++i)
				{
					l=0;
					for(int j=1;j<(width+1);++j)
					{
						paddingArray[i][j]=(unsigned char)duplicateImageData[k][l][2];
						
						++l;
				
					}
					++k;
				}
		
				for(int k=0;k<height;++k)
				{
					for(int l=0;l<width;++l)
					{
						double min1=255;
						for(int i=k;i<(k+3);++i)
						{
							for(int j=l;j<(l+3);++j)
							{
								double x1=(unsigned char)paddingArray[i][j];
								min1=min(min1,x1);	
							}
						}
						duplicateImageData[k][l][2]=min1;
					}
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
