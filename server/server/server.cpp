#include "server.h"
//json全家桶
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>
#include <string>
#include <iostream>

#define WEBSend(x) s->send (hdl, x, sizeof (x), websocketpp::frame::opcode::TEXT)

extern Register myRegister;
extern Server mServer;

using namespace std;
using namespace boost::property_tree;
using websocketpp::lib::bind;

//服务器构造
Server::Server(){
	onlineNum = 1;
	mode = one;
	ChessBoardMap = new map<string, cChessboard*>;
	cout << "Sever generation" << endl;
}

Server::~Server(){
	delete ChessBoardMap;
	cout << "Server degeneration" << endl;
}

//websocket设置
// pull out the type of messages sent by our config
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg){
	string status = "";//状态控制
	string received = msg->get_payload();
	cout << endl << "recieve:" << received << endl << endl;
	//从string中解析json串  
	istringstream iss;
	iss.str(received);
	ptree item;
	try{
		read_json(iss, item);
	}
	catch (ptree_error){
		cout << "all:failed to read json" << endl;
		return;
	}

	//获取名称
	string name;
	try{
		name = item.get<string>("name");
	}
	catch (ptree_error){
		cout << "play:failed to read room name" << endl;
		return;
	}

	//读取 id
	string key = item.get<string>("key");

	//分类
	if (key == "start"){
	}
	//停止
	else if (key == "stop"){
		//获取房间指针
		map<string, cChessboard*>::iterator key = mServer.ChessBoardMap->find(name);
		if (key == mServer.ChessBoardMap->end()){
			cout << "stop:room is not exist!" << endl;
			return;
		}

		//销毁房间
		cChessboard* p = key->second;
		mServer.ChessBoardMap->erase (key);
		if (p != nullptr)
			delete p;
		else
			cout << "stop:room is empty!" << endl;

	}
	//模式切换
	//else if (key == "mode"){
	//  int _mode;
	//  try{
	//    _mode = item.get<int>("mode");
	//  }
	//  catch (ptree_error& e){
	//    cout  <<  "mode解析失败" << endl;
	//    return;
	//  }
	//  switch (_mode){
	//  case 1: mode = one;
	//    break;
	//  case 2: mode = two;
	//    break;
	//  default: break;
	//  }
	//}

	//登录
	else if (key == "login"){
		//if (mode == one && onlineNum >= 1 || mode == two && onlineNum >= 2)//人数溢出
		//{
		//  WEBSend ("OVERFLOW");
		//  return;
		//}
		string name, password;
		try{
			name = item.get<string>("name");
			password = item.get<string>("password");
		}
		catch (ptree_error){
			cout << "login:failed to read json" << endl;
			return;
		}

		int flag = myRegister.login (name, password);
		switch (flag){
		case OK: WEBSend ("LOGINOK");
			break;
		case NAMEEXIST: WEBSend ("NAMEEXIST");
			break;
		case EMAILEXIST: WEBSend ("EMAILEXIST");
			break;
		default: ;
		}

		if(flag== OK){
			//创建房间
			cChessboard* p = new cChessboard ();
			//压入房间名和房间指针
			mServer.ChessBoardMap->insert (make_pair (name, p));
		}

	}

	//注册
	else if (key == "signup"){
		string name, password, email;
		try{
			name = item.get<string>("name");
			password = item.get<string>("password");
			email = item.get<string>("email");
		}
		catch (ptree_error){
			cout << "signup:failed to read json" << endl;
			return;
		}

		if (name == "" || password == "" || email == ""){
			WEBSend ("PARAERROR");
			return ;
		}			

		switch (myRegister.signup(name, password, email)){
		case OK: WEBSend ("SIGNUPOK");
			break;
		case NAMEERROR: WEBSend ("NAMEERROR");
			break;
		case PASSWORDERROR: WEBSend ("PASSWORDERROR");
			break;
		default: ;
		}
	}

	//游戏处理
	else if (key == "play"){
		//获取房间指针
		map<string, cChessboard*>::iterator key = mServer.ChessBoardMap->find(name);
		if (key == mServer.ChessBoardMap->end()){
			cout << "stop:room is not exist!" << endl;
			return;

		}
		cChessboard* Chessboard = key->second;

		//单机版
		if (mServer.mode == one && mServer.onlineNum == 1){
			if (Chessboard != nullptr){//要有棋盘
				int row, col;
				try{
					row = item.get<int>("row");
					col = item.get<int>("col");
				}
				catch (ptree_error){
					cout << "play:failed to read json" << endl;
					return;
				}
				//棋盘处理
				play(row, col, Chessboard);
				//发送整个棋盘
				try{
					s->send(hdl, Chessboard->pan, sizeof (Chessboard->pan), websocketpp::frame::opcode::BINARY);
				}
				catch (const error_code& e){
					cout << "Sending message failed because: " << e << "(" << e.message() << ")" << endl;
				}
			}
			else{
				// 没有棋盘
				WEBSend ("noChessboard");
			}
		}
		////双人
		//else if (mode == two && onlineNum == 2)
		//{
		//  if (Chessboard != nullptr){//要有棋盘
		//    int row, col;
		//    try{
		//      row = item.get<int>("row");
		//      col = item.get<int>("col");
		//    }
		//    catch (ptree_error& e){
		//      cout  <<  "play解析失败" << endl;
		//      return;
		//    }
		//    //棋盘处理
		//    play(row, col);
		//    //发送整个棋盘
		//    try{
		//      s->send(hdl, Chessboard->pan, sizeof (Chessboard->pan), websocketpp::frame::opcode::BINARY);
		//    }
		//    catch (const error_code& e){
		//      cout  <<  "Sending message failed because: "  <<  e  <<  "("  <<  e.message()  <<  ")"  <<  endl;
		//    }
		//  }
		//  else{
		//    // 没有棋盘
		//    WEBSend ("noChessboard");
		//  }
		//}
		//else {
		//  WEBSend ("PeopleNumError");
		//  return;
		//}
	}

}

void WebSocketInit(){
	// Create a server endpoint
	server echo_server;

	try{
		// Set logging settings
		echo_server.set_access_channels(websocketpp::log::alevel::all);
		echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize Asio
		echo_server.init_asio();

		// Register our message handler
		echo_server.set_message_handler(bind(&on_message, &echo_server, placeholders::_1, placeholders::_2));

		// Listen on port 8181
		echo_server.listen(8181);

		// Start the server accept loop
		echo_server.start_accept();

		// Start the ASIO io_service run loop
		echo_server.run();
	}
	catch (websocketpp::exception const& e){
		cout << e.what() << endl;
	}
	catch (...){
		cout << "other exception" << endl;
	}
}
