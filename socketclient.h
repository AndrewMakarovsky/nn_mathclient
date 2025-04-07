#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstring> 
#include <iostream> 
#include <stdio.h>
#include <string>
#include <vector>
#include "socketfunctions.h"
#include "socketxml.h"

using namespace std;
using namespace socketxml;

#define headerBufferSize 1024
#define bufferSize 65536
#define traceBufCnt 65536
#define MaxErrorQnt 32
#define MaxErrorMessLength 128
#define MyErrMessLength 512

class MySocketClient
{
public:

	MySocketClient();
	~MySocketClient();
	int skConnect();
	void skClose(int client);
	void sendBuffer(int client, int qnt);
	int receiveBuffer(int client);
	bool sendBlock(int client, char* buf, int qnt);
	void sendBigData(int client, char* buf, int qnt);
	bool receiveBlock(int client, char* buf, int qnt);
	int receiveBigData(int client, char* buf);
	int getLastError(int client);
	void copyToBuffer(char* src, int n);
	void copyFromBuffer(char* dest, int n);
	char* getDataBuffer();

	long long nnCreate(int client, int n, int _nlayers, int _pqnt, int _pqnt_blocks, int _nthreads, double _rand_epsilon, int* nout, int _tracebufcnt, int opt);
	long long nnLoad(int client, char* fname, int tbcnt, int opt);
	bool nnDelete(int client, long long nnet);
	//void nnSetPatterns(int client, long long nnet, int n, int m, double* _X, double* _Y);
	void nnSetPatternsFromFiles(int client, long long nnet, int n, int m, int norm_type, char* f1, char* f2);
	bool nnLearn(int client, long long nnet, int maxiter, double lambda, double epsilon, double lambdareg);
	bool nnRecognize(int client, long long nnet, int n, int m, double* _X, double* _Y);
	void nnSave(int client, long long nnet, char* fname);
	int nnGetOutQnt(int client, long long nnet);
	void nnGetCurrentDirectory(int client, long long nnet, char* dir);
	bool nnSetCurrentDirectory(int client, long long nnet, char* dir);
	void nnWriteFile(int client, char* fn, int n);
	void nnWriteFileBlock(int client, int filesize, int blockqnt, int blockno, int blocksize, int lastblocksize, char* data);
	int nnReadFile(int client, long long nnet, char* fn, int n, double* x);
	long long nndpCreate(int client, int _qnt, int _n_in, int _n_out, double* x);
	int nndpGetQnt(int client, long long dp);
	void nndpGetX(int client, long long dp, double* x);
	double nndpGetMax(int client, long long dp);
	double nndpGetMin(int client, long long dp);
	int nndpNormalyze(int client, long long dp, int qnt, double* in, double* out);
	void nndpDelete(int client, long long dp);
	void testMySocketServer(int client, char* s);
	void closeMySocketServer(int client);

private:
	char *dataBuffer;
};


