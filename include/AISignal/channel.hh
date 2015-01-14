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

#ifndef LIBAISIGNAL_CHANNEL_HH
#define LIBAISIGNAL_CHANNEL_HH

#include <stdlib.h>
#include <time.h>
#include <string>
#include <list>
#include <KNM/sync/KMutex.hh>
#include <AISignal/server.hh>

using namespace std;
using namespace KNM;

namespace AISignal
{
  class   server;

  class   channel
  {
  public:
    channel();                                   /**< Default constructor. */
    channel(size_t);                             /**< Construct + size limit. */

    void      insert(const signal &);           /**< Insert a signal. */
    void      add_server(AISignal::server *);   /**< Add subscriber.*/
    void      del_server(AISignal::server *);   /**< Remove subscriber. */
    //auto      get_server(AISignal::server *);   /**< Return server link. */

  private:
    void            init();     /**< Initialize. */

    list<signal>            siglist;    /**< List of signals. */
    list<AISignal::server*> servers;    /**< Subscribers list. */
    size_t                  limit;      /**< Maximum size of signal stack. */
    KMutex                  siglock;    /**< Signal stack state mutex. */
    KMutex                  srvlock;    /**< Sub. list mutex. */
  };
}

#endif
