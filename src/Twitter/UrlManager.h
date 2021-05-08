#pragma once

#include <string>
#include <curl/curl.h>

class UrlManager
{
public:

	UrlManager();
	UrlManager(const char* url);
	~UrlManager() {}


	void setURL(const char* url);

	char* getResponse();

	// This is a blocking perform.
	bool easyPerform();

	// For cases when password is needed, like asking for a token. This is a blocking perform
	bool easyPerform(const char* password);

	// This is for the set up of the non blocking perform.
	bool setUpMultiPerform();

	// For cases when a header is needed. This is for the set up of the non blocking perform.
	bool setUpMultiPerform(const char* header);

	// Non blocking perform. Should be called in a while look while asking isStillReceiving().
	bool multiPerform();

	// To be asked in a while loop while multi performing.
	bool isStillReceiving();

	bool isOk();

	//urlError getError()

protected:

	//Called on easy and multi perform, contains configurations and options. 
	bool basicEasyPerform();

	bool basicSetUpMultiPerform();


	//urlError ????

	std::string url;

	CURLcode easyR;

	CURLMcode multiR;

	CURL* curl;

	CURLM* multiHandler;

	std::string response;

	int stillReceiving;

};
