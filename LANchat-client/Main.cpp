/*
 * Main.cpp
 *
 *  Created on: 2016-7-28
 *      Author: root
 */
#include <iostream>
using namespace std;
#include "Client.h"
#include "wrapunix.h"
#include <pthread.h>

void *thread_fn(void * arg)
{
	class Client * copy = (class Client *)arg;
	copy->Send();

	return ((void *)0);
}

int main(int argc,char **argv)
{
	if(argc<2)
	{
		cout<<"please input the name"<<endl;
		exit(0);
	}

	Client c((char *)"127.0.0.4",(char *)"55500",(char *)argv[1]);
	c.Link();

	pthread_t tid;
	pthread_create(&tid,NULL,thread_fn,(void *)&c);

	c.Recv();

	return 0;
}
