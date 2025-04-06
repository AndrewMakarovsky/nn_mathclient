#pragma once

#ifndef NNOBJECT_H
#define NNOBJECT_H

#include <string>

using namespace std;

class IniFile
{
public:
	int portno;
	string CurrentDirectory;
	string DataDirectory;

	static IniFile* GetIniFile();
	~IniFile();

	bool Load(string fname);
	bool Save(string fname);

private:
	static int count;
	static IniFile* iniFile;

protected:
	IniFile() {};

	string filename;
};

class NNObject
{
public:
	NNObject();
	NNObject(string s);
	~NNObject();

private:
	string name;
	IniFile* iniFile;
};

#endif
