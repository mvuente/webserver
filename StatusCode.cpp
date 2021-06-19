#include "StatusCode.hpp"

// How to use: StatusCode::getDescStatusCode("200")
// Result: OK

std::string StatusCode::getDescStatusCode(std::string statusCode)
{
	std::map<std::string, std::string> code;
	//1×× Informational
	code["100"] = "Continue";
	code["101"] = "Switching Protocols";
	code["102"] = "Processing";

	//2×× Success
	code["200"] = "OK";
	code["201"] = "Created";
	code["202"] = "Accepted";
	code["203"] = "Non-authoritative Information";
	code["204"] = "No Content";
	code["205"] = "Reset Content";
	code["206"] = "Partial Content";
	code["207"] = "Multi-Status";
	code["208"] = "Already Reported";
	code["226"] = "IM Used";

	//3×× Redirection
	code["301"] = "Moved Permanently";
	code["300"] = "Multiple Choices";
	code["302"] = "Found";
	code["303"] = "See Other";
	code["304"] = "Not Modified";
	code["305"] = "Use Proxy";
	code["307"] = "Temporary Redirect";
	code["308"] = "Permanent Redirect";

	//4×× Client Error
	code["400"] = "Bad Request";
	code["401"] = "Unauthorized";
	code["402"] = "Payment Required";
	code["403"] = "Forbidden";
	code["404"] = "Not Found";
	code["405"] = "Method Not Allowed";
	code["406"] = "Not Acceptable";
	code["407"] = "Proxy Authentication Required";
	code["408"] = "Request Timeout";
	code["409"] = "Conflict";
	code["410"] = "Gone";
	code["411"] = "Length Required";
	code["412"] = "Precondition Failed";
	code["413"] = "Payload Too Large";
	code["414"] = "Request-URI Too Long";
	code["415"] = "Unsupported Media Type";
	code["416"] = "Requested Range Not Satisfiable";
	code["417"] = "Expectation Failed";
	code["418"] = "I'm a teapot";
	code["421"] = "Misdirected Request";
	code["422"] = "Unprocessable Entity";
	code["423"] = "Locked";
	code["424"] = "Failed Dependency";
	code["426"] = "Upgrade Required";
	code["428"] = "Precondition Required";
	code["429"] = "Too Many Requests";
	code["431"] = "Request Header Fields Too Large";
	code["444"] = "Connection Closed Without Response";
	code["451"] = "Unavailable For Legal Reasons";
	code["499"] = "Client Closed Request";

	//5×× Server Error
	code["500"] = "Internal Server Error";
	code["501"] = "Not Implemented";
	code["502"] = "Bad Gateway";
	code["503"] = "Service Unavailable";
	code["504"] = "Gateway Timeout";
	code["505"] = "HTTP Version Not Supported";
	code["506"] = "Variant Also Negotiates";
	code["507"] = "Insufficient Storage";
	code["508"] = "Loop Detected";
	code["510"] = "Not Extended";
	code["511"] = "Network Authentication Required";
	code["599"] = "Network Connect Timeout Error";

	if (code.find(statusCode) != code.end())
		return code[statusCode];
	return ("");
}

StatusCode::StatusCode()
{

}

StatusCode::~StatusCode()
{

}