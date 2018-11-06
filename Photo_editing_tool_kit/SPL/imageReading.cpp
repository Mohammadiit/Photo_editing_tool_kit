#include<iostream>
#include<string.h>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
#include<fstream>
#include<bitset>
#include<sstream>


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
	
void grayscale(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);
void rotate(int &height,int &width,int &depth ,int &restPartHeader,int degree,int sizeHeaderPart,int sizeHeaderInfo);
void flip(int &height,int &width,int &depth ,int &restPartHeader,char ch,int sizeHeaderPart,int sizeHeaderInfo);
void negative(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);
void sepia(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);
void brightness(int &height,int &width,int &depth ,int &restPartHeader,int brightnessChange,int sizeHeaderPart,int sizeHeaderInfo);
void contrast(int &height,int &width,int &depth ,int &restPartHeader,int contrastValue,int sizeHeaderPart,int sizeHeaderInfo);
void gammaCorrection(int &height,int &width,int &depth ,int &restPartHeader,double gammaValue,int sizeHeaderPart,int sizeHeaderInfo);
void dilation(int &height,int &width,int &depth ,int &restPartHeader,int dilateTime,int sizeHeaderPart,int sizeHeaderInfo);
void erosion(int &height,int &width,int &depth ,int &restPartHeader,int eroseTime,int sizeHeaderPart,int sizeHeaderInfo);
void blur(int &height,int &width,int &depth ,int &restPartHeader,int blurTime,int sizeHeaderPart,int sizeHeaderInfo);
void sobel(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);
void sharpen(int &height,int &width,int &depth ,int &restPartHeader,int sharpTime,int sizeHeaderPart,int sizeHeaderInfo);
void posterization(int &height,int &width,int &depth ,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo);


void write(ofstream &oFile,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo)
{
	char flag[sizeHeaderPart],flag1[sizeHeaderInfo];
	memcpy(flag,&headerPart,sizeHeaderPart);
	memcpy(flag1,&headerInfo,sizeHeaderInfo);
	
	oFile.write(flag,sizeHeaderPart);
	oFile.write(flag1,sizeHeaderInfo);
	oFile.write(restPartOfHeader,restPartHeader);
}

void duplicateHeaderWrite(ofstream &oFile,int &restPartHeader,int sizeHeaderPart,int sizeHeaderInfo)
{
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
	char flag[sizeHeaderPart],flag1[sizeHeaderInfo];
	memcpy(flag,&headerPart,sizeHeaderPart);
	memcpy(flag1,&duplicateHeaderInfo,sizeHeaderInfo);
	
	oFile.write(flag,sizeHeaderPart);
	oFile.write(flag1,sizeHeaderInfo);
	oFile.write(restPartOfHeader,restPartHeader);
}

int calculate(char *array,int length){

	long long ans=0;
	for(int i=0;i<length;i++){
		
		int x= array[i];
		
		if(x<0) x= 256+x;
		
		ans += x*pow(256,i);

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

int getInt()
{
	string input;
	int num;
	while(1)
	{
		getline(cin,input);
		stringstream ss(input);
		if(ss>>num && !(ss>>input))
		{
			return num;
		}
		cout<<"wrong input .Please try again"<<endl;
	}
}

double getDouble()
{
	string input;
	double num;
	while(1)
	{
		getline(cin,input);
		stringstream ss(input);
		if(ss>>num && !(ss>>input))
		{
			return num;
		}
		cout<<"wrong input .Please try again"<<endl;
	}
}

char getChar()
{
	string input;
	char ch;
	while(1)
	{
		getline(cin,input);
		stringstream ss(input);
		if(ss>>ch && !(ss>>input))
		{
			return ch;
		}
		cout<<"wrong input .Please try again"<<endl;
	}
}

void processor(int choice,int &height,int &width,int &depth ,int &restPartHeader)
{
	if(choice ==1)
	{
		cout<<"\nIf you want to rotate the image 90 degree enter 90\nto rotate 180 degree enter 180\nto rotate 270 degree enter 270 ";
		cout<<"\nto rotate 360 degree enter 360\n";
		int degree=getInt();
		
		rotate(height,width,depth,restPartHeader,degree,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else if(choice==2)
	{
		cout<<"To do vertical flip press v to do horizontal flip press h :"<<endl;
		char ch=getChar();
		if(ch=='v' ||ch=='h')
		{
			flip(height,width,depth,restPartHeader,ch,sizeof(headerPart),sizeof(headerInfo));
		}
		else
		{
			cout<<"you enter wrong detection"<<endl;
			
		}

	}
	else if(choice==3)
	{
		negative(height,width,depth,restPartHeader,sizeof(headerPart),sizeof(headerInfo));
	}
	else if(choice==4)
	{
		grayscale(height,width,depth,restPartHeader,sizeof(headerPart),sizeof(headerInfo));
	}
	else if(choice==5)
	{
		sepia(height,width,depth,restPartHeader,sizeof(headerPart),sizeof(headerInfo));
	}
	else if(choice==6)
	{
		cout<<"Enter the amount of brightness change :";
		int brightnessChange=getInt();
		
		cout<<"\n";
		brightness(height,width,depth,restPartHeader,brightnessChange,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else if(choice==7)
	{
		cout<<"Enter the amount of contrast :";
		int contrastValue=getInt();
		cout<<"\n";
		contrast(height,width,depth,restPartHeader,contrastValue,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else if(choice==8)
	{
		cout<<"Enter gammaValue (should be in .01-7.99) :";
		double gammaValue=getDouble();
		cout<<"\n";
		gammaCorrection(height,width,depth,restPartHeader,gammaValue,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else if(choice==9)
	{
		cout<<"Enter how time's to dilate :";
		int dilateTime=getInt();
		
		cout<<"\n";
		dilation(height,width,depth,restPartHeader,dilateTime,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else if(choice==10)
	{
		cout<<"Enter how time's to erose :";
		int eroseTime=getInt();
		cout<<"\n";
		erosion(height,width,depth,restPartHeader,eroseTime,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else if(choice==11)
	{
		cout<<"Enter how time to blur :";
		int blurTime=getInt();
		cout<<"\n";
		blur(height,width,depth,restPartHeader,blurTime,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else if(choice==12)
	{
		sobel(height,width,depth,restPartHeader,sizeof(headerPart),sizeof(headerInfo));
	}
	else if(choice==13)
	{
		posterization(height,width,depth,restPartHeader,sizeof(headerPart),sizeof(headerInfo));
	}
	else if(choice==14)
	{
		cout<<"Enter how time to sharp :";
		int sharpTime=getInt();
		cout<<"\n";
		sharpen(height,width,depth,restPartHeader,sharpTime,sizeof(headerPart),sizeof(headerInfo));
		
	}
	else {
		cout<<"you enter wrong detection"<<endl;
		return;
	}
}

int main()
{
	ifstream iFile;
	int height,width,depth,restPartHeader,choice;
	string fileName;
	
	cout<<"Please enter the name of iamge File:";
	getline(cin,fileName);
	
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
			cout<<"Enter the choce :";
			
			choice=getInt();
			if(choice==0) break;
			processor(choice,height,width,depth,restPartHeader);
		}
		
	}
	else
	{
		cout<<"\nCannot open file\n";
	}

}





















