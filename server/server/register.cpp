#include "register.h"


Register::Register(){
	namemap = new map<string, string>;
	emailmap = new map<string, string>;
	cout << "Register����\n";
}

Register::~Register(){
	delete namemap;
	delete emailmap;
	cout << "Register����\n";
}

int Register::signup(const string name, const string password, const string email){
	map<string, string, string>::iterator key = namemap->find("name");
	if (key == namemap->end()){
		cout << "�û����ѱ�ע��\n";
		return NAMEEXIST;
	}

	key = emailmap->find("email");
	if (key == namemap->end()){
		cout << "�����ѱ�ע��\n";
		return EMAILEXIST;
	}

	namemap->insert(make_pair(name, password));
	emailmap->insert(make_pair(email, password));

	cout << "ע��ɹ�\n";
	return OK;

}

int Register::login(const string name, const string password){
	map<string, string, string>::iterator key = namemap->find("name");
	if (key == namemap->end()){
		cout << "�û��������䲻����\n";
		return NAMEERROR;
	}

	if (key->second.c_str() != password){
		cout << "�������\n";
		return PASSWORDERROR;
	}

	cout << "��¼�ɹ�\n";
	return OK;
}
