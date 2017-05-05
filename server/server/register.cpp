#include "register.h"
#include <string>
#include <iostream>

Register::Register(){
	namemap = new map<string, string>;
	emailmap = new map<string, string>;
	cout << "Register generation" << endl;
}

Register::~Register(){
	delete namemap;
	delete emailmap;
	cout << "Register degeneration" << endl;
}

int Register::signup(const string name, const string password, const string email) const{
	map<string, string, string>::iterator key = namemap->find(name);
	if (key != namemap->end()){
		cout << name << ": user name has been registered" << endl;
		return NAMEEXIST;
	}

	key = emailmap->find(email);
	if (key != emailmap->end()){
		cout << email << ": email address has been registered"<<endl;
		return EMAILEXIST;
	}

	namemap->insert(make_pair(name, password));
	emailmap->insert(make_pair(email, password));

	cout << name <<": regitered succeed" << endl;
	return OK;

}

int Register::login(const string name, const string password) const{
	map<string, string, string>::iterator key = namemap->find(name);
	if (key == namemap->end()){
		cout << name << ": user name or email address is not existed!" << endl;
		return NAMEERROR;
	}

	if (key->second.c_str() != password){
		cout << name << ": passwaord error" << endl;
		return PASSWORDERROR;
	}

	cout << name <<": log in succeed" << endl;
	return OK;
}
