#pragma once
#include <algorithm>
#include <cctype>
#include <iostream>
#include <locale>
#include <cassert>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

bool isRuntimeObj(string str)
{
    return ((str.find('{')) != string::npos) && 
        (str.find("object") != string::npos) && 
        (str.find("_deployed") != string::npos);
}

vector<string> removePreamble(vector<string> lines)
{
	for (size_t i = 0; i != lines.size(); ++i)	
	{
		if (lines[i].find("object") != string::npos)
		{
			return vector<string>(lines.begin() + i, lines.end());
		}
		else
		{
			continue;
		}
	}
	assert(0==1);
	return vector<string>();
}

vector<string> getRuntimeYul(vector<string> yulContents)
{
	vector<string> lines = removePreamble(yulContents);
	int start = lines[0].find("\"");
	int end = lines[0].find("\"", start + 1);
	string objectName = lines[0].substr(start, end - start);
	string deployedObjName = "object " + objectName + "_deployed";
	// replace(deployedObjName.begin(), deployedObjName.end(), '\"', '\0');
	trim_left(deployedObjName);
	trim_right(deployedObjName);

	for (size_t i = 1; i != lines.size(); ++i)
	{
		string lineCopy = lines[i];
		// replace(lineCopy.begin(), lineCopy.end(), '\"', '\0');
        trim_left(lineCopy);
        trim_right(lineCopy);
		if (isRuntimeObj(lineCopy))
		{
            vector<string> runtimeObj = vector<string>(lines.begin() + i, lines.end());
            runtimeObj.insert(runtimeObj.begin(), lines[0]);
            return runtimeObj;
		}
	}
	return lines;
}

vector<string> removeDeploymentCode(vector<string> code)
{
	vector<string> cleanedCode;
	int start;	
	for (size_t i = 1; i != code.size(); ++i)
	{
		string lineCopy = code[i];
        trim_left(lineCopy);
        trim_right(lineCopy);
		if (string(lineCopy.begin(), lineCopy.begin() + 8) == "function")
		{
			start = i;
			break;
		}
	}
	auto onlyDefinitions = vector<string>(code.begin() + start, code.end());
	onlyDefinitions.insert(onlyDefinitions.begin(), code.begin(), code.begin() + 3);
	return onlyDefinitions;
}

vector<string> splitStr(const string& str)
{
	vector<string> strings;

	string::size_type pos = 0;
	string::size_type prev = 0;
	while ((pos = str.find('\n', prev)) != string::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}

string cleanYul(string code)
{
	auto yul = splitStr(code);
	auto runtimeYul = getRuntimeYul(yul);
	auto clean = removeDeploymentCode(runtimeYul);
	string yulStr;
	auto placeHolder = "\tcode {\n\t\t//holder\n\t}\n";
	clean.insert(clean.begin()+ 1 , placeHolder);
	for (auto line : clean)
	{
		yulStr += line + "\n";
	}	
	return yulStr;
}