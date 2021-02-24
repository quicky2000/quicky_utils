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

#ifdef QUICKY_UTILS_SELF_TEST
#include "multi_thread_signal_handler.h"
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

namespace quicky_utils
{
    /**
     * Class representing a multi thread application
     */
    class test_multi_thread:
            public multi_thread_signal_handler_listener_if
    {
      public:
        test_multi_thread();

        /**
         * Method ot launch a thread
         * @param p_this This pointer to be able to launch object method
         * @param p_thread_index thread index
         */
        static void
        launch_worker(test_multi_thread & p_this,
                      unsigned int p_thread_index
                     );

        /**
         * Object method executed by thread
         * @param p_thread_index thread index
         */
        void
        run_worker(unsigned int p_thread_index);

        /**
         * Method treating signal sent to a thread
         * @param p_signal signal value
         * @param p_thread_id index of thread receiving signal
         */
        void
        handle_signal(int p_signal,
                      unsigned int p_thread_id
                     ) override;

        /**
         * test method
         */
        void
        run();

      private:

        /**
         * Number of threads
         */
        static const unsigned int m_nb_thread = 4;

        /**
         * COrrespondancy between thread index and thread id
         */
        std::array<std::thread::id, m_nb_thread> m_thread_ids;

        /**
         * Thread objects
         */
        std::vector<std::thread *> m_threads;

        /**
         * Array indicating for each thread if it should stop
         */
        std::array<bool, m_nb_thread> m_thread_finished;

    };

    //-------------------------------------------------------------------------
    void
    test_multi_thread::launch_worker(test_multi_thread & p_this,
                                     unsigned int p_thread_index
                                    )
    {
        std::cout << "Launch thread " << p_thread_index << std::endl;
        p_this.run_worker(p_thread_index);
    }

    //-------------------------------------------------------------------------
    void
    test_multi_thread::run_worker(unsigned int p_thread_index)
    {
        std::cout << "Thread " << p_thread_index << " starting with id " << std::this_thread::get_id() << std::endl;
        while (!m_thread_finished[p_thread_index])
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        std::cout << "Thread " << p_thread_index << " ending" << std::endl;
    }

    //-------------------------------------------------------------------------
    void
    test_multi_thread::handle_signal(int p_signal,
                                     unsigned int p_thread_id
                                    )
    {
        std::cout << "Thread " << p_thread_id << " receive signal " << p_signal;
        m_thread_finished[p_thread_id] = true;
    }

    //-------------------------------------------------------------------------
    void
    test_multi_thread::run()
    {
        std::cout << "Pthread identifier of main thread: " << pthread_self() << std::endl;
        multi_thread_signal_handler<m_nb_thread>::create_unique_instance(*this,
                                                                         m_thread_ids,
                                                                         {SIGUSR1}
                                                                        );
        std::cout << "Create " << m_nb_thread << " threads" << std::endl;
        for (unsigned int l_index = 0;
             l_index < m_nb_thread;
             ++l_index
                )
        {
            m_threads.emplace_back(new std::thread(test_multi_thread::launch_worker,
                                                   std::ref(*this),
                                                   l_index
                                                  )
                                  );
            m_thread_ids[l_index] = m_threads.back()->get_id();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        for (auto l_iter: m_threads
                )
        {
            pthread_kill(l_iter->native_handle(),
                         SIGUSR1
                        );
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "Waiting for the end of threads" << std::endl;
        for (auto l_iter: m_threads
                )
        {
            l_iter->join();
            delete l_iter;
        }

        multi_thread_signal_handler<m_nb_thread>::delete_unique_instance();
    }

    //-------------------------------------------------------------------------
    test_multi_thread::test_multi_thread()
    :m_thread_finished{}
    {
        m_thread_finished.fill(false);
    }

    //-------------------------------------------------------------------------
    bool
    test_multi_thread_signal_handler()
    {
        test_multi_thread l_test_multi_thread;
        l_test_multi_thread.run();
        return true;
    }

}
#endif // QUICKY_UTILS_SELF_TEST
// EOF
