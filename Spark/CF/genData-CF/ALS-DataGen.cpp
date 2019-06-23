#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])
{
	int DataFileSize = atoi(argv[1]);
	
	ofstream f("als_input.txt", ios::out|ios::app);
	if (!f)
	{
		cout<<"Open file ERROR!"<<endl;
		return 0;
	}
	cout<<"Open file success!"<<endl;
	cout<<"Begin writing file..."<<endl;
	srand((unsigned)time(0));
	int randnum1 = 0;
	string str;
	for (long int i = 1; i < (int)sqrt(DataFileSize)*200; i++)
	{
		for(long int j=1;j<=(int)sqrt(DataFileSize)*1000;j++)
	        {
			if(rand()%2==0)
			{
     				ostringstream oss;
				oss<<resetiosflags(ios::right)<<i<<","<<j<<","<<rand()%10<<endl;
				f<<oss.str();
			}
		}

	}
	f.close();
	cout<<"Complete!"<<endl;
	return 0;
}
