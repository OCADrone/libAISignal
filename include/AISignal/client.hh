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

#ifndef LIBAISIGNAL_CLIENT_HH
#define LIBAISIGNAL_CLIENT_HH

#include <string>
#include <map>
#include <list>
#include <KNM/base.hh>
#include <KNM/sync.hh>
#include <KNM/net.hh>
#include <KNM/data/KTree.hpp>
#include <AISignal/request.hh>
#include <AISignal/answer.hh>
#include <AISignal/mode.hh>

using namespace std;
using namespace KNM;

namespace AISignal
{
	/**
	 * Connect to a signal server.
	 * Store signals locally, and send command to server instance.
	 * Provide methods to subscribe to channels and send signals.
	 */
	class 	client
	{
	public:
		client();
		client(KSocket *);												/**< Build with link. */
		client(const string &, int = 0);					/**< Build with server link. */
		void reset();															/**< Reset initial state. */
		~client();																/**< Destructor. */

		// Network settings
		void 	set_server(KSocket *);							/**< Set server from a socket. */
		void 	set_server(const string &,
						   			 int = 0);								/**< Set server link. */
		void 	set_address(const string &);				/**< Set server address. */
		void 	set_port(int);											/**< Set server port. */
		void 	connect();													/**< Connect to server. */
		void 	disconnect();												/**< Close server link. */

		// Signal and channels
		void 	subscribe(const string &);					/**< Register to channel. */
		void 	unsubscribe(const string &);				/**< Unregister from channel. */
		void 	set_limit(size_t);									/**< Set a stack limit. */
		signal fetch(const string &,
								 enum AISignal::mode = FIFO);	/**< Fetch in given channel. */
		void 				 send(const string &,
											const string &);				/**< Send signal to channel. */

	private:
		void 			init();													/**< Initialize object. */
		void 			receive();											/**< Receive new signals. */

		// Signals and channels
		map<string, list<signal>* > 	signals;		/**< Signals list. */
		size_t 												limit;			/**< Signals list max size. */
		KMutex 												mutex;			/**< Signals list mutex. */
		signal 												empty;			/**< Default return signal. */

		// Network
		KSocket 		*hsock;												/**< Server socket. */
		bool 				selfsock;											/**< True if sock is managed. */

		// Protocol
		KThread 		receiver;											/**< Signal receiver thread. */
		request 		creq;													/**< Last send request. */
		answer 			cans;													/**< Last received answer. */
	};
}

#endif
