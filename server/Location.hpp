#include <iostream>
#include <list>
#include <map>

class Location
{
private:
	std::string path;
	std::string root;
	std::string index;
	std::string autoindex;
	std::string uploadStore;
	int maxBodySize;
	std::string cgiExtension;
	std::string cgiPass;
	std::map<std::string, int> methodAllowed;
	std::map<int, std::string> redirect;



public:
	Location(/* args */);
	~Location();

	//Getters
	std::string getPath();
	std::string getUploadStore();
	std::string getRoot();
	std::string getIndex();
	int getMaxBodySize();
	std::string getAutoindex();
	std::string getCgiExtension();
	std::string getCgiPass();
	std::map<std::string, int> getMethodAllowed();
	std::map<int, std::string> getRedirect();
	void allowAllMethods();

	//Setters
	void setPath(std::string path);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void setAutoindex(std::string option);
	void setCgiExtension(std::string cgiextension);
	void setCgiPass(std::string cgiPass);
	void setMaxBodySize(int maxBodySize);
	void setUploadStore(std::string uploadStore);
	void setRedirect(int code, std::string path);
	int allowMethod(std::string method);
	void denyAllMethods();
};

