#pragma once

#include <string>
#include <curl/curl.h>

class Url
{
public:

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
	

private:

	bool stillReceiving; 

};