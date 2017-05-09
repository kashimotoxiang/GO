#pragma once
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "register.h"
#include "process.h"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

typedef enum
{
	one=1,
	two,
}eMode;

typedef struct
{
	int name[30];//ǰ30��Ϊ����
	int pan[19][19];
}mMessage;

class Server
{
public:
	 int onlineNum;//��������
	 //int mode;//����&˫��
	 map<string, cChessboard*> *ChessBoardMap = nullptr;

	 Server ();
	 ~Server ();
};

void WebSocketInit ();