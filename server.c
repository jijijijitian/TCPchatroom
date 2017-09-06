#include "myhead.h"

void *read_msg(void *argc);
void insert(struct message M);                      //注册：将用户信息添加到注册库
void create();                                      //创建数据库
int init(node* head);                               //创建在线链表头结点
int insert_link(node head, struct message M);       //登录：从注册库导入到在线链表
int chat(node head, struct message M);              //私聊
int chatgroup(node head, struct message M);         //群发
int showuser(node head, struct message M);          //查看在线用户
int alterpasswd(node head, struct message M);       //修改密码
void getroot(node head, struct message M);          //获得超级权限
void ban(node head, struct message M);              //禁言
void lift(node head, struct message M);             //解禁
void conout(node head, struct message M);           //踢人
void createMes();                                   //创建离线链表数据库

node head1 = NULL;

int main()
{
	int ret = 0;
	int socketfd = 0;								//局部变量保存在栈空间，而栈空间是脏的==》里面还是保存的是上一次这个区域里面保存的值；
	int clientfd = 0;
	
	pid_t pid = 0;
	pthread_t th = 0;
	
	struct sockaddr_in sock_server = {0};  			//变量类型保存在netinet/in.h里面的；
	struct sockaddr_in sock_client = {0};			//保存连接的客户端那边的信息；
	
	socklen_t len = sizeof(struct sockaddr);
	
	init(&head1);
	create();
	createMes();
	
	//第一步：创建套接字；
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if(socketfd == -1)
	{
		perror("socket");
		return -1;
	}
	printf("socket success...\n");					//确保前面的代码是运行正确的；
	
	//第二步：给套接字绑定必要的信息；
	sock_server.sin_family = AF_INET;				//给服务程序绑定地址族；
	sock_server.sin_port = htons(MYPORT);			//给服务器程序设定个端口号；
	sock_server.sin_addr.s_addr = inet_addr(MYADDR);//绑定任意ip地址；
	
	ret = bind(socketfd,(struct sockaddr *)&sock_server,sizeof(struct sockaddr));
	if(ret == -1)
	{
		perror("bind");
		return -1;
	}
	printf("bind success..\n");
	
	ret = listen(socketfd,10);
	if(ret == -1)
	{
		perror("listen");
		return -1;
	}
	printf("listen success...\n");
	
	char recvbuff[20] = {0};
	int recvcnt = 0;
	
	while(1)
	{
		clientfd = accept(socketfd,(struct sockaddr *)&sock_client,&len);
		if(clientfd == -1)
		{
			perror("accept");
			return -1;
		}
		printf("accept success... 客户端 = %d\n",clientfd);	//4
		
		ret = pthread_create(&th, NULL, read_msg, &clientfd);		//传递clientfd;要知道从哪一个客户端来读取数据；
		if(ret != 0)
		{
			perror("pthread_create");
			return -1;
		}
	}
	close(socketfd);
	
	return 0;
}

void *read_msg(void *argc)
{
	//argc首先是void类型的指针，经过(int *)argc强制转化为int *的指针，接下来要从argc所指向的地址里面取值；
	//===>*((int *)argc);===>经过以上的操作就将pthread_create()里面传递的参数，赋值给fd;
	
	int fd = *((int *)argc);
	
	char recvbuff[20] ={0};
	int recvcnt = 0;
	struct message Msg = {0};
	
	node p;
	
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
		else
		{
			if (1 == Msg.action)
			{
				Msg.fromid = fd;
				p = head1;
				chat(p, Msg);
			}
			if (2 == Msg.action)
			{
				Msg.fromid = fd;
				insert(Msg);
			}
			if (3 == Msg.action)
			{
				p = head1;
				Msg.fromid = fd;			
				insert_link(p, Msg);
			}
			if (4 == Msg.action)
			{
				p = head1;
				Msg.fromid = fd;
				chatgroup(p, Msg);
			}
			if (5 == Msg.action)
			{
				p = head1;
				Msg.fromid = fd;
				showuser(p, Msg);
			}
			if (6 == Msg.action)
			{
				p = head1;
				Msg.fromid = fd;
				alterpasswd(p, Msg);
			}
			if (7 == Msg.action)
			{
				p = head1;
				getroot(p, Msg);
			}
			if (8 == Msg.action)
			{
				p = head1;
				ban(p, Msg);
			}
			if (9 == Msg.action)
			{
				p = head1;
				lift(p, Msg);
			}
			if (10 == Msg.action)
			{
				p = head1;
				conout(p, Msg);
			}
		}	
	}
	close(fd);
	
	return NULL;
}

void conout(node head, struct message M)
{
	node q1 = head;
	while (strcmp(q1->next->name, M.fromname) != 0)
	{
		q1 = q1->next;
	}
	if (strcmp(q1->next->passwd, M.passwd) != 0)
	{
		printf("密码错误！\n");
	}
	else if (q1->next->rootflag != 1)
	{
		printf("您不是超级用户！\n");
	}
	else
	{
		node q2 = head;
		while (strcmp(q2->next->name, M.name) != 0)
		{
			q2 = q2->next;
		}
		node q3 = q2->next;
		q2->next = q3->next;
		q3->next = NULL;
	}
}

void lift(node head, struct message M)
{
	node q1 = head;
	while (strcmp(q1->next->name, M.fromname) != 0)
	{
		q1 = q1->next;
	}
	if (strcmp(q1->next->passwd, M.passwd) != 0)
	{
		printf("密码错误！\n");
	}
	else if (q1->next->rootflag != 1)
	{
		printf("您不是超级用户！\n");
	}
	else
	{
		node q2 = head;
		while (strcmp(q2->next->name, M.name) != 0)
		{
			q2 = q2->next;
		}
		q2->next->chatflag = 0;
	}
}

void ban(node head, struct message M)
{
	node q1 = head;
	while (strcmp(q1->next->name, M.fromname) != 0)
	{
		q1 = q1->next;
	}
	if (strcmp(q1->next->passwd, M.passwd) != 0)
	{
		printf("密码错误！\n");
	}
	else if (q1->next->rootflag != 1)
	{
		printf("您不是超级用户！\n");
	}
	else
	{
		node q2 = head;
		while (strcmp(q2->next->name, M.name) != 0)
		{
			q2 = q2->next;
		}
		q2->next->chatflag = 1;
	}
}

void getroot(node head, struct message M)
{
	node q = head;
	while (strcmp(q->next->name, M.fromname) != 0)
	{
		q = q->next;
	}
	q->next->rootflag = 1;
}

int alterpasswd(node head, struct message M)
{
	sqlite3* db;
	int ret = sqlite3_open("register.db", &db);
	char* errmsg;
	char **result;
	int row, column;
	char sql1[1024];
	//判断旧密码是否正确
	sprintf(sql1, "select * from register where name = '%s';", M.fromname);

	ret = sqlite3_get_table(db, sql1, &result, &row, &column, &errmsg);
	if (row == 0)
	{
		printf("用户名错误！\n");
		return -1;
	}
	else
	{
		if (strcmp(result[5], M.passwd) == 0)
		{
			//在注册表里修改新密码
			char sql[1024];
			sprintf(sql, "update register set passwd = '%s' where name = '%s';", M.newpasswd, M.fromname);
			ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
			memset(sql, 0, 1024 * sizeof(char));
			
			ret = sqlite3_close(db);
			printf("修改成功！\n");
		}
		else
		{
			printf("密码错误！\n");
		}
	}
}

int showuser(node head, struct message M)
{
	int i = 0, sendcnt;
	node q = head;
	while (q->next->fd != M.fromid)                              //遍历在线链表根据fromid查找发送者昵称
	{
		q = q->next;
	}
	
	while (head->next != NULL)
	{
		strcpy(M.onlinename[i], head->next->name);
		head = head->next;
		i++;
	}
	sendcnt = write(M.fromid, &M, sizeof(M));		
	if(sendcnt == -1)
	{
		perror("send");
		return -1;
	}
	else
	{
		printf("发送给客户端%s在线名单。\n", q->next->name);
	}
}

int chatgroup(node head, struct message M)
{
	int sendcnt = 0;
	node q = head;
	while (q->next != NULL && q->next->fd != M.fromid)                              //遍历在线链表根据fromid查找发送者昵称
	{
		q = q->next;
	}
	if (q->next == NULL)
	{
		printf("发送方%d没有登录！\n", M.fromid);
		return -1;
	}
	else
	{
		strcpy(M.fromname, q->next->name);
		if(q->next->chatflag == 1)
		{
			M.chatflag = 1;
			sendcnt = write(M.fromid, &M, sizeof(M));	
		}
		else
		{
			while (head->next != NULL)
			{
				if (head->next->name == M.fromname)
				{
					continue;
				}
				else
				{
					sendcnt = write(head->next->fd, &M, sizeof(M));		
					if(sendcnt == -1)
					{
						perror("send");
						return -1;
					}	
					head = head->next;
				}
			}
			printf("群发消息完毕！\n");
		}
	}
}

int chat(node head, struct message M)
{
	int sendcnt = 0;
	node q = head;
	while (q->next != NULL && q->next->fd != M.fromid)                              //遍历在线链表根据fromid查找发送者昵称
	{
		q = q->next;
	}
	if (q->next == NULL)
	{
		printf("发送方%d没有登录！\n", M.fromid);
		return -1;
	}
	else
	{
		strcpy(M.fromname, q->next->name);
		if(q->next->chatflag == 1)
		{
			M.chatflag = 1;
			sendcnt = write(M.fromid, &M, sizeof(M));	
		}
		else
		{
			while (head->next != NULL && (strcmp(head->next->name, M.name) != 0))               //遍历在线链表寻找名字对应的进程号
			{
				head = head->next;
			}
			if (NULL == head->next)                                      //保存离线消息
			{
				printf("该用户没有上线！\n");
				int ret;
				sqlite3 *db;
				char *errmsg;

				ret = sqlite3_open("offmsg.db", &db);
				
				char sql[1024] = {0};
				sprintf(sql, "insert into offmsg values('%s', '%s', '%s');", M.fromname, M.name, M.buff);

				ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
				ret = sqlite3_close(db);
			}
			else
			{
					sendcnt = write(head->next->fd, &M, sizeof(M));		
					if(sendcnt == -1)
					{
						perror("send");
						return -1;
					}
					else
					{
						printf("发送给客户端 %s :%s\n", M.name, M.buff);
					}
			}
		}
	}
}

void insert(struct message M)
{
    int ret;
	sqlite3 *db;
	char *errmsg;

	ret = sqlite3_open("register.db", &db);

	srand((unsigned)time(NULL));
	M.id = (rand() % 30000);
	
	char sql[1024] = {0};
    sprintf(sql, "insert into register values(%d, '%s', '%s');", M.id, M.name, M.passwd);

	ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	ret = sqlite3_close(db);
}

void create()
{
    int ret;
	sqlite3 *db;
	char *errmsg;

	ret = sqlite3_open("register.db", &db);
	char *sql = "create table register(id integer, name text, passwd text)";
	ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	
	ret = sqlite3_close(db);
}

void createMes()
{
    int ret;
	sqlite3 *db;
	char *errmsg;

	ret = sqlite3_open("offmsg.db", &db);
	char *sql = "create table offmsg(fromname text, toname text, message text)";
	ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	
	ret = sqlite3_close(db);
}

int init(node* head)
{
    node newnode = (node)malloc(sizeof(struct onlinelink));
    if (NULL == newnode)
    {
		return -1;
    }
    newnode->next = NULL;
    *head = newnode;
}

int insert_link(node head, struct message M)
{
	int ret;
	sqlite3* db;
	char* errmsg;
	char **result;
	int row, column;
	ret = sqlite3_open("register.db", &db);

	char sql[1024];
    sprintf(sql, "select * from register where name = '%s';", M.name);

	ret = sqlite3_get_table(db, sql, &result, &row, &column, &errmsg);
	if (row == 0)
	{
		printf("用户名错误！\n");
		return -1;
	}
	else
	{
		if (strcmp(result[5], M.passwd) == 0)
		{
			node newnode = (node)malloc(sizeof(struct message));
			if (NULL == newnode)
			{
				return -1;
			}

			newnode->id = M.id;
			newnode->fd = M.fromid;
			strcpy(newnode->name, M.name);
			strcpy(newnode->passwd, M.passwd);

			newnode->next = head->next;
			head->next = newnode;
			printf("登陆成功！\n");
		}
		else
		{
			printf("密码错误！\n");
		}		
	}
	
	printf("目前在线用户：\n");
	while (head->next != NULL)
	{
		printf("昵称：%s\n", head->next->name);
		head = head->next;
	}

	//sqlite3_free_table(result);

	ret = sqlite3_close(db);
	
	//判断该用户是否有待收离线消息
	sqlite3* db1;
	char **result1;
	ret = sqlite3_open("offmsg.db", &db1);

	char sql1[1024];
    sprintf(sql1, "select * from offmsg where toname = '%s';", M.name);

	ret = sqlite3_get_table(db1, sql1, &result1, &row, &column, &errmsg);
	if (row == 0)
	{
		printf("%s没有离线消息！\n", M.name);
	}
	else
	{
		strcpy(M.fromname, result1[3]);
		strcpy(M.name, result1[4]);
		strcpy(M.buff, result1[5]);
		M.offmsgflag = 1;
		
		int sendcnt = write(M.fromid, &M, sizeof(M));
		if(sendcnt == -1)
		{
			perror("send");
			return -1;
		}
		else
		{
			printf("发送给客户端 %s 离线消息:%s\n", M.name, M.buff);
		}
		sprintf(sql1, "delete from offmsg where toname = '%s';", M.name);    //离线消息发送完毕后删除数据库中该消息
		ret = sqlite3_exec(db, sql1, NULL, NULL, &errmsg);
	}	
	ret = sqlite3_close(db1);
}