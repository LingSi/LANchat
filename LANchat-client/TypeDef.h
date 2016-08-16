/*
 * TypeDef.h
 *
 *  Created on: 2016-7-28
 *      Author: root
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

/*
 * include
 */
#include <netinet/in.h>

/*
 * define
 */
#define NAMELEN 10
#define MSGLEN 255

#define USR_LOGIN	1	//用户登录
#define USR_LOGOUT	2	//用户退出
#define USR_ONLINE	3	//获取在线用户
#define MSG_CHAT	4	//好友聊天
#define MSG_ALL		5	//群聊
#define MSG_SYS		6	//系统消息

#define SER_QUIT	8	//服务器退出

/*
 * typedef
 */
typedef unsigned int UserId_t;

typedef struct User
{
	UserId_t id;
	char name[NAMELEN];
}User_t;

typedef struct Message
{
	int type;
	UserId_t sndid;
	UserId_t recvid;
	char text[MSGLEN];
}Msg_t;

typedef struct ClientNode
{
	User_t user;
	struct sockaddr_in addr;
	struct ClientNode *next;
}Client_t;

#endif /* TYPEDEF_H_ */
