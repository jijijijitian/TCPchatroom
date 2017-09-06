#include "myhead.h"

void *read_msg(void *argc);
void interface();                             //初始界面
void interfaceline();                         //登陆后界面

int main(int argc, char* argv[])
{
	int socketfd = 0;
	int ret = 0;
	struct sockaddr_in sock_server = {0};
	
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == socketfd)
	{
		perror("socket");
		return -1;
	}
	printf("socket success...\n");
	
	sock_server.sin_family = AF_INET;
	sock_server.sin_port = htons(MYPORT);
	//sock_server.sin_addr.s_addr = inet_addr(MYADDR);
	sock_server.sin_addr.s_addr = inet_addr(argv[1]);
	
	ret = connect(socketfd,(struct sockaddr *)&sock_server,sizeof(struct sockaddr));
	if(ret == -1)
	{
		perror("connect");
		return -1;
	}
	
	int back = 1, back2 = 1;
	int sendcnt = 0;
	int action, action1;
	struct message Msg = {0};
	pthread_t th = 0;
	
	ret = pthread_create(&th,NULL,read_msg,&socketfd);	
    if(ret != 0)
    {
		perror("pthread_create");
		return -1;
	}	
	
	while(back)
	{		
		back2 = 1;
		interface();
		scanf("%d", &action);
		switch (action)
		{
			case 1:
				printf("输入昵称：");
				scanf("%s", Msg.name);
				printf("输入密码: ");
				scanf("%s", Msg.passwd);
				Msg.action = 2;
				
				sendcnt  = write(socketfd, &Msg, sizeof(Msg));
				if(sendcnt == -1)
				{
					perror("send");
					return -1;
				}
				else
				{
					printf("您的昵称:%s, 密码:%s\n", Msg.name, Msg.passwd);
				}
				break;
			case 2:
				printf("输入昵称：");
				scanf("%s", Msg.name);
				printf("输入密码: ");
				scanf("%s", Msg.passwd);
				Msg.action = 3;
				
				sendcnt  = write(socketfd, &Msg, sizeof(Msg));
				if(sendcnt == -1)
				{
					perror("send");
					return -1;
				}
				else
				{
					printf("您的昵称:%s, 密码:%s\n", Msg.name, Msg.passwd);
				}
				
				while (back2)
				{
					interfaceline();
					scanf("%d", &action1);
					switch (action1)
					{
						case 1:
							printf("选择发送的对象：");
							scanf("%s", Msg.name);
							printf("输入发送的消息：");
							scanf("%s", Msg.buff);
							Msg.action = 1;
							
							sendcnt = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("发送给客户端 %s :%s\n", Msg.name, Msg.buff);
							}
							break;
						case 2:
							printf("输入发送的消息：");
							scanf("%s", Msg.buff);
							Msg.action = 4;
							
							sendcnt = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("群发消息：%s\n", Msg.buff);
							}
							break;
						case 3:
							Msg.action = 5;
							sendcnt = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("在线好友：\n");
							}
							break;
						case 4:
							printf("输入昵称：");
							scanf("%s", Msg.fromname);
							printf("输入旧密码: ");
							scanf("%s", Msg.passwd);
							printf("输入新密码: ");
							scanf("%s", Msg.newpasswd);
							Msg.action = 6;
							
							sendcnt  = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("您的昵称:%s, 新密码:%s\n", Msg.fromname, Msg.newpasswd);
							}
							break;
						case 5:
							printf("输入昵称：");
							scanf("%s", Msg.fromname);
							Msg.action = 7;
							
							sendcnt  = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("%s，您已成为超级用户！\n", Msg.fromname);
							}
							break;
						case 6:
							printf("输入昵称：");
							scanf("%s", Msg.fromname);
							printf("输入密码: ");
							scanf("%s", Msg.passwd);
							printf("输入要禁言的人：");
							scanf("%s", Msg.name);
							Msg.action = 8;
							
							sendcnt  = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("您的昵称:%s，密码:%s，禁言%s\n", Msg.fromname, Msg.passwd, Msg.name);
							}
							break;
						case 7:
							printf("输入昵称：");
							scanf("%s", Msg.fromname);
							printf("输入密码: ");
							scanf("%s", Msg.passwd);
							printf("输入要解禁的人：");
							scanf("%s", Msg.name);
							Msg.action = 9;
							
							sendcnt  = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("您的昵称:%s，密码:%s，解禁%s\n", Msg.fromname, Msg.passwd, Msg.name);
							}
							break;
						case 8:
							printf("输入昵称：");
							scanf("%s", Msg.fromname);
							printf("输入密码: ");
							scanf("%s", Msg.passwd);
							printf("输入要踢的人：");
							scanf("%s", Msg.name);
							Msg.action = 10;
							
							sendcnt  = write(socketfd, &Msg, sizeof(Msg));
							if(sendcnt == -1)
							{
								perror("send");
								return -1;
							}
							else
							{
								printf("您的昵称:%s，密码:%s，踢掉%s\n", Msg.fromname, Msg.passwd, Msg.name);
							}
							break;
						case 9:
							back2 = 0;break;
					}
				}
				break;
			case 3:
				back = 0;break;
			default: 
				printf("请重新输入！\n");back = 0;break;
		}
	}
		
	return 0;
}

void *read_msg(void *argc)
{
	int fd = *((int *)argc);
	int recvcnt = 0;
	struct message Msg = {0};
	
	while(1)
	{
		bzero(&Msg, sizeof(Msg));
		recvcnt = read(fd, &Msg, sizeof(Msg));
		if(recvcnt == -1)
		{
			perror("recv");
			return NULL;
		}
		else if(recvcnt == 0)
		{
			printf("The Client is closed!\n");
			break;
		}
		else if(Msg.onlinename[0][0])
		{
			int ii;
			while (Msg.onlinename[ii][0])
			{
				printf("昵称：%s\n", Msg.onlinename[ii]);
				ii++;
			}
		}
		else if(Msg.chatflag == 1)
		{
			printf("您已被禁言！\n");
		}
		else if(Msg.offmsgflag == 1)
		{
			printf("从客户端 %s 收到离线消息：%s\n", Msg.fromname, Msg.buff);
		}
		else
		{
			printf("从客户端 %s 收到:%s\n", Msg.fromname, Msg.buff);
		}	
	}
	close(fd);
	
	return NULL;
}

void interface()
{
    printf("\n************************************\n");
	printf("**            选择操作：          **\n");
    printf("**   <1> 注册         <2>登录     **\n");
	printf("**   <3> 退出                     **\n");	
    printf("************************************\n\n");
}

void interfaceline()
{
    printf("\n************************************\n");
	printf("**            选择操作：          **\n");
    printf("**   <1> 私聊         <2> 群聊    **\n");
    printf("**   <3> 查看在线用户 <4> 修改密码**\n");
	printf("**   <5> 申请超级权限 <6> 禁言    **\n");
	printf("**   <7> 解禁         <8> 踢人    **\n");
	printf("**   <9> 退出                     **\n");	
    printf("************************************\n\n");
}
