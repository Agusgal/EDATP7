#include "Tweet.h"



Tweet::Tweet()
{
}

Tweet::Tweet(std::string tweet_, std::string twitter_, std::string tweetedAt_)
{
	setTweet(tweet_);
	setTwitter(twitter_);
	setTweetedAt(tweetedAt_);
}


Tweet::~Tweet()
{
}

std::string Tweet::getTweet()
{
	return tweet;
}

std::string Tweet::getTwitter()
{
	return twitter;
}

std::string Tweet::getTweetedAt()
{
	return tweetedAt;
}

void Tweet::setTweet(std::string tweet_)
{
	tweet = tweet_;
}

void Tweet::setTwitter(std::string twitter_)
{
	twitter = twitter_;
}

void Tweet::setTweetedAt(std::string tweetedAt_)
{
	tweetedAt = tweetedAt_;
}