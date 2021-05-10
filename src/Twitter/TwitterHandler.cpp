#include "TwitterHandler.h"


//Change keys
#define API_KEY "HCB39Q15wIoH61KIkY5faRDf6"
#define API_SECRET_KEY "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6"


TwitterHandler::TwitterHandler() : tweetsList(), TwitterAnswer()
{
	this->APIKey = API_KEY;
	this->APISecretKey = API_SECRET_KEY;
	setTweetsAmount(0);
}

TwitterHandler::TwitterHandler(const char* account, int tweetsN) : tweetsList(), TwitterAnswer()
{
	this->APIKey = API_KEY;
	this->APISecretKey = API_SECRET_KEY;
	setAccountSource(account);
	setTweetsAmount(tweetsN);

	updateURL();
}


TwitterHandler::~TwitterHandler()
{
}

void TwitterHandler::setAccountSource(const char* account)
{
	this->account = account;
}

void TwitterHandler::setTweetsAmount(int tweetsN)
{
	this->tweetsN = tweetsN;
}

bool TwitterHandler::createTwitterToken()
{
	setURL("https://api.twitter.com/oauth2/token");
	std::string password = APIKey + ":" + APISecretKey;
	bool ret = easyPerform(password.c_str());		// Executing an easyPerform to get the token using a password.

	TwitterAnswer = json::parse(response);
	try
	{
		std::string aux = TwitterAnswer["access_token"];
		token = aux;
	}
	catch (std::exception& e)
	{
		//Error handling
		//err.setErrType(ErrType::CHILD_ERROR);
		//err.setErrDetail(string("Exception raised while parsing Twitter's JSON.") + e.what());
		return false;
	}

	updateURL();
	return ret;
}

void TwitterHandler::updateURL()
{
	initQuery();
	setURL(query.c_str());						// Setting URL with parent method.
}

bool TwitterHandler::setUpTwitterConnection()
{
	if (token.empty())
	{
		return false;						// If the token was not initialised, an empty vector is returned.
	}
	else
	{
		std::string aux = "Authorization: Bearer ";
		aux += token;
		setUpMultiPerform(aux.c_str());				// Setting up multi perform with the authentication header as specified by Twitter, using the token.
		return true;
	}
}

std::vector<Tweet> TwitterHandler::getTweetsList()
{
	TwitterAnswer = json::parse(response);				// Parsing Twitter's response as a JSON object.
	try
	{
		for (auto tweetElement : TwitterAnswer)
		{
			std::string tweet = tweetElement["text"];
			int extended = tweet.find("https");
			tweet = tweet.substr(0, extended);			// The URL to continue reading the tweet gets eliminated.

			std::string tweetedAt = tweetElement["created_at"];
			std::string twitter = tweetElement["user"]["name"];

			tweetsList.push_back(Tweet(tweet, twitter, tweetedAt));
		}
		return tweetsList;
	}
	catch (std::exception& e)
	{
		//Error handling
		//err.setErrType(ErrType::CHILD_ERROR);
		//err.setErrDetail("User Not found.");
		return std::vector<Tweet>();
	}
}

void TwitterHandler::initQuery()
{
	query = std::string("https://api.twitter.com/1.1/statuses/user_timeline.json") + "?" + "screen_name=" + account;

	if (tweetsN != 0)
	{
		query += std::string("&") + "count=" + std::to_string(tweetsN);
	}
}