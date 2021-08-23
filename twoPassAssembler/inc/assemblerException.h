#ifndef ASSEMBLEREXCEPTION_H
#define ASSEMBLEREXCEPTION_H

#include <exception>

using namespace std;

class AssemblerException : public exception
{
public:
	AssemblerException(string msg) : msg(msg) {};
	string getMsg() { return msg; }
private:
	string msg;
};

#endif