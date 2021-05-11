#include "Tweet.h"

#include "boost/algorithm/string/replace.hpp"

const char* monthToNumber(const std::string& month);

using namespace std;

Tweet::Tweet(string username, string content, string date)
{
	this->username = username;
	
	string month = monthToNumber(date.substr(4, 3));
	string day = date.substr(8, 2);
	string year = date.substr(date.length() - 2, 2);
	string time = date.substr(11, 5);

	this->date = day + '/' + month + '/' + year + " - " + time;

	this->content = this->username + " - " + content.substr(0, content.find("http"));


	ParseCharacters();
}


Tweet::~Tweet()
{
}

std::string Tweet::getUser()
{
	return this->username;
}

std::string Tweet::getDate()
{
	return this->date;
}

std::string Tweet::getContent()
{
	return this->content;
}

//Tweet printing.
std::ostream& operator<< (std::ostream& o, const Tweet& tw) 
{
	o << tw.username << std::endl;
	int pos = tw.content.find("http");
	if (pos != std::string::npos)
		o << tw.content.substr(0, pos) << std::endl;
	else
		o << tw.content << std::endl;
	o << tw.date << std::endl;
	return o;
}



const char* monthToNumber(const std::string& month) 
{
	if (month == "Jan")
		return "01";
	if (month == "Feb")
		return "02";
	if (month == "Mar")
		return "03";
	if (month == "Apr")
		return "04";
	if (month == "May")
		return "05";
	if (month == "Jun")
		return "06";
	if (month == "Jul")
		return "07";
	if (month == "Aug")
		return "08";
	if (month == "Sep")
		return "09";
	if (month == "Oct")
		return "10";
	if (month == "Nov")
		return "11";
	if (month == "Dic")
		return "12";
	else
		return "00";
}


void Tweet::ParseCharacters(void)
{
	boost::replace_all(content, "á", "a");
	boost::replace_all(content, "é", "e");
	boost::replace_all(content, "í", "i");
	boost::replace_all(content, "ó", "o");
	boost::replace_all(content, "ú", "u");
	boost::replace_all(content, "Á", "A");
	boost::replace_all(content, "É", "E");
	boost::replace_all(content, "Í", "I");
	boost::replace_all(content, "Ó", "O");
	boost::replace_all(content, "Ú", "U");
	boost::replace_all(content, "ñ", "n");
	boost::replace_all(content, "Ñ", "N");
	boost::replace_all(content, "¿", "?");
	boost::replace_all(content, "¡", "!");
	boost::replace_all(content, "@", "_");
}