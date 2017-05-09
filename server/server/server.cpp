#include "server.h"
//json全家桶
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>
#include <string>
#include <iostream>
//console 颜色
#include "ConsoleColor.h"

#define WEBSend(x) s->send (hdl, x, sizeof (x), websocketpp::frame::opcode::TEXT)

extern Register myRegister;
extern Server mServer;

using namespace std;
using namespace boost::property_tree;
using websocketpp::lib::bind;

//服务器构造
Server::Server () {
	onlineNum = 1;
	//mode = one;
	ChessBoardMap = new map<string, cChessboard*>;
	cout << blue << "Sever generation" << white << endl;
}

Server::~Server () {
	delete ChessBoardMap;
	cout << blue << "Server degeneration" << white << endl;
}

//websocket设置
// pull out the type of messages sent by our config
void on_message (server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	string status = "";//状态控制
	string received = msg->get_payload ();
	cout << endl << green << "recieve:" << received << white << endl << endl;
	//从string中解析json串  
	istringstream iss;
	iss.str (received);
	ptree item;
	try {
		read_json (iss, item);
	}
	catch (ptree_error) {
		cout << red << "all:failed to read json" << white << endl;
		return;
	}

	//获取名称 模式
	string name, mode;
	try {
		name = item.get<string> ("name");
		mode = item.get<string> ("mode");
	}
	catch (ptree_error) {
		cout << red << "play:failed to read room name or mode" << white << endl;
		return;
	}

	//读取 id
	string key = item.get<string> ("key");

	//分类
	if (key == "start") {
		//创建房间
		cChessboard* p = new cChessboard ();
		//压入房间名和房间指针
		mServer.ChessBoardMap->insert (make_pair (name, p));

		//发送整个棋盘
		try {
			//创建消息对象
			mMessage mp;

			//name转换
			int i = 0;
			for (; i < name.length (); i++)
				mp.name[i] = name[i];
			for (; i < sizeof (mp.name) / sizeof (mp.name[0]); i++)
				mp.name[i] = -1;

			//棋盘转换
			for (int i = (*p).lengthBegin; i < (*p).lengthEnd; i++)
				for (int j = (*p).widthBegin; j < (*p).widthEnd; j++)
					mp.pan[i - (*p).lengthBegin][j - (*p).widthBegin] = p->pan[i][j];

			//发送消息
			s->send (hdl, &mp, sizeof (mp), websocketpp::frame::opcode::BINARY);
		}
		catch (const error_code& e) {
			cout << red << "Sending message failed because: " << e << "(" << e.message () << ")" << white << endl;
		}

	}
	//停止
	else if (key == "stop") {
		//获取房间指针
		map<string, cChessboard*>::iterator key = mServer.ChessBoardMap->find (name);
		if (key == mServer.ChessBoardMap->end ()) {
			cout << yellow << "stop:room is not exist!" << white << endl;
			return;
		}

		//销毁房间
		cChessboard* p = key->second;
		mServer.ChessBoardMap->erase (key);
		if (p != nullptr)
			delete p;
		else
			cout << yellow << "stop:room is empty!" << white << endl;

	}

	//登录
	else if (key == "login") {
		string password;
		try {
			password = item.get<string> ("password");
		}
		catch (ptree_error) {
			cout << red << "login:failed to read json" << white << endl;
			return;
		}

		int flag = myRegister.login (name, password);
		switch (flag) {
		case OK: WEBSend ("LOGINOK");
			break;
		case NAMEERROR: WEBSend ("NAMEERROR");
			break;
		case PASSWORDERROR: WEBSend ("PASSWORDERROR");
			break;
		default:;
		}
	}

	//注册
	else if (key == "signup") {
		string  password, email;
		try {
			password = item.get<string> ("password");
			email = item.get<string> ("email");
		}
		catch (ptree_error) {
			cout << red << "signup:failed to read json" << white << endl;
			return;
		}

		if (name == "" || password == "" || email == "") {
			WEBSend ("PARAERROR");
			return;
		}

		switch (myRegister.signup (name, password, email)) {
		case OK: WEBSend ("SIGNUPOK");
			break;
		case NAMEEXIST: WEBSend ("NAMEEXIST");
			break;
		case EMAILEXIST: WEBSend ("EMAILEXIST");
			break;
		default:;
		}
	}

	//游戏处理
	else if (key == "play") {
		//获取房间指针
		map<string, cChessboard*>::iterator key = mServer.ChessBoardMap->find (name);
		if (key == mServer.ChessBoardMap->end ()) {
			cout << yellow << "play:room is not exist!" << white << endl;
			return;
		}

		cChessboard* Chessboard = key->second;

		if (Chessboard != nullptr) {//要有棋盘
			int row, col;
			try {
				row = item.get<int> ("row");
				col = item.get<int> ("col");
			}
			catch (ptree_error) {
				cout << red << "play:failed to read json" << white << endl;
				return;
			}
			//棋盘处理
			play (row + 1, col + 1, Chessboard);
			//发送整个棋盘
			try {
				//创建消息对象
				mMessage p;

				//name转换
				int i = 0;
				for (; i < name.length (); i++)
					p.name[i] = name[i];
				for (; i < sizeof (p.name) / sizeof (p.name[0]); i++)
					p.name[i] = -1;

				//棋盘转换
				for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++)
					for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++)
						p.pan[i - (*Chessboard).lengthBegin][j - (*Chessboard).widthBegin] = Chessboard->pan[i][j];

				//发送消息
				s->send (hdl, &p, sizeof (p), websocketpp::frame::opcode::BINARY);
			}
			catch (const error_code& e) {
				cout << red << "Sending message failed because: " << e << "(" << e.message () << ")" << white << endl;
			}
		}
		else {
			// 没有棋盘
			WEBSend ("noChessboard");
		}
	}
}

void WebSocketInit () {
	// Create a server endpoint
	server echo_server;

	try {
		// Set logging settings
		echo_server.set_access_channels (websocketpp::log::alevel::all);
		echo_server.clear_access_channels (websocketpp::log::alevel::frame_payload);

		// Initialize Asio
		echo_server.init_asio ();

		// Register our message handler
		echo_server.set_message_handler (bind (&on_message, &echo_server, placeholders::_1, placeholders::_2));

		// Listen on port 8181
		echo_server.listen (8181);

		// Start the server accept loop
		echo_server.start_accept ();

		// Start the ASIO io_service run loop
		echo_server.run ();
	}
	catch (websocketpp::exception const& e) {
		cout << white << e.what () << white << endl;
	}
	catch (...) {
		cout << white << "other exception" << white << endl;
	}
}
