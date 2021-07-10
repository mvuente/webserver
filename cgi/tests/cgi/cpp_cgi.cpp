#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

int	main (void)
{
	int			n;
	std::string body;
	
	n =0;
	printf("Script_name: %s, content_length: %s, content_type: %s\n\n", getenv("SCRIPT_NAME"),  getenv("CONTENT_LENGTH"),  getenv("CONTENT_TYPE"));
	std::cout << "Body: ";
	while (!std::cin.eof())
	{
		getline(std::cin, body);
		std::cout << body << std::endl;
		n += body.length();
	}
	std::cout << "Body size: " << n << std::endl;
}
