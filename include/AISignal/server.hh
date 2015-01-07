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

#ifndef LIBAISIGNAL_SERVER_HH
#define LIBAISIGNAL_SERVER_HH

#include <KNM/sync/KThread.hpp>
#include <KNM/sync/KMutex.hh>
#include <KNM/data/KTree.hpp>
#include <KNM/net.hh>
#include <AISignal/request.hh>
#include <AISignal/answer.hh>
#include <AISignal/channel.hh>

using namespace KNM;

namespace AISignal
{
	/**
	 * Dialog with a ::client.
	 * Network bridge between a registry client and
	 * an registry tree.
	 */
	class 	server: public KThread
	{
	public:
		server();
		server(KSocket *);													/**< Construct + sock. */
		server(KTree<channel*> *, KMutex * = NULL);	/**< Construct + DB link. */
		server(KSocket*, KTree<channel*> *,
			   	 KMutex * = NULL); 										/**< Construct + sock & DB. */
		void reset();																/**< Reset initial state. */
		~server();																	/**< Destructor. */

		void 	set_client(KSocket *);								/**< Set client socket. */
		void 	set_regtree(KTree<channel*> *);				/**< Set DB link. */
		void 	set_reglock(KMutex *);								/**< Set DB mutex. */

	private:
		void 			init();														/**< Initialize object. */
		void 			*tmain(void * = NULL);						/**< Start thread. */
		void 			*tend();													/**< Stop thread. */
		void  		handle();													/**< Process last request. */

		KSocket 				*csock;											/**< Client socket. */
		KTree<channel*>	*sigtree;										/**< DB link. */
		KMutex 					*siglock;										/**< DB mutex. */
		request 				creq;												/**< Client last request. */
		answer					cans;												/**< Last answer to send. */
		size_t 					state;											/**< Request state. */
	};
}

#endif
