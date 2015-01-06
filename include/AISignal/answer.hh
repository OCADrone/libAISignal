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

#ifndef LIBAIREGISTRY_ANSWER_HH
#define LIBAIREGISTRY_ANSWER_HH

#include <string>
#include <AISignal/commands.hh>
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
		void 	set(bool state,
					const string & = "");						/**< Set request content. */
		bool 	send();													/**< Send a request. */
		bool 	receive();											/**< Receive a request. */

		// Result handling
		bool 			get_state();								/**< Return last command state. */
		const string 	&get_data();						/**< Return last command data. */

	private:
		KSocket 	*sock;											/**< Socket. */
		bool 			state;											/**< Command execution state. */
		string 		data;												/**< Answer data / failure message. */
	};
}

#endif
