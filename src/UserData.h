#pragma once

#include <vector>
#include <string>


class UserData
{
public:
	UserData();
	~UserData();

	void setuser(std::string);
	void setTwittN(int n);
	void setOpen(void);
	void setClosed(void);
	void setId(std::string);

	std::string& getUser(void);
	int& getTwittN(void);
	std::string& getId(void);
	bool& getOpenFlag(void);

	std::string getIdNR(void);
	std::string getUserNR(void);
	int getTwittNR(void);

	bool isOpen(void);

private:

	std::string twitterUser;
	int twittN;

	bool displayIsOpen;
	std::string displayId;
};