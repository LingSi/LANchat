/*
 * Client.h
 *
 *  Created on: 2016-7-28
 *      Author: root
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "TypeDef.h"
#include "ClientList.h"
class ClientList;
class Client {
public:
	Client(char *ip,char *port,char *name);
	virtual ~Client();

	void Link(void);
	void Send(void);
	void Recv(void);

private:
	void LogIn(void);

	void Console(void);
	void ChatSend(void);
	void GetOnlineUser(void);
	void LogOut(void);

	inline void RecvMsg(Msg_t *msg);
	void ChatRecv(const Msg_t* const msg);
	void UserLogIn(const Msg_t* const msg);
	void UserLogOut(const Msg_t* const msg);
	void OnlineUser(Msg_t* const msg);
	void ServerQuit(void);

	User_t user;
	ClientList online;
	int sockfd;
	struct sockaddr_in server_addr;
	int flag;
	pthread_mutex_t mutex;

};

#endif /* CLIENT_H_ */
