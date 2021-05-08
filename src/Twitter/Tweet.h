#pragma once

#include <string>

class Tweet
{
public:
	Tweet();
	Tweet(std::string tweet, std::string twitter, std::string loc);
	~Tweet();



private:
	std::string tweet;
	std::string twitter;
	std::string loc;
};