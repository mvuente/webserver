//
// Created by Marceline Vuente on 6/17/21.
//

#include "Dispatcher.hpp"

Dispatcher::Dispatcher(int serv_fd, struct sockaddr_in address, int addrlen) : _serv_fd(serv_fd), _address(address), _addrlen(addrlen)
{
	//важно! слушающий fd навсегда лежит в массиве fd'шников

	FD_SET(_serv_fd, &_fd_set); //сделать проверку на результат?
	_maxfd = _serv_fd; // на старте max выставляем на слушателя. Потом регулярно проверять
	_debag = 0;
}

Dispatcher::~Dispatcher() {}

void Dispatcher::run()
{
	std::set<int>	cli_rqst;
	std::set<int>	cli_rspns;
	int 			active(0); // fd который выбрал select
	cli_rqst.clear();
	cli_rspns.clear();



	while(1)
	{
		int 			active(0); // fd который выбрал select
		// Заполняем множество сокетов при каждом прохождении цикла заново, так как select множества видоизменяет
		fd_set	readset;
		fd_set	writeset;
		FD_ZERO(&readset);
		FD_ZERO(&writeset);
		timeval timeout;

		while (active == 0) // старт работы, когда никого нет в очереди
		{
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;
			memcpy(&readset, &_fd_set, sizeof(_fd_set)); //заполнил множество читающих fd (при старте там только главный слушатель)
			FD_ZERO(&writeset);
			for (std::set<int>::iterator it = cli_rspns.begin() ; it != cli_rspns.end() ; it++)
				FD_SET(*it, &writeset); // прошел по всем fd готовым на запись и положил их в массив fd на отправку (при старте здесь пусто)

			std::cout << "\rWaiting on a connection" << std::flush;

			_maxfd = std::max(_maxfd, this->mymax(cli_rspns));  // выбираем максимальный fd из имеющихся
			active = select(_maxfd + 1, &readset, &writeset, NULL, &timeout); // выбираем, есть ли fd, ожидающий реакции
			if (active < 0)
				std::cout << "Errno is " << errno << std::endl;
		}
		// отработать случай active < 0 (ошибка)
		// старт отработан, в очереди есть fd

		// 1. ПРоверяем наличие fd на отправку и выполняем их, если есть
		for (std::set<int>::iterator it = cli_rspns.begin(); it != cli_rspns.end() ; it++)
		{
			if (FD_ISSET(*it, &writeset))
			{
				long	sent = _srvm.ResponseCrtr(*it); //  ServerMachine должен сформировать и отправить ответ сервера
				std::cout << "SENT " << sent << " BYTES" << std::endl;
				// надо обработать возврат write'а предыдущей строки
				FD_CLR(*it, &_fd_set); // убрать fd из текущего набора?
				FD_CLR(*it, &writeset); // убрать fd из write набора?
				cli_rspns.erase(it); // убрать fd из общего наборра на отправку
				close(*it);
				active = 0; // может быть полезно в дальнейшем?
				break;
			}
		}

		// 2. ПРоверяем наличие fd на прием и выполняем их, если есть
		// 2.2. Если инфа по текущему fd, принять сообщение
		for (std::set<int>::iterator it = cli_rqst.begin(); it != cli_rqst.end() ; it++)
		{
			int flag = FD_ISSET(*it, &readset);
			if (flag)
			{
				Request		rqst(*it); //здесь делать класс rqst!
				//отработать возвращаемый результат кол-ва считанных байт
				Response	rspns = Response(rqst);

				this->_srvm = ServerMachine(rqst, rspns);
				cli_rspns.insert(*it);
				cli_rqst.erase(it);
				FD_CLR(*it, &_fd_set);
				FD_CLR(*it, &readset);
				it = cli_rqst.begin();
				break;
			}
		}
		// 2.1. Если сработал главгый слушатель, то запустить accept
		if (FD_ISSET(_serv_fd, &readset))
		{
			int	new_socket = accept(_serv_fd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen);
			if (new_socket != -1)
			{
				FD_SET(new_socket, &_fd_set); // сохраняет сокет для следующего цикла select
				cli_rqst.insert(new_socket); // сокет сохранен для дальнейшего чтения
				_maxfd = std::max(new_socket, _maxfd);
				_debag = new_socket;
			}
			//else
			// делать ли обработку ошибки?
		}
	}
	return ;
}

int 	Dispatcher::mymax(std::set<int> myset)
{
	if (!myset.size())
		return 0;
	return (*std::max_element(myset.begin(), myset.end()));
}
