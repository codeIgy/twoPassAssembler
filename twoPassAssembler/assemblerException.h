#ifndef ASSEMBLEREXCEPTION_H
#define ASSEMBLEREXCEPTION_H

#include <exception>

using namespace std;

class AssemblerException : public exception
{
public:
	AssemblerException(const char* msg) : msg(msg) {};
	const char* getMsg() { return msg; }
private:
	const char* msg;
};

#endif