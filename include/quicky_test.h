/*    This file is part of quicky_utils
      Copyright (C) 2018  Julien Thevenon ( julien_thevenon at yahoo.fr )

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

#ifndef QUICKY_UTILS_QUICKY_TEST_H
#define QUICKY_UTILS_QUICKY_TEST_H

#include <string>
#include <sstream>
#include <iostream>
#include <functional>

namespace quicky_utils
{
    /**
     * Some utilities to test software
     */
    class quicky_test
    {
      public:
        /**
         * Check provided value vs expected value and return a boolean
         * indicating if comparison was OK or not
         * @tparam T Type of values to compare
         * @param p_value Value to check
         * @param p_expected Expected value
         * @param p_message Additional message to print before check result
         * @param p_stream Output stream where to send the message
         * @param p_quiet if false a message is send to output stream
         * @return true in case of equality false in contrary case
         */
        template<typename T>
        static bool
        check_expected(const T & p_value,
                       const T & p_expected,
                       const std::string & p_message = "",
                       std::ostream & p_stream = std::cout,
                       bool p_quiet = false
                      );

        /**
         * Check if a code raise an exception and return a boolean indicating
         * if code behaviour is the expected one
         * @tparam EXCEPTION exception type
         * @param p_function code to execute
         * @param p_exception_expected indicate if an exception is expected
         * @param p_message optionnal message to display
         * @param p_ostream output stream where diagnostic message is sent
         * @param p_quiet if false a message is send to output stream
         * @return true if expected exception was raised
         */
        template<class EXCEPTION>
        static bool
        check_exception(const std::function<void(void)> & p_function,
                        bool p_exception_expected = true,
                        const std::string & p_message = "",
                        std::ostream & p_ostream = std::cout,
                        bool p_quiet = false
                       );

        /**
         * Check result of ostream operator
         * @tparam T Type of object applied to ostream operator
         * @param p_object object applied to ostream operator
         * @param p_expected expected displayed string
         * @param p_message optionnal message to display
         * @param p_ostream output stream where diagnostic message is sent
         * @param p_quiet if false a message is send to output stream
         * @return true if displayed string is the expected one
         */
        template<typename T>
        static bool
        check_ostream_operator(const T & p_object,
                               const std::string & p_expected,
                               const std::string & p_message = "",
                               std::ostream & p_ostream = std::cout,
                               bool p_quiet = false
                              );

        /**
         * Generate automatic message with file info
         * @param p_file_name file name
         * @param p_line line index
         * @return generated message
         */
        static inline std::string auto_message(const std::string & p_file_name,
                                               const unsigned int & p_line
                                              );
    };

    //-------------------------------------------------------------------------
    template<typename T>
    bool
    quicky_test::check_expected(const T & p_value,
                                const T & p_expected,
                                const std::string & p_message,
                                std::ostream & p_stream,
                                bool p_quiet
                               )
    {
        if(!p_message.empty())
        {
            std::cout << p_message << ": ";
        }
        bool l_result = p_value == p_expected;
        if(!p_quiet)
        {
            std::cout << "Expected value : " << p_expected << "\tValue : " << p_value << " => " << (l_result ? "PASSED" : "FAILED") << std::endl;
        }
        return l_result;
    }

    //-------------------------------------------------------------------------
    template<>
    bool
    quicky_test::check_expected(const std::string & p_value,
                                const std::string & p_expected,
                                const std::string & p_message,
                                std::ostream & p_stream,
                                bool p_quiet
                               )
    {
        if(!p_message.empty())
        {
            std::cout << p_message << ": ";
        }
        bool l_result = p_value == p_expected;
        if(!p_quiet)
        {
            std::cout << "Expected value : \"" << p_expected << "\"\tValue : \"" << p_value << "\" => " << (l_result ? "PASSED" : "FAILED") << std::endl;
        }
        return l_result;
    }

    //-------------------------------------------------------------------------
    template <class EXCEPTION>
    bool
    quicky_test::check_exception(const std::function<void(void)> & p_function,
                                     bool p_exception_expected,
                                     const std::string & p_message,
                                     std::ostream & p_ostream,
                                     bool p_quiet
                                    )
    {
        bool l_exception_raised = false;
        try
        {
            p_function();
        }
        catch(EXCEPTION & e)
        {
            l_exception_raised = true;
        }
        bool l_result = p_exception_expected == l_exception_raised;
        if(!p_message.empty())
        {
            std::cout << p_message << ": ";
        }
        if(!p_quiet)
        {
            std::cout << "Exception expected : " << (p_exception_expected ? "YES" : "NO") << "\tException raised : " << (l_exception_raised ? "YES" : "NO") << " => " << (l_result ? "PASSED" : "FAILED") << std::endl;
        }
        return l_result;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    quicky_test::check_ostream_operator(const T & p_object,
                                        const std::string & p_expected,
                                        const std::string & p_message,
                                        std::ostream & p_ostream,
                                        bool p_quiet
                                       )
    {
        std::stringstream l_stream;
        l_stream << p_object;
        std::string l_string = l_stream.str();
        return check_expected(l_string, p_expected, p_message, p_ostream, p_quiet);
    }

    //-------------------------------------------------------------------------
    std::string
    quicky_test::auto_message(const std::string & p_file_name,
                              const unsigned int & p_line
                             )
    {
        return p_file_name + ":" + std::to_string(p_line);
    }

}
#endif //QUICKY_UTILS_QUICKY_TEST_H
// EOF