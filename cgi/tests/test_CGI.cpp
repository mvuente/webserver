#include  "CGI.hpp"
#include  "Request.hpp"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

static string req = "POST /cgi/ubuntu_cgi_tester?name=A&surname=B&age=22 HTTP/1.1\r\nHost: localhost:8000\r\nContent-Length: 30\r\nDate: Wed, 10 Aut 2016 13:17:18 GMT" 
							"User-Agent: Mozilla/5.0\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\n\r\n" +
							string("=======123456789\0\0\0test body!\0", 30);
static string req2 = "POST http://localhost:4000/cgi/ubuntu_cgi_tester?name=A&surname=B&age=22 HTTP/1.1\r\nHost: localhost:8000\r\nContent-Length: 30\r\nDate: Wed, 10 Aut 2016 13:17:18 GMT" 
							"User-Agent: Mozilla/5.0\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\n\r\n" +
							string("=======123456789\0\0\0test body!\0", 30);
static string req3 = "POST /cgi/youpi.bla?name=A&surname=B&age=22 HTTP/1.1\r\nHost: localhost:8000\r\nContent-Length: 30\r\nDate: Wed, 10 Aut 2016 13:17:18 GMT" 
							"User-Agent: Mozilla/5.0\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\n\r\n" +
							string("=======123456789\0\0\0test body!\0", 30);

void printRequest(Request &r)
{
	typedef std::map<std::string, std::string > t_parsed_data;
	
	t_parsed_data d = r.getParsedData();

	cout << "Request: {" 
			<< "httpVersion: not implemented"
			<< ", method: " << r.getMethod()
			<< ", url: " << r.getSource()
			<< ", headers: {";
	for (t_parsed_data::iterator it = d.begin(); it != d.end();)
	{
		cout << it->first << ":" << it->second;
		if (++it != d.end()) cout << " ";
	}
	cout << "}";
	cout << ", body: " << r.getBody();
	cout << ", body_size: " << r.getBody().length();
	cout << " } " << endl;
}

int	main ()
{
	CGI cgi(".");
	CGI cgi2(".");
	CGI cgi3(".");

	cout << "add metavar err: " << cgi.addMetaVar("PATH_INFO", "/cgi/cpp_cgi.cgi") << endl;
	cout << "add metavar err: " << cgi.addMetaVar("SCRIPT_NAME", "/cpp_cgi.cgi") << endl;
	cout << "add metavar err: " << cgi.addMetaVar("CONTENT_LENGTH", "28") << endl;
	cout << "add metavar err: " << cgi.addMetaVar("AUTH_TYPE", "Basic") << endl;
	cout << "add metavar err: " << cgi.addMetaVar("CONTENT_TYPE", "text/plain") << endl;
	cgi.setInputData(string("What a wonderful world!!!!!!!")); // need FIX ?..
	cout << cgi << endl;
	if (cgi.run() == -1)
	{
		cout << "Error!" << endl;
	}
	cout << cgi.getOutput() << endl;

	cout << "--------------" << endl;
	
	/*
	cgi2.addMetaVar("path_info", "/ubuntu_cgi_tester");
	cgi2.addMetaVar("script_name", "ubuntu_cgi_tester");
	cgi2.addMetaVar("content_length", "28");
	cgi2.addMetaVar("auth_type", "Basic");
	cgi2.addMetaVar("content_type", "text/plain");
	cgi2.setInputData(string("What a wonderful world!!!!!!!"));
	if (cgi2.run() == -1)
	{
		cout << "Error!" << endl;
	}
	cout << cgi2.getOutput() << endl;

	cout << "--------------" << endl;

	cgi3.addMetaVar("path_info", "./cpp_cgi.cgi");
	cgi3.addMetaVar("script_name", "cpp_cgi.cgi");
	cgi3.addMetaVar("content_length", "28");
	cgi3.addMetaVar("auth_type", "Basic");
	cgi3.addMetaVar("content_type", "text/plain");
	cgi3.setInputData(string("What a wonderful world!!!!!!!"));
	if (cgi3.run() == -1)
	{
		cout << "Error!" << endl;
	}
	cout << cgi3.getOutput() << endl;
	*/
	/*
	cout << "--------------" << endl;

	Request r(req);
	CGI cgi4(r, ".");
	cout << cgi4 << endl;
	if (cgi4.run() == -1)
	{
		cout << "Error!" << endl;
	}
	cout << cgi4.getOutput() << endl;

	cout << "--------------" << endl;

	Request r2(req2);
	CGI cgi5(r2, ".");
	printRequest(r2);
	cout << cgi5 << endl;
	if (cgi5.run() == -1)
	{
		cout << "Error!" << endl;
	}
	cout << cgi5.getOutput() << endl;
	
	cout << "--------------" << endl;

	Request r3(req3);
	printRequest(r3);
	CGI cgi6(r3, ".");
	cgi6.pass("./cgi/python_cgi.cgi");
	cgi6.run();
	cout << cgi6.getOutput() << endl;
	
	cout << "--------------" << endl;

	*/
	CGI cgi7(".");
	
	cgi7.addMetaVar("PATH_INFO", "/cgi/cgi_tester");
	cgi7.addMetaVar("REQUEST_URI", "/cgi/cgi_tester");
	cgi7.addMetaVar("SCRIPT_NAME", "/cgi_tester");
	cgi7.addMetaVar("CONTENT_LENGTH", "28");
	cgi7.addMetaVar("AUTH_TYPE", "Basic");
	cgi7.addMetaVar("CONTENT_TYPE", "text/plain");
	cgi7.setInputData(string("What a wonderful world!!!!!!!"));

	// cgi7.pass("./cgi/python_cgi.cgi");
	if (cgi7.run() == -1)
		cout << "Error!" << endl;

	cout << cgi7.getOutput() << endl;
	
	cout << "--------------" << endl;

	return 0;
}
