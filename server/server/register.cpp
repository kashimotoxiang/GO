#include "register.h"
#include <string>
#include <iostream>
//console ÑÕÉ«
#include "ConsoleColor.h"

Register::Register(){
	namemap = new map<string, string>;
	emailmap = new map<string, string>;
	cout << blue << "Register generation" << white<< endl;
}

Register::~Register(){
	delete namemap;
	delete emailmap;
	cout << blue << "Register degeneration" << white<< endl;
}

int Register::signup(const string name, const string password, const string email) const{
	map<string, string, string>::iterator key = namemap->find(name);
	if (key != namemap->end()){
		cout<< red << name << ": user name has been registered" << white<< endl;
		return NAMEEXIST;
	}

	key = emailmap->find(email);
	if (key != emailmap->end()){
		cout << red << email << ": email address has been registered" << white <<endl;
		return EMAILEXIST;
	}

	namemap->insert(make_pair(name, password));
	emailmap->insert(make_pair(email, password));

	cout << green << name <<": regitered succeed" << white<< endl;
	return OK;

}

int Register::login(const string name, const string password) const{
	map<string, string, string>::iterator key = namemap->find(name);
	if (key == namemap->end()){
		cout << red << name << ": user name or email address is not existed!" << white<< endl;
		return NAMEERROR;
	}

	if (key->second.c_str() != password){
		cout << red << name << ": passwaord error" << white<< endl;
		return PASSWORDERROR;
	}

	cout << green << name <<": log in succeed" << white<< endl;
	return OK;
}
