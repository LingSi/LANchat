/*
 * Client.cpp
 *
 *  Created on: 2016-7-28
 *      Author: root
 */
using namespace std;
#include <iostream>
#include <string.h>

#include "Client.h"
#include "wrapsock.h"
#include "wrapstdio.h"
#include "wrapunix.h"

/*================================================================================
Part 1:
		Start UDP socket link
		Login UDP server
================================================================================*/
void Client::Link(void)
{
	cout<<"Link..."<<endl;

	/*
	 * create a socket
	 */
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	/*
	 * login
	 */
	LogIn();

	cout<<"LogIn Successfully!"<<endl;
}

void Client::LogIn(void)
{
	Msg_t msg;
	msg.type = USR_LOGIN;
	msg.sndid = user.id;
	msg.recvid = 0;
	memset((char*)msg.text,0,MSGLEN);
	strncpy(msg.text,user.name,NAMELEN);

	Sendto(sockfd, &msg, sizeof(msg), 0,
		(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
}

/*================================================================================
Part 2:
		Send message
		Get online user
		Logout
================================================================================*/
void Client::Send(void)
{
	while(1)
	{
		Console();
		switch(flag)
		{
			case 1:
				ChatSend();
				break;
			case 2:
				GetOnlineUser();
				break;
			case 3:
				LogOut();
				break;
			case 4:
				Console();
				break;
			default:
				break;
		}
	}
}

void Client::Console(void)
{
	cout<<"\t\t\t\t1.chat"<<endl;
	cout<<"\t\t\t\t2.get online user"<<endl;
	cout<<"\t\t\t\t3.logout"<<endl;
	cout<<"\t\t\t\t:.return menu(when send message to ':')"<<endl;
	cin>>flag;
}
void Client::ChatSend(void)
{
	/*
	 * generate a message
	 */
	Msg_t msg;
	msg.sndid = user.id;

	while(1)
	{
		cout<<"to:";
		char buf[6]={0};
		while((buf[0]=getchar())=='\n');
		if(buf[0]==':')
		{
			flag=4;
			return;
		}
		Fgets(buf+1, 5, stdin);
		msg.recvid=atoi(buf);

		cout<<"text:";
		memset((char*)msg.text,0,MSGLEN);
		Fgets(msg.text, MSGLEN, stdin);
		msg.text[strlen(msg.text)-1]='\0';

		if(msg.recvid!=0)
			msg.type = MSG_CHAT;
		else
			msg.type = MSG_ALL;

		Sendto(sockfd, &msg, sizeof(msg), 0,
			(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	}
}
void Client::GetOnlineUser(void)
{
	/*
	 * lock the online object
	 */
	if(pthread_mutex_lock(&mutex)!=0)
	{
		cout<<"pthread_mutex_lock error!"<<endl;
		exit(0);
	}

	cout<<"=====| Online User |======"<<endl;
	const Client_t *p = online.head->next;
	while(p!=NULL)
	{
		cout<<p->user.id<<" <--------> "<<p->user.name<<endl;
		p = p->next;
	}
	cout<<"======== ======== ========"<<endl;

	/*
	 * unlock the online object
	 */
	if(pthread_mutex_unlock(&mutex)!=0)
	{
		cout<<"pthread_mutex_unlock error!"<<endl;
		exit(0);
	}
}
void Client::LogOut(void)
{
	Msg_t msg;
	msg.type = USR_LOGOUT;
	msg.sndid = user.id;
	msg.recvid = 0;
	memset((char*)msg.text,0,MSGLEN);
	strncpy(msg.text,user.name,NAMELEN);
	Sendto(sockfd, &msg, sizeof(msg), 0,
		(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

	cout<<"client logout!"<<endl;
	exit(0);
}

/*================================================================================
Part 3:
		Receive the chat message
		Change online user list
		Server quit
================================================================================*/
void Client::Recv(void)
{
	Msg_t msg;
	while(1)
	{
		RecvMsg(&msg);
		switch(msg.type)
		{
			case MSG_CHAT:
			case MSG_ALL:
			case MSG_SYS:
				ChatRecv(&msg);
				break;
			case USR_LOGIN:
				UserLogIn(&msg);
				break;
			case USR_LOGOUT:
				UserLogOut(&msg);
				break;
			case USR_ONLINE:
				OnlineUser(&msg);
				break;
			case SER_QUIT:
				ServerQuit();
				break;
			default:
				break;
		}
	}
}

inline void Client::RecvMsg(Msg_t *msg)
{
	memset((Msg_t*)msg, 0, sizeof(Msg_t));
	Recvfrom(sockfd,(Msg_t*)msg, sizeof(Msg_t), 0, NULL, NULL);
}
void Client::ChatRecv(const Msg_t* const msg)
{
	cout<<"\n>>>>>>>> >>>>>>>> >>>>>>>> >>>>>>>>>"<<endl;
	cout<<"sender : "<<msg->sndid<<endl;
	cout<<"text : "<<msg->text<<endl;
	cout<<"<<<<<<<< <<<<<<<< <<<<<<<< <<<<<<<<<"<<endl;
}
void Client::UserLogIn(const Msg_t* const msg)
{
	if(user.id!=0)
	{
		Client_t *client = new Client_t;
		memcpy((Client_t*)client,(char*)msg->text,sizeof(Client_t));

		cout<<"\n++++++++| LogIn |++++++++"<<endl;
		cout<<" "<<client->user.id<<" -- "<<client->user.name<<endl;
		cout<<"++++++++ +++++++ ++++++++"<<endl;

		/*
		 * lock the online object
		 */
		if(pthread_mutex_lock(&mutex)!=0)
		{
			cout<<"pthread_mutex_lock error!"<<endl;
			exit(0);
		}

		online.InsetClient(client);

		/*
		 * unlock the online object
		 */
		if(pthread_mutex_unlock(&mutex)!=0)
		{
			cout<<"pthread_mutex_unlock error!"<<endl;
			exit(0);
		}
	}
	else
		user.id=msg->recvid;

}
void Client::UserLogOut(const Msg_t* const msg)
{
	cout<<"\n--------| LogOut |--------"<<endl;
	cout<<" "<<msg->sndid<<" -- "<<msg->text<<endl;
	cout<<"-------- -------- --------"<<endl;

	/*
	 * lock the online object
	 */
	if(pthread_mutex_lock(&mutex)!=0)
	{
		cout<<"pthread_mutex_lock error!"<<endl;
		exit(0);
	}

	online.DeleteClient(msg->sndid);

	/*
	 * unlock the online object
	 */
	if(pthread_mutex_unlock(&mutex)!=0)
	{
		cout<<"pthread_mutex_unlock error!"<<endl;
		exit(0);
	}
}
void Client::ServerQuit(void)
{
	cout<<"!!!!!! server error !!!!!!"<<endl;
	exit(0);
}
void Client::OnlineUser(Msg_t* const msg)
{
	/*
	 * lock the online object
	 */
	if(pthread_mutex_lock(&mutex)!=0)
	{
		cout<<"pthread_mutex_lock error!"<<endl;
		exit(0);
	}

	char *pt = msg->text;
	Client_t *pc = online.head;
	while(pc!=NULL && pt<(msg->text+200))
	{
		Client_t *client = new Client_t;
		memcpy((Client_t*)client,(char*)pt,sizeof(Client_t));
		pc = client->next;
		pt += sizeof(Client_t);
		online.InsetClient(client);
	}

	/*
	 * unlock the online object
	 */
	if(pthread_mutex_unlock(&mutex)!=0)
	{
		cout<<"pthread_mutex_unlock error!"<<endl;
		exit(0);
	}
}


Client::Client(char *ip,char *port,char *name)
{
	/*
	 * initialize the user
	 */
	user.id=0;
	strncpy(user.name,name,NAMELEN);
	/*
	 * initialize server_addr
	 */
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(port));
	Inet_pton(AF_INET, ip, (struct sockaddr *)&server_addr.sin_addr);
	/*
	 * initialize the mutex
	 */
	if(pthread_mutex_init(&mutex,NULL)!=0)
	{
		printf("ipthread_mutex_init error!");
		exit(0);
	}
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}
