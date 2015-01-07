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
#include <AISignal/filter.hh>

using namespace std;
using namespace KNM;

namespace AISignal
{
  class   channel
  {
  public:
    channel();                            /**< Default constructor. */
    channel(size_t);                      /**< Construct with a size limit. */

    void      insert(const string &);     /**< Insert a new signal. */
    string    *getsig();                  /**< Return a copy of a signal. */
    size_t    getstate();                 /**< Return current state value. */

  private:
    void            init();               /**< Initialize object. */

    list<string>    siglist;              /**< List of signals. */
    size_t          limit;                /**< Maximum size of signal stack. */
    KMutex          slock;                /**< Signal stack state mutex. */
    size_t          state;                /**< Signal stack state. */
  };
}

#endif
