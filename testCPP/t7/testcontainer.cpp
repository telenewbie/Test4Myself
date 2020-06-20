// 这个是用来测试 容器类的，但是不止于此，还要深入了解每个 容器具体的实现

// 目前了解到 vector  list  array forword_list dequeue
// 有序容器 顺序容器 无序容器
// set map unorderedset 等

#include <vector>
#include <list>
#include <array>
//#include <forword_list>
//#include <dequeue>

#include <iostream>//#include <stdio.h>
using namespace std;
#pragma pack (4)
int main(int argc,char* argv[])
{
    if(argc<2){
       cout<<"input more than 2 params"<<endl;
        return -1;
    }
    std::cout<<argv[1]<<std::endl;

    typedef struct WAVE_HEADER{  
		char         fccID[4];        
		unsigned   long    dwSize;            
		char         fccType[4];    
	}WAVE_HEADER;
    typedef struct WAVE_FMT{  
		char         fccID[4];        
		unsigned   long       dwSize;            
		unsigned   short     wFormatTag;    
		unsigned   short     wChannels;  
		unsigned   long       dwSamplesPerSec;  
		unsigned   long       dwAvgBytesPerSec;  
		unsigned   short     wBlockAlign;  
		unsigned   short     uiBitsPerSample;  
	}WAVE_FMT;  
 
	typedef struct WAVE_DATA{  
		char       fccID[4];          
		unsigned long dwSize;              
	}WAVE_DATA;
    
    cout<<sizeof(WAVE_HEADER)<<endl;
    cout<<sizeof(WAVE_FMT)<<endl;
    cout<<sizeof(WAVE_DATA)<<endl;

}
#pragma pack ()