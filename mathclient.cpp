#include <iostream>
#include <fstream>
#include <string>
#include "socketfunctions.h"
#include "socketclient.h"
#include "neuralnetw.h"
#include "parsexml.h"
#include "socketxml.h"

#define CLOSE_OPEN_SOCKET  scl.skClose(ClientSock); \
                           ClientSock = scl.skConnect(); \
                           if (ClientSock == 0) \
                           { \
                               return 0; \
                           }

using namespace std;
using namespace socketxml;
using namespace parsexml;

//void EvalNeuralNetwork(char* fname, int tbcnt, char* buf);

double Y1[5000];
double X[5000 * 400];
double Y[5000 * 10];
double YO[10];
long long obj;
long long dataobj;
double* xp;
double* xp1;
double* yp;
double* yp1;
double* xt;
double* yt;
//NeuralNetwork* nt;

double ArrLin[3][4] = { {1, 1, 1, 6}, {1, 2, 2, 11}, {1, 3, 4, 19} };
double XLin[3];

void cpy(double* out, double* in, int qnt)
{
    for (int i = 0; i < qnt; i++)
    {
        *out++ = *in++;
    }
}

int main()
{
    /*ifstream in("t1.bin", ios_base::binary);
    if (in.is_open())
    {
        in.read((char*)Y1, sizeof(Y1));
    }

    in.close();

    std::ifstream in2("t2.bin", std::ifstream::binary);
    if (in2.is_open())
    {
        in2.read((char*)X, sizeof(X));
    }

    in2.close();

    for (int i = 0; i < 50000; i++)
    {
        Y[i] = 0;
    }

    int k = 0;
    for (int i = 0; i < 5000; i++)
    {
        k = (int)(Y1[i] + 0.1);
        if (k == 10)
        {
            k = 0;
        }
        Y[i * 10 + k] = 1.0;
    }

    double mx = -99999;
    double mn = 99999;

    for (int i = 0; i < (5000 * 400); i++)
    {
        if (mx < X[i])
        {
            mx = X[i];
        }
        if (mn > X[i])
        {
            mn = X[i];
        }
    }
    */

    MySocketClient scl;

    int nout[2] = { 25, 10 };

    int ClientSock = scl.skConnect();
    if (ClientSock == 0)
    {
        return 0;
    }

    int cmd = testsocketserver_Function;
    send(ClientSock, (const char*)&cmd, sizeof(int), 0);
    
    const char* str = "Testing server";
    scl.copyToBuffer((char*)str, (int)strlength(str) + 1);
    scl.sendBuffer(ClientSock, (int)strlength(str) + 1);

    cmd = scl.receiveBuffer(ClientSock);
    cout << scl.getDataBuffer() << endl;

    //CLOSE_OPEN_SOCKET

    /*int fsize = 5000 * 400 * sizeof(double);
    int blocksize = 30000;
    int blockqnt = fsize / blocksize;
    int lastblocksize = fsize - blockqnt * blocksize;
    if (lastblocksize == 0)
    {
        lastblocksize = blocksize;
    }
    else
    {
        blockqnt++;
    }
    for(int blockno = 0; blockno < blockqnt; blockno++)
    {
        CLOSE_OPEN_SOCKET

        scl.nnWriteFileBlock(ClientSock, fsize, blockqnt, blockno, blocksize, lastblocksize, ((char*)X) + blockno * blocksize);
        //scl.skClose(ClientSock);
        cout << "Block" << blockno << endl;
    }

    CLOSE_OPEN_SOCKET

    scl.nnWriteFile(ClientSock, (char*)"test.t", fsize);
    cout << "nnWriteFile" << endl;*/

    CLOSE_OPEN_SOCKET

    obj = scl.nnCreate(ClientSock, 400, 3, 5000, 10, 2, 0.12, nout, traceBufCnt, OPTIMIZATION_CPP);
    cout << "nnCreate" << endl;

    CLOSE_OPEN_SOCKET

    /*obj = scl.nnLoad(ClientSock, (char*)"nn.dat", traceBufCnt, OPTIMIZATION_ASM64);

    CLOSE_OPEN_SOCKET

    int errqnt = scl.getLastError(ClientSock);
    if (errqnt > 0)
    {
        cout << "Error \"" << scl.getDataBuffer() << "\"" << endl;
    }

    CLOSE_OPEN_SOCKET*/

    scl.nnSetPatternsFromFiles(ClientSock, obj, 400 * 5000, 10 * 5000, NORMALYZE_NO, (char*)"tx.bin", (char*)"ty.bin");
    cout << "nnSetPatternsFromFiles" << endl;

    CLOSE_OPEN_SOCKET

    nnLearnResultParms lrp;
    scl.nnLearn(ClientSock, obj, 500, 2, 0.02, 0);
    scl.copyFromBuffer((char*)&lrp, sizeof(lrp));
    cout << "nnLearn" << endl;
    cout << "Iterations: " << lrp.iter << ", success: " << lrp.success_qnt << ", norma: " << lrp.norma << endl;
    
    CLOSE_OPEN_SOCKET

    /*scl.nnSave(ClientSock, obj, (char*)"nn.dat");
    cout << "nnSave" << endl;

    CLOSE_OPEN_SOCKET*/

    scl.nnRecognize(ClientSock, obj, 400, 10, X + 400 * 972, YO);

    CLOSE_OPEN_SOCKET

    /*
    obj = scl.nnLoad(ClientSock, (char*)"nn.dat", traceBufCnt, OPTIMIZATION_ASM64);

    scl.Close(ClientSock);
    ClientSock = scl.Connect();
    if (ClientSock == NULL)
    {
        return 0;
    }*/
    /*
    scl.nnRecognize(ClientSock, obj, 400, 10, X + 400 * 1972, YO);

    scl.skClose(ClientSock);
    ClientSock = scl.skConnect();
    if (ClientSock == 0)
    {
        return 0;
    }

    errqnt = scl.getLastError(ClientSock);
    if (errqnt > 0)
    {
        cout << "Error \"" << scl.getDataBuffer() << "\"" << endl;
    }
    scl.skClose(ClientSock);
    ClientSock = scl.skConnect();
    if (ClientSock == 0)
    {
        return 0;
    }
    */

    bool fl = scl.nnDelete(ClientSock, obj);
    if (!fl)
    {
        cout << "Error deleting neural network" << endl;
    }

    CLOSE_OPEN_SOCKET

    cmd = closeMySocketServer_Function;
    send(ClientSock, (const char*)&cmd, sizeof(int), 0);
    scl.skClose(ClientSock);
}

