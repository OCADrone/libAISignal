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

#include <AISignal/answer.hh>

using namespace std;
using namespace KNM;
using namespace AISignal;

answer::answer()
{
	sock  = NULL;
}

/**
 * @param _ref Reference to copy from.
 */
answer::answer(const answer &_ref)
{
	if (&_ref != this)
		*this = _ref;
}

/**
 * @param _ref Reference to copy from.
 */
answer 	&answer::operator=(const answer &_ref)
{
	if (&_ref != this)
	{
		sock        = _ref.sock;
		sig.channel = _ref.sig.channel;
		sig.data    = _ref.sig.data;
	}
	return (*this);
}

/**
 * @param _sock Socket to use.
 */
answer::answer(KSocket *_sock)
{
	sock  = _sock;
}

void 	answer::reset()
{
	if (sock){sock->close();}
}

answer::~answer()
{
	if (sock){sock->close();}
}

/**
 * @param _sock Socket to use.
 */
void 	answer::set_sock(KSocket *_sock)
{
	sock = _sock;
}

void 	answer::flush()
{
	sig.channel.clear();
	sig.data.clear();
}

/**
 * @param _sig Signal.
 */
void 	answer::set(const signal &_sig)
{
	sig.channel = _sig.channel;
	sig.data    = _sig.data;
}

/**
 * @param _channel Signal channel.
 * @param _data Signal content.
 */
void 	answer::set(const string &_channel, const string &_data)
{
	sig.channel = _channel;
	sig.data    = _data;
}

bool 	answer::send()
{
	if (sock)
	{
		try
		{
			sock->send((int)sig.channel.size());
			sock->send((int)sig.data.size());
			if (sig.channel.size() > 0) {sock->send(sig.channel);}
			if (sig.data.size() > 0) {sock->send(sig.data);}
		}
		catch (const KError &error)
		{
			//error.dump();
			return false;
		}
		return true;
	}
	return false;
}

bool 		answer::receive()
{
	int 	channel_len = 0;
	int 	data_len    = 0;
	char 	*buffer     = NULL;

	if (sock)
	{
		try
		{
			sock->receive((int*)&channel_len);
			sock->receive((int*)&data_len);

			if (channel_len > 0)
			{
				buffer = new char[channel_len + 1];
				memset(buffer, 0, channel_len + 1);
				sock->receive(buffer, channel_len);
				sig.channel.assign(buffer);
				delete[] buffer;
			}

			if (data_len > 0)
			{
				buffer = new char[data_len + 1];
				memset(buffer, 0, data_len + 1);
				sock->receive(buffer, data_len);
				sig.data.assign(buffer);
				delete[] buffer;
			}

		}
		catch (const KError &error)
		{
			//error.dump();
			return false;
		}
		return true;
	}
	return false;
}


const signal 	&answer::get_signal()
{
	return sig;
}
