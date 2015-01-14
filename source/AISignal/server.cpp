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
	cout << "server is destroying itself" << endl;
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

void 	server::send(const signal &_sig)
{
	cans.set(_sig);
	cans.send();
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
			creq.flush();
			if (!creq.receive() ) {break;}
			handle();
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
	channel 	*chan;
	signal 		sig;

	switch(creq.get_command())
	{
		// SUBSCRIBE
		case SUBSCRIBE:
			try{chan = *sigtree->find(creq.get_channel());}
			catch(...){break;}
			if (chan)
				chan->add_server(this);
			break;

		// UNSUBSCRIBE
		case UNSUBSCRIBE:
			try{chan = *sigtree->find(creq.get_channel());}
			catch(const KError &error){break;}
			if (chan)
				chan->del_server(this);
			break;

		// SEND
		case SEND:
			try{chan = *sigtree->find(creq.get_channel());}
			catch(const KError &error){break;}
			sig.channel = creq.get_channel();
			sig.data = creq.get_data();
			if (chan)
				chan->insert(sig);
	}
}
