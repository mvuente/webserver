#ifndef HTML_TEMPLATE_HPP
#define HTML_TEMPLATE_HPP

#include <string>
#include <fstream>
#include <sstream>

class HTMLTemplate
{
	private:
		std::stringstream	_templateBody;
		bool				_readError;
	public:
		HTMLTemplate(const std::string &templateFilePath) :
			_templateBody(), _readError(false)
		{
			std::ifstream tmpl(templateFilePath.c_str());
			
			if (tmpl)
			{
				_templateBody << tmpl.rdbuf();
				tmpl.close();
			}
			else
				_readError = true;
		}

		HTMLTemplate(const HTMLTemplate &copy) :
			_templateBody(copy.getContent()), 
			_readError(copy.isReadError())
		{}

		bool		isReadError() const { return _readError; }
		
		std::string	getContent() const { return _templateBody.str(); }
		
		std::string getContentOrMessage(const std::string &msg) const
		{
			if (isReadError())
				return msg;
			else
				return _templateBody.str();
		}
};

#endif
