//**********************
// Deep Learning and Applications
// Piji Li
// lipiji.sdu@gmail.com
// http://www.zhizhihu.com
//*********************/

#ifndef DEEP_H
#define DEEP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

double sample_from_gaussian(double miu, double sigma);
int max_i_(double *x, int l);

class Conf
{
	public:
		string f_train_x;
		string f_train_y;
		string f_test_x;
		string f_test_y;

		int epoch;
		int batch_size;
		int *hidden_layer_size;
		int cd_k;
		double learning_rate;
		int n_layers;
		int n_labels; //num of classes
        double lamda; // penalty factor
		Conf(string, string, int, int, int*, int, double, int, int, double);
		~Conf();

};

class Dataset
{
	public:
		int N;
		int n_f;

		vector<vector<double> > X;
		vector<double> Y;

		int batch_index;
		Dataset(Conf);
		~Dataset();
        void reloadx(Conf, char*, vector<vector<double> >&);
        int dataNum(int,int,int);
};

class Dataset1
{
	public:
		int N;
		int n_f;

		vector<vector<double> > X1;
		vector<double> Y1;

		int batch_index;
		Dataset1(Conf);
		~Dataset1();
        void reloadx(Conf, char*, vector<vector<double> >&);
        int dataNum(int,int,int);
};

class RBM
{
	public:
		int n_samples;
		int n_visible;
		int n_hidden;
		double **W;
		double *hbias;
		double *vbias;
        double error;
        double lamda;
        double momentum;

		RBM(int, int, int,  double*, double*, double*, double, double);
		~RBM();
		void train(double*, double, int);
		void activate_hidden(double*, double*, int*, int, int);
		void activate_visible(int*, double*, int*, int, int);
};
class RBM1
{
	public:
		int n_samples;
		int n_visible;
		int n_hidden;
		double **W;
		double *hbias;
		double *vbias;
        double error;
        double lamda;
        double momentum;
		RBM1(int, int, int,  double*, double*, double*, double, double);
		~RBM1();
		void train(double*, double, int);
		void activate_hidden(double*, double*, int*, int, int);
		void activate_visible(int*, double*, int*, int, int);
};




class LR
{
    public:
        int n_samples;
        int n_features;
        int n_labels;
        double lamda;
        double **W;
        double *b;
        LR(Dataset, Conf);
        LR(int, int, int, double);
        LR(int, int, int, double,double*,double*);

        ~LR();
        void train(double*, int*, double, double);
        void softmax(double*);
        void predict(double*, double*);

};
class LR1
{
    public:
        int n_samples;
        int n_features;
        int n_labels;
        double lamda;
        double **W;
        double *b;
//        LR(Dataset, Conf);
//        LR(int, int, int, double);
        LR1(int, int, int, double,double*,double*);

        ~LR1();
        void train(double*, int*, double);
        void softmax(double*);
        void predict(double*, double*);

};
/*
class DBN
{
	public:
		int n_samples;
		int n_features;
		int n_layers;
		int n_labels;
		int *hidden_layer_size;
        double lamda;
        double alpha;
		RBM **rbm_layers;
        LR *lr_layer;
		DBN(Dataset, Conf);
		//DBN(Dataset,Conf,double***,double**,double**);
		DBN(Dataset, Conf,double***,double**,double**,double**,double*);
		~DBN();
		void pretrain(Dataset, Conf);
		void finetune(Dataset, Conf);
		int predict(double*, double*, int);

};*/
#endif //DEEP_H



