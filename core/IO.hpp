#ifndef IO_HPP
#define IO_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cstddef>

class IO
{
	protected:
		virtual void _update() = 0;
	public:
		virtual bool error() const = 0;
};

class Sender : public IO
{
	private:
		
		bool			_isError;

		void _update()
		{
			_isError = false;
		}

	public:
		Sender() :
			IO(), _isError(false)
		{}

		bool error() const
		{
			return _isError;
		}

		ssize_t	send(int fd, const std::string& msg, int flags = 0)
		{
			int n = 0;

			_update();
			if ((n = ::send(fd, msg.c_str(), msg.length(), flags)) == -1)
				_isError = true;
			return n;
		}
};

class Reader : public IO
{
	enum State
	{
		RD_NORMAL, RD_EOF, RD_ERROR
	};

	private:
		static const int BUF_SZ = 50000;
		
		char			_buffer[BUF_SZ];
		std::string		_result;
		State			_state;

		void _update()
		{
			_result = "";
			_state = RD_NORMAL;
		}

	public:
		Reader() :
			IO(), _result(), _state(RD_NORMAL)
		{}

		bool				eof() const
		{
			return _state == RD_EOF;
		}

		bool				error() const
		{
			return _state == RD_ERROR;
		}

		bool				success() const
		{
			return _state == RD_NORMAL;
		}

		const std::string&	read(int fd, int flags = 0)
		{
			int n = 0;
			
			_update();
			switch ((n = recv(fd, _buffer, BUF_SZ, flags)))
			{
				case 0:
					_state = RD_EOF;
					break ;
				case -1:
					_state = RD_ERROR;
					break ;
				default:
					_result = std::string(_buffer, n);
			}
			
			return _result;
		}
};

#endif