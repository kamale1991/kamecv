/**
 * @file communicator.h
 * @author kameta
 * @brief
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef COM_COMMUNICATOR_H_
#define COM_COMMUNICATOR_H_

#include <sys/ipc.h>
#include <sys/msg.h>

namespace COM
{

	void send_msg()
	{
		key_t msgkey = 101; // key to be passed to msgget()
		int msgflg;					// msgflg to be passed to msgget()
		int msqid;					// return value from msgget()

		msqid = msgget(ftok("example", key), (IPC_CREAT | IPC_EXCL | 0400));

		if (msgctl(msqid, IPC_STAT, &buf) == -1)
		{
			std::err << "msgctl: msgctl failed" << std::endl;
		}
	};

} // namespace COM

#endif