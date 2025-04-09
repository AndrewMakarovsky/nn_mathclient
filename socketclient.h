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
	//bool sendBlock(int client, char* buf, int qnt);
	//void sendBigData(int client, char* buf, int qnt);
	//bool receiveBlock(int client, char* buf, int qnt);
	//int receiveBigData(int client, char* buf);
	int getLastError(int client);  //Returns server last error in dataBuffer, dataBuffer[0] == 0 if no error
	void copyToBuffer(char* src, int n); //Copy array of char to dataBuffer, n - number of characters
	void copyFromBuffer(char* dest, int n); //Copy dataBuffer to array of char, n - number of characters
	char* getDataBuffer(); //Returns dataBuffer

	//Neural network functions, client - opened socket id
	//Creating a neural network
	//Parameters:
	//int n - dimension of the vector of the training pattern
	//int nlayers - number of layers (training pattern is 1 layer)
	// int pqnt - number of the training patterns
	// int pqnt_blocks - number of blocks
	// int nthreads - number of threads
	// double rand_epsilon - a constant for initializing weight matrices with random numbers
	// int* nout - an array of the number of outputs of each layer
	// int tracebufcnt - size of trace buffer
	// int opt - C++ only or C++ and Assembler
	// Returns NeuralNetwork object pointer as long long (8 bytes)
	long long nnCreate(int client, int n, int _nlayers, int _pqnt, int _pqnt_blocks, int _nthreads, double _rand_epsilon, int* nout, int _tracebufcnt, int opt);
	
	// Downloads a saved trained neural network from a file fname, tbcnt - size of trace buffer, opt like in nnCreate
	// Returns NeuralNetwork object pointer as long long (8 bytes)
	long long nnLoad(int client, char* fname, int tbcnt, int opt);
	
	//Deletes NeuralNetwork object which was created with nnCreate or nnLoad
	bool nnDelete(int client, long long nnet);

	//void nnSetPatterns(int client, long long nnet, int n, int m, double* _X, double* _Y);

	// Loades patterns from files f1 (X - input data) and f2 (Y - output data), norm_type - normalization type (for X),
	// n and m - quantity of doubles in files f1 and f2
	void nnSetPatternsFromFiles(int client, long long nnet, int n, int m, int norm_type, char* f1, char* f2);

	// Training neural network, maxiter - number of iterations
	bool nnLearn(int client, long long nnet, int maxiter, double lambda, double epsilon, double lambdareg);

	// Recognize pattern X
	// n and m - dimensions X and Y
	// Returns Y
	bool nnRecognize(int client, long long nnet, int n, int m, double* _X, double* _Y);

	// Save NeuralNetwork object to file fname
	void nnSave(int client, long long nnet, char* fname);

	int nnGetOutQnt(int client, long long nnet);

	void nnGetCurrentDirectory(int client, long long nnet, char* dir);

	bool nnSetCurrentDirectory(int client, long long nnet, char* dir);

	// Functions for send array to server and save it to file fn
	void nnWriteFile(int client, char* fn, int n);
	void nnWriteFileBlock(int client, int filesize, int blockqnt, int blockno, int blocksize, int lastblocksize, char* data);

	//int nnReadFile(int client, long long nnet, char* fn, int n, double* x);

	// This functions are not implemented 
	long long nndpCreate(int client, int _qnt, int _n_in, int _n_out, double* x);
	int nndpGetQnt(int client, long long dp);
	void nndpGetX(int client, long long dp, double* x);
	double nndpGetMax(int client, long long dp);
	double nndpGetMin(int client, long long dp);
	int nndpNormalyze(int client, long long dp, int qnt, double* in, double* out);
	void nndpDelete(int client, long long dp);

	// Testing socket server, send string s to server and receive this string in dataBuffer
	void testMySocketServer(int client, char* s);

	// Closes socket server
	void closeMySocketServer(int client);

private:
	char *dataBuffer;
};


