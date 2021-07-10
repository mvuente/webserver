#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include "date.hpp"

#define COL_BLUE "\033[1;34m"
#define COL_DEFAULT "\033[0m"

class Logger
{
	public:
		enum LOG_LVL
		{
			NEVER, TEST, ALWAYS
		};

	private:
		static LOG_LVL _log_lvl;

		static void _printDate()
		{
			std::cerr << COL_BLUE << "[" << date::getDate() << "]" << COL_DEFAULT;
		}
		template <typename T>
		static void _printWS(const T &msg)
		{
			std::cerr << " " << msg;
		}

		static bool _isRequiredLvl(LOG_LVL lvl)
		{
			return (_log_lvl == ALWAYS && lvl != NEVER) || lvl == ALWAYS || (lvl == TEST && _log_lvl == TEST);
		}

	public:
		static void	setLogLvl(LOG_LVL lvl)
		{
			_log_lvl = lvl;
		}

		static LOG_LVL	getLogLvl()
		{
			return _log_lvl;	
		}

		template <typename T>
		static void println(const T &msg, LOG_LVL lvl = TEST)
		{
			if (_isRequiredLvl(lvl))
			{
				_printDate();
				_printWS(msg);
				std::cerr << std::endl;
			}
			else
				(void)msg;
		}

		template <typename T1, typename T2>
		static void println(const T1 &m1, const T2 &m2, LOG_LVL lvl = TEST)
		{
			if (_isRequiredLvl(lvl))
			{
				_printDate();
				_printWS(m1);
				_printWS(m2);
				std::cerr << std::endl;
			}
			else
			{
				(void)m1;
				(void)m2;
			}
		}

		template <typename T1, typename T2, typename T3>
		static void println(const T1 &m1, const T2 &m2, const T3 &m3, LOG_LVL lvl = TEST)
		{
			if (_isRequiredLvl(lvl))
			{
				_printDate();
				_printWS(m1);
				_printWS(m2);
				_printWS(m3);
				std::cerr << std::endl;
			}
			else
			{
				(void)m1;
				(void)m2;
				(void)m3;
			}
		}
};

#endif