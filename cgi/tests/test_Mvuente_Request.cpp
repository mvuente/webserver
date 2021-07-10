#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "Request.hpp"

using std::string;
using std::cout;
using std::endl;

static string req1 = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: Mozilla/5.0\r\nAccept-Language: en-US, en;q=0.5\r\nConnection: keep-alive\r\n"
					 "Date: Wed, 10 Aut 2016 13:17:18 GMT\r\n\r\n";
static string req2 = "POST / HTTP/1.1\r\nHost: localhost:8000\r\nContent-Length: 30\r\nDate: Wed, 10 Aut 2016 13:17:18 GMT\r\n\r\n" 
							"User-Agent: Mozilla/5.0\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\n\r\n" +
							string("=======123456789\0\0\0test body!\0", 30);
static string req3 = "DELTE /path/to/section  HTTP1.1    \r\nHost:   site.com \r\nContent-Length: 250\r\nPseudo-Header: TEST\r\n\r\n" +
							string("Its's cool story Bob!.. \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\n\n\n\n\n\n\n\n\n\r\n\r\n\r\n\n\\xCAF\\x124####@@"
									"============================egdeeeeeeeeeeeeeeeeeeeeedqqqqqqqqqqqqqqqqqssssssssssssvvvvvvvvvvqqqqqqqqqqqqqqqqqqq"
									"=evgqklkkkkkkklkkkkkkkkkkkkkkkkkkkkkkkkkkkk\ed\e\egv\eh\eh\ene\rrrwdrf\rfr\r\n\0\0", 250);

void printRequest(Request &r)
{
	typedef std::map<std::string, std::vector<std::string> > t_parsed_data;

	t_parsed_data d = r.getParsedData();

	cout << "Request: {" 
			<< "httpVersion: not implemented"
			<< ", method: " << r.getMethod()
			<< ", url: " << r.getSource()
			<< ", headers: {";
	for (t_parsed_data::iterator it = d.begin(); it != d.end();)
	{
		cout << it->first << " ";
		for (std::vector<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end();)
		{
			cout << *it2;
			if (++it2 != it->second.end()) cout << " | ";
		}
		if (++it != d.end()) cout << " ";
	}
	cout << "}";
	cout << ", body: " << r.getBody();
	cout << ", body_size: " << r.getBody().length();
	cout << " } " << endl;
}

int main()
{
	Request r1(req1);
	Request r2(req2);
	Request r3(req3);

	printRequest(r1);
	printRequest(r2);
	printRequest(r3);
}