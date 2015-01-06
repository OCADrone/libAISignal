/*
* libAISignal
* Copyright (C) 2014 Jean-Philippe Clipffel
* Email: jp.clipffel@gmail.com
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
* USA
*/

#include <iostream>
#include <AISignal/server.hh>

using namespace std;
using namespace AISignal;

server::server()
{
	init();
}

/**
 * @param _csock Client socket.
 */
server::server(KSocket *_csock)
{
	init();
	csock = _csock;
}

/**
 * @param _sigtree Tree signals DB.
 * @param _siglock Mutex.
 */
server::server(KTree<channel*> *_sigtree, KMutex *_siglock)
{
	init();
	sigtree = _sigtree;
	siglock = _siglock;
}

/**
 * @param _csock Client socket.
 * @param _sigtree Tree signals DB.
 * @param _siglock Mutex.
 */
server::server(KSocket *_csock,
			   			 KTree<channel*> *_sigtree, KMutex *_siglock)
{
	init();
	csock   = _csock;
	sigtree = _sigtree;
	siglock = _siglock;
}

server::~server()
{
	if (csock){csock->disconnect();}
}

/**
 * @param _csock Client socket.
 */
void 	server::set_client(KSocket *_csock)
{
	csock = _csock;
}

/**
 * @param _sigtree Tree signals DB.
 */
void 	server::set_regtree(KTree<channel*> *_sigtree)
{
	sigtree = _sigtree;
}

/**
 * @param _siglock Mutex.
 */
void 	server::set_reglock(KMutex *_siglock)
{
	siglock = _siglock;
}

/**
 * @param _args Thread arguments.
 */
void 	*server::tmain(void __attribute__((unused))*_args)
{
	if (!csock || !sigtree || !siglock)
		return NULL;

	try
	{
		csock->set_nagle(false);
		creq.set_sock(csock);
		cans.set_sock(csock);

		while (1)
		{
			if (!creq.receive() ) {break;}
			handle();
			if (!cans.send() ) {break;}
		}
	}
	catch(const KError &error)
	{
		//error.dump();
	}

	return NULL;
}

void 	*server::tend()
{
	return NULL;
}

void 	server::init()
{
	csock   = NULL;
	sigtree = NULL;
	siglock = NULL;
}

void 			server::handle()
{
	string 	*buffer = nullptr;
	channel	*chan   = nullptr;

	switch (creq.get_command())
	{
			case FETCH:
			//siglock->lock();
			try
			{
				chan = *sigtree->find(creq.get_channel());

				if (state != chan->getstate())
				{
					buffer = chan->getsig();
					state  = chan->getstate();
				}

				if (buffer)
					cans.set(true, *buffer);
				else
					cans.set(false, "");
			}
			catch (const KError &error)
			{
				cans.set(false);
				//error.dump();
			}
			//siglock->unlock();
			break;



		case SEND:
			//siglock->lock();
			try
			{
				chan = *sigtree->find(creq.get_channel());
				//cout << "server::case SEND: channel: " << creq.get_channel() << endl;
			}
			catch (const KError &error)
			{
				//error.dump();
				//cout << "send: creating node: " << creq.get_channel() << endl;
				sigtree->insert(creq.get_channel(), new channel());
			}

			try
			{
				chan = *sigtree->find(creq.get_channel());
				//cout << "chan getted" << endl;

				if (chan)
					chan->insert(creq.get_data());
			}
			catch (const KError &error)
			{
				//error.dump();
			}

			//sigtree->insert(creq.get_path(), /**(new string(*/creq.get_data()/*))*/);
			//siglock->unlock();
			cans.set(true);
			break;


		default:
			cans.set(false);
	}
}
