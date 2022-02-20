#include "Parser.h"

#include <regex>
#include <sstream>
#include <stack>

Parser::Parser(std::string const& fileName)
{
	in.open(fileName);

	if (!in.is_open())
		throw std::runtime_error("File not opened");

	root = new Node();
}

Parser::Parser()
{
	root = nullptr;
}

Parser::~Parser()
{
	in.close();
	clear();
}

void Parser::clear()
{
	std::stack<Node*> nodesStack;

	nodesStack.push(root);

	while (!nodesStack.empty())
	{
		Node* node = nodesStack.top();
		nodesStack.pop();

		for (Node* n : node->children)
		{
			nodesStack.push(n);
		}

		delete node;
	}
}

void Parser::parse()
{
	const std::regex rOpen("\\s*<\\s*" + tagName + "\\s*(\\w+)\\s*(.*)\\s*>\\s*");
	const std::regex rClose("\\s*<\\s*\/\\s*" + tagName + "\\s*>\\s*");
	const std::regex rEmptyStr("\\s*");
	const std::regex rText(".*");

	size_t numNotClosedTag = 0;

	Node* curNode = root;

	while (in)
	{
		std::string str = getNextLine();

		std::cmatch matchs;
		std::string name;
		if (std::regex_match(str.c_str(), matchs, rOpen))
		{
			name = matchs[1];
			numNotClosedTag++;

			std::stringstream ss;
			ss << matchs[2];

			std::vector<std::string> atributes;
			std::string atribute;
			while (ss >> atribute)
			{
				atributes.push_back(atribute);
			}

			curNode = addNode(name, atributes, "", curNode);
		}
		else if (std::regex_match(str, rClose))
		{
			numNotClosedTag--;
			if (numNotClosedTag >= 0)
				curNode = curNode->parent;
			else
				throw std::runtime_error("Too many closer tags");
		}
		else if (std::regex_match(str.c_str(), matchs, rText))
		{
			curNode->text = matchs[0];
		}
		else if (std::regex_match(str, rEmptyStr))
		{
			continue;
		}
		else
		{
			throw std::runtime_error("Not true tag");
		}

	}
}

Parser::Node* Parser::find(std::string name)
{
	std::stack<Node*> nodesStack;

	nodesStack.push(root);

	while (!nodesStack.empty())
	{
		Node* node = nodesStack.top();
		nodesStack.pop();

		for (Node* n : node->children)
		{
			nodesStack.push(n);
		}

		if (node->name == name)
			return node;
	}

	return nullptr;
}

void Parser::addNewNode(std::string nodeName, std::string name, std::vector<std::string> atributes, std::string text)
{
	Node* node = find(name);

	addNode(nodeName, atributes, text, node);

	return;
}

void Parser::print(Node* node, int numSpaces)
{
	if (node == nullptr)
		return;

	for (int i = 0; i < numSpaces; i++)
		std::cout << " ";

	for (const auto& n : node->children)
	{
		std::cout << "<" << tagName << " " << n->name;
		if (n->atributes.size() != 0)
		{
			for (auto const& atribute : n->atributes)
				std::cout << " " << atribute;
		}
		std::cout << ">" << std::endl;
		if (n->text != "")
			std::cout << " " << n->text << std::endl;
		print(n, numSpaces + 2);
		std::cout << "</" << tagName << ">" << std::endl;
	}
}

void Parser::printInConsole()
{
	print(root, 0);
}

void Parser::openStream(std::string fileName)
{
	if (!in.is_open())
		in.open(fileName);
	else
		throw std::runtime_error("File not opened, because other open");
}

void Parser::closeStream()
{
	in.close();
}

std::string Parser::getNextLine()
{
	std::string line;
	std::getline(in, line);

	return line;
}

Parser::Node* Parser::addNode(std::string name, std::vector<std::string> atributes, std::string text, Node* dest)
{
	Node* newNode = new Node();

	newNode->name = name;
	newNode->parent = dest;
	newNode->atributes = atributes;
	newNode->text = text;

	dest->children.push_back(newNode);

	return newNode;
}
