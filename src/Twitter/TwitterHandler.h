#pragma once

#include "curl/curl.h" 
#include "json.hpp"
#include <vector>
#include "Tweet.h"


class TwitterHandler 
{
public:
	TwitterHandler(const std::string& username_, const int& tweetCount_ = NULL);	
	TwitterHandler();

	~TwitterHandler();

	void init(void);
	bool requestTweet(void);

	std::vector<Tweet>& getTweets(void);

	void newUsername(const char*);
	void newtweetCount(int n);


private:

	void configureTokenClient(void);
	void configureTweetClient(void);
	void loadTweetVector(const nlohmann::json&);

	CURL* handler;
	CURL *multiHandler;
	CURLcode errorEasy;
	CURLMcode errorMulti;

	std::string token;
	std::string query;
	std::string username;
	std::string answer;

	int tweetsN;
	int stillRunning;
	
	std::vector<Tweet> tweetList;

	

};

