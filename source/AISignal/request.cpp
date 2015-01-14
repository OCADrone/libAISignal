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

#include <AISignal/request.hh>

using namespace std;
using namespace KNM;
using namespace AISignal;

request::request()
{
	command = SEND;
	option  = 0;
	sock 	  = NULL;
}

/**
 * @param _ref Reference to copy from.
 */
request::request(const request &_ref)
{
	if (&_ref != this)
	{
		*this = _ref;
	}
}

/**
 * @param _ref Reference to copy from.
 */
request	&request::operator=(const request &_ref)
{
	if (&_ref != this)
	{
		reset();
		this->sock 		 = _ref.sock;
		this->command  = _ref.command;
		this->data 		 = _ref.data;
		this->channel  = _ref.channel;
		this->option   = _ref.option;
	}
	return *(this);
}

/**
 * @param _sock Socket to use.
 */
request::request(KSocket *_sock)
{
	command = SEND;
	option  = 0;
	sock 	  = _sock;
}

void 	request::reset()
{
	if (sock){sock->close();}
	command   = SEND;
	data 	    = "";
	channel 	= "";
	option    = 0;
}

request::~request()
{
	if (sock){sock->close();}
}

/**
 * @param _sock Socket to use.
 */
void 	request::set_sock(KSocket *_sock)
{
	sock = _sock;
}

void 			request::flush()
{
	data    = "";
	channel = "";
	command = SEND;
	option  = 0;
}

/**
 * @param _cmd Command to perform.
 * @param _path To to access or create.
 * @param _data Data to insert.
 */
void 	request::set(enum commands _cmd, const string &_path,
									 const string &_data, int _option)
{
	command   = _cmd;
	data 	    = _path;
	channel 	= _data;
	option    = _option;
}

bool 	request::send()
{
	if (sock)
	{
		try
		{
			sock->send((int)command);
			sock->send(channel.size());
			sock->send(data.size());
			if (channel.size() > 0) {sock->send(channel);}
			if (data.size() > 0) {sock->send(data);}
		}
		catch (const KError &error)
		{
			error.dump();
			return false;
		}
		return true;
	}
	return false;
}

bool 	request::receive()
{
	size_t 	data_len    = 0;
	size_t 	channel_len = 0;
	char 	  *buffer     = NULL;

	if (sock)
	{
		try
		{
			sock->receive((int*)&command);
			sock->receive((int*)&channel_len);
			sock->receive((int*)&data_len);
			//sock->receive((int*)&option);

			if (channel_len > 0)
			{
				buffer = new char[channel_len + 1];
				memset(buffer, 0, channel_len + 1);
				sock->receive(buffer, channel_len);
				channel.assign(buffer);
				delete[] buffer;
			}

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

enum commands 	request::get_command()
{
	return command;
}

const string 	&request::get_data()
{
	return data;
}

const string 	&request::get_channel()
{
	return channel;
}

int 					request::get_option()
{
	return option;
}
