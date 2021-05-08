#pragma once

#include "UrlManager.h"
#include "json.hpp"
#include <vector>
#include "Tweet.h"


using json = nlohmann::json;

class TwitterHandler : public UrlManager
{
public:

	TwitterHandler();
	TwitterHandler();

	virtual ~TwitterHandler();

private:

	void initQuery();

	std::string APIkey;
	std::string APISecretkey;
	std::string account;

	int tweetN;

	json TwitterAnswer;


	std::string token;
	
	std::vector<Tweet>tweetlist;

	std::string query;

};