#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "HTTPRequestParser.hpp"
#include "Request.hpp"

using std::string;
using std::cout;
using std::endl;

static const string req1 = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: Mozilla/5.0\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\n\r\n";
static const string req2 = "POST / HTTP/1.1\r\nHost: localhost:8000\r\nContent-Length: 30\r\n" 
							"User-Agent: Mozilla/5.0\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\n\r\n" +
							string("=======123456789\0\0\0test body!\0", 30);
static const string req3 = "DELTE /path/to/section  HTTP1.1    \r\nHost:   site.com \r\nContent-Length: 250\r\nPseudo-Header: TEST\r\n\r\n" +
							string("Its's cool story Bob!.. \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\n\n\n\n\n\n\n\n\n\r\n\r\n\r\n\n\\xCAF\\x124####@@"
									"============================egdeeeeeeeeeeeeeeeeeeeeedqqqqqqqqqqqqqqqqqssssssssssssvvvvvvvvvvqqqqqqqqqqqqqqqqqqq"
									"=evgqklkkkkkkklkkkkkkkkkkkkkkkkkkkkkkkkkkkk\ed\e\egv\eh\eh\ene\rrrwdrf\rfr\r\n\0\0", 250);

void test_parse(HTTPRequestParser &parser, const char *data, size_t size, bool expectedEndParse)
{
	parser.addNewData((void *)data, size);
	parser.parse();
	if (parser.isEndOfParse())
	{
		if (expectedEndParse)
		{
			Request *r = parser.getParsedRequest();
			cout << *r << endl;
			delete r;
		}
		else
			cout << "Error!" << endl;
	}
	else if (expectedEndParse)
		cout << "Error!" << endl;
}

int	main (void)
{
	HTTPRequestParser p;

	cout << "===== TEST 1: full request parse in one time =====" << endl;
	test_parse(p, req1.c_str(), req1.size(), 1);

	cout << "\n===== TEST 2: full request parse in one time with body =====" << endl;
	cout << "Request size: " << req2.size() << endl;
	p.clear();
	test_parse(p, req2.c_str(), req2.size(), 1);

	cout << "\n===== TEST 3: request parse in 3 times with body =====" << endl;
	const char *data = req2.c_str();
	size_t portion = req2.size() / 3;
	size_t remainder = req2.size() % 3;

	cout << "Request size: " << req2.size() << endl;
	cout << "Portion size: " << portion << endl;
	cout << "Remainder size: " << remainder << endl;
	p.clear();
	cout << "[PART 1]" << endl;
	test_parse(p, data, portion, 0);
	cout << "[PART 2]" << endl;
	test_parse(p2, data + portion, portion, 0);
	cout << "[PART 3]" << endl;
	test_parse(p2, data + portion * 2, portion + remainder, 1);
	
	cout << "\n===== TEST 4: request parse in 4 times (different portions) with body =====" << endl;
	cout << "Request size: " << req3.size() << endl;
	data = req3.c_str();
	p.clear();

	cout << "[PART 1] portion = 142" << endl;
	test_parse(p, data, 142, 0);
	cout << "[PART 2] portion = 10" << endl;
	test_parse(p, data + 142, 10, 0);
	cout << "[PART 3] portion = 43" << endl;
	test_parse(p, data + 10, 43, 0);
	cout << "[PART 4] portion = 155" << endl;
	test_parse(p, data + 43, 155, 1);

	cout << "\n===== TEST 5: request parse in 1 time with body from binary file =====" << endl;
	std::ifstream fin("test_parser.out", std::ios::binary);
	std::ofstream fout("body_of_tested_request.out", std::ios::binary);
	std::ostringstream datastream;
	std::ostringstream content_length_stream;
	string body_data;
	string content_length;

	datastream << fin.rdbuf();
	content_length_stream << "Content-Length: " << fin.tellg();
	content_length = content_length_stream.str();
	body_data = datastream.str();
	const string req4 = "PUT /long/path/to/endpoint/plus/plus/plus/plus/plus/content.pdf HTTP/1.1\r\nContent-type: file/binary\r\n" +
						content_length + "\r\n\r\n" + body_data;

	cout << "Request size: " << req4.size() << endl;
	p.clear();
	test_parse(p, req4.c_str(), req4.size(), 1);
	Request *r = p.getParsedRequest();
	fout << r->getBody();
	fout.close();
	fin.close();
}

