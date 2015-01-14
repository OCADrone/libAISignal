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

#include <AISignal/client.hh>

using namespace std;
using namespace AISignal;

client::client()
{
	init();
}

/**
 * @param _hsock Server socket.
 */
client::client(KSocket *_hsock)
{
	init();
	hsock    = _hsock;
	selfsock = false;
}

/**
 * @param _address Server address.
 * @param _port Server port.
 */
client::client(const string &_address, int _port)
{
	init();
	hsock    = new KSocket(KSocket::TCP4, _address, _port);
	selfsock = true;
}

void 	client::reset()
{
	try
	{
		hsock->disconnect();
	}
	catch(const KError &error)
	{}

	if (selfsock && hsock)
		delete hsock;
}

client::~client()
{
	reset();
}


/**
 * NETWORK CONFIGURATION
 * =====================
 */

/**
 * @param _hsock Server socket.
 */
void 	client::set_server(KSocket *_hsock)
{
	if (hsock && selfsock)
		reset();
	hsock = _hsock;
}

/**
 * @param _address Server address.
 * @param _port Server port.
 */
void 	client::set_server(const string &_address, int _port)
{
	if (!hsock)
		hsock = new KSocket(KSocket::TCP4);

	hsock->set_address(_address);
	hsock->set_port(_port);
}

/**
 * @param _address Server address.
 */
void 	client::set_address(const string &_address)
{
	if (!hsock)
		hsock = new KSocket(KSocket::TCP4);

	hsock->set_address(_address);
}

/**
 * @param _port Server port.
 */
void 	client::set_port(int _port)
{
	if (!hsock)
		hsock = new KSocket(KSocket::TCP4);

	hsock->set_port(_port);
}

void 	client::connect()
{
	if (hsock)
	{
		hsock->connect();
		hsock->set_nagle(false);
		cans.set_sock(hsock);
		creq.set_sock(hsock);

		receiver.start<AISignal::client, void>(this, &AISignal::client::receive);
	}
	else
		throw KError("AISignal::client::connect", "socket not initialized");
}

void 	client::disconnect()
{
	cout << "client::disconnect()" << endl;
	if (hsock)
		hsock->disconnect();
	else
		throw KError("AISignal::client::disconnect", "socket not initialized");
}


/**
 * ========================
 * PUBLIC COMMANDS HANDLING
 * ========================
 */

/**
 * @param _channel Channel to subscribe to.
 * @param _mode Fetch mode (FIFO, LIFO)
 */
void 					client::subscribe(const string &_channel)
{
	auto				chan = signals.begin();

	mutex.lock();
	if (hsock)
	{
		cans.flush();
		creq.flush();

		try
		{
			// Init local list
			chan = signals.find(_channel);
			if (chan == signals.end())
			{
				// Set local channels list
				signals.emplace(_channel, new list<signal>);
				// Send server command
				creq.set(SUBSCRIBE, "", _channel);
				creq.send();
			}

		}
		catch(const KError &error)
		{

		}
	}
	mutex.unlock();
}

/**
 * @param _channel Channel to unsubscribe from.
 */
void 					client::unsubscribe(const string &_channel)
{
	if (hsock)
	{
		cans.flush();
		creq.flush();

		try
		{
			creq.set(UNSUBSCRIBE, "", _channel);
			creq.send();
		}
		catch(const KError &error)
		{

		}
	}
}

/**
 * @param _channel Channel to setup.
 * @param _limit Client stack limit.
 */
void 					client::set_limit(size_t _limit)
{
	limit = _limit;
}

/**
 * @param _channel Channel to get new signal from (must subscribe before).
 */
AISignal::signal 					client::fetch(const string &_channel,
																				enum AISignal::mode _mode)
{
	auto									  chan = signals.begin();
	list<signal> 						*sigs;
	list<signal>::iterator 	itr;
	AISignal::signal 				copy;


	mutex.lock();
	// Check fif we have channels (aka subscriptions)
	if (signals.size() < 1)
	{
		mutex.unlock();
		return empty;
	}

	try
	{
		// Get tuple <channel name, channel list> iterator
		chan = signals.find(_channel);

		if ( chan == signals.end())
		{
			mutex.unlock();
			return empty;
		}

		// Get channel list pointer
		sigs = chan->second;

		// Check if we have new signals
		if (sigs->size() < 1)
		{
			mutex.unlock();
			return empty;
		}

		if (_mode == FIFO)
		{
			itr = sigs->begin();
		}
		else
		{
			itr = sigs->end();
			itr--;
		}
	}
	catch(...)
	{
		mutex.unlock();
		throw;
	}

	mutex.unlock();
	copy = *itr;
	sigs->erase(itr);
	return copy;
}

/**
 * @param _data Data to send.
 * @param _channel Channel to send data on.
 */
void 		client::send(const string &_data, const string &_channel)
{
	if (hsock)
	{
		try
		{
			creq.set(SEND, _channel, _data);
			creq.send();
		}
		catch (const KError &error)
		{
			error.dump();
			disconnect();
		}
	}
	else
		throw KError("AISignal::client::send()", "connection is not configured");

	cans.flush();
	creq.flush();
	return;
}

void 											client::receive()
{
	auto									  chan = signals.begin();
	list<signal>						*sigs;
	list<signal>::iterator 	itr;

	try
	{
		while(true)
		{
			cans.flush();
			cans.receive();

			chan = signals.find(cans.get_signal().channel);
			if (chan != signals.end())
			{
				sigs = chan->second;
				mutex.lock();
				if ( (sigs->size() + 1) > limit )
					sigs->pop_front();
				sigs->push_back(cans.get_signal());
				mutex.unlock();
			}
		}
	}
	catch(const KError &error)
	{
		mutex.unlock();
		return;
	}
}

void 	client::init()
{
	hsock    = NULL;
	selfsock = false;
	limit    = 20;
}
