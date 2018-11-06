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

void sobel(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo)
{
	ofstream oFile;
	oFile.open("sobel.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
		/*extern struct headerBmp headerPart;
		
		extern struct infoHeader headerInfo;*/
		
		extern char ***imageData;
		
		//write(oFile,restPartHeader);
		write(oFile,restPartHeader,sizeHeaderPart,sizeHeaderInfo);
		/*char flag[sizeHeaderPart],flag1[sizeHeaderInfo];
		memcpy(flag,&headerPart,sizeHeaderPart);
		memcpy(flag1,&headerInfo,sizeHeaderInfo);
		
		oFile.write(flag,sizeHeaderPart);
		oFile.write(flag1,sizeHeaderInfo);
		oFile.write(restPartOfHeader,restPartHeader);*/
	
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
		if(depth==3)
		{
			for(int i=0;i<height;++i)
			{
				for(int j=0;j<width;++j)
				{
					double x1=(unsigned char)duplicateImageData[i][j][0];
					double x2=(unsigned char)duplicateImageData[i][j][1];
					double x3=(unsigned char)duplicateImageData[i][j][2];
			
					double average=(x1+x2+x3)/3;
			
					duplicateImageData[i][j][0]=average;
					duplicateImageData[i][j][1]=average;
					duplicateImageData[i][j][2]=average;
				}
			}
		}
	
		char paddingArray[height+2][width+2];
		for(int i=0;i<(width+2);++i)
		{
			paddingArray[0][i]=0;
			paddingArray[height+1][i]=0;
		}
		for(int i=0;i<(height+2);++i)
		{
			paddingArray[i][0]=0;
			paddingArray[i][width+1]=0;
		}
	

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
				double flag=0,z1,z2,z3,z4,z6,z7,z8,z9;
					for(int i=k;i<(k+3);++i)
					{
						for(int j=l;j<(l+3);++j)
						{
							++flag;
							double x1=(unsigned char)paddingArray[i][j];
							if(flag==1) z1=x1;
							else if(flag==2) z2=x1;
							else if(flag==3) z3=x1;
							else if(flag==4) z4=x1;
							else if(flag==6) z6=x1;
							else if(flag==7) z7=x1;
							else if(flag==8) z8=x1;
							else if(flag==9) z9=x1;
							
						}
					}
					double Gx=(z7+(2*z8)+z9)-(z1+(2*z2)+z3);
					double Gy=(z3+(2*z6)+z9)-(z1+(2*z4)+z7);
					Gx=Gx*Gx;Gy=Gy*Gy;
					Gx=sqrt(Gx+Gy);
					//Gx=truncate(Gx);
					duplicateImageData[k][l][0]=(unsigned char)Gx;
				
					duplicateImageData[k][l][1]=(unsigned char)Gx;
					duplicateImageData[k][l][2]=(unsigned char)Gx;
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
