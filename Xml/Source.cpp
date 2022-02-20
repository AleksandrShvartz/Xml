#include "Parser.h"

int main()
{
	try
	{
		Parser xml("data.txt");

		xml.parse();

		xml.addNewNode("newNode", "name1", std::vector<std::string>{("asd")}, "data");

		xml.printInConsole();

		xml.closeStream();
	}
	catch (const std::runtime_error& ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	return 0;
}