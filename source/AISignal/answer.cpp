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
	state = false;
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
		sock  = _ref.sock;
		state = _ref.state;
		data  = _ref.data;
	}
	return (*this);
}

/**
 * @param _sock Socket to use.
 */
answer::answer(KSocket *_sock)
{
	sock  = _sock;
	state = false;
}

void 	answer::reset()
{
	if (sock){sock->close();}
	state = false;
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
	state = false;
	data  = "";
}

/**
 * @param _state Set answer state.
 * @param _data Set answer data.
 */
void 	answer::set(bool _state, const string &_data)
{
	state = _state;
	data  = _data;
}

bool 	answer::send()
{
	if (sock)
	{
		try
		{
			sock->send((int)state);
			sock->send((int)data.size());
			if (data.size() > 0) {sock->send(data);}
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

bool 	answer::receive()
{
	int 	data_len = 0;
	char 	*buffer  = NULL;

	if (sock)
	{
		try
		{
			sock->receive((int*)&state);
			sock->receive((int*)&data_len);

			if (data_len > 0)
			{
				buffer = new char[data_len + 1];
				memset(buffer, 0, data_len + 1);
				sock->receive(buffer, data_len);
				data.assign(buffer);
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

bool 	answer::get_state()
{
	return state;
}

const string 	&answer::get_data()
{
	return data;
}
