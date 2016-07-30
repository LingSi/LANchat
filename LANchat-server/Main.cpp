/*
 * Main.cpp
 *
 *  Created on: 2016-7-29
 *      Author: root
 */
#include <iostream>
using namespace std;
#include "Server.h"
#include "wrapunix.h"
#include <pthread.h>

void *thread_fn(void * arg)
{
	class Server * copy = (class Server *)arg;
	copy->Send();

	return ((void *)0);
}
#include <string.h>
int main(void)
{

	Server s((char *)"127.0.0.4",(char *)"55500");
	s.StartServer();

	pthread_t tid;
	pthread_create(&tid,NULL,thread_fn,(void *)&s);

	s.Recv();


/*
	Msg_t msg;
	memset((char*)msg.text,0,MSGLEN);

	Client_t *client2 = new Client_t;
	Client_t *client = new Client_t;

	memset((Client_t*)client,0,sizeof(Client_t));
	memset((Client_t*)client2,0,sizeof(Client_t));

	client->next=client2;
	client2->next=client;
	client->user.id=1234;
	client2->user.id=5678;

	cout<<client->user.id<<endl;
	cout<<client2->user.id<<endl;
	cout<<client<<endl;
	cout<<client2<<endl;

	char *p = msg.text;
	memcpy((char*)p,(Client_t*)client,sizeof(Client_t));
	p += sizeof(Client_t);
	memcpy((char*)p,(Client_t*)client2,sizeof(Client_t));

	p=msg.text;
	memcpy((Client_t*)client2,(char*)p,sizeof(Client_t));
	p += sizeof(Client_t);
	memcpy((Client_t*)client,(char*)p,sizeof(Client_t));

	cout<<client->user.id<<endl;
	cout<<client2->user.id<<endl;
	cout<<client->next<<endl;
	cout<<client2->next<<endl;
*/


	return 0;
}
