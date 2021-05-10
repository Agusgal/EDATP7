#pragma once

#include <string>

class Tweet
{
public:
	Tweet();
	Tweet(std::string tweet, std::string twitter, std::string tweetedAt);
	~Tweet();

	std::string getTweet();
	std::string getTwitter();
	std::string getTweetedAt();

	void setTweet(std::string tweet_);
	void setTwitter(std::string twitter_);
	void setTweetedAt(std::string tweetedAt_);

private:
	std::string tweet;
	std::string twitter;
	std::string tweetedAt;
};