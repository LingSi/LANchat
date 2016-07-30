/*
 * Server.cpp
 *
 *  Created on: 2016-7-29
 *      Author: root
 */
using namespace std;
#include <iostream>
#include <list>
#include <string.h>

#include "wrapsock.h"
#include "wrapstdio.h"
#include "wrapunix.h"

#include "Server.h"


/*================================================================================
Part 1:
		Start UDP server
================================================================================*/
void Server::StartServer(void)
{
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	Bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

/*================================================================================
Part 1:
		Switch operation
		Send system message
		Server quit
================================================================================*/
void Server::Send(void)
{
	while(1)
	{
		Console();
		switch(flag)
		{
			case 1:
				SerChat();
				break;
			case 2:
				SerQuit();
				break;
			case 3:
				Console();
				break;
			default:
				break;
		}
	}
}

void Server::Console(void)
{
	cout<<"\t\t\t\tswitch function:"<<endl;
	cout<<"\t\t\t\t1.send"<<endl;
	cout<<"\t\t\t\t2.quit"<<endl;
	cout<<"\t\t\t\t3.return menu"<<endl;
	cin>>flag;
}
void Server::SerChat(void)
{
	Msg_t msg;
	msg.sndid = 0;
	msg.type = MSG_SYS;

	cout<<"to->:";
	char buf[6]={0};
	while(getchar()!='\n');
	Fgets(buf, 6, stdin);
	msg.recvid=atoi(buf);

	cout<<"text:";
	memset((char*)msg.text,0,MSGLEN);
	Fgets(msg.text, MSGLEN, stdin);
	msg.text[strlen(msg.text)-1]='\0';

	if(msg.recvid!=0)
		SendtoOne(&msg);
	else
		SendtoAll(&msg);
}
void Server::SerQuit(void)
{
	Msg_t msg;
	msg.type = SER_QUIT;
	msg.sndid = 0;
	msg.recvid = 0;

	SendtoAll(&msg);

	cout<<"server quit!"<<endl;
	exit(0);
}

/*================================================================================
Part 3:
		Receive the chat message
		Change online user list when login
		Change online user list when logout
================================================================================*/
void Server::Recv(void)
{
	Msg_t msg;
	while(1)
	{
		RecvMsg(&msg);
		switch(msg.type)
		{
			case MSG_CHAT:
			case MSG_ALL:
				ClientChat(&msg);
				break;
			case USR_LOGIN:
				ClientLogIn(&msg);
				OnlineUser(&msg);
				break;
			case USR_LOGOUT:
				ClientLogOut(&msg);
				break;
			default:
				break;
		}
	}
}

inline void Server::RecvMsg(Msg_t *msg)
{
	memset(msg, 0, sizeof(msg));
	Recvfrom(sockfd, (Msg_t*)msg, sizeof(Msg_t), 0,
			(struct sockaddr *)&client_addr, &addrlen);
}
void Server::ClientChat(Msg_t *msg)
{
	cout<<"from:"<<msg->sndid<<" to:"<<msg->recvid<<endl;
	cout<<"text:"<<msg->text<<endl;
	if(msg->recvid>0)
	{
		if(SendtoOne(msg)!=0)
			cout<<"!!!the user is not exist!"<<endl;
	}
	else
		SendtoAll(msg);
}
void Server::ClientLogIn(Msg_t *msg)
{
	if(msg->sndid!=0)
		return;

	Client_t *client = new Client_t;
	client->user.id = (++maxid);
	strncpy(client->user.name,msg->text,NAMELEN);
	memcpy((struct sockaddr_in*)&client->addr,
			(struct sockaddr_in*)&client_addr,
			sizeof(struct sockaddr_in));
	client->next=NULL;
	online.InsetClient(client);

	cout<<"++++++++| LogIn |++++++++"<<endl;
	cout<<" "<<client->user.id<<" -- "<<client->user.name<<endl;
	cout<<"++++++++ +++++++ ++++++++"<<endl;

	/*
	 * return the user id to the new client and tell others client
	 */
	msg->recvid=client->user.id;
	memcpy((char*)msg->text,(Client_t*)client,sizeof(Client_t));
	SendtoOne(msg);
	SendtoAll(msg);
}
void Server::ClientLogOut(Msg_t *msg)
{
	online.DeleteClient(msg->sndid);

	cout<<"--------| LogOut |--------"<<endl;
	cout<<" "<<msg->sndid<<" -- "<<msg->text<<endl;
	cout<<"-------- -------- --------"<<endl;

	SendtoAll(msg);
}
void Server::OnlineUser(Msg_t *msg)
{
	msg->type = USR_ONLINE;
	msg->recvid = online.head->next->user.id;
	msg->sndid = 0;
	memset((char*)msg->text,0,MSGLEN);

	Client_t *pc = online.head->next;
	char *pt = msg->text;
	while(pc!=NULL && (pt<(msg->text+200)))
	{
		memcpy((char*)pt,(Client_t*)pc,sizeof(Client_t));
		pc = pc->next;
		pt += sizeof(Client_t);
	}
	SendtoOne(msg);
}
int Server::SendtoOne(const Msg_t *const msg)
{
	const Client_t *p = online.head->next;
	while(p!=NULL)
	{
		if(p->user.id==msg->recvid)
		{
			Sendto(sockfd, msg, sizeof(Msg_t), 0,
					(struct sockaddr *)&p->addr,
					sizeof(struct sockaddr));
			return 0;
		}
		p = p->next;
	}
	return -1;
}
void Server::SendtoAll(Msg_t *const msg)
{
	const Client_t *p = online.head->next;
	while(p!=NULL)
	{
		if(p->user.id!=msg->recvid)
		{
			msg->recvid = p->user.id;
			Sendto(sockfd, msg, sizeof(Msg_t), 0,
					(struct sockaddr *)&p->addr,
					sizeof(struct sockaddr));
		}
		p = p->next;
	}
}


Server::Server(char *ip,char *port)
{
	/*
	 * initialize server_addr
	 */
	maxid=0;
	memset(&client_addr,0,sizeof(client_addr));
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(port));
	Inet_pton(AF_INET, ip, (struct sockaddr *)&server_addr.sin_addr);
}
Server::~Server() {
	// TODO Auto-generated destructor stub
}
