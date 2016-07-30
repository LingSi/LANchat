/*
 * Server.h
 *
 *  Created on: 2016-7-29
 *      Author: root
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "TypeDef.h"
#include "ClientList.h"
class ClientList;
class Server {
public:
	Server(char *ip,char *port);
	virtual ~Server();

	void StartServer(void);
	void Send(void);
	void Recv(void);

	ClientList online;

private:
	void Console(void);
	void SerQuit(void);
	void SerChat(void);

	inline void RecvMsg(Msg_t *msg);
	void ClientChat(Msg_t *msg);
	void ClientLogIn(Msg_t *msg);
	void ClientLogOut(Msg_t *msg);

	void OnlineUser(Msg_t *msg);
	int SendtoOne(const Msg_t * msg);
	void SendtoAll(Msg_t * msg);

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t addrlen;
	int sockfd;
	int flag;
	UserId_t maxid;

};

#endif /* SERVER_H_ */
