#include <cstdio>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

#define MAX 150000

map<string , int >word;

string splitStr(char* str, int p, int num)
{
	char* subStr=new char[num];
	for(int i=p;i<p+num-1;i++)
		subStr[i-p]=str[i];
	subStr[num-1]='\0';
	string result=subStr;
	delete[] subStr;
	return result;
}

int main(int argc, char *argv[])
{
    	if (argc != 3) {
        	fprintf(stderr, "usage: %s input output\n", argv[0]);
        	return 1;
   	}   
      
    	char* infile=argv[1];
    	char* outfile=argv[2];
    	int len;
    	string md5Val;
	double begin, end, diff;

	
	ifstream textFile(infile);
	ofstream outFile(outfile);
	string line;
    
	ostringstream oss;

	if(!textFile)
	{
		cout<<"cannot open input file"<<endl;
		return 0;
	}
	if(!outFile)
	{
		cout<<"cannot open output file"<<endl;
		return 0;
	}

	int lineNum=0;
	while(getline(textFile,line))
	{ 	
		lineNum++;
		istringstream iss(line);
    		do
    		{
        		string sub;
        		iss >> sub;
			if( (sub[0]>='a' && sub[0]<='z') || (sub[0]>='A' && sub[0]<='Z')){
				if(!word.empty() && word.find(sub) != word.end())
					word[sub] += 1;
				else
					word.insert(pair<string,int>(sub,1));
			}
    		} while (iss);

		map<string,int>::iterator iter=word.begin();
		for(;iter!=word.end();iter++){
			outFile<<iter->first<<" "<<iter->second<<" "; 
		}
		
		outFile<<endl;
		word.clear();
	}

     	return 0;
}
	
