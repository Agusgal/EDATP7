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
	TwitterHandler(const char* account, int tweetsN);

	virtual ~TwitterHandler();

	void setAccountSource(const char* account);
	void setTweetsAmount(int tweetsN);
	// To be called after setting new account and/or tweetsN. If constructed with constructor by default, this method bust be called so that there are no errors.
	void updateURL();

	// Must be called before any other request, to get the token from Twitter.
	bool createTwitterToken();
	// To be called after creating the token.
	bool setUpTwitterConnection();
	// To be called after setting up Twitter connection.
	std::vector<Tweet> getTweetsList();


private:

	void initQuery();

	std::string APIKey;
	std::string APISecretKey;
	std::string account;

	int tweetsN;

	json TwitterAnswer;


	std::string token;
	
	std::vector<Tweet> tweetsList;

	std::string query;

};

/*
HOW TO IMPLEMENT THIS CLASS:
1.a. Construct with TweetHandler(char * account_, int tweetsN_ = 0)
  b. Construct with TweetHandler() and then call setAccountSource(char * account_) (setTweetsAmount(int tweetsN_) is optional).
	 After this, call updateURL().
2. Call isOK(). If it returns false, call getError().
3. Create token with createTwitterToken().
4. Set up Twitter connection with setUpTwitterConnection().
5. Write the following while loop:
	do
	{
		testHandler.multiPerform();
		// Here you can do whatever you want.
	} while (testHandler.isStillReceiving());
6. Get the information you want with getTweetsList().
*/