#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

class Parser {
private:
	std::ifstream in;
	struct Node {
		std::vector<Node*> children;
		Node* parent = nullptr;
		std::string name;
		std::vector<std::string> atributes;
		std::string text;
	};
	Node* root;

	enum TAG
	{
		open,
		close
	};

	const std::string tagName = "node";
public:
	Parser(std::string const& fileName);
	Parser();
	~Parser();

	void parse();

	void addNewNode(std::string nodeName, std::string name, std::vector<std::string> atributes, std::string text);

	Node* find(std::string name);

	void print(Node* node, int numSpaces);

	void printInConsole();

	void openStream(std::string fileName);

	void closeStream();

private:
	std::string getNextLine();
	Node* addNode(std::string name, std::vector<std::string> atributes, std::string text, Node* dest);
	void clear();
};
