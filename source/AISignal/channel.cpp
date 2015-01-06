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

channel::channel(): limit(0), state(0)
{
  init();
}

/**
 * @param _limit Channel size limit.
 */
channel::channel(size_t _limit): limit(_limit), state(0)
{
  init();
}

/**
 * @param _data Signal content.
 */
void  channel::insert(const string &_data)
{
  //slock.lock();
  if (siglist.size() > limit)
    siglist.pop_back();

  siglist.push_front(_data);
  state = (rand() % 10000 + rand() + 100);
  //slock.unlock();
}

string    *channel::getsig()
{
  string  *data;
  auto    itr = siglist.begin();

  data = new string((*itr));
  return data;
}

size_t    channel::getstate()
{
  return state;
}

void      channel::init()
{
  srand(time(NULL));
  state = (rand() % 10000 + rand() + 100);
}
