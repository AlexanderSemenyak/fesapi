/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#include "FileCoreProperties.h"

#include <sstream>
#if defined(_WIN32)
	#include <windows.h>
#elif defined(__gnu_linux__)
	#include <unistd.h>
	#include <pwd.h>
#endif

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "../tools/date.h"

using namespace std; // in order not to prefix by "std::" for each class in the "std" namespace. Never use "using namespace" in *.h file but only in*.cpp file!!!
using namespace epc; // in order not to prefix by "epc::" for each class in the "epc" namespace. Never use "using namespace" in *.h file but only in*.cpp file!!!

CoreProperty FileCoreProperties::getProperty(CoreProperty::TypeProperty index) const
{
	return properties[index];
}

void FileCoreProperties::initDefaultCoreProperties()
{
	// CREATOR
	if (properties[CoreProperty::creator].isEmpty()) {
#if defined(__gnu_linux__) 
        struct passwd *pw;
        uid_t uid;
                
        uid = geteuid ();
        pw = getpwuid (uid);	// may rise a false positive memory leak with Valgrind
								// (https://stackoverflow.com/questions/40226297/struct-passwd-is-source-of-memory-leak-how-to-properly-free?rq=1)
        if (pw)
        	setCreator(pw->pw_name);
        else
        	setCreator("unknown");
#elif defined(_WIN32)
		char acUserName[32];
		DWORD nUserName = sizeof(acUserName);
		GetUserName(acUserName, &nUserName);
		setCreator(acUserName);
#endif
	}

	// CREATED
	if (properties[CoreProperty::created].isEmpty()) {
		auto now = std::chrono::system_clock::now();
		setCreated(date::format("%FT%TZ", date::floor<std::chrono::seconds>(now)));
	}

	// VERSION
	if (properties[CoreProperty::version].isEmpty()) {
		setVersion("1.0");
	}

	// Identifier
	if (properties[CoreProperty::identifier].isEmpty()) {
		boost::uuids::random_generator gen;
		setIdentifier("urn:uuid:" + boost::uuids::to_string(gen()));
	}
}

std::string FileCoreProperties::toString() const 
{
	// XML def + namespaces def
	ostringstream oss;
	oss << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	oss << "<coreProperties xmlns=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\"" << endl;
	oss << "xmlns:dcterms=\"http://purl.org/dc/terms/\"" << endl,
	oss << "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"" << endl,
	oss << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" << endl;

	// content
	for (unsigned int i = 0; i < CoreProperty::undefinedCoreProperty; ++i) {
		if (!properties[i].isEmpty()) {
			oss << "\t" << properties[i].toString() << endl;
		}
	}

	// end tag
	oss << "</coreProperties>" << endl;

	return oss.str();
}

void FileCoreProperties::setCategory(const std::string & value)
{
	properties[CoreProperty::category].setCategory(value);
}

void FileCoreProperties::setContentStatus(const std::string & value)
{
	properties[CoreProperty::contentStatus].setContentStatus(value);
}

void FileCoreProperties::setCreated(const std::string & value)
{
	properties[CoreProperty::created].setCreated(value);
}

void FileCoreProperties::setCreator(const std::string & value)
{
	properties[CoreProperty::creator].setCreator(value);
}

void FileCoreProperties::setDescription(const std::string & value)
{
	properties[CoreProperty::description].setDescription(value);
}

void FileCoreProperties::setIdentifier(const std::string & value)
{
	properties[CoreProperty::identifier].setIdentifier(value);
}

void FileCoreProperties::setKeywords(const std::string & value)
{
	properties[CoreProperty::keywords].setKeywords(value);
}

void FileCoreProperties::addKeywords(const std::string & value)
{
	properties[CoreProperty::keywords].addKeywords(value);
}

void FileCoreProperties::setLanguage(const std::string & value)
{
	properties[CoreProperty::language].setLanguage(value);
}

void FileCoreProperties::setLastModifiedBy(const std::string & value)
{
	properties[CoreProperty::lastModifiedBy].setLastModifiedBy(value);
}

void FileCoreProperties::setLastPrinted(const std::string & value)
{
	properties[CoreProperty::lastPrinted].setLastPrinted(value);
}

void FileCoreProperties::setModified(const std::string & value)
{
	properties[CoreProperty::modified].setModified(value);
}

void FileCoreProperties::setRevision(const std::string & value)
{
	properties[CoreProperty::revision].setRevision(value);
}

void FileCoreProperties::setSubject(const std::string & value)
{
	properties[CoreProperty::subject].setSubject(value);
}

void FileCoreProperties::setTitle(const std::string & value)
{
	properties[CoreProperty::title].setTitle(value);
}

void FileCoreProperties::setVersion(const std::string & value)
{
	properties[CoreProperty::version].setVersion(value);
}

void FileCoreProperties::readFromString(const string & textInput)
{
	for (unsigned int i = 0; i < CoreProperty::undefinedCoreProperty; ++i) {
		properties[i].setTypeProperty(CoreProperty::undefinedCoreProperty);
	}

	size_t start = textInput.find("<coreProperties ");
	if (start != string::npos) {
		start = textInput.find("<coreProperties>");
	}
	size_t end = textInput.find("</coreProperties>", start);
		
	//identifier
	size_t attStart = textInput.find("identifier", start);
	if (attStart != string::npos && attStart < end) {
		attStart += 11;
		size_t attEnd = textInput.find("</", attStart);
		if (attStart != string::npos && attEnd != string::npos) {
			properties[CoreProperty::identifier].setIdentifier(textInput.substr(attStart, attEnd - attStart));
		}
	}
}
