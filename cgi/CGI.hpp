#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <utility>	 // pair
#include <cstdlib>
#include <cstddef>	 // size_t
#include <cstdio>	 // sscanf
#include <cstring>	 // memcpy
#include <algorithm> // std::replace
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ostream>
#include <fcntl.h>
#include <map>
#include "path.hpp"
#include "to_size_t.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "URL.hpp"

class CGI
{
	typedef std::pair<std::string, std::string> t_metav;
	typedef std::map<std::string, std::string>	t_httph;

	private:
		t_metav		_auth_type;
		t_metav		_content_length;
		t_metav		_content_type;
		t_metav		_gateway_interface;
		t_metav		_path_info;
		t_metav		_path_translated;
		t_metav		_query_string;
		t_metav		_remote_addr;
		t_metav		_remote_ident;
		t_metav		_remote_user;
		t_metav		_request_method;
		t_metav		_request_uri;
		t_metav		_script_name;
		t_metav		_server_name;
		t_metav		_server_port;
		t_metav		_server_protocol;
		t_metav		_server_softwar;

		t_httph		_headers;

		std::string _executor_program_name;

		std::string _pass_to;
		bool		__need_pass;

		std::string			_input_data; // Request body
		std::string			_output_data;
		std::string			_root; // server root location
		t_metav**			_meta_variables;

		void				_addResponseHeaders(Response &resp);
		void 				_setResponseBody(Response &resp);
		void 				_setResponseStatusLine(Response &resp);

		void				_init(const std::string &root);
		void				_setOutputData(const std::string &d);
		void				_setArrayOfMetaVar();
		size_t				_getContentLength() const;
		const std::string&	_getExecutor() const;
		const std::string	_getFullPathToScript() const;
		bool				_need_pass() const;
		int 				_recvOutputData(int pipe_fd);
		int 				_sendRequestBodyData(int pipe_fd) const;
		int					_putMetaVars() const;
		int					_putHTTPHeader(const std::string &key, const std::string val) const;
		int					_putHTTPHeaders() const;
		int					_exec() const;
	
		friend std::ostream &operator<<(std::ostream &os, CGI const &cgi);

	public:
		CGI(const std::string &root = "/");
		CGI(Request &r, const std::string &root = "/");
		CGI(const CGI &c);
		~CGI();
		CGI&				operator=(const CGI &c);
		int					run();
		void				pass(const std::string &passTo);
		void				setInputData(const std::string &d);
		void				setExecutor(const std::string &execName);
		void				setRoot(const std::string &rootPath);
		int					addMetaVar(const std::string &key, const std::string &val);
		void 				completeResponse(Response &resp);
		const std::string&	getOutput() const;
};

inline void CGI::pass(const std::string &passTo)
{
	_pass_to = passTo;
	__need_pass = true;
}

inline const std::string &CGI::getOutput() const
{
	return _output_data;
}

inline void CGI::setRoot(const std::string &root)
{
	_root = root;
}

inline void	CGI::setInputData(const std::string &d)
{
	_input_data = d;
}
 
inline void	CGI::_setOutputData(const std::string &d)
{
	_output_data = d;
}
 
inline void	CGI::setExecutor(const std::string &execName)
{
	_executor_program_name = execName;
}

inline bool CGI::_need_pass() const
{
	return __need_pass;
}

inline size_t CGI::_getContentLength() const
{
	size_t res;

	if (util::to_size_t(_content_length.second, &res) != 1)
		res = _input_data.length();
	return res;
}

inline const std::string CGI::_getFullPathToScript() const
{
	if (_need_pass())
		return _pass_to;
	return path::path_join(_root, _path_info.second);
}

inline const std::string &CGI::_getExecutor() const
{
	return _executor_program_name;
}

inline std::ostream &operator<<(std::ostream &os, CGI const &cgi)
{
	os << "CGI: { meta_vars { ";
	for (CGI::t_metav **mvar = cgi._meta_variables; *mvar != NULL; ++mvar)
		os << (*mvar)->first << ":" << (*mvar)->second << " ";
	os << " } headers { ";
	for (CGI::t_httph::const_iterator it = cgi._headers.begin(); it != cgi._headers.end(); ++it)
		os << it->first << ":" << it->second << " ";
	os << " } root: " << cgi._root;
	//os << " input: " << cgi._input_data;
	//os << " output: " << cgi._output_data;
	os << " pass: " << cgi._pass_to;
	os << " need_pass: " << cgi.__need_pass;
	os << " }";
	return os;
}

class PipeManager
{
	public:
		class PipeManagerException : std::exception {};
	private:
		int pipe_fd[2];

		void _close(int n)
		{
			if (close(pipe_fd[n])) throw PipeManagerException();
			pipe_fd[n] = -1;
		}
		void _dup(int n, int fd)
		{
			if (dup2(pipe_fd[n], fd) == -1) throw PipeManagerException();
		}
		int _get(int n) { return pipe_fd[n]; }
	public:
		PipeManager()
		{
			if (pipe(pipe_fd))
				throw PipeManagerException();
		}
		~PipeManager()
		{
			if (pipe_fd[0] != -1) close(pipe_fd[0]);
			if (pipe_fd[1] != -1) close(pipe_fd[1]);
		}
		void close0() { _close(0); }
		void close1() { _close(1); }
		void dup0(int fd) { _dup(0, fd); };
		void dup1(int fd) { _dup(1, fd); };
		int get0() { return _get(0); }
		int get1() { return _get(1); }
};

class TempFile
{
	public:
		class TempFileException : std::exception {};
	private:
		int _fd;
	public:
		TempFile(const std::string &path = "")
		{
			if (path == "") 
			{
				std::FILE *tmp = std::tmpfile();
			
				if (tmp == NULL) throw TempFileException();
				_fd = fileno(tmp);
			}
			else
			{
				_fd = open(path.c_str(), O_RDWR | O_CREAT, S_IRWXU);
			}
			if (_fd == -1) throw TempFileException();
		}

		~TempFile()
		{
			if (_fd != -1) ::close(_fd);
		}

		void close()
		{ 
			if (::close(_fd)) throw TempFileException();
			_fd = -1;
		}
		void dup(int fd) const { if (dup2(_fd, fd) == -1) throw TempFileException(); };

		int getFd() const { return _fd; }

		void toBeginning() const { if (lseek(_fd, 0, SEEK_SET) == -1) throw TempFileException(); }
};

#endif
