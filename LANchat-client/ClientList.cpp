/*
 * ClientList.cpp
 *
 *  Created on: 2016-7-29
 *      Author: root
 */

#include "ClientList.h"
#include <string.h>
#include <iostream>
using namespace std;

void ClientList::InsetClient(Client_t *client)
{
	client->next = head->next;
	head->next = client;
}
void ClientList::DeleteClient(UserId_t id)
{
	Client_t *front = head;
	Client_t *cur = head->next;

	while(cur!=NULL && cur->user.id!=id)
	{
		front=cur;
		cur=cur->next;
	}
	if(cur!=NULL)
	{
		front->next = cur->next;
		delete(cur);
		cur=NULL;
	}
}

ClientList::ClientList()
{
	head = new Client_t;
	head->next=NULL;
}

ClientList::~ClientList()
{
	Client_t *tmp = NULL;
	while(head!=NULL)
	{
		tmp=head;
		head=head->next;
		delete tmp;
		tmp=NULL;
	}
}
