//**********************
// Deep Learning and Applications
// Piji Li
// lipiji.sdu@gmail.com
// http://www.zhizhihu.com
//*********************/

#include "deep.h"

double sample_from_gaussian(double miu, double sigma)
{
    //std::default_random_engine generator;
    //std::normal_distribution<double> distribution(miu, sigma);
    //reutrn distribution(generator);

    static double V1, V2, S;
    static int phase = 0;
    double X;

    if (phase == 0)
    {
        do
        {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return X * sigma + miu;
}

double sigmoid(double x) 
{
    return 1.0 / (1.0 + exp(-x));
}

int binomial(int n, double p) 
{
    if(p < 0 || p > 1) return 0;

    int c = 0;
    double r;

    for(int i=0; i<n; i++) {
        r = rand() / (RAND_MAX + 1.0);
        if (r < p) c++;
    }

    return c;
}

double square_error(double *v1, double *v2, int size)
{
    double error = 0;
    for(int i=0; i<size; i++)
        error += pow((v1[i] - v2[i]), 2);

    return error;
}

int max_i_(double *x, int l)
{
    double max_v = x[0];
    double max_i = 0;
    for(int i=0; i<l; i++)
    {
        if(x[i] > max_v)
        {
            max_v = x[i];
            max_i = i;
        }
    }
    return max_i;
}


Conf::Conf(string ftx, string fty, int epc, int bs, int *hls, int k, double lr, int n_ly, int n_lb, double lbd)
{
    f_train_x = ftx;
    f_train_y = fty;
    epoch = epc;
    batch_size = bs;
    hidden_layer_size = hls;
    cd_k = k;
    learning_rate = lr;
    n_layers = n_ly;
    n_labels = n_lb;
    lamda = lbd;
}
Conf::~Conf(){}
/*
Dataset::Dataset(Conf conf)
{

    N = 0;
    n_f = 0;
    batch_index = 0;
    rankDataNum = 0;
    rankN = 0;
    if(conf.batch_size >= 0)
    {
        int Nx = 0;
        int Ny = 0;
        int Nf = 0; // dim of x
        //cout<<"begeiny"<<endl;
        //read the label file
        ifstream fin_y(conf.f_train_y.c_str());
        if(!fin_y)
        {
            cout << "Error opening " << conf.f_train_y << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
           // cout<<"beginready"<<endl;
            while(getline(fin_y, s))
            {
                Y.push_back(atof(s.c_str()));
                if(conf.batch_size > 0 && Ny >= conf.batch_size)
                    break;
                ++Ny;
            }
            //cout<<"endready"<<endl;
        }
        fin_y.close();

       // cout<<"beginx"<<endl;
        // read the x file
        ifstream fin_x(conf.f_train_x.c_str());
       // cout<<"beginxread:"<<conf.f_train_x.c_str()<<endl;
        cout<<fin_x<<endl;
        if(!fin_x)
        {
            cout << "Error opening " << conf.f_train_x << " for input" << endl;
            exit(-1);
        }
        else
        {
        	//cout<<"beginreadx"<<endl;
            string s;
            while(getline(fin_x, s))
            {
                Nf = 0;
                vector<double> x;
                const char *split = " \t";

                char *line=new char[s.size()+1];
                strcpy(line, s.c_str());
                char *p = strtok(line, split);
                x.push_back(atof(p));

                Nf = 1;
                while(1)
                {
                    p = strtok(NULL, split);
                    if(p == NULL || *p == '\n')
                        break;
                    x.push_back(atof(p));
                    ++Nf;
                }
                X.push_back(x);
                n_f = Nf;

                vector<double>().swap(x);
                if(conf.batch_size > 0 && Nx >= conf.batch_size)
                    break;
                ++Nx;
            }
            //cout<<"endreadx"<<endl;
        }
        fin_x.close();
        if(Nx == Ny)
        {
            N = Nx;
            n_f = Nf;
        }
        else
        {
            cout << "Dataset error: size(x) != size(y)." << endl;
            exit(-1);
        }

        cout << "Data loaded: size=" << N <<", dim=" << n_f << endl;
    }

}
*/

Dataset::Dataset(Conf conf)
{

    N = 0;
    n_f = 0;
    batch_index = 0;
    //cout<<"rank is: "<<rank<<endl;

    if(conf.batch_size >= 0)
    {
        int Nx = 0;
        int Ny = 0;
        int Nf = 0; // dim of x

        //read the label file
        ifstream fin_y(conf.f_train_y.c_str());
        if(!fin_y) 
        {   
            cout << "Error opening " << conf.f_train_y << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
            while(getline(fin_y, s))
            {	
                Y.push_back(atof(s.c_str()));
                if(conf.batch_size > 0 && Ny >= conf.batch_size)
                    break;
                ++Ny;
            }
        }
        fin_y.close();

        // read the x file
        ifstream fin_x(conf.f_train_x.c_str());
        if(!fin_x) 
        {   
            cout << "Error opening " << conf.f_train_x << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
            while(getline(fin_x, s))
            {	
                Nf = 0;
                vector<double> x;
                const char *split = " \t";

                char *line=new char[s.size()+1]; //?+1
                strcpy(line, s.c_str());
                char *p = strtok(line, split);
                x.push_back(atof(p));

                Nf = 1;
                while(1)
                {
                    p = strtok(NULL, split);
                    if(p == NULL || *p == '\n')
                        break;
                    x.push_back(atof(p));
                    ++Nf;
                }
                X.push_back(x);
                n_f = Nf;

                vector<double>().swap(x);
                if(conf.batch_size > 0 && Nx >= conf.batch_size)
                    break;
                ++Nx;
            }
        }
        fin_x.close();

        if(Nx == Ny)
        {
            N = Nx;
            n_f = Nf;
        }
        else
        {
            cout << "Dataset error: size(x) != size(y)." << endl;
            exit(-1);
        }

        cout << "Data loaded: size=" << N <<", dim=" << n_f << endl;
    }

}

int Dataset::dataNum(int datasize,int ranksize,int rank){
	int num =0;
	if(datasize%ranksize!=0){
	    num = (datasize -datasize%ranksize)/(ranksize-1);
        if((datasize-(rank+1)*num)>0){
    	   return num;
        }
        else{
    	   return datasize-rank*num;
        }
	}
	else{
		return datasize/ranksize;
	}

}

void Dataset::reloadx(Conf conf, char* xl, vector<vector<double> >& xlayer)
{

    N = 0;
    n_f = 0;
    batch_index = 0;

    if(conf.batch_size >= 0)
    {
        int Nx = 0;
        int Ny = 0;
        int Nf = 0; // dim of x

        //read the label file
        ifstream fin_y(conf.f_train_y.c_str());
        if(!fin_y) 
        {   
            cout << "Error opening " << conf.f_train_y << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
            while(getline(fin_y, s))
            {	
                Y.push_back(atof(s.c_str()));
                if(conf.batch_size > 0 && Ny >= conf.batch_size)
                    break;
                ++Ny;
            }
        }
        fin_y.close();

        // read the x file
        ifstream fin_x(xl);
        if(!fin_x) 
        {   
            cout << "Error opening " << conf.f_train_x << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
            while(getline(fin_x, s))
            {	
                Nf = 0;
                vector<double> x;
                const char *split = " \t";

                char *line=new char[s.size()+1];
                strcpy(line, s.c_str());
                char *p = strtok(line, split);
                x.push_back(atof(p));

                Nf = 1;
                while(1)
                {
                    p = strtok(NULL, split);
                    if(p == NULL || *p == '\n')
                        break;
                    x.push_back(atof(p));
                    ++Nf;
                }
                xlayer.push_back(x);
                n_f = Nf;

                vector<double>().swap(x);
                if(conf.batch_size > 0 && Nx >= conf.batch_size)
                    break;
                ++Nx;
            }
        }
        fin_x.close();

        if(Nx == Ny)
        {
            N = Nx;
            n_f = Nf;
        }
        else
        {
            cout << "Dataset error: size(x) != size(y)." << endl;
            exit(-1);
        }

        cout << "Data loaded: size=" << N <<", dim=" << n_f << endl;
    }

}
Dataset::~Dataset()
{
    vector<vector<double> >().swap(X);
    vector<double>().swap(Y);
}
Dataset1::Dataset1(Conf conf)
{


    N = 0;
    n_f = 0;
    batch_index = 0;

    if(conf.batch_size >= 0)
    {
        int Nx = 0;
        int Ny = 0;
        int Nf = 0; // dim of x
        //cout<<"begeiny"<<endl;
        //read the label file
        int testn =10;
        ifstream fin_y1(conf.f_train_y.c_str());
        if(!fin_y1)
        {
            cout << "Error opening " << conf.f_train_y << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
           // cout<<"beginready"<<endl;
            while(getline(fin_y1, s))
            {
                Y1.push_back(atof(s.c_str()));
                if(conf.batch_size > 0 && Ny >= conf.batch_size)
                   break;
                ++Ny;

            }
            cout<<"Ny:"<<Ny<<flush<<endl;
        }


        fin_y1.close();



        ifstream fin_x(conf.f_train_x.c_str());

        if(!fin_x)
        {
            cout << "Error opening " << conf.f_train_x << " for input" << endl;
            exit(-1);
        }
        else
        {
        	//cout<<"beginreadx"<<endl;
            string s;
            while(getline(fin_x, s))
            {
                Nf = 0;
                vector<double> x;
                const char *split = " \t";

                char *line=new char[s.size()+1];
                strcpy(line, s.c_str());
                char *p = strtok(line, split);
                x.push_back(atof(p));

                Nf = 1;
                while(1)
                {
                    p = strtok(NULL, split);
                    if(p == NULL || *p == '\n')
                        break;
                    x.push_back(atof(p));
                    ++Nf;
                }
                X1.push_back(x);
                n_f = Nf;

                vector<double>().swap(x);
                if(conf.batch_size > 0 && Nx >= conf.batch_size)
                    break;
                ++Nx;
            }
            cout<<Nx<<flush<<endl;
            //cout<<"endreadx"<<endl;
        }
        fin_x.close();
        if(Nx == Ny)
        {
            N = Nx;
            n_f = Nf;
        }
        else
        {
            cout << "Dataset error: size(x) != size(y)." << endl;
            exit(-1);
        }

        cout << "Data loaded: size=" << N <<", dim=" << n_f << endl;



    }

    }



int Dataset1::dataNum(int datasize,int ranksize,int rank){
	int num =0;
	if(datasize%ranksize!=0){
	    num = (datasize -datasize%ranksize)/(ranksize-1);
        if((datasize-(rank+1)*num)>0){
    	   return num;
        }
        else{
    	   return datasize-rank*num;
        }
	}
	else{
		return datasize/ranksize;
	}

}

void Dataset1::reloadx(Conf conf, char* xl, vector<vector<double> >& xlayer)
{

    N = 0;
    n_f = 0;
    batch_index = 0;

    if(conf.batch_size >= 0)
    {
        int Nx = 0;
        int Ny = 0;
        int Nf = 0; // dim of x

        //read the label file
        ifstream fin_y(conf.f_train_y.c_str());
        if(!fin_y)
        {
            cout << "Error opening " << conf.f_train_y << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
            while(getline(fin_y, s))
            {
                Y1.push_back(atof(s.c_str()));
                if(conf.batch_size > 0 && Ny >= conf.batch_size)
                    break;
                ++Ny;
            }
        }
        fin_y.close();

        // read the x file
        ifstream fin_x(xl);
        if(!fin_x)
        {
            cout << "Error opening " << conf.f_train_x << " for input" << endl;
            exit(-1);
        }
        else
        {
            string s;
            while(getline(fin_x, s))
            {
                Nf = 0;
                vector<double> x;
                const char *split = " \t";

                char *line=new char[s.size()+1];
                strcpy(line, s.c_str());
                char *p = strtok(line, split);
                x.push_back(atof(p));

                Nf = 1;
                while(1)
                {
                    p = strtok(NULL, split);
                    if(p == NULL || *p == '\n')
                        break;
                    x.push_back(atof(p));
                    ++Nf;
                }
                xlayer.push_back(x);
                n_f = Nf;

                vector<double>().swap(x);
                if(conf.batch_size > 0 && Nx >= conf.batch_size)
                    break;
                ++Nx;
            }
        }
        fin_x.close();

        if(Nx == Ny)
        {
            N = Nx;
            n_f = Nf;
        }
        else
        {
            cout << "Dataset error: size(x) != size(y)." << endl;
            exit(-1);
        }

        cout << "Data loaded: size=" << N <<", dim=" << n_f << endl;
    }

}
Dataset1::~Dataset1()
{
    vector<vector<double> >().swap(X1);
    vector<double>().swap(Y1);
}








RBM::RBM(int N, int n_f, int n_h, double *w, double *hb, double *vb, double lbd, double mmt)
{
    n_samples = N;
    n_visible = n_f;
    n_hidden = n_h;
    error = 0.0;
    lamda = lbd;
    momentum = mmt;

    if(w == NULL)
    {
        W = new double*[n_hidden];
        for(int i=0; i<n_hidden; i++)
            W[i] = new double[n_visible];
        for(int i=0; i<n_hidden; i++)
            for(int j=0; j<n_visible; j++)
                W[i][j] = sample_from_gaussian(0, 0.01);
    }
    else
    {
        //W = w;
        W = new double*[n_hidden];
                for(int i=0; i<n_hidden; i++)
                	W[i] = &w[i*n_visible];
    }

    if(hb == NULL)
    {
        hbias = new double[n_hidden];
        for(int i=0; i<n_hidden; i++)
            hbias[i] = 0;
    }
    else
        hbias = hb;

    if(vb == NULL)
    {
        vbias = new double[n_visible];
        for(int i=0; i<n_visible; i++)
            vbias[i] = 0;
    }
    else
        vbias = vb;


}
void RBM::activate_hidden(double *v_prob, double *h_prob, int *h_state, int n_visible, int n_hidden)
{	
    //v->h
    for(int i=0; i<n_hidden; i++)
    {
        double vh_prob = 0;
        for(int j=0; j<n_visible; j++)
        {
            // prob or state?
            vh_prob += v_prob[j] * W[i][j];
        }
        vh_prob += hbias[i];
        h_prob[i] = sigmoid(vh_prob);
        if(h_state != NULL)
            h_state[i] = binomial(1, h_prob[i]);
    }

}
void RBM::activate_visible(int *h_state, double *v_prob, int *v_state, int n_hidden, int n_visible)
{	
    //h->v
    for(int i=0; i<n_visible; i++)
    {
        double hv_prob = 0;
        for(int j=0; j<n_hidden; j++)
        {
            hv_prob += h_state[j] * W[j][i];
        }
        hv_prob += vbias[i];
        v_prob[i] = sigmoid(hv_prob);
        v_state[i] = binomial(1, v_prob[i]);
    }	
}

void RBM::train(double *x, double gamma, int cd_k)
{
    double *pos_h_prob = new double[n_hidden];
    int *pos_h_state = new int[n_hidden];
    double *neg_v_prob = new double[n_visible];
    int *neg_v_state = new int[n_visible];
    double *neg_h_prob = new double[n_hidden];
    int *neg_h_state = new int[n_hidden];
    // use prob or state?
    // many tricks in 
    // A Practical Guide to Training Restricted Boltzmann Machines

    // postive phase
    activate_hidden(x, pos_h_prob, pos_h_state, n_visible, n_hidden);
    // negative phase
    for(int k=1; k<=cd_k; k++)
    {
        //h0->v1
        activate_visible(pos_h_state, neg_v_prob, neg_v_state, n_hidden, n_visible);
        //v1->h1	
        activate_hidden(neg_v_prob, neg_h_prob, neg_h_state, n_visible, n_hidden);

    }
    // update parameters
    for(int i=0; i<n_hidden; i++)
    {
        for(int j=0; j<n_visible; j++)
        {
            W[i][j] += momentum * 0 + gamma * ((x[j] * pos_h_prob[i] - neg_v_prob[j] * neg_h_prob[i]) / 1 - lamda * W[i][j]); 
        }
        hbias[i] += momentum * 0 + gamma * (pos_h_prob[i] - neg_h_prob[i]) / 1;
    }
    for(int i=0; i<n_visible; i++)
    {
        vbias[i] += momentum * 0 + gamma * (x[i] - neg_v_prob[i]) / 1; // 1 is #samples
    }

    error = square_error(x, neg_v_prob, n_visible);

    delete[] pos_h_prob;
    delete[] pos_h_state;
    delete[] neg_v_prob;
    delete[] neg_v_state;
    delete[] neg_h_prob;
    delete[] neg_h_state;

}
RBM::~RBM()
{
    for(int i=0; i < n_hidden; i++)
        delete[] W[i];
    delete[] W;
    delete[] hbias;
    delete[] vbias;
}


RBM1::RBM1(int N, int n_f, int n_h, double *w, double *hb, double *vb, double lbd, double mmt)
{
    n_samples = N;
    n_visible = n_f;
    n_hidden = n_h;
    error = 0.0;
    lamda = lbd;
    momentum = mmt;

    if(w == NULL)
    {
        W = new double*[n_hidden];
        for(int i=0; i<n_hidden; i++)
            W[i] = new double[n_visible];
        for(int i=0; i<n_hidden; i++)
            for(int j=0; j<n_visible; j++)
                W[i][j] = sample_from_gaussian(0, 0.01);
    }
    else
    {

        W = new double*[n_hidden];
        for(int i=0; i<n_hidden; i++)
        	W[i] = &w[i*n_visible];


    }

    if(hb == NULL)
    {
        hbias = new double[n_hidden];
        for(int i=0; i<n_hidden; i++)
            hbias[i] = 0;
    }
    else
        hbias = hb;

    if(vb == NULL)
    {
        vbias = new double[n_visible];
        for(int i=0; i<n_visible; i++)
            vbias[i] = 0;
    }
    else
        vbias = vb;


}
void RBM1::activate_hidden(double *v_prob, double *h_prob, int *h_state, int n_visible, int n_hidden)
{
    //v->h
    for(int i=0; i<n_hidden; i++)
    {
        double vh_prob = 0;
        for(int j=0; j<n_visible; j++)
        {
            // prob or state?
            vh_prob += v_prob[j] * W[i][j];
        }
        vh_prob += hbias[i];
        h_prob[i] = sigmoid(vh_prob);
        if(h_state != NULL)
            h_state[i] = binomial(1, h_prob[i]);
    }

}
void RBM1::activate_visible(int *h_state, double *v_prob, int *v_state, int n_hidden, int n_visible)
{
    //h->v
    for(int i=0; i<n_visible; i++)
    {
        double hv_prob = 0;
        for(int j=0; j<n_hidden; j++)
        {
            hv_prob += h_state[j] * W[j][i];
        }
        hv_prob += vbias[i];
        v_prob[i] = sigmoid(hv_prob);
        v_state[i] = binomial(1, v_prob[i]);
    }
}

void RBM1::train(double *x, double gamma, int cd_k)
{
    double *pos_h_prob = new double[n_hidden];
    int *pos_h_state = new int[n_hidden];
    double *neg_v_prob = new double[n_visible];
    int *neg_v_state = new int[n_visible];
    double *neg_h_prob = new double[n_hidden];
    int *neg_h_state = new int[n_hidden];
    // use prob or state?
    // many tricks in
    // A Practical Guide to Training Restricted Boltzmann Machines

    // postive phase
    activate_hidden(x, pos_h_prob, pos_h_state, n_visible, n_hidden);
    // negative phase
    for(int k=1; k<=cd_k; k++)
    {
        //h0->v1
        activate_visible(pos_h_state, neg_v_prob, neg_v_state, n_hidden, n_visible);
        //v1->h1
        activate_hidden(neg_v_prob, neg_h_prob, neg_h_state, n_visible, n_hidden);

    }
    // update parameters
    for(int i=0; i<n_hidden; i++)
    {
        for(int j=0; j<n_visible; j++)
        {
            W[i][j] += momentum * 0 + gamma * ((x[j] * pos_h_prob[i] - neg_v_prob[j] * neg_h_prob[i]) / 1 - lamda * W[i][j]);
        }
        hbias[i] += momentum * 0 + gamma * (pos_h_prob[i] - neg_h_prob[i]) / 1;
    }
    for(int i=0; i<n_visible; i++)
    {
        vbias[i] += momentum * 0 + gamma * (x[i] - neg_v_prob[i]) / 1; // 1 is #samples
    }

    error = square_error(x, neg_v_prob, n_visible);

    delete[] pos_h_prob;
    delete[] pos_h_state;
    delete[] neg_v_prob;
    delete[] neg_v_state;
    delete[] neg_h_prob;
    delete[] neg_h_state;

}
RBM1::~RBM1()
{
    for(int i=0; i < n_hidden; i++)
        delete[] W[i];
    delete[] W;
    delete[] hbias;
    delete[] vbias;
}

LR::LR(Dataset data, Conf conf)
{
    n_samples = data.N;
    n_features = data.n_f;
    n_labels = conf.n_labels;
    lamda = conf.lamda;

    W = new double*[n_labels];
    for(int i=0; i<n_labels; i++)
        W[i] = new double[n_features];
    b = new double[n_labels];

    for(int i=0; i<n_labels; i++)
    {
        for(int j=0; j<n_features; j++)
            W[i][j] = 0;
        b[i] = 0;
    }
}
LR::LR(int N, int n_f, int n_lb, double lbd)
{
    n_samples = N;
    n_features = n_f;
    n_labels = n_lb;
    lamda = lbd;

    W = new double*[n_labels];
    for(int i=0; i<n_labels; i++)
        W[i] = new double[n_features];
    b = new double[n_labels];

    for(int i=0; i<n_labels; i++)
    {
        for(int j=0; j<n_features; j++)
            W[i][j] = 0;
        b[i] = 0;
    }
}
LR::LR(int N, int n_f, int n_lb, double lbd,double *lrw,double *lrb)
{
    n_samples = N;
    n_features = n_f;
    n_labels = n_lb;
    lamda = lbd;


    W = new double*[n_labels];
    for(int i=0; i<n_labels; i++)
      W[i] = &lrw[i*n_features];

    b = lrb;

}

LR::~LR()
{
    for(int i=0; i<n_labels; i++)
        delete[] W[i];
    delete[] W;
    delete[] b;
}
void LR::train(double *x, int *y, double gamma, double caseNum)
{
    double *h_theta_x = new double[n_labels];
    double *dy = new double[n_labels];

    for(int i=0; i<n_labels; i++) 
    {
        h_theta_x[i] = 0;
        for(int j=0; j<n_features; j++) 
        {
            h_theta_x[i] += W[i][j] * x[j];
        }
        h_theta_x[i] += b[i];
    }
    softmax(h_theta_x);

    for(int i=0; i<n_labels; i++) 
    {
        dy[i] = (y[i]==1 ? 1:0) - h_theta_x[i];

        for(int j=0; j<n_features; j++) 
        {
           // W[i][j] -= gamma * (-1*dy[i] * x[j] + lamda * W[i][j]);
           W[i][j] += lamda * (dy[i]*x[j]/caseNum - gamma*W[i][j]);
        }

        b[i] += gamma * dy[i];
    }
    delete[] h_theta_x;
    delete[] dy;
}
void LR::softmax(double *x)
{
    double sum = 0.0;
    for(int i=0; i<n_labels; i++) 
    {
        x[i] = exp(x[i]);
        sum += x[i];
    } 

    for(int i=0; i<n_labels; i++) 
        x[i] /= sum;
}
void LR::predict(double *x, double *y)
{
    for(int i=0; i<n_labels; i++)
    {
        y[i] = 0;
        for(int j=0; j<n_features; j++)
        {
          //  y[i] += W[i][j] * x[j];
        }
        //y[i] += b[i];
    }
    softmax(y);

}







LR1::LR1(int N, int n_f, int n_lb, double lbd,double *lrw,double *lrb)
{
    n_samples = N;
    n_features = n_f;
    n_labels = n_lb;
    lamda = lbd;


    W = new double*[n_labels];
    for(int i=0; i<n_labels; i++)
      W[i] = &lrw[i*n_features];

    b = lrb;

}

LR1::~LR1()
{
    for(int i=0; i<n_labels; i++)
        delete[] W[i];
    delete[] W;
    delete[] b;
}
void LR1::train(double *x, int *y, double gamma)
{
    double *h_theta_x = new double[n_labels];
    double *dy = new double[n_labels];

    for(int i=0; i<n_labels; i++)
    {
        h_theta_x[i] = 0;
        for(int j=0; j<n_features; j++)
        {
            h_theta_x[i] += W[i][j] * x[j];
        }
        h_theta_x[i] += b[i];
    }
    softmax(h_theta_x);

    for(int i=0; i<n_labels; i++)
    {
        dy[i] = (y[i]==1 ? 1:0) - h_theta_x[i];

        for(int j=0; j<n_features; j++)
        {
            W[i][j] -= gamma * (-1*dy[i] * x[j] + lamda * W[i][j]);
        }

        b[i] += gamma * dy[i];
    }
    delete[] h_theta_x;
    delete[] dy;
}
void LR1::softmax(double *x)
{
    double sum = 0.0;
    for(int i=0; i<n_labels; i++)
    {
        x[i] = exp(x[i]);
        sum += x[i];
    }

    for(int i=0; i<n_labels; i++)
        x[i] /= sum;
}
void LR1::predict(double *x, double *y)
{
    for(int i=0; i<n_labels; i++)
    {
        y[i] = 0;
        for(int j=0; j<n_features; j++)
        {
            y[i] += W[i][j] * x[j];
        }
        y[i] += b[i];
    }
    softmax(y);
}
