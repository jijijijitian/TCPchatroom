#ifndef _MYHEAD_H_
#define _MYHEAD_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <sqlite3.h>
#include<time.h>

#define MYPORT 5685			  //1024以下的是保留的端口号，用大于1024的；
#define MYADDR "192.168.1.223"

struct message
{
	int action;               //第一次选择
	char name[20];            //发送给谁
	char fromname[20];        //发送者
	char passwd[20];          //密码
	char newpasswd[20];       //修改密码时的新密码
	int id;                   //id
	int toid;
	int fromid;
	char buff[20];            //消息
	char onlinename[20][20];  //所有在线成员名
	int rootflag;             //是否为超级用户，是1，否0
	int chatflag;             //是否被禁言，是1，否0
	int offmsgflag;           //是否有离线消息，是1，否0
};

struct onlinelink
{
    int id;
    char name[20];
    char passwd[20];
	char motto[50];           //个性签名
    int fd;                   //客户端号
	char buff[20];
	int rootflag;             //是否为超级用户，是1，否0
	int chatflag;             //是否被禁言，是1，否0
	struct onlinelink* next;
};
typedef struct onlinelink* node;

#endif