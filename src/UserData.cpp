#include "UserData.h"


UserData::UserData()
{
	this->twitterUser = "";
	this->twittN = 0;

	this->displayId = "";
	this->displayIsOpen = false;
}

UserData::~UserData()
{

}


void UserData::setuser(std::string usr)
{
	twitterUser = usr;
}

void UserData::setTwittN(int n)
{
	twittN = n;
}

void UserData::setId(std::string id)
{
	displayId = id;
}

void UserData::setOpen(void)
{
	displayIsOpen = true;
}

void UserData::setClosed(void)
{
	displayIsOpen = false;
}

std::string& UserData::getUser(void)
{
	return twitterUser;
}

int& UserData::getTwittN(void)
{
	return twittN;
}

bool UserData::isOpen(void)
{
	return displayIsOpen;
}

std::string& UserData::getId(void)
{
	return displayId;
}

bool& UserData::getOpenFlag(void)
{
	return displayIsOpen;
}


std::string UserData::getIdNR(void)
{
	return displayId;
}

std::string UserData::getUserNR(void)
{
	return this->twitterUser;
}

int UserData::getTwittNR(void)
{
	return this->twittN;
}

