//#include "/root/mpich2-install/include/mpi.h"
#include "mpi.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <map>

/*data should partition ahead by user, then allocate same adress on each machine*/
using namespace std;

void localCaculate(char * infile, string  & categoryFre_s, string &  termCatFre_s)
{
	map<string,int> categoryFre;
	map<string,int> termCatFre;
	ifstream infs(infile);
	string line;
	string alpha="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string delimiter=" ,.!?:;\"-/=()\t";

	while(getline(infs,line))
	{
		string category;
		string content;
		int begin=0;
		int end=0;
		end=line.find("\t",begin);
		category=line.substr(begin,end-begin);
		if(categoryFre.count(category)==0)
			categoryFre.insert(pair<string,int>(category,1));
		else
			categoryFre[category]+=1;
		
		begin=line.find_first_of(alpha,end+1);
		string term;
		string termCat;
		while((end=line.find_first_of(delimiter,begin))!=string::npos)
		{
			term=line.substr(begin,end-begin);
			termCat=category+"[[:]]"+term;
			if(termCatFre.count(termCat)==0)
				termCatFre.insert(pair<string,int>(termCat,1));
			else
				termCatFre[termCat]+=1;
			begin=line.find_first_of(alpha,end+1);

		}
	}
}

void localCaculate(char * infile, map<string,int>  & categoryFre, map<string,int> &  termCatFre)
{
	ifstream infs(infile);
	string line;
	string alpha="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string delimiter=" ,.!?:;\"-/=()\t";

	while(getline(infs,line))
	{
		string category;
		string content;
		int begin=0;
		int end=0;
		end=line.find("\t",begin);
		category=line.substr(begin,end-begin);
		if(categoryFre.count(category)==0)
			categoryFre.insert(pair<string,int>(category,1));
		else
			categoryFre[category]+=1;
		
		begin=line.find_first_of(alpha,end+1);
		string term;
		string termCat;
		while((end=line.find_first_of(delimiter,begin))!=string::npos)
		{
			term=line.substr(begin,end-begin);
			termCat=category+"[[:]]"+term;
			if(termCatFre.count(termCat)==0)
				termCatFre.insert(pair<string,int>(termCat,1));
			else
				termCatFre[termCat]+=1;
			begin=line.find_first_of(alpha,end+1);

		}
	}
}

void decode(char * buf,map<string,int> & mymap)
{
	string bufs(buf);
	int lengh=bufs.size();
	int begin=0;
	int end=begin;
	string category;
	int fre;
	while((end=bufs.find("[[->]]",begin))!=string::npos)
	{
	
		category=bufs.substr(begin,end-begin);
		begin=end+6;
		end=bufs.find("[[|]]",begin);
		fre=stoi(bufs.substr(begin,end-begin));

		if(mymap.count(category)==0)
			mymap.insert(pair<string,int>(category,fre));
		else
			mymap[category]+=fre;

		begin=end+5;
		if(begin>=lengh)
			break;
	}
}

int main(int argc, char* argv[])
{
	MPI_Init(&argc,&argv);
	string cmd=argv[0];
	string usage=cmd+" -i input -o output";

	int opt;
	char * infile;
	char * outfile;
	while((opt=getopt(argc,argv,"i:o:"))!=-1)
	{
		switch(opt)
		{
			case 'i':
				infile=optarg;
				break;
			case 'o':
				outfile=optarg;
				break;
		}
	}

	if(infile==NULL ||outfile==NULL)
	{
		cout<<usage<<endl;
	}
	

	int myrank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	//todo
	//data partition 
	//task allocation
	
	if(myrank==0)
	{
		map<string,int> categoryFre;
		map<string,int> termCatFre;
		localCaculate(infile, categoryFre, termCatFre);

		//receive
		for(int srank=1;srank<size;srank++)
		{
			MPI_Status * status1;
			int  count1;
			MPI_Probe(srank,1,MPI_COMM_WORLD,status1);
			MPI_Get_count(status1,MPI_CHAR,&count1);
			char * buf1 = new char[count1];
			MPI_Recv(buf1,count1,MPI_CHAR,srank,1,MPI_COMM_WORLD,status1);

			decode(buf1,categoryFre);


			MPI_Status * status2;
			int  count2;
			MPI_Probe(srank,2,MPI_COMM_WORLD,status2);
			MPI_Get_count(status2,MPI_CHAR,&count2);
			char *  buf2 = new char[count2];
			MPI_Recv(buf2,count2,MPI_CHAR,srank,2,MPI_COMM_WORLD,status2);

			decode(buf2,termCatFre);
		}
		

		int totalCat=categoryFre.size();
		int totalDoc=0;
		map<string,float> categoryPro;
		for(map<string,int>::iterator it=categoryFre.begin();it!=categoryFre.end();it++)
		{
			totalDoc+=it->second;
		}

		for(map<string,int>::iterator it=categoryFre.begin();it!=categoryFre.end();it++)
		{
			categoryPro.insert(pair<string,float>(it->first, float(it->second) / float(totalDoc)) );
		}

		map<string,float> termCatPros[totalCat];
		int totalTerm[totalCat];
		map<string,int> catToIndex;
		string indexToCat[totalCat];


		int ind=0;
		for(map<string,int>::iterator it=categoryFre.begin();it!=categoryFre.end();it++)
		{
			catToIndex.insert(pair<string,int>(it->first,ind));
			indexToCat[ind]=it->first;
			ind++;
		}

		for(map<string,int>::iterator it=termCatFre.begin();it!=termCatFre.end();it++)
		{
			int begin,end;
			end=it->first.find("[[:]]");
			string cat=it->first.substr(begin,end-begin);
			begin=end+5;
			string term=it->first.substr(begin);
			termCatPros[catToIndex[cat]].insert(pair<string,float>(term,float(it->second)));
			totalTerm[catToIndex[cat]] += it->second;
		}

		for(int i=0;i<totalCat;i++)
		{
			for(map<string,float>::iterator it=termCatPros[i].begin();it!=termCatPros[i].end();it++)
			{
				it->second = it->second / float(totalTerm[i]); 
			}
		}

		//out put 
		ofstream ofile(outfile);
		for(map<string,float>::iterator it=categoryPro.begin();it!=categoryPro.end();it++)
		{
			ofile<<"#model para :categoryPro"<<endl;
			ofile<<it->first<<"[[:]]"<<it->second<<endl;
		}
		for(int i=0;i<totalCat;i++)
		{
			ofile<<"#model para :"<<indexToCat<<endl;
			for(map<string,float>::iterator it=termCatPros[i].begin();it!=termCatPros[i].end();it++)
			{
				ofile<<it->first<<"[[:]]"<<it->second<<endl;
			}
		}


	}

	else
	{
		string  categoryFre_s;
		string  termCatFre_s;
		localCaculate(infile,categoryFre_s,termCatFre_s);

		int count1=categoryFre_s.size();
		char * buf1 =new char[count1+1];
		buf1==categoryFre_s.c_str();
		MPI_Send(buf1,count1,MPI_CHAR,0,1,MPI_COMM_WORLD);
		
		int count2=termCatFre_s.size();
		char * buf2 = new char[count2+1];
		buf2==termCatFre_s.c_str();
		MPI_Send(buf2,count2,MPI_CHAR,0,2,MPI_COMM_WORLD);
	}

	MPI_Finalize();
}
