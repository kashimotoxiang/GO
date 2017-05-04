#pragma once

#include <string>
#include <map>
#include <iostream>
#include "register.h"
using namespace std;

typedef enum
{
	OK = 0,
	NAMEEXIST,
	EMAILEXIST,
	NAMEERROR,
	PASSWORDERROR,
} RegisterStatu;

class Register
{
private:
	map<string, string>* namemap = nullptr;
	map<string, string>* emailmap = nullptr;
public:
	Register();
	~Register();
	int signup(const string name, const string password, const string email);
	int login(const string name, const string password);
};
