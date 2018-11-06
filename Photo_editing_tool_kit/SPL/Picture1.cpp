#include<iostream>
#include<string.h>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
#include<fstream>
#include<bitset>

using namespace std;

struct headerBmp{
	
	char signature[2];
	char fullFileSize[4];
	char reserved[2];
	char reserved1[2];
	char offsetToStartImage[4];
	}headerPart;
	
struct infoHeader{

	char infoHeaderSize[4];
	char imageWidth[4];
	char imageHeight[4];
	char numberOfPlanes[2];
	char numberOfBits[2];
	char compression[4];
	char sizeOfImageData[4];
	char horizontalResolution[4];
	char verticalResolution[4];
	char numberOfColor[4];
	char numberOfImportantColor[4];
	}headerInfo,duplicateHeaderInfo;
	
	char *restPartOfHeader,***imageData;
	
long long calculate(char *array,int length){
	
	
	int k=0;
	long long ans=0;
	for(int i=0;i<length;i++){
	
		bitset<8> binary(array[i]);
		
		//cout << binary << endl;
		
		for(int j=0;j<binary.size();j++){
		
			if(binary.test(j)==true) {
			
				ans += pow(2,k);
				//cout << k << endl;
				
				}
			k++;
		}
		
	}

	return ans;
}

int truncate(int value)
{
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}

	
void readHeaderPart(ifstream &iFile)
{
	iFile.read(headerPart.signature,2);	
	iFile.read(headerPart.fullFileSize,4);
	iFile.read(headerPart.reserved,2);
	iFile.read(headerPart.reserved1,2);
	iFile.read(headerPart.offsetToStartImage,4);
	
	
	iFile.read(headerInfo.infoHeaderSize,4);
	iFile.read(headerInfo.imageWidth,4);
	iFile.read(headerInfo.imageHeight,4);
	iFile.read(headerInfo.numberOfPlanes,2);
	iFile.read(headerInfo.numberOfBits,2);
	iFile.read(headerInfo.compression,4);
	iFile.read(headerInfo.sizeOfImageData,4);
	iFile.read(headerInfo.horizontalResolution,4);
	iFile.read(headerInfo.verticalResolution,4);
	iFile.read(headerInfo.numberOfColor,4);
	iFile.read(headerInfo.numberOfImportantColor,4);
	
}
	
void readImagePart(ifstream &iFile,int &height,int &width,int &depth ,int &restPartHeader)
{
	height=calculate(headerInfo.imageHeight,4);
	width=calculate(headerInfo.imageWidth,4);
	depth= calculate(headerInfo.numberOfBits,2) /8;
	
	restPartHeader=calculate(headerPart.offsetToStartImage,4)-54;
	
	imageData = new char **[height];
	for(int i=0;i<height;++i)
	{
		imageData[i]=new char*[width];
		for(int j=0;j<width;++j)
		{
			imageData[i][j]=new char[depth];
		}
	}
	restPartOfHeader =new char [restPartHeader];
	
	iFile.read(restPartOfHeader,restPartHeader);
	
	for(int i=0;i<height;++i)
	{
		for(int j=0;j<width;++j)
		{
			iFile.read(imageData[i][j],depth);
		}
	}
}

void rotate1(ofstream &oFile,int &height,int &width,int &depth ,int &restPartHeader,int &degree)
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
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.infoHeaderSize[i]=headerInfo.infoHeaderSize[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.imageWidth[i]=headerInfo.imageWidth[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.imageHeight[i]=headerInfo.imageHeight[i];
	}
	for(int i=0;i<2;++i)
	{
		duplicateHeaderInfo.numberOfPlanes[i]=headerInfo.numberOfPlanes[i];
	}
	for(int i=0;i<2;++i)
	{
		duplicateHeaderInfo.numberOfBits[i]=headerInfo.numberOfBits[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.compression[i]=headerInfo.compression[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.sizeOfImageData[i]=headerInfo.sizeOfImageData[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.horizontalResolution[i]=headerInfo.horizontalResolution[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.verticalResolution[i]=headerInfo.verticalResolution[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.numberOfColor[i]=headerInfo.numberOfColor[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.numberOfImportantColor[i]=headerInfo.numberOfImportantColor[i];
	}
	char swap[4];
		
	//a=bmpHeaderInfo.widthOfImagePixel;
	for(int i=0;i<4;++i)
	{
		swap[i]=duplicateHeaderInfo.imageWidth[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.imageWidth[i]=duplicateHeaderInfo.imageHeight[i];
	}
	for(int i=0;i<4;++i)
	{
		duplicateHeaderInfo.imageHeight[i]=swap[i];
	}
	char flag[sizeof(headerPart)],flag1[sizeof(duplicateHeaderInfo)];
	memcpy(flag,&headerPart,sizeof(headerPart));
	memcpy(flag1,&duplicateHeaderInfo,sizeof(duplicateHeaderInfo));
	
	oFile.write(flag,sizeof(headerPart));
	oFile.write(flag1,sizeof(duplicateHeaderInfo));
	oFile.write(restPartOfHeader,restPartHeader);
	
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
		
		
void rotate(int &height,int &width,int &depth ,int &restPartHeader,int degree)
{
	ofstream oFile;
	oFile.open("rotated.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		if(degree==90||degree==270)
		{
			rotate1(oFile,height,width,depth,restPartHeader,degree);
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
			char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
			memcpy(flag,&headerPart,sizeof(headerPart));
			memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
			oFile.write(flag,sizeof(headerPart));
			oFile.write(flag1,sizeof(headerInfo));
			oFile.write(restPartOfHeader,restPartHeader);
		
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
	}
	else 
	{
		cout<<"You enter wrong input"<<endl;
	}
}

void flip(int &height,int &width,int &depth ,int &restPartHeader,char ch)
{
	ofstream oFile;
	oFile.open("flipped.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
		
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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
		
void negative(int &height,int &width,int &depth ,int &restPartHeader)
{
	ofstream oFile;
	oFile.open("negative.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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

void grayscale(int &height,int &width,int &depth ,int &restPartHeader)
{
	ofstream oFile;
	oFile.open("grayscale.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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
				
				double average=(x1+x2+x3)/3;
				
				duplicateImageData[i][j][0]=average;
				duplicateImageData[i][j][1]=average;
				duplicateImageData[i][j][2]=average;
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

void sepia(int &height,int &width,int &depth ,int &restPartHeader)
{
	ofstream oFile;
	oFile.open("sepia.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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

void brightness(int &height,int &width,int &depth ,int &restPartHeader,int brightnessChange)
{
	ofstream oFile;
	oFile.open("brightnessAdjusted.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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

void contrast(int &height,int &width,int &depth ,int &restPartHeader,int contrastValue)
{
	ofstream oFile;
	oFile.open("contrastAdjusted.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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

void gammaCorrection(int &height,int &width,int &depth ,int &restPartHeader,double gammaValue)
{
	ofstream oFile;
	oFile.open("gammaAdjusted.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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

void erosion(int &height,int &width,int &depth ,int &restPartHeader,int eroseTime)
{
	ofstream oFile;
	oFile.open("erosion.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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
					//cout<<k<<"\t"<<height<<endl;
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
void dilation(int &height,int &width,int &depth ,int &restPartHeader,int dilateTime)
{
	ofstream oFile;
	oFile.open("dilation.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
	char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
	memcpy(flag,&headerPart,sizeof(headerPart));
	memcpy(flag1,&headerInfo,sizeof(headerInfo));
	
	oFile.write(flag,sizeof(headerPart));
	oFile.write(flag1,sizeof(headerInfo));
	oFile.write(restPartOfHeader,restPartHeader);
	
	
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
		paddingArray[0][i]=0;
		paddingArray[height+1][i]=0;
	}
	for(int i=0;i<(height+2);++i)
	{
		paddingArray[i][0]=0;
		paddingArray[i][width+1]=0;
	}
	for(int time=0;time<dilateTime;++time)
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
				double max1=0;
				for(int i=k;i<(k+3);++i)
				{
					for(int j=l;j<(l+3);++j)
					{
						double x1=(unsigned char)paddingArray[i][j];
						max1=max(max1,x1);
					}
				}
				duplicateImageData[k][l][0]=max1;
				
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
					double max1=0;
					for(int i=k;i<(k+3);++i)
					{
						for(int j=l;j<(l+3);++j)
						{
							double x1=(unsigned char)paddingArray[i][j];
							max1=max(max1,x1);
						}
					}
					duplicateImageData[k][l][1]=max1;
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
					double max1=0;
					for(int i=k;i<(k+3);++i)
					{
						for(int j=l;j<(l+3);++j)
						{
							double x1=(unsigned char)paddingArray[i][j];
							max1=max(max1,x1);
							
						}
					}
					duplicateImageData[k][l][2]=max1;
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

void blur(int &height,int &width,int &depth ,int &restPartHeader,int blurTime)
{
	
	ofstream oFile;
	oFile.open("blur.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
		
		
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
			paddingArray[0][i]=0;
			paddingArray[height+1][i]=0;
		}
		for(int i=0;i<(height+2);++i)
		{
			paddingArray[i][0]=0;
			paddingArray[i][width+1]=0;
		}
		for(int time=0;time<blurTime;++time)
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
					double sum=0;
					for(int i=k;i<(k+3);++i)
					{
						for(int j=l;j<(l+3);++j)
						{
							double x1=(unsigned char)paddingArray[i][j];
							sum=sum+x1;
						}
					}
					duplicateImageData[k][l][0]=(sum/9);
					
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
						double sum=0;
						for(int i=k;i<(k+3);++i)
						{
							for(int j=l;j<(l+3);++j)
							{
								double x1=(unsigned char)paddingArray[i][j];
								sum=sum+x1;
							}
						}
						duplicateImageData[k][l][1]=(sum/9);
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
						double sum=0;
						for(int i=k;i<(k+3);++i)
						{
							for(int j=l;j<(l+3);++j)
							{
								double x1=(unsigned char)paddingArray[i][j];
								sum=sum+x1;
								
							}
						}
						duplicateImageData[k][l][2]=(sum/9);
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

void sharpen(int &height,int &width,int &depth ,int &restPartHeader,int sharpTime)
{
	
	ofstream oFile;
	oFile.open("sharp.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
		
		
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
			paddingArray[0][i]=0;
			paddingArray[height+1][i]=0;
		}
		for(int i=0;i<(height+2);++i)
		{
			paddingArray[i][0]=0;
			paddingArray[i][width+1]=0;
		}
		for(int time=0;time<sharpTime;++time)
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
					double sum=0;
					for(int i=k;i<(k+3);++i)
					{
						for(int j=l;j<(l+3);++j)
						{
							double x1=(unsigned char)paddingArray[i][j];
							if((i-k)==1&&(j-l)==1)
							{
								x1=x1*24;
								sum=sum+x1;
							}
							else
							{
								x1=x1*(-2);
								sum=sum+x1;
							}
						}
					}
					duplicateImageData[k][l][0]=truncate((sum/8));
					
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
						double sum=0;
						for(int i=k;i<(k+3);++i)
						{
							for(int j=l;j<(l+3);++j)
							{
								double x1=(unsigned char)paddingArray[i][j];
								if((i-k)==1&&(j-l)==1)
								{
									x1=x1*24;
									sum=sum+x1;
								}
								else
								{
									x1=x1*(-2);
									sum=sum+x1;
								}
							}
						}
						duplicateImageData[k][l][1]=truncate((sum/8));
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
						double sum=0;
						for(int i=k;i<(k+3);++i)
						{
							for(int j=l;j<(l+3);++j)
							{
								double x1=(unsigned char)paddingArray[i][j];
								if((i-k)==1&&(j-l)==1)
								{
									x1=x1*24;
									sum=sum+x1;
								}
								else
								{
									x1=x1*(-2);
									sum=sum+x1;
								}
								
							}
						}
						duplicateImageData[k][l][2]=truncate((sum/8));
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

void posterization(int &height,int &width,int &depth ,int &restPartHeader)
{
	ofstream oFile;
	oFile.open("posterization.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
	
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
		
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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

void sobel(int &height,int &width,int &depth ,int &restPartHeader)
{
	ofstream oFile;
	oFile.open("sobel.bmp", ios::binary | ios::out);
	
	if(oFile.is_open()){
		char flag[sizeof(headerPart)],flag1[sizeof(headerInfo)];
		memcpy(flag,&headerPart,sizeof(headerPart));
		memcpy(flag1,&headerInfo,sizeof(headerInfo));
	
		oFile.write(flag,sizeof(headerPart));
		oFile.write(flag1,sizeof(headerInfo));
		oFile.write(restPartOfHeader,restPartHeader);
	
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
			
void processor(int choice,int &height,int &width,int &depth ,int &restPartHeader)
{
	if(choice ==1)
	{
		cout<<"\nIf you want to rotate the image 90 degree enter 90\nto rotate 180 degree enter 180\nto rotate 270 degree enter 270 ";
		cout<<"\nto rotate 360 degree enter 360\n";
		int degree;
		cin>>degree;
		rotate(height,width,depth,restPartHeader,degree);
	}
	else if(choice==2)
	{
		cout<<"To do vertical flip press v to do horizontal flip press h"<<endl;
		char ch;
		cin>>ch;
		flip(height,width,depth,restPartHeader,ch);
	}
	else if(choice==3)
	{
		negative(height,width,depth,restPartHeader);
	}
	else if(choice==4)
	{
		grayscale(height,width,depth,restPartHeader);
	}
	else if(choice==5)
	{
		sepia(height,width,depth,restPartHeader);
	}
	else if(choice==6)
	{
		cout<<"Enter the amount of brightness change\n";
		int brightnessChange;
		cin>>brightnessChange;
		brightness(height,width,depth,restPartHeader,brightnessChange);
	}
	else if(choice==7)
	{
		cout<<"Enter the amount of contrast \n";
		int contrastValue;
		cin>>contrastValue;
		contrast(height,width,depth,restPartHeader,contrastValue);
	}
	else if(choice==8)
	{
		cout<<"Enter gammaValue (should be in .01-7.99)\n";
		double gammaValue;
		cin>>gammaValue;
		gammaCorrection(height,width,depth,restPartHeader,gammaValue);
	}
	else if(choice==9)
	{
		cout<<"Enter how time's to dilate\n";
		int dilateTime;
		cin>>dilateTime;
		dilation(height,width,depth,restPartHeader,dilateTime);
	}
	else if(choice==10)
	{
		cout<<"Enter how time's to erose\n";
		int eroseTime;
		cin>>eroseTime;
		erosion(height,width,depth,restPartHeader,eroseTime);
	}
	else if(choice==11)
	{
		cout<<"Enter how time to blur\n";
		int blurTime;
		cin>>blurTime;
		blur(height,width,depth,restPartHeader,blurTime);
	}
	else if(choice==12)
	{
		sobel(height,width,depth,restPartHeader);
	}
	else if(choice==13)
	{
		posterization(height,width,depth,restPartHeader);
	}
	else if(choice==14)
	{
		cout<<"Enter how time to sharp\n";
		int sharpTime;
		cin>>sharpTime;
		sharpen(height,width,depth,restPartHeader,sharpTime);
	}
	else {
		cout<<"you enter wrong detection"<<endl;
		return;
	}
}
main()
{
	ifstream iFile;
	int height,width,depth,restPartHeader,choice;
	string fileName;
	
	cout<<"Please enter the name of iamge File:";
	cin>>fileName;;
	
	cout<<endl;
	
	iFile.open(fileName.c_str(), ios::binary | ios::in);
	if(iFile.is_open())
	{
		readHeaderPart(iFile);
		readImagePart(iFile,height,width,depth,restPartHeader);
		for(;;)
		{
			cout<<"\nIf you want to do raotate operation press 1\nto do flip image press 2\nto do photo negative press 3\nto do grayscale ";
			cout<<"press 4  (only RGB)";
			cout<<"\nto do sepia press 5  (only RGB) \nto change brightness press 6\nto do contrast press 7 \nto do gamma correction press 8";
			cout<<"\nto do dilation press 9\nto do erosion press 10\nto do blur press 11\nto do sobel press 12\nto do posterization ";
			cout<<"press 13 \nto do sharpen press 14\nto stop the process press 0\n\n";
			cout<<"Enter the choce";
			cin>>choice;
			if(choice==0) break;
			processor(choice,height,width,depth,restPartHeader);
		}
		
	}
	else{
		
		cout<<"\nCannot open file\n";
		}

}

































