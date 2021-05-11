#pragma once

#include <string>

class Tweet
{
public:
	Tweet(std::string username, std::string content, std::string date);
	~Tweet();

	std::string getUser();
	std::string getContent();
	std::string getDate();

	//orinting for debugging
	friend std::ostream& operator <<(std::ostream& o, const Tweet& tw);

private:
	std::string username;
	std::string content;
	std::string date;

	void ParseCharacters();
};