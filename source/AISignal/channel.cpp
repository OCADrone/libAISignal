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

#include <AISignal/channel.hh>
#include <AISignal/signal.hh>

using namespace AISignal;

channel::channel(): limit(0)
{
  init();
}

/**
 * @param _limit Channel size limit.
 */
channel::channel(size_t _limit): limit(_limit)
{
  init();
}

/**
 * @param _data Signal content.
 */
void  channel::insert(const signal &_data)
{
  auto  itr = servers.begin();

  siglock.lock();
  //cout << "channel::insert()" << endl;

  // Insert code here
  for (; itr != servers.end(); ++itr)
  {
    //cout << "channel::insert() : send to " << *itr << endl;
    (*itr)->send(_data);
  }

  siglock.unlock();
}

void  channel::add_server(AISignal::server *_server)
{
  list<AISignal::server*>::iterator  itr;

  srvlock.lock();
  //cout << "channel::add_server()" << endl;

  for(itr = servers.begin(); itr != servers.end(); itr++)
  {
    if ( *itr == _server )
    {
      //cout << "-- server already added" << endl;
      srvlock.unlock();
      return;
    }
  }
  //cout << "-- adding server " << _server << endl;
  servers.push_back(_server);
  srvlock.unlock();
}

void  channel::del_server(AISignal::server *_server)
{
  list<AISignal::server*>::iterator  itr;

  srvlock.lock();
  cout << "channel::del_server()" << endl;

  for(itr = servers.begin(); itr != servers.end(); itr++)
  {
    if ( *itr == _server )
    {
      cout << "-- removing server " << _server << endl;
      servers.remove(*itr);
      srvlock.unlock();
      return;
    }
  }
  cout << "-- server not added" << endl;
  srvlock.unlock();
}

/*auto    channel::get_server(AISignal::server *_server)
{
  auto  itr = servers.begin();

  //  for (; itr != servers.end(); )
  return itr;
}*/

void  channel::init()
{
  return;
}
