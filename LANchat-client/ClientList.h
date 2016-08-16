/*
 * ClientList.h
 *
 *  Created on: 2016-7-29
 *      Author: root
 */

#ifndef CLIENTLIST_H_
#define CLIENTLIST_H_

#include "TypeDef.h"

class ClientList {
public:
	ClientList();
	virtual ~ClientList();

	void InsetClient(Client_t *client);
	void DeleteClient(UserId_t id);

	Client_t *head;
};

#endif /* CLIENTLIST_H_ */
