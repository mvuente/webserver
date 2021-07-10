#include "CGI.hpp"

CGI::CGI(const std::string &root)
{
	_init(root);
}

CGI::CGI(Request &r, const std::string &root)
{
	URL 		url(r.getSource());
	std::string path = url.getPath();
	
	_init(root);
	_headers = r.getParsedData();
	_input_data = r.getBody();
	_content_length.second = _headers["content-length"];
	_content_type.second = _headers["content-type"];
	_request_method.second = r.getMethod();
	_request_uri.second = url.getRawURI();
	_query_string.second = url.getQueryString();
	_server_name.second = url.getHost();
	_server_port.second = url.getPort();
	_script_name.second = path.substr(path.rfind('/'));
	_remote_addr.second = _headers["x-forwarded-for"];
	_path_info.second = url.getPath();
}

CGI::CGI(const CGI &c)
{
	*this = c;
}

CGI::~CGI()
{
	delete [] _meta_variables;
}

CGI& CGI::operator=(const CGI &c)
{
	if (this != &c)
	{
		for (int i = 0; c._meta_variables[i] != NULL; ++i)
			_meta_variables[i]->second = c._meta_variables[i]->second;
		_executor_program_name = c._executor_program_name;
		_headers = c._headers;
		_input_data	= c._input_data;
		_output_data = c._output_data;
		__need_pass = c._need_pass();
		_pass_to = c._pass_to;
	}
	return *this;
}

void CGI::_init(const std::string &root)
{
	_auth_type.first = "AUTH_TYPE";
	_content_length.first = "CONTENT_LENGTH";
	_content_type.first = "CONTENT_TYPE";
	_gateway_interface.first = "GATEWAY_INTERFACE", _gateway_interface.second =  "CGI/1.1";
	_path_info.first = "PATH_INFO";
	_path_translated.first = "PATH_TRANSLATED"; // ?
	_query_string.first = "QUERY_STRING";
	_remote_addr.first = "REMOTE_ADDR"; // ?
	_remote_ident.first = "REMOTE_IDENT"; // ?
	_remote_user.first = "REMOTE_USER"; // ?
	_request_method.first = "REQUEST_METHOD", _request_method.second = "GET";
	_request_uri.first = "REQUEST_URI";
	_script_name.first = "SCRIPT_NAME"; //
	_server_name.first = "SERVER_NAME"; // +-
	_server_port.first = "SERVER_PORT"; // +-
	_server_protocol.first = "SERVER_PROTOCOL", _server_protocol.second = "HTTP/1.1";
	_server_softwar.first = "SERVER_SOFTWARE", _server_softwar.second = "webserv"; // +-
	_input_data = "";
	_output_data = "";
	_executor_program_name = "";
	__need_pass = false;
	_pass_to = "";
	_root = root;

	_setArrayOfMetaVar();
}

void CGI::_setArrayOfMetaVar()
{
	const t_metav *meta_vars[] =
	{
		&_auth_type, &_content_length, &_content_type, 			\
		&_gateway_interface, &_path_info, &_path_translated,	\
		&_query_string, &_remote_addr, &_remote_ident, 			\
		&_remote_user, &_request_method, &_request_uri, 		\
		&_script_name, &_server_name, &_server_port, 			\
		&_server_protocol, &_server_softwar, NULL
	};
	const size_t sz = sizeof(meta_vars) / sizeof(*meta_vars);

	_meta_variables = new t_metav *[sz];
	std::memcpy(_meta_variables, meta_vars, sizeof(meta_vars));
}

int	CGI::addMetaVar(const std::string &key, const std::string &val)
{
	for (t_metav **mvar = _meta_variables; *mvar != NULL; ++mvar)
		if (key == (*mvar)->first)
		{
			(*mvar)->second = val;
			return 0; 
		}
	return -1;
}

int CGI::_putMetaVars() const
{
	for (t_metav **mvar = _meta_variables; *mvar != NULL; ++mvar)
	{
		if (setenv((*mvar)->first.c_str(), (*mvar)->second.c_str(), 1))
			return -1;
	}
	return 0;
}

int CGI::_putHTTPHeader(const std::string &key, const std::string val) const
{
	std::string fkey = key;

	std::replace(fkey.begin(), fkey.end(), '-', '_');
	return setenv(("HTTP_" + fkey).c_str(), val.c_str(), 1);
}

int CGI::_putHTTPHeaders() const
{
	for (t_httph::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		if (_putHTTPHeader(it->first, it->second))
			return -1;
	return 0;
}

int CGI::_sendRequestBodyData(int pipe_fd) const
{
	return write(pipe_fd, _input_data.c_str(), _getContentLength());
}

int CGI::_recvOutputData(int pipe_fd)
{
	static const int	buf_sz = 128;
	char				buf[buf_sz];
	int					n;

	while ((n = read(pipe_fd, buf, buf_sz)) > 0)
		_output_data += std::string(buf, n); // string constructor: if buf == {'b', 'i' '\0', 'n'}
	return n;
}

int CGI::_exec() const
{
	const std::string	fullPath	= _getFullPathToScript();
	const std::string	parentDir	= path::pardir(fullPath);
	const std::string	script		= path::filename(fullPath);

	if (chdir(parentDir.c_str()) == -1)
		return -1;
	if (_getExecutor() != "")
		return execl("/bin/sh", _getExecutor().c_str(), script.c_str(), 0);
	else
		return execl(script.c_str(), script.c_str(), 0);
}

void CGI::_addResponseHeaders(Response &resp)
{
	static const std::string	body_delim		= Response::LB + Response::LB;
	std::string::size_type		first_line_end	= getOutput().find(Response::LB);
	std::string::size_type		body_delim_idx	= getOutput().find(body_delim);
	
	if (first_line_end != std::string::npos)
	{
		static const char				colon 	= ':';
		const std::string::size_type 	from 	= first_line_end + Response::LB.length();
		std::string			header;
		std::stringstream	headers(getOutput().substr(from, body_delim_idx - from));

		while (std::getline(headers, header))
		{
			std::string::size_type header_delim_idx = header.find(colon);

			if (header_delim_idx != std::string::npos)
				resp.setRSP_HEADER(header.substr(0, header_delim_idx + 1), header.substr(header_delim_idx + 1));
			else if (header != "")
				resp.setRSP_HEADER(header + colon, "");
		}
	}
}

void CGI::_setResponseBody(Response &resp)
{
	static const std::string	body_delim		= Response::LB + Response::LB;
	std::string::size_type		body_delim_idx	= getOutput().find(body_delim);

	if (body_delim_idx != std::string::npos)
	{
		const std::string body = getOutput().substr(body_delim_idx + body_delim.length());
		
		if (body.length() > 0)
			resp.setRSP_BODY(body);
	}

}

void CGI::_setResponseStatusLine(Response &resp)
{
	static const std::string 	repl 			= "Status:";
	std::string::size_type		index			= getOutput().find(repl);
	std::string::size_type		first_line_end	= getOutput().find(Response::LB);
	static const std::string	repl_to			= "HTTP/1.1";

	if (index != std::string::npos)
	{
		resp.setRSP_STATUS_LINE(getOutput().substr(0, first_line_end).replace(index, repl.length(), repl_to));
	}
	else
		resp.setCode(200);
}

void CGI::completeResponse(Response &resp)
{
	_setResponseStatusLine(resp);
	_setResponseBody(resp);
	_addResponseHeaders(resp);
}

int CGI::run()
{
	pid_t			p;
	int				status;

	try
	{
		TempFile out;

		p = fork();
		if (p < 0)
			return -1;
		if (p == 0)
		{
			TempFile in;
			
			if (!_input_data.empty())
			{
				if (_sendRequestBodyData(in.getFd()) == -1)
				return -1;
			}
			in.toBeginning();
			if (_putMetaVars() || _putHTTPHeaders()) exit(4);
			in.dup(0);
			out.dup(1);
			_exec();
			exit(10);
		}

		if (waitpid(p, &status, 0) == -1 || WEXITSTATUS(status) != 0)
		{
			return -1;
		}
		out.toBeginning();
		if (_recvOutputData(out.getFd()) == -1)
		{
			return -1;
		}
	}
	catch (const TempFile::TempFileException &e)
	{
		return -1;
	}
	return 0;
}
