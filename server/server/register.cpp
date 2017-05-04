#include "register.h"


Register::Register(){
	namemap = new map<string, string>;
	emailmap = new map<string, string>;
	cout << "Register构造\n";
}

Register::~Register(){
	delete namemap;
	delete emailmap;
	cout << "Register析构\n";
}

int Register::signup(const string name, const string password, const string email){
	map<string, string, string>::iterator key = namemap->find("name");
	if (key == namemap->end()){
		cout << "用户名已被注册\n";
		return NAMEEXIST;
	}

	key = emailmap->find("email");
	if (key == namemap->end()){
		cout << "邮箱已被注册\n";
		return EMAILEXIST;
	}

	namemap->insert(make_pair(name, password));
	emailmap->insert(make_pair(email, password));

	cout << "注册成功\n";
	return OK;

}

int Register::login(const string name, const string password){
	map<string, string, string>::iterator key = namemap->find("name");
	if (key == namemap->end()){
		cout << "用户名或邮箱不存在\n";
		return NAMEERROR;
	}

	if (key->second.c_str() != password){
		cout << "密码错误\n";
		return PASSWORDERROR;
	}

	cout << "登录成功\n";
	return OK;
}
