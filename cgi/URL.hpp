#ifndef URL_HPP
#define URL_HPP

#include <string>
#include <ostream>
#include <algorithm>

#include <iomanip>
#include <sstream>
#include <string>

class URL {
	private:
		std::string _rawURI;
		std::string _queryString;
		std::string _path;
		std::string _protocol;
		std::string _host;
		std::string _port;
	public:
		URL() {};
		URL(const URL &copy) { *this = copy; };
		URL &operator=(const URL &u) { if (this != &u) _rawURI = u.getRawURI(); return *this; }
		~URL() {};
		void setRawURI(const std::string &rawURI) { _rawURI = rawURI; };
		
		const std::string &getRawURI() const { return _rawURI; };
		const std::string &getQueryString() const { return _queryString; };
		const std::string &getPath() const { return _path; };
		const std::string &getProtocol() const { return _protocol; };
		const std::string &getHost() const { return _host; };
		const std::string &getPort() const { return _port; };

		static std::string encode(const std::string &value)
		{
		    std::ostringstream escaped;
			escaped.fill('0');
   			escaped << std::hex;

			for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
			{
				std::string::value_type c = (*i);

				if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
				{
					escaped << c;
					continue;
				}
				escaped << std::uppercase;
				escaped << '%' << std::setw(2) << int((unsigned char) c);
				escaped << std::nouppercase;
			}
    		return escaped.str();
		}

		URL(const std::string &uri)
		{
			typedef std::string::const_iterator iterator_t;

			if (uri.length() == 0)
				return ;

			_rawURI = uri;
			iterator_t uriEnd = uri.end();

			// get query start
			iterator_t queryStart = std::find(uri.begin(), uriEnd, '?');

			// protocol
			iterator_t protocolStart = uri.begin();
			iterator_t protocolEnd = std::find(protocolStart, uriEnd, ':');            //"://");

			if (protocolEnd != uriEnd)
			{
				std::string prot = &*(protocolEnd);
				if ((prot.length() > 3) && (prot.substr(0, 3) == "://"))
				{
					_protocol = std::string(protocolStart, protocolEnd);
					protocolEnd += 3;   //      ://
				}
				else
					protocolEnd = uri.begin();  // no protocol
			}
			else
				protocolEnd = uri.begin();  // no protocol

			// host
			iterator_t hostStart = protocolEnd;
			iterator_t pathStart = std::find(hostStart, uriEnd, '/');  // get pathStart

			iterator_t hostEnd = std::find(protocolEnd, 
				(pathStart != uriEnd) ? pathStart : queryStart,
				':');  // check for port

			_host = std::string(hostStart, hostEnd);

			// port
			if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == ':'))  // we have a port
			{
				hostEnd++;
				iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
				_port = std::string(hostEnd, portEnd);
			}

			// path
			if (pathStart != uriEnd)
				_path = std::string(pathStart, queryStart);

			// query
			if (queryStart != uriEnd)
				_queryString = std::string(queryStart + 1, uri.end());
		}
};

static inline std::ostream &operator<<(std::ostream &os, URL const &u)
{
	os << u.getRawURI();
	return os;
}

#endif
