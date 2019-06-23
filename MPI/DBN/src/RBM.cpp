#include <stdio.h>
#include "mpi.h"
#include "deep.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;

int main( int argc, char *argv[] )
{
    int rank;
    int size;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char str[] = "../data/data_";
    char x_l[256];
    x_l[0]='\0';
    char strlabel[] = "../data/label_";


    char x_label[256];
    x_label[0]='\0';

    int dataPara[4] = {0};//rank参数 0：总数据个数 1：特征数 2：进程数据个数 3：末尾进程数据个数
    int dataNf = 0;
//    if(rank==0){
       //open file test

//
//
//
//
//    //open file test
//    string result1 = "";
//    std::ostringstream oss1;
//
////save the samples
//        char str2[] = "/home/eric/workspace/model/x_layer_rank_";
//
//     //   char x_l1[256];
//      //  x_l[0]='\0';
//       // sprintf(x_l, "%s%d%s%d",str, (l+2),str_rank,rank);
//      //  sprintf(x_l1, "%s%d",str1,rank);
//        //cout<<rank<<"save:"<<x_l1<<endl;
//        oss1 << str2 << rank;
//        result1 += oss1.str();
//        ofstream fout4(result1.c_str());
//       // ifstream fout3("/home/eric/workspace/model/x_layer_rank_0.txt",ios::app);
//        if(!fout4){
//        	cout<<"open error"<<endl;
//        	exit(-1);
//        }
//
//        cout<<"open success"<<endl;


    /*系统参数设置*/
    int epoch = 2;
    int batch_size = 0;
    double gamma = 0.1; // learning rate
    int k = 1; //Contrastive Divergence k
    int hls[] = {500}; //DBN 网络结构
    int n_layers = sizeof(hls) / sizeof(hls[0]);
    int n_lables = 10;
    double lbd = 0.0002; // weight cost

    int FeatureNum;
    int labelNum;


    MPI_Status status;

   /*************************数据分发*******************************************/
    if(rank == 0)
    {
    	cout<<"data distribution"<<endl;
    	string ftx = "../data/train_x.txt";
        string fty = "../data/train_y.txt";

        Conf conf(ftx, fty, epoch, batch_size, hls, k, gamma, n_layers, n_lables, lbd);
        Dataset data(conf);
        int DataPar[4] = {0};
        DataPar[0] = data.N;
        DataPar[1] = data.n_f;
        FeatureNum= data.n_f;
        labelNum = conf.n_labels;
        //data
    //    cout<<data.n_f<<endl;

        int RankNum = data.N/size;
        int RanKEndNum = data.N-(size-1)*RankNum; //尾节点数据个数
        DataPar[2] = RankNum;
        DataPar[3] = RanKEndNum;
        sprintf(x_l, "%s%d", str, rank);

        //test
//        char fxstr[] = "/home/eric/workspace/mpiDBN/data/data_";
//        char fx_l2[256];
//        fx_l2[0]='\0';
//        sprintf(fx_l2, "%s%d", fxstr, rank);
//        string ftx2(fx_l2);
//        char fystr[] = "/home/eric/workspace/mpiDBN/data/label_";
//        char fy_l2[256];
//        fy_l2[0]='\0';
//        sprintf(fy_l2, "%s%d", fystr, rank);
//
//        string fty2(fy_l2);
//        Conf conf2(ftx2, fty2, epoch, batch_size, hls, k, gamma, n_layers, n_lables, lbd);
//        cout<<"begin:"<<rank<<endl;
//        Dataset data2(conf2);
//        cout<<"test Success"<<endl;

        ofstream RankData(x_l);

        for(int i=0;i<RankNum;i++)
        {
        	for(int j=0;j<data.n_f;j++)
            {
        		RankData << data.X[i][j]<<" ";
        	}
        	RankData << endl;

        }
        RankData << flush;
        RankData.close();

        //label

        sprintf(x_label, "%s%d", strlabel, rank);

        ofstream RankLabel(x_label);


        for(int i=0;i<RankNum;i++)
        {
        	RankLabel << data.Y[i]<<endl;
        }
        RankLabel<< flush;
        RankLabel.close();

//        cout<<rank<<endl;
//        cout<<size<<endl;

        //注意 i要从1开始！
        for(int i=1;i<size;i++)
        {
        	MPI_Send(DataPar,4,MPI_INT,i,10,MPI_COMM_WORLD);
        }

        for(int i=1;i<size;i++)
        {
        	if(i<size-1)
            {
        		//1.发送数据
        		double* RankX= new double[RankNum*data.n_f];
        		for(int j=0;j<RankNum;j++)
                {
        		    int ZeroNum = 0;
        		    for(int k=0;k<data.n_f;k++)
                    {
        		        RankX[j*data.n_f+k] = data.X[i*RankNum+j][k];
        		        /*if(RankX[j*data.n_f+k]==0){
        		        ZeroNum++;
        		        */

        		    }
        		        //cout<<"RowNUM:"<<j<<","<<"ZeroNum:"<<ZeroNum<<endl;
        		}

        		MPI_Send(RankX,RankNum*data.n_f,MPI_DOUBLE,i,i,MPI_COMM_WORLD);
        		delete[] RankX;

        		//2.发送标签
        		double* RankY= new double[RankNum];
        		for(int j=0;j<RankNum;j++)
                {
        			RankY[j] = data.Y[i*RankNum+j];

        		}
        		int LabelTag= 10+i;
        		MPI_Send(RankY,RankNum,MPI_DOUBLE,i,LabelTag,MPI_COMM_WORLD);
        		delete[] RankY;


        	}
        	else
            {
        		//1.发送数据
        		double* RankX= new double[RanKEndNum*data.n_f];
        		for(int j=0;j<RanKEndNum;j++)
                {
        		    int ZeroNum = 0;
        		    for(int k=0;k<data.n_f;k++)
                    {
        		        RankX[j*data.n_f+k] = data.X[i*RankNum+j][k];
        		        /*if(RankX[j*data.n_f+k]==0){
        		        ZeroNum++;
        		        */

        		    }
        		}

        		MPI_Send(RankX,RanKEndNum*data.n_f,MPI_DOUBLE,i,i,MPI_COMM_WORLD);
        		delete[] RankX;

        		//2.发送标签
        		double* RankY= new double[RanKEndNum];
        		for(int j=0;j<RanKEndNum;j++)
                {
        			RankY[j] = data.Y[i*RankNum+j];
        		}
        		int LabelTag= 10+i;

        		MPI_Send(RankY,RanKEndNum,MPI_DOUBLE,i,LabelTag,MPI_COMM_WORLD);

        		delete[] RankY;
        	}
        }
    }//if rank==0 END
    else
    {
    	MPI_Recv(dataPara,4,MPI_INT,0,10,MPI_COMM_WORLD,&status);
    	//MPI_Recv(&dataNf,1,MPI_INT,0,11,MPI_COMM_WORLD,&status);
    	cout<<rank<<"N"<<dataPara[0]<<endl;
    	cout<<rank<<"n_f"<<dataPara[1]<<endl;
    	cout<<rank<<"RankNum"<<dataPara[2]<<endl;

    	if(rank <(size-1))
        {

    		//1.数据

    		double* RankX= new double[dataPara[2]*dataPara[1]];
    		MPI_Recv(RankX,(dataPara[2]*dataPara[1]),MPI_DOUBLE,0,rank,MPI_COMM_WORLD,&status);

    		//保存
    		sprintf(x_l, "%s%d", str, rank);
    		//cout<<x_l<<endl;
    		ofstream Data(x_l);

    		int DataSize = dataPara[2];
    		            //cout<<Rank[0]<<endl;
    		for(int i=0;i<DataSize;i++)
            {
    			int ZeroNum = 0;
    		    for(int j=0;j<dataPara[1];j++)
                {
    		        Data<<RankX[i*dataPara[1]+j]<<" ";
    	                /*if(RankX[i*dataPara[1]+j]==0){
    	                	ZeroNum++;
    	                }*/

    		    }
    		    Data<<endl;
    		    //cout<<"RowNUM:"<<i<<","<<"ZeroNum:"<<ZeroNum<<endl;


            }
            Data.close();
            delete[] RankX;
    		//2.label
    		double* RankY= new double[dataPara[2]];
    		int LabelTag= 10+rank;
    		MPI_Recv(RankY,dataPara[2],MPI_DOUBLE,0,LabelTag,MPI_COMM_WORLD,&status);
    		//保存
    		sprintf(x_label, "%s%d", strlabel, rank);
    		ofstream DataY(x_label);
    		for(int i=0;i<DataSize;i++)
            {
    		    DataY<<RankY[i]<<" ";
    		    DataY<<endl;
    		}
    		DataY.close();
    		delete[] RankY;
        }
    	else
        {
    	    //1.数据
    	    double* RankX= new double[dataPara[3]*dataPara[1]];
    		MPI_Recv(RankX,(dataPara[3]*dataPara[1]),MPI_DOUBLE,0,rank,MPI_COMM_WORLD,&status);
    		sprintf(x_l, "%s%d", str, rank);
    		ofstream DataX(x_l);
    		int DataSize = dataPara[3];
    		for(int i=0;i<DataSize;i++)
            {
    		    int ZeroNum = 0;
    			for(int j=0;j<dataPara[1];j++)
                {
    				DataX<<RankX[i*dataPara[1]+j]<<" ";
    			}
    			DataX<<endl;
    		}
            DataX.close();
    		delete[] RankX;
    		//2.标签
            double* RankY= new double[dataPara[3]];
    		int LabelTag= 10+rank;
    		MPI_Recv(RankY,dataPara[3],MPI_DOUBLE,0,LabelTag,MPI_COMM_WORLD,&status);
    		//保存
    		sprintf(x_label, "%s%d", strlabel, rank);
    		ofstream DataY(x_label);
    		for(int i=0;i<DataSize;i++)
            {
    		    DataY<<RankY[i]<<" ";
    			DataY<<endl;
    		}
    		DataY.close();
    		delete[] RankY;
        }
    }//rank != 0 END

    cout<<"RBM Parameter initializaion"<<endl;
    /*******************************DBN网络参数空间初始化********************/

    int layers[] = {500};
    int layerNum = 1;
    int PreLayerNum= 784;

    //初始化W_storage

    double** W_storage = new double*[layerNum];
    for(int i=0;i<layerNum;i++)
    {
    	W_storage[i] = new double[PreLayerNum*layers[i]];
    	PreLayerNum = layers[i];
    }
    PreLayerNum = 784;
    //初始化W
//    double***W = new double**[layerNum];
//    for(int i=0;i<layerNum;i++){
//    	W[i] = new double*[PreLayerNum];
//    	PreLayerNum = layers[i];
//
//    }
//    PreLayerNum = 784;
//    for(int i=0;i<layerNum;i++){
//    	for(int j=0;j<layers[i];j++){
//    		W[i][j] = &W_storage[i][j*PreLayerNum];
//
//
//    	}
//    	PreLayerNum = layers[i];
//
//    }
//    PreLayerNum = 784;

//    for(int i= 0;i<layerNum;i++){
//    	for(int j=0;j<layers[i];j++){
//    		for(int k=0;k<PreLayerNum;k++){
//    			W[i][j][k] = 1;
//    		}
//    	}
//    	PreLayerNum = layers[i];
//    }

    //初始化可视层bias
	double** Vb = new double*[layerNum];
	for(int i=0;i<layerNum;i++)
    {
		Vb[i] = new double[PreLayerNum];
		PreLayerNum = layers[i];
	}

    //初始化隐层bias
	double** Hb = new double*[layerNum];
	for(int i=0;i<layerNum;i++)
    {
		Hb[i] = new double[layers[i]];
	}

    //LR参数
	double** LrW = new double*[n_lables];
	for(int i=0; i<n_lables; i++)
	    LrW[i] = new double[layers[(layerNum-1)]];
	double* Lrb = new double[n_lables];
	double *Lrwstorage = new double[layers[(layerNum-1)]*n_lables];

	cout<<"RBM Parameter initializaion broadcast"<<endl;
    /********************************初始化参数broadcast************************************/
	PreLayerNum = 784;
	if(rank ==0)
    {
//	    	//W
//	    	for(int i= 0;i<layerNum;i++){
//	    	    	for(int j=0;j<layers[i];j++){
//	    	    		for(int k=0;k<PreLayerNum;k++){
//	    	    			W[i][j][k] = sample_from_gaussian(0, 0.01);
//	    	    		}
//	    	    	}
//	    	    	PreLayerNum = layers[i];
//	    	    }
	    	//W_storage
	    for(int i=0;i<layerNum;i++)
        {
	        //  W_storage[i] = new double[PreLayerNum*layers[i]];
	    	for(int j=0;j<PreLayerNum*layers[i];j++)
            {
	    	    W_storage[i][j]=sample_from_gaussian(0, 0.01);
	    	}
	    	PreLayerNum = layers[i];
	    }
	    //Vb
	    for(int i=0;i<layerNum;i++)
        {
	        for(int j=0;j<PreLayerNum;j++)
            {
	    		Vb[i][j] = 0;
	    	}
	    	PreLayerNum = layers[i];
	    }
	    //Hb
	    for(int i=0;i<layerNum;i++)
        {
	    	for(int j=0;j<layers[i];j++)
            {
	    	    Hb[i][j] = 0;
	    	}
	    }
	    //LrW
	    for(int i=0;i<layers[(layerNum-1)]*n_lables;i++)
        {
	    	Lrwstorage[i]=0;
	    }
	    //Lrb
	    for(int i=0;i<n_lables;i++)
        {
	    	Lrb[i] = 0;
	    }
	}// rank==0 END

    //广播RBM层参数
	PreLayerNum = 784;
	for(int i=0;i<layerNum;i++)
    {
	    MPI_Bcast(W_storage[i],PreLayerNum*layers[i],MPI_DOUBLE,0,MPI_COMM_WORLD);
	    MPI_Bcast(Hb[i],layers[i],MPI_DOUBLE,0,MPI_COMM_WORLD);
	    MPI_Bcast(Vb[i],PreLayerNum,MPI_DOUBLE,0,MPI_COMM_WORLD);
	    PreLayerNum = layers[i];
	}

	//广播LR层参数

//	    for(int i=0;i<n_lables;i++){
//	    	MPI_Bcast(LrW[i],layers[(layerNum-1)],MPI_DOUBLE,0,MPI_COMM_WORLD);
//	    }
	MPI_Bcast(Lrb,n_lables,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(Lrwstorage,layers[(layerNum-1)]*n_lables,MPI_DOUBLE,0,MPI_COMM_WORLD);
	//   cout<<"rank"<<rank<<"+W[2][1][1]:"<<W[2][1][1]<<endl;
	MPI_Barrier(MPI_COMM_WORLD);
	//  cout<<"begin:"<<rank<<endl;
    cout<<"one layer RBM pretrain"<<endl;
/************************************模型训练******************************************/
	char fxstr[] = "../data/data_";
	char fx_l2[256];
	fx_l2[0]='\0';
    sprintf(fx_l2, "%s%d", fxstr, rank);
	string ftx2(fx_l2);
    char fystr[] = "../data/label_";
	char fy_l2[256];
	fy_l2[0]='\0';
    sprintf(fy_l2, "%s%d", fystr, rank);

    string fty2(fy_l2);
	Conf conf2(ftx2, fty2, epoch, batch_size, hls, k, gamma, n_layers, n_lables, lbd);
    Dataset data2(conf2);
//     cout<<"W_storage[1][10]"<<W_storage[1][10]<<endl;
//     cout<<"W_storage[1][11]"<<W_storage[1][11]<<endl;

    int n_samples = data2.N;
    int n_features = data2.n_f;

    int lamda = conf2.lamda;
    int alpha = conf2.learning_rate;
    int hidden_layer_size[] ={500};
    int pre_epoches = 1000;
    RBM**  rbm_layers = new RBM*[n_layers];

    PreLayerNum = 784;
    //cout<<"n_layers == "<<n_layers<<endl;
    for(int l=0;l<n_layers;l++)
    {

        // rbm_layers[l] = new RBM(n_samples, n_features,hls[l],W_storage[l],Hb[l],Vb[l],conf2.lamda, 0.5);
        if(l == 0)
        {
            rbm_layers[l] = new RBM(n_samples, n_features,hidden_layer_size[l],W_storage[l],Hb[l],Vb[l],conf2.lamda, 0.5);
        }
        else
        {
            rbm_layers[l] = new RBM(n_samples, hidden_layer_size[l-1], hidden_layer_size[l],W_storage[l],Hb[l],Vb[l],conf2.lamda, 0.5);
        }
        double *rbm_input = NULL;
        double *pre_rbm_input = NULL;
   // int l = 0;
        vector<vector<double> > xlayer;

        int pre_size;
        cout<<"layer: "<<l<<": rank:"<<rank<<endl;
   // MPI_Barrier(MPI_COMM_WORLD);
        if(l>0)
            data2.reloadx(conf2, x_l, xlayer);

        MPI_Barrier(MPI_COMM_WORLD);

        for(int epoch=0; epoch<pre_epoches; epoch++)
        {
            if(epoch < 5)
                rbm_layers[l]->momentum = 0.5;
            else
                rbm_layers[l]->momentum = 0.9;

            cout<<rank<<"start epoch："<<epoch<<endl;

		    double error = 0;
		    for(int i=0; i<n_samples; i++)
		    {
		        if(l == 0)
		        {
		            rbm_input = new double[n_features];
		    	    for(int f=0; f<n_features; f++)
		    	        rbm_input[f] = data2.X[i][f];
		        }
		        else
		        {
		    	    rbm_input = new double[hidden_layer_size[l-1]];
		    	    for(int f=0; f<hidden_layer_size[l-1]; f++)
		    	        rbm_input[f] = xlayer[i][f];
		        }
	        // if(l==2)
		    //   cout<<"layer:"<<l<<"  train rank:"<<endl;

		        rbm_layers[l]->train(rbm_input, conf2.learning_rate, conf2.cd_k);
		        delete[] rbm_input;
		        error += rbm_layers[l]->error;
		    }

		    cout <<rank<< "Layer: " << (l+1) << ", Epoch: " << epoch << ", Error: " << error << endl;
		    MPI_Allreduce(MPI_IN_PLACE,W_storage[l],PreLayerNum*layers[l],MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
		    MPI_Allreduce(MPI_IN_PLACE,Hb[l],layers[l],MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
		    MPI_Allreduce(MPI_IN_PLACE,Vb[l],PreLayerNum,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
		    MPI_Barrier(MPI_COMM_WORLD);
		    
            for(int i =0;i<PreLayerNum*layers[l];i++)
            {
		        W_storage[l][i] = W_storage[l][i]/size;
		    }

		    for(int i =0;i<layers[l];i++)
            {
		        Hb[l][i] = Hb[l][i]/size;
		    }

		    for(int i=0;i<PreLayerNum;i++)
            {
		        Vb[l][i] = Vb[l][i]/size;
		    }
		    //PreLayerNum = layers[l];
	    }// end one layer
		PreLayerNum = layers[l];
        //save samples
        // cout<<"save samples"<<endl;
        char filename[] ="../model/rank_";
	    string resultf = "";
	    std::ostringstream ossf;
	    ossf << filename << rank<<l;
	    resultf += ossf.str();
	    strcpy(x_l,resultf.c_str());
	    ofstream fout(resultf.c_str());
	    if(!fout)
        {
	        cout<<"open error"<<endl;
		    exit(-1);
	    }
	    if(l == 0)
	        pre_size = n_features;
	    else
            pre_size = hidden_layer_size[l-1];
            pre_rbm_input = new double[pre_size];

	    for(int i=0; i<n_samples; i++)
	    {
		    for(int f=0; f<pre_size; f++)
		    {
		        if(l == 0)
		    	    pre_rbm_input[f] =  data2.X[i][f];
		        else
		    	    pre_rbm_input[f] =  xlayer[i][f];
		    }
		    rbm_input = new double[hidden_layer_size[l]];
		    rbm_layers[l]->activate_hidden(pre_rbm_input, rbm_input, NULL, pre_size, hidden_layer_size[l]);

		    for(int f=0; f<hidden_layer_size[l]; f++)
		    {
		        fout << rbm_input[f] << " ";
		    }
		    fout << endl;
		    delete[] rbm_input;
	    }
	    delete[] pre_rbm_input;
        vector<vector<double> >().swap(xlayer);
	    fout.close();
    }
    //delete[] rbm_input;
    //delete[] pre_rbm_input;
    cout<<"pre-training END!"<<endl;

    MPI_Finalize();
    return 0;
}
