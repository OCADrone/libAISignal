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

#ifndef LIBAISIGNAL_ANSWER_HH
#define LIBAISIGNAL_ANSWER_HH

#include <string>
#include <AISignal/commands.hh>
#include <AISignal/signal.hh>
#include <KNM/base/KError.hh>
#include <KNM/net/KSocket.hh>

using namespace KNM;

namespace AISignal
{
	/**
	 * Registry answer packet.
	 * Wrap client <-> server answers.
	 */
	class answer
	{
	public:
		answer();															/**< Default constructor. */
		answer(const answer &);								/**< Copy constructor. */
		answer &operator=(const answer &);		/**< Assignment operator. */
		answer(KSocket *);										/**< Construct with a connection. */
		void reset();													/**< Reset obect to its initial state. */
		~answer();														/**< Destructor. */

		// Configuration
		void 	set_sock(KSocket *);						/**< Set socket. */
		void 	flush();												/**< Clear buffers. */

		// Packet handling
		void 	set(const signal &);						/**< Set answer content. */
		void 	set(const string &,
							const string &);						/**< Set answer content. */
		bool 	send();													/**< Send answer. */
		bool 	receive();											/**< Receive answer. */

		// Result handling
		const signal 	&get_signal();					/**< Return received signal. */

	private:
		KSocket 	*sock;											/**< Socket. */
		signal 		sig;												/**< Answer data / failure message. */
	};
}

#endif
