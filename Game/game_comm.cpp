/**
 * @file game_comm.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
//#include <stdint.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <string>

const size_t msg_buf_size = 256;

struct MsgBuf
{
	int32_t mtype;
	char mdata[msg_buf_size];
};

void receiver()
{

	MsgBuf msgdata, *p;

	p = &msgdata;

	key_t msgkey = ftok("msgq.key", 'X');

	int msqid = msgget(msgkey, IPC_CREAT | 0666);

	while (1)
	{

		// メッセージを受信
		if (msgrcv(msqid, p, sizeof(p->mdata), 0, IPC_NOWAIT) <= 0)
		{
			std::cout << "No message" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(3));
			continue;
		}

		std::cout << "Message received from " << p->mtype << std::endl;
		std::string str_data = std::string(p->mdata);
		std::cout << str_data << std::endl;
		if (str_data == std::string("quit"))
		{
			break;
		}
	}

	msgctl(msqid, IPC_RMID, 0);
};

void sender(char *message_char)
{
	key_t msgkey = ftok("msgq.key", 'X');

	int msqid = msgget(msgkey, 0666);
	std::cout << "QID = " << msqid << std::endl;

	MsgBuf *p = new MsgBuf();
	p->mtype = getpid();
	size_t msg_len = std::string(message_char).length();
	size_t max_msg_len = std::min(msg_len, msg_buf_size);
	for (int i = 0; i < max_msg_len; i++)
	{
		p->mdata[i] = message_char[i];
	}

	msgsnd(msqid, p, sizeof(p->mdata), 0);
};

void usage()
{
	std::cout << "sender  : game_comm snd <message>" << std::endl;
	std::cout << "receiver: game_comm rcv" << std::endl;
}

int main(int argc, char **argv)
{
	std::string arg1 = std::string(argv[1]);
	std::string receive = "rcv";
	std::string send = "snd";
	if (arg1 == receive)
	{
		std::cout << "Start waiting message from sender..." << std::endl;
		receiver();
		return 0;
	}
	if (arg1 == send)
	{
		if (argc != 3)
		{
			std::cerr << "Error: need two argument" << std::endl;
			usage();
			return 0;
		}
		sender(argv[2]);
		return 0;
	}
	usage();
	return 0;
};