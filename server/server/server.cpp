#include "process.h"
#include "server.h"


//json全家桶
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>
#include <string>
#include "register.h"

#define WEBSend(x) s->send (hdl, x, sizeof (x), websocketpp::frame::opcode::TEXT)


extern cChessboard* Chessboard;
extern Register myRegister;

using namespace std;
using namespace boost::property_tree;

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::bind;
// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;
volatile bool gazetracking = false;

void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg){
	string status = "";//状态控制
	string received = msg->get_payload();
	//从string中解析json串  
	istringstream iss;
	iss.str(received);
	ptree item;
	try{
		read_json(iss, item);
	}
	catch (ptree_error& e){
		cout << "json解析失败\n";
		return;
	}
	ptree::iterator root_it = item.begin();

	//读取 id
	string key = item.get<string> ("key");;//key ID

	//分类
	if (key == "start"){
		if (Chessboard == nullptr)
			Chessboard = new cChessboard;
	}
	else if (key == "stop"){
		if (Chessboard != nullptr)
			delete Chessboard;
	}
	else if (key == "play"){
		if (Chessboard != nullptr){//要有棋盘
			int row, col;
			try{
				row = item.get<int>("row");
				col = item.get<int>("col");
			}
			catch (ptree_error& e){
				cout << "play解析失败\n";
				return;
			}
			//string sROW = received.substr (4, 2);//格式化读取字符串
			//string sCOL = received.substr (6, 2	);
			//int row = atoi (sROW.c_str ());//str2num
			//int col = atoi (sCOL.c_str ());
			play(row, col);

			//发送整个棋盘
			try{
				s->send(hdl, Chessboard->pan, sizeof (Chessboard->pan), websocketpp::frame::opcode::BINARY);
			}
			catch (const error_code& e){
				cout << "Sending message failed because: " << e
					<< "(" << e.message() << ")" << endl;
			}
		}
		else{
			// 没有棋盘
			WEBSend ("noChessboard");
		}
	}
	else if (key == "login"){
		string name, password;
		try{
			name = item.get<string>("name");
			password = item.get<string>("password");
		}
		catch (ptree_error& e){
			cout << "login解析失败\n";
			return;
		}

		switch (myRegister.login(name, password)){
		case OK: WEBSend ("OK");
			break;
		case NAMEEXIST: WEBSend ("NAMEEXIST");
			break;
		case EMAILEXIST: WEBSend ("EMAILEXIST");
			break;
		default: ;
		}

	}
	else if (key == "signup"){
		string name, password, email;
		try{
			name = item.get<string>("name");
			password = item.get<string>("password");
			email = item.get<string>("email");
		}
		catch (ptree_error& e){
			cout << "signup解析失败\n";
			return;
		}

		switch (myRegister.login(name, password)){
		case OK: WEBSend ("OK");
			break;
		case NAMEERROR: WEBSend ("NAMEERROR");
			break;
		case PASSWORDERROR: WEBSend ("PASSWORDERROR");
			break;
		default: ;
		}
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
