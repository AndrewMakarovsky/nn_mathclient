#include "socketclient.h"

MySocketClient::MySocketClient()
{
    dataBuffer = new char[bufferSize];
}

MySocketClient::~MySocketClient()
{
    delete[] dataBuffer;
}

int MySocketClient::skConnect()
{
    sockaddr_in servInfo;

    int ClientSock = socket(AF_INET, SOCK_STREAM, 0);

    if (ClientSock < 0) {
        std::cout << "Error initialization socket # " << errno << std::endl;
        close(ClientSock);
        return NULL;
    }
    else
        std::cout << "Server socket initialization is OK" << std::endl;

    in_addr ip_to_num;
    const char* ipadr = "127.0.0.1";
    int erStat = inet_pton(AF_INET, ipadr, &ip_to_num);
    if (erStat <= 0) {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return NULL;
    }

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;	  // Server's IPv4 after inet_pton() function
    servInfo.sin_port = htons(5000);

    erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

    if (erStat != 0) {
        std::cout << "Connection to Server is FAILED. Error # " << errno << std::endl;
        close(ClientSock);
        return NULL;
    }
    else
    {
        std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server"
            << std::endl;
    }

    return ClientSock;
}

void MySocketClient::skClose(int client)
{
    close(client);
}

void MySocketClient::sendBuffer(int client, int qnt)
{
    send(client, (char*)&qnt, sizeof(int), 0);
    send(client, (char*)dataBuffer, qnt, 0);
}

int MySocketClient::receiveBuffer(int client)
{
    int qnt;

    recv(client, (char*)&qnt, sizeof(int), 0);
    recv(client, (char*)dataBuffer, qnt, 0);

    return qnt;
}

/*bool MySocketClient::sendBlock(int client, char* buf, int qnt)
{
    int rqnt, rqnt2;

    send(client, (char*)&qnt, sizeof(int), 0);
    send(client, (char*)buf, qnt, 0);
    rqnt2 = recv(client, (char*)&rqnt, sizeof(int), 0);
    if (rqnt2 != sizeof(int))
    {
        return false;
    }
    else
    {
        if (rqnt != qnt)
        {
            return false;
        }
    }

    return true;
}*/

/*void MySocketClient::sendBigData(int client, char* buf, int qnt)
{
    char* p = buf;

    int nblocks = qnt / bufferSize;
    int lastblock = qnt % bufferSize;

    send(client, (char*)&qnt, sizeof(int), 0);

    for (int i = 0; i < nblocks; i++)
    {
        int count = 0;
        while (!sendBlock(client, p, bufferSize) && count < 3)
        {
            count++;
        }

        p += bufferSize;
    }

    if (lastblock > 0)
    {
        int count = 0;
        while (!sendBlock(client, p, lastblock) && count < 3)
        {
            count++;
        }
    }
}

bool MySocketClient::receiveBlock(int client, char* buf, int qnt)
{
    int rqnt;

    rqnt = recv(client, (char*)&qnt, sizeof(int), 0);
    rqnt = recv(client, (char*)buf, qnt, 0);
    if (rqnt != sizeof(int))
    {
        return false;
    }
    else
    {
        if (rqnt != qnt)
        {
            return false;
        }
    }
    send(client, (char*)&rqnt, sizeof(int), 0);

    return true;
}

int MySocketClient::receiveBigData(int client, char* buf)
{
    int qnt;
    char* p = buf;

    recv(client, (char*)&qnt, sizeof(int), 0);

    int nblocks = qnt / bufferSize;
    int lastblock = qnt % bufferSize;

    for (int i = 0; i < nblocks; i++)
    {
        int count = 0;
        while (!receiveBlock(client, p, bufferSize) && count < 3)
        {
            count++;
        }

        p += bufferSize;
    }

    if (lastblock > 0)
    {
        int count = 0;
        while (!receiveBlock(client, p, lastblock) && count < 3)
        {
            count++;
        }
    }

    return qnt;
}*/

void MySocketClient::copyToBuffer(char* src, int n)
{
    char* p = dataBuffer;

    for (int i = 0; i < n; i++)
    {
        *p++ = *src++;
    }
}
void MySocketClient::copyFromBuffer(char* dest, int n)
{
    char* p = dataBuffer;

    for (int i = 0; i < n; i++)
    {
        *dest++ = *p++;
    }
}

char* MySocketClient::getDataBuffer()
{
    return dataBuffer;
}

long long MySocketClient::nnCreate(int client, int n, int nlayers, int pqnt, int pqnt_blocks, int nthreads, double rand_epsilon, int* nout, int tracebufcnt, int opt)
{
    long long ret = 0;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnCreateParms parms;
    int cmd = nnCreate_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addInt(n);
    cxml.addInt(nlayers);
    cxml.addInt(pqnt);
    cxml.addInt(pqnt_blocks);
    cxml.addInt(nthreads);
    cxml.addDouble(rand_epsilon);
    cxml.addArrayInt(nout, nlayers - 1);
    cxml.addInt(tracebufcnt);
    cxml.addInt(opt);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == sizeof(long long))
    {
        ret = *((long long*)dataBuffer);
    }

    return ret;
}

long long MySocketClient::nnLoad(int client, char* fname, int tbcnt, int opt)
{
    long long ret = 0;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnLoadParms parms;
    int cmd = nnLoad_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addString(fname);
    cxml.addInt(tbcnt);
    cxml.addInt(opt);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, fname, tbcnt, opt);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == sizeof(long long))
    {
        ret = *((long long*)dataBuffer);
    }

    return ret;
}

bool MySocketClient::nnDelete(int client, long long nnet)
{
    int cmd = nnDelete_Function;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnDeleteParms parms;

    send(client, (const char*)&cmd, sizeof(int), 0);

    /*parms.nnet = nnet;
    copyToBuffer((char*)&parms, sizeof(parms));
    sendBuffer(client, sizeof(parms));*/

    cxml.addLong(nnet);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);
    /*if (cmd != traceBufCnt)
    {
        return false;
    }*/

    return true;
}

//n и m - количество элементов типа double  в массивах _X и _Y
/*
void MySocketClient::nnSetPatterns(int client, long long nnet, int n, int m, double* _X, double* _Y)
{
    int cmd = nnSetPatterns_Function;
    int xn, yn, sqnt;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnSetPatternsParms parms;

    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);
    cxml.addInt(n);
    cxml.addInt(m);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, n, m);
    copyToBuffer(xml, (int)strlength(xml) + 1);
    sendBuffer(client, (int)strlength(xml) + 1);
    delete[] xml;

    xn = n * sizeof(double);
    yn = m * sizeof(double);
    sqnt = write(client, (void*)_X, xn);
    if (sqnt != xn)
    {
        cout << "Error 1 send in nnSetPatterns " << errno << ", sqnt = " << sqnt << ", xn = " << xn << endl;
        return;
    }
    sqnt = write(client, (void*)_Y, yn);
    if (sqnt != yn)
    {
        cout << "Error 2 send in nnSetPatterns " << errno << ", sqnt = " << sqnt << ", yn = " << yn << endl;
        return;
    }

    //sendBigData(client, (char*)_X, n * sizeof(double));
    //sendBigData(client, (char*)_Y, m * sizeof(double));
}*/

void MySocketClient::nnSetPatternsFromFiles(int client, long long nnet, int n, int m, int norm_type, char* f1, char* f2)
{
    int cmd = nnSetPatternsFromFiles_Function;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnSetPatternsParms parms;

    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);
    cxml.addInt(n);
    cxml.addInt(m);
    cxml.addInt(norm_type);
    cxml.addString(f1);
    cxml.addString(f2);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, n, m, f1, f2);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;
}

bool MySocketClient::nnLearn(int client, long long nnet, int maxiter, double lambda, double epsilon, double lambdareg)
{
    int cmd = nnLearn_Function;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnLearnParms parms;

    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);
    cxml.addInt(maxiter);
    cxml.addDouble(lambda);
    cxml.addDouble(epsilon);
    cxml.addDouble(lambdareg);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, maxiter, lambda, epsilon, lambdareg);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);
    /*if (cmd != traceBufCnt)
    {
        return false;
    }*/

    return true;
}

bool MySocketClient::nnRecognize(int client, long long nnet, int n, int m, double* _X, double* _Y)
{
    int cmd = nnRecognize_Function;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnRecognizeParms parms;

    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);
    cxml.addInt(n);
    cxml.addInt(m);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, n, m);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    copyToBuffer((char*)_X, n * sizeof(double));
    sendBuffer(client, n * sizeof(double));
    /*copyToBuffer((char*)_Y, m * sizeof(double));
    sendBuffer(client, m * sizeof(double));*/

    cmd = receiveBuffer(client);
    if (cmd != m * sizeof(double))
    {
        return false;
    }
    copyFromBuffer((char*)_Y, m * sizeof(double));

    return true;
}

void MySocketClient::nnSave(int client, long long nnet, char* fname)
{
    //nnSaveParms parms;
    char* xml = new char[1024];
    CreateXML cxml;
    int cmd = nnSave_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);
    cxml.addString(fname);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, fname);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;
}

int MySocketClient::nnGetOutQnt(int client, long long nnet)
{
    //nnGetOutQntParms parms;
    int cmd = nnGetOutQnt_Function;
    char* xml = new char[1024];
    CreateXML cxml;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);

    return cmd;
}

void MySocketClient::nnGetCurrentDirectory(int client, long long nnet, char* dir)
{
    char* xml = new char[1024];
    CreateXML cxml;
    //nnLoadParms parms;
    int cmd = nnGetCurrentDirectory_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);
    copyFromBuffer(dir, cmd);
}

bool MySocketClient::nnSetCurrentDirectory(int client, long long nnet, char* dir)
{
    int ret = 0;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnLoadParms parms;
    int cmd = nnSetCurrentDirectory_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);
    cxml.addString(dir);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, dir);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == sizeof(int))
    {
        ret = *((int*)dataBuffer);
    }

    return (ret != 0 ? false : true);
}

void MySocketClient::nnWriteFileBlock(int client, int filesize, int blockqnt, int blockno, int blocksize, int lastblocksize, char* data)
{
    char* xml = new char[1024];
    CreateXML cxml;
    //nnLoadParms parms;
    int cmd = nnWriteFileBlock_Function;
    int qnt;
    send(client, (char*)&cmd, sizeof(int), 0);

    cxml.addInt(filesize);
    cxml.addInt(blockqnt);
    cxml.addInt(blockno);
    cxml.addInt(blocksize);
    cxml.addInt(lastblocksize);

    for(int i = 0; i < 1024; i++)
    {
        xml[i] = 0;
    }

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    copyToBuffer(xml, (int)strlength(xml) + 1);
    sendBuffer(client, (int)strlength(xml) + 1);

    qnt = blocksize;
    if (blockno == blockqnt - 1)
    {
        qnt = lastblocksize;
    }

    delete[] xml;

    send(client, (char*)data, qnt, 0);
}

void MySocketClient::nnWriteFile(int client, char* fn, int n)
{
    int ret = 0;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnLoadParms parms;
    int cmd = nnWriteFile_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addInt(n);
    cxml.addString(fn);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, n, fn);
    copyToBuffer(xml, (int)strlength(xml) + 1);
    sendBuffer(client, (int)strlength(xml) + 1);
    delete[] xml;
}

/*int MySocketClient::nnReadFile(int client, long long nnet, char* fn, int n, double* x)
{
    int ret = 0;
    char* xml = new char[1024];
    CreateXML cxml;
    //nnLoadParms parms;
    int cmd = nnReadFile_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    cxml.addLong(nnet);
    cxml.addString(fn);
    cxml.addInt(n);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, nnet, fn, n);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBigData(client, (char*)x);

    if (cmd == n * sizeof(double))
    {
        ret = cmd;
    }

    return ret;
}*/

long long MySocketClient::nndpCreate(int client, int _qnt, int _n_in, int _n_out, double* _X)
{
    long long ret = 0;
    //nndpCreateParms parms;
    int cmd = nndpCreate_Function;
    char* xml = new char[1024];
    send(client, (const char*)&cmd, sizeof(int), 0);

    //getXML(1024, (char*)xml, _qnt, _n_in, _n_out);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));

    //sendBigData(client, (char*)_X, _qnt * _n_in * sizeof(double));

    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == sizeof(long long))
    {
        ret = *((long long*)dataBuffer);
    }

    return ret;
}

int MySocketClient::nndpGetQnt(int client, long long dp)
{
    int ret = 0;
    //nndpGetQntParms parms;
    int cmd = nndpGetQnt_Function;
    char* xml = new char[1024];
    send(client, (const char*)&cmd, sizeof(int), 0);

    //getXML(1024, (char*)xml, dp);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));

    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == sizeof(int))
    {
        ret = *((int*)dataBuffer);
    }

    return ret;
}

void MySocketClient::nndpGetX(int client, long long dp, double* _X)
{
    //nndpGetXParms parms;
    int cmd = nndpGetX_Function;
    char* xml = new char[1024];
    send(client, (const char*)&cmd, sizeof(int), 0);

    //getXML(1024, (char*)xml, dp);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    //cmd = receiveBigData(client, (char*)_X);
}

double MySocketClient::nndpGetMax(int client, long long dp)
{
    double ret = 0;
    //nndpGetMaxParms parms;
    int cmd = nndpGetMax_Function;
    char* xml = new char[1024];
    send(client, (const char*)&cmd, sizeof(int), 0);

    //getXML(1024, (char*)xml, dp);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));

    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == sizeof(int))
    {
        ret = *((int*)dataBuffer);
    }

    return ret;
}

double MySocketClient::nndpGetMin(int client, long long dp)
{
    double ret = 0;
    //nndpGetMinParms parms;
    int cmd = nndpGetMin_Function;
    char* xml = new char[1024];
    send(client, (const char*)&cmd, sizeof(int), 0);

    //getXML(1024, (char*)xml, dp);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));

    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == sizeof(int))
    {
        ret = *((int*)dataBuffer);
    }

    return ret;
}

int MySocketClient::nndpNormalyze(int client, long long dp, int qnt, double* in, double* out)
{
    //nndpNormalyzeParms parms;
    int cmd = nndpNormalyze_Function;
    char* xml = new char[1024];
    send(client, (const char*)&cmd, sizeof(int), 0);

    //getXML(1024, (char*)xml, dp, qnt);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));

    //sendBigData(client, (char*)in, qnt);

    delete[] xml;

    //cmd = receiveBigData(client, (char*)out);

    return cmd;
}

void MySocketClient::nndpDelete(int client, long long dp)
{
    //nndpDeleteParms parms;
    int cmd = nndpDelete_Function;
    char* xml = new char[1024];
    send(client, (const char*)&cmd, sizeof(int), 0);

    //getXML(1024, (char*)xml, dp);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;
}

int MySocketClient::getLastError(int client)
{
    int cmd = nnGeLastError_Function;
    int qnt, rqnt;

    send(client, (void*)&cmd, sizeof(int), 0);
    rqnt = recv(client, (void*)&qnt, sizeof(int), 0);
    if (rqnt != sizeof(int))
    {
        cout << "Error 1 receive in getLastError " << errno << endl;
        return - 1;
    }
    if (qnt > 0)
    {
        rqnt = recv(client, (void*)dataBuffer, qnt, 0);

        if (rqnt != qnt)
        {
            cout << "Error 2 receive in getLastError " << errno << endl;
            return - 1;
        }

        dataBuffer[qnt] = 0;
    }

    return qnt;
}

void MySocketClient::testMySocketServer(int client, char* s)
{
    char* xml = new char[1024];
    socketxml::CreateXML cxml;
    int cmd = testsocketserver_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);

    /*parms.tbcnt = tbcnt;
    for (cmd = 0; cmd < 256; cmd++)
    {
        parms.fname[cmd] = fname[cmd];

        if (fname[cmd] == 0)
        {
            cmd++;
            break;
        }
    }
    parms.fname_qnt = cmd;
    copyToBuffer((char*)&parms, sizeof(parms));
    sendBuffer(client, sizeof(parms));*/

    cxml.addString(s);

    copystr(xml, (char*)cxml.getXML().c_str(), 1024);
    //getXML(1024, (char*)xml, s);
    copyToBuffer(xml, (int)strlength(xml));
    sendBuffer(client, (int)strlength(xml));
    delete[] xml;

    cmd = receiveBuffer(client);

    if (cmd == strlength(xml))
    {
        //ret = *((long long*)dataBuffer);
    }
}

void MySocketClient::closeMySocketServer(int client)
{
    int cmd = closeMySocketServer_Function;
    send(client, (const char*)&cmd, sizeof(int), 0);
}
