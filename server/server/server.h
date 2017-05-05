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
class Server
{
public:
	 int onlineNum;//��������
	 int mode;//����&˫��
	 map<string, cChessboard*> *ChessBoardMap = nullptr;

	 Server ();
	 ~Server ();
};

void WebSocketInit ();