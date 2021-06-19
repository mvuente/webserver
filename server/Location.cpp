#include "Location.hpp"

	Location::Location()
	{
		allowAllMethods();
	}

	Location::~Location()
	{

	}

	
	//Getters
	std::string Location::getPath()
	{
		return (this->path);
	}

	std::string Location::getRoot()
	{
		return(this->root);
	}

	std::string Location::getIndex()
	{
		return (this->index);
	}

	int Location::getMaxBodySize()
	{
		return (this->maxBodySize);
	}

	std::string Location::getCgiExtension()
	{
		return (this->cgiExtension);
	}

	std::map<std::string, int> Location::getMethodAllowed()
	{
		return (this->methodAllowed);
	}

	std::map<int, std::string> Location::getRedirect()
	{
		return (this->redirect);
	}

	//Setters
	void Location::setPath(std::string path)
	{
		this->path = path;
	}

	void Location::setRoot(std::string root)
	{
		this->root = root;
	}

	void Location::setIndex(std::string index)
	{
		this->index = index;
	}

	void Location::setMaxBodySize(int maxBodySize)
	{
		this->maxBodySize = maxBodySize;
	}

	void Location::setCgiExtension(std::string cgiExtension)
	{
		this->cgiExtension = cgiExtension;
	}

	void Location::allowAllMethods()
	{
		this->methodAllowed["GET"] = 1;
		this->methodAllowed["POST"] = 1;
		this->methodAllowed["DELETE"] = 1;
	}

	void Location::denyAllMethods()
	{
		this->methodAllowed["GET"] = 0;
		this->methodAllowed["POST"] = 0;
		this->methodAllowed["DELETE"] = 0;
	}

	int Location::allowMethod(std::string method)
	{
		std::map<std::string, int>::iterator it = this->methodAllowed.begin();
		it = this->methodAllowed.find(method);
		if (it != this->methodAllowed.end())
			it->second = 1;
		return (0);
	}

	void Location::setAutoindex(std::string option)
	{
		this->autoindex = option;
	}

	std::string Location::getAutoindex()
	{
		return (this->autoindex);
	}

	std::string Location::getCgiPass()
	{
		return (this->cgiPass);
	}

	void Location::setCgiPass(std::string cgiPass)
	{
		this->cgiPass = cgiPass;
	}

	void Location::setRedirect(int code, std::string path)
	{
		this->redirect[code] = path;
	}

	std::string Location::getUploadStore()
	{
		return (this->uploadStore);
	}

	void Location::setUploadStore(std::string uploadStore)
	{
		this->uploadStore = uploadStore;
	}
