/*    This file is part of quicky_utils
      Copyright (C) 2019  Julien Thevenon ( julien_thevenon at yahoo.fr )

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef QUICKY_UTILS_MULTI_THREAD_SIGNAL_HANDLER_LISTENER_IF_H
#define QUICKY_UTILS_MULTI_THREAD_SIGNAL_HANDLER_LISTENER_IF_H

namespace quicky_utils
{
    class multi_thread_signal_handler_listener_if
    {
      public:
        virtual void
        handle_signal(int p_signal
                     ,unsigned int p_thread_id
                     ) = 0;

        inline
        virtual ~multi_thread_signal_handler_listener_if(void) = default;
    };
}
#endif //QUICKY_UTILS_MULTI_THREAD_SIGNAL_HANDLER_LISTENER_IF_H
// EOF