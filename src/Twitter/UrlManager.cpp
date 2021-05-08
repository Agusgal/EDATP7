#include "urlManager.h"


// Callback function to register curl's response.
static size_t curlCallback(void* contents, size_t size, size_t nmemb, void* userp);

UrlManager::UrlManager()
{
	this->stillReceiving = false;
	this->response = "";
	//errorhandler
}


UrlManager::UrlManager(const char* url)
{
	setURL(url);
	this->response = "";
	this->stillReceiving = false;
}


void UrlManager::setURL(const char* url)
{
	this->url = url;
	//no errors
}


char* UrlManager::getResponse()
{
	return (char*)this->response.c_str();
}


bool UrlManager::easyPerform()
{
	curl = curl_easy_init();
	response = "";

	if (curl)
	{
		return basicEasyPerform();
	}
	else
	{
		//error handling
		curl_easy_cleanup(curl);
		return false;
	}
}


bool UrlManager::easyPerform(const char* password)
{
	curl = curl_easy_init();
	response = "";

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERPWD, password);

		struct curl_slist* list = NULL;
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		std::string data = "grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		return basicEasyPerform();
	}
	else
	{
		//error handling
		return false;
	}
}


bool UrlManager::setUpMultiPerform()
{
	curl = curl_easy_init();
	multiHandler = curl_multi_init();
	response = "";
	stillReceiving = false;


	if (multiHandler != NULL && curl != NULL)
	{
		return basicSetUpMultiPerform();
	}
	else
	{
		//error handling
		return false;
	}
}


bool UrlManager::setUpMultiPerform(const char* header)
{
	curl = curl_easy_init();
	multiHandler = curl_multi_init();
	response = "";
	stillReceiving = false;

	if (multiHandler != NULL && curl != NULL)
	{
		struct curl_slist* list = NULL;
		list = curl_slist_append(list, header);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		return basicSetUpMultiPerform();
	}
	else
	{
		//Error handling
		curl_easy_cleanup(curl);
		return false;
	}


}

bool UrlManager::multiPerform()
{
	multiR = curl_multi_perform(multiHandler, &stillReceiving);
	if (multiR != CURLE_OK)
	{
		//Error hanlding
		curl_easy_cleanup(curl);
		return false;
	}
	else
	{
		if (!stillReceiving)
		{
			curl_easy_cleanup(curl);
			curl_multi_cleanup(multiHandler);
		}
		return true;
	}
}


bool UrlManager::basicEasyPerform()
{
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);									// Telling CURL to follow redirection in case page we visit redirects us somewhere else.
	curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);					// Specifying callback function to be called when curl has to write the response.
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	easyR = curl_easy_perform(curl);

	if (easyR != CURLE_OK)
	{
		//Error handling 
		curl_easy_cleanup(curl);
		return false;
	}

	curl_easy_cleanup(curl);
	return true;
}


bool UrlManager::basicSetUpMultiPerform()
{
	curl_multi_add_handle(multiHandler, curl);

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);


	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);						// Specifying callback function to be called when curl has to write the response.
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	return true;
}


bool UrlManager::isStillReceiving()
{
	return stillReceiving;
}

static size_t curlCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;
}