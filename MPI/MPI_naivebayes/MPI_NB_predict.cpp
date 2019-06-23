#include "mpi.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <map>
#include <set>
#include <string.h>

using namespace std;

void readModel(char *modelfile, map<string,float>  & categoryPros, map<string,float> &  termCatPros)
{
	string line;
	ifstream modelfs(modelfile);
	while(getline(modelfs,line))
	{
		int begin=0;
		int end=0;
		end=line.find("[[:]]",begin);
		string ct=line.substr(begin,end-begin);
		if(ct=="#catPara")
		{
			begin=end+5;
			end=line.find("[[:]]",begin);
			string category=line.substr(begin,end-begin);
			begin=end+5;
			end=line.find("[[:]]",begin);
			float categoryPro=stof(line.substr(begin,end-begin));
			categoryPros.insert(pair<string,float>(category,categoryPro));
		}
		else if(ct=="#catTermPara")
		{
			begin=end+5;
			end=line.find("[[:]]",begin);
			begin=end+5;
			end=line.find("[[:]]",begin);
			string termCat=line.substr(begin,end-begin);
			begin=end+5;
			end=line.find("[[:]]",begin);
			float termCatPro=stof(line.substr(begin,end-begin));
			termCatPros.insert(pair<string,float>(termCat,termCatPro));
		}
	}
}


void localCaculate(char * infile, char* outfile_char, int myrank, int sizePerNode, map<string,float>  & categoryPros, map<string,float> &  termCatPros)
{
	int modeLine = myrank % sizePerNode;
	ifstream infs(infile);
	string outfile(outfile_char);
	outfile += to_string(myrank);
	ofstream outfs(outfile.c_str());
	string line;
	string alpha="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string delimiter=" ,.!?:;\"-/=()\t";

	int lineNum=0;
	while(getline(infs,line))
	{
		if(lineNum % sizePerNode == modeLine)
		{
			string category;
			string content;
			int begin=0;
			int end=0;
			end=line.find("\t",begin);
			category=line.substr(begin,end-begin); //real category or random category if unknow

			begin=line.find_first_of(alpha,end+1);
			string term;
			string termCat;
			map<string,float> predictCatPros(categoryPros);

			while((end=line.find_first_of(delimiter,begin))!=string::npos)
			{
				term=line.substr(begin,end-begin);
				float termCatPro;
	
				for(map<string,float>::iterator it=categoryPros.begin();it!=categoryPros.end();it++)
				{

					termCat=it->first+"[[:]]"+term;
					if(termCatPros.find(termCat) == termCatPros.end())
					{
						termCatPro=0.00001;
					}
					else
						termCatPro=termCatPros[termCat];

					predictCatPros[it->first] *= termCatPro;
				}

				begin=line.find_first_of(alpha,end+1);

			}
			
			float maxPro=0;
			string maxCat="";
			for(map<string,float>::iterator it=predictCatPros.begin();it!=predictCatPros.end();it++)
			{
				if(it->second >= maxPro)
				{
					maxPro=it->second;
					maxCat=it->first;
				}
			}
			outfs<<lineNum<<" "<<maxCat<<endl;
		}

		lineNum++;
	}
}


int main(int argc, char* argv[])
{
	MPI_Init(&argc,&argv);
	
	string cmd=argv[0];
	string usage=cmd+" -m model -i input -o output";

	int opt;
	char * infile;
	char * outfile;
	char * modelfile;
	while((opt=getopt(argc,argv,"m:i:o:"))!=-1)
	{
		switch(opt)
		{
			case 'm':
				modelfile=optarg;
				break;
			case 'i':
				infile=optarg;
				break;
			case 'o':
				outfile=optarg;
				break;
		}
	}

	if(outfile==NULL || infile==NULL ||modelfile==NULL)
	{
		cout<<usage<<endl;
	}
	

	int myrank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);


	// load model
	map<string,float> categoryPros;
	map<string,float> termCatPros;
	readModel(modelfile, categoryPros, termCatPros);



	char *procesor_name = new char[MPI_MAX_PROCESSOR_NAME];
	memset(procesor_name,'-',MPI_MAX_PROCESSOR_NAME);
	int pro_name_len;
	MPI_Get_processor_name(procesor_name,&pro_name_len);
	char *pro_name_all = new char[MPI_MAX_PROCESSOR_NAME*size];
	memset(pro_name_all,'-',MPI_MAX_PROCESSOR_NAME*size);
	MPI_Gather(procesor_name,MPI_MAX_PROCESSOR_NAME,MPI_CHAR,pro_name_all,MPI_MAX_PROCESSOR_NAME,MPI_CHAR,0,MPI_COMM_WORLD);
	
	int sizePerNode; //number of process per node
    if(myrank==0)
    {
        set<string> pro_set;
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<MPI_MAX_PROCESSOR_NAME;j++)
            {
                procesor_name[j]=pro_name_all[i*MPI_MAX_PROCESSOR_NAME+j];
            }
            pro_set.insert(procesor_name);
        }
        sizePerNode=size/pro_set.size();
     }
    delete [] procesor_name;
    delete [] pro_name_all;
    MPI_Bcast(&sizePerNode,1,MPI_INT,0,MPI_COMM_WORLD);

    localCaculate(infile, outfile, myrank, sizePerNode, categoryPros, termCatPros);

	MPI_Finalize();
}
