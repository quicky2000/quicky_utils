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
#ifndef QUICKY_UTILS_MULTI_THREAD_SIGNAL_HANDLER_H
#define QUICKY_UTILS_MULTI_THREAD_SIGNAL_HANDLER_H

#include <signal.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "multi_thread_signal_handler_listener_if.h"
#include "quicky_exception.h"
#include <string>
#include <vector>
#include <thread>
#include <cassert>
#include <sstream>
#include <array>

namespace quicky_utils
{
    template <unsigned int NB_THREAD>
    class multi_thread_signal_handler
    {
      public:
        /**
         * Create a unique instance of signal handler
         * @param p_listener
         * @param p_thread_ids
         * @param p_signals
         */
        static inline
        void create_unique_instance(multi_thread_signal_handler_listener_if & p_listener
                ,const std::array<std::thread::id, NB_THREAD> & p_thread_ids
                ,const std::vector<int> & p_signals
                                   );

        /**
         * Delete unique instance of signal handler
         */
        static inline
        void delete_unique_instance();

      private:

        /**
         * Constructor
         * @param p_listener object to be notified when signal is sent
         * @param p_thread_id thread id
         * @param p_signals signal handler will be associated with
         */
        inline
        multi_thread_signal_handler(multi_thread_signal_handler_listener_if & p_listener
                                   ,const std::array<std::thread::id, NB_THREAD> & p_thread_ids
                                   ,const std::vector<int> & p_signals
                                   );

        inline static
        void signal_handler(int p_signal);

        inline
        void handle_signal(int p_signal);

        static inline
        multi_thread_signal_handler & get_unique_instance();

        multi_thread_signal_handler_listener_if & m_listener;

        /**
         * IDs of all threads to retrieve their index
         */
        const std::array<std::thread::id, NB_THREAD> & m_thread_ids;

        static
        multi_thread_signal_handler * m_unique_instance;
    };

#ifdef QUICKY_UTILS_SELF_TEST
    bool
    test_multi_thread_signal_handler();
#endif // QUICKY_UTILS_SELF_TEST

    //-------------------------------------------------------------------------
    template <unsigned int NB_THREAD>
    multi_thread_signal_handler<NB_THREAD>::multi_thread_signal_handler(multi_thread_signal_handler_listener_if & p_listener
                                                                       ,const std::array<std::thread::id, NB_THREAD> & p_thread_ids
                                                                       ,const std::vector<int> & p_signals
                                                                       )
                                                                       : m_listener(p_listener)
                                                                       , m_thread_ids(p_thread_ids)
    {
#ifndef _WIN32
        //Preparing signal handling to manage stop
        // Structure declaration to put handlers in place
        struct sigaction l_signal_action;

        // Filling the structure
        // with handler address
        l_signal_action.sa_handler = signal_handler;

        // This flag is theorically ignored but put as zero to be clean
        l_signal_action.sa_flags = 0;

        // we don't block specific signals
        sigemptyset(&l_signal_action.sa_mask);
#endif // _WIN32

        for(auto l_iter: p_signals)
        {
#ifndef _WIN32
            sigaction(l_iter, &l_signal_action, nullptr);
#else // _WIN32
            signal(SIGINT,handler);
#endif // _WIN32
        }
    }

    //-------------------------------------------------------------------------
    template <unsigned int NB_THREAD>
    void
    multi_thread_signal_handler<NB_THREAD>::signal_handler(int p_signal)
    {
        get_unique_instance().handle_signal(p_signal);
    }

    //-------------------------------------------------------------------------
    template <unsigned int NB_THREAD>
    void
    multi_thread_signal_handler<NB_THREAD>::create_unique_instance(multi_thread_signal_handler_listener_if & p_listener
                                                                  ,const std::array<std::thread::id, NB_THREAD> & p_thread_ids
                                                                  ,const std::vector<int> & p_signals
                                                                  )
    {
        assert(m_unique_instance == NULL);
        m_unique_instance = new multi_thread_signal_handler(p_listener, p_thread_ids, p_signals);
    }

    //-------------------------------------------------------------------------
    template <unsigned int NB_THREAD>
    multi_thread_signal_handler<NB_THREAD> &
    multi_thread_signal_handler<NB_THREAD>::get_unique_instance()
    {
        assert(m_unique_instance);
        return *m_unique_instance;
    }

    //-------------------------------------------------------------------------
    template <unsigned int NB_THREAD>
    void
    multi_thread_signal_handler<NB_THREAD>::handle_signal(int p_signal)
    {
        unsigned int l_index = 0;
        std::thread::id l_thread_id = std::this_thread::get_id();
        while(l_index < m_thread_ids.size())
        {
            if(m_thread_ids[l_index] == l_thread_id)
            {
                m_listener.handle_signal(p_signal, l_index);
                return;
            }
            ++l_index;
        }
        std::stringstream l_stream;
        l_stream << l_thread_id;
        throw quicky_exception::quicky_logic_exception("Unknown thread Id " + l_stream.str(), __LINE__, __FILE__);
    }

    //-------------------------------------------------------------------------
    template <unsigned int NB_THREAD>
    void
    multi_thread_signal_handler<NB_THREAD>::delete_unique_instance()
    {
        delete m_unique_instance;
    }

    template<unsigned int NB_THREAD>
    multi_thread_signal_handler<NB_THREAD> * multi_thread_signal_handler<NB_THREAD>::m_unique_instance = NULL;


}
#endif //QUICKY_UTILS_MULTI_THREAD_SIGNAL_HANDLER_H
// EOF