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

#ifndef LIBAIREGISTRY_CLIENT_HH
#define LIBAIREGISTRY_CLIENT_HH

#include <string>
#include <KNM/base.hh>
#include <KNM/sync.hh>
#include <KNM/net.hh>
#include <KNM/data/KTree.hpp>
#include <AISignal/request.hh>
#include <AISignal/answer.hh>

using namespace std;
using namespace KNM;

namespace AISignal
{
	/**
	 * Connect to a signal server.
	 * Contains a server socket.
	 */
	class 	client
	{
	public:
		client();
		client(KSocket *);										/**< Build with existing server link. */
		client(const string &, int = 0);			/**< Build with server link. */
		void reset();													/**< Reset initial state. */
		~client();														/**< Destructor. */

		void 	set_server(KSocket *);					/**< Set server from a socket. */
		void 	set_server(const string &,
						   int = 0);									/**< Set server link. */
		void 	set_address(const string &);		/**< Set server address. */
		void 	set_port(int);									/**< Set server port. */
		void 	connect();											/**< Connect to server. */
		void 	disconnect();										/**< Close server link. */

		const string &fetch(const string &);	/**< Fetch last signal in given channel. */
		void 				 send(const string &,
											const string &);		/**< Send a new signal to channel. */
		const string &get_data();							/**< Return last signal content. */
		bool 				 get_state();							/**< Return last query state. */

	private:
		void 			init();											/**< Initialize object. */

		KSocket 		*hsock;										/**< Server (Host) link. */
		bool 				selfsock;									/**< true if sock is self owned. */
		request 		creq;											/**< Last send request. */
		answer 			cans;											/**< Last received answer. */
		string 			result;										/**< Query result. */
	};
}

#endif
