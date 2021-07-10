#include "Client.hpp"

Client::Client(int fd, const Server &server_info) :
	_fd(fd),
	_rest_of_resp(),
	_request(),
	_server_info(server_info),
	_keep_alive(false)
{

}

void Client::_setNewResponse()
{
	ServerMachine sm(_request, _server_info);
	Response resp = sm.ResponseCrtr();
	
	_rest_of_resp = resp.getRSP_MSG();
	_keep_alive = resp.getConnection() == "keep-alive";
}

bool Client::_sendResponse()
{
	static Sender sender;
	const ssize_t n = sender.send(getFd(), _rest_of_resp);
	
	if (n > 0)
	{
		_rest_of_resp = _rest_of_resp.substr(n);
		return true;
	}
	Logger::println("[ FD:", getFd(), " ]");
	Logger::println("send", n, "bytes");

	return false;
}

bool Client::tryToRecvRequest()
{
	static Reader reader;
	const std::string rawData = reader.read(getFd());

	if (reader.success())
	{
		_request.addNewChunk(rawData);
		if (_request.completed())
		{
			if (_request.ignoreUntilNext() && _keep_alive)
				_clearOldRequest();
			else
				_request.parse();
		}
	}
	return reader.eof();
}

bool Client::tryToSendResponse()
{
	if (_request.ready())
	{
		if (_responseIsFullySent())
			_setNewResponse();

		if (_sendResponse() && _responseIsFullySent())
		{
			_clearOldRequest();
			return !_keepConnectionAlive();
		}
	}
	return false;
}