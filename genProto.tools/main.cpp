﻿/*
 * genProto License
 * -----------
 * 
 * genProto is licensed under the terms of the MIT license reproduced below.
 * This means that genProto is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 * 
 * 
 * ===============================================================================
 * 
 * Copyright (C) 2014-2015 YaweiZhang <yawei.zhang@foxmail.com>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ===============================================================================
 * 
 * (end of COPYRIGHT)
 */



#include "utility.h"
#include "log4z.h"
#include "tinyxml2.h"
#include "genProto.h"

using namespace zsummer::utility;

int main(int argc, char *argv[])
{
	zsummer::log4z::ILog4zManager::getRef().setLoggerFileLine(LOG4Z_MAIN_LOGGER_ID, false);
	zsummer::log4z::ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID, false);
	zsummer::log4z::ILog4zManager::getRef().start();
	std::vector<_FileInfo> files;
	if (!searchFiles("./", files))
	{
		LOGE("searchFiles error.");
		return 0;
	}

	for (auto & file : files)
	{
		std::string filename = file.filename;
		if (filename.size() <= 4)
		{
			continue;
		}
		std::string xmlattr = filename.substr(filename.length() - 4, 4);
		if (xmlattr != ".xml")
		{
			continue;
		}
		filename = filename.substr(0, filename.size() - 4);
		
		genProto gen(filename);
		ParseCode pc = gen.parseCache();
		if (pc == PC_ERROR)
		{
			LOGE("LoadCache [" << filename << "] Error.");
			break;
		}
		pc = gen.parseConfig();
		if (pc == PC_ERROR)
		{
			LOGE("parseConfig [" << filename << "] Error.");
			break;
		}
		pc = gen.genCode();
		if (pc == PC_ERROR)
		{
			LOGE("genProto [" << filename << "] Error.");
			break;
		}
		else if (pc == PC_NEEDSKIP)
		{
			LOGI("SKIP [" << filename << "] .");
			zsummer::utility::SleepMillisecond(120);

			continue;
		}
		else
		{
			pc = gen.writeCache();
			if (pc == PC_ERROR)
			{
				LOGE("Cached [" << filename << "] Error.");
				break;
			}
			LOGI("Cached [" << filename << "] Success.");
		}
	}
	
	LOGA("All Success..");
	getchar();

	return 0;
}






