/*
 * ======================================================================================
 *  __    ____  __  __  ____  ___
 * (  )  (_  _)(  \/  )( ___)/ __)
 *  )(__  _)(_  )    (  )__) \__ \
 * (____)(____)(_/\/\_)(____)(___/
 *
 *  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#include "./limes_assert.h"
#include "../limes_namespace.h"
#include <limes_platform.h>
#include <iostream>
#include <sstream>
#include <atomic>
#include "../text/StringUtils.h"
#include <cstdlib>
#include <mutex>
#include "../files/file.h"
#include "./debugger.h"

LIMES_BEGIN_NAMESPACE

namespace assert
{

static files::File logFile;
static std::mutex  logFileMutex;

void setAssertionLogFile (const std::filesystem::path& logFileToUse) noexcept
{
	files::File newLogFile { logFileToUse };
	newLogFile.makeAbsoluteRelativeToCWD();

	const std::lock_guard g { logFileMutex };

	logFile = newLogFile;
}

std::filesystem::path getAssertionLogFile()
{
	{
		const std::lock_guard g { logFileMutex };

		if (logFile.isValid())
			return logFile.getAbsolutePath();
	}

	if (const auto* filename = std::getenv ("LIMES_ASSERTION_LOG_FILE"))
	{
		files::File result { filename };

		result.makeAbsoluteRelativeToCWD();

		return result.getAbsolutePath();
	}

	return {};
}

static inline void log_assertion (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept
{
	try
	{
		std::stringstream stream;

		stream << "Assertion failure in file " << fileName << ", function " << functionName << ", line " << lineNum;

		if (condition == nullptr)
			stream << " (LIMES_ASSERT_FALSE)";
		else
			stream << ": condition '" << condition << "' failed";

		auto log = stream.str();

		std::cerr << log << std::endl;

		const auto logOutput = files::File { getAssertionLogFile() };

		if (logOutput.isValid())
		{
			log += text::new_line;
			logOutput.append (log);
		}
	}
	catch (...)
	{
	}
}

void fire_assertion_internal (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept
{
	log_assertion (fileName, functionName, lineNum, condition);

	if (debugger::isRunningUnderDebugger())
		debugger::breakInDebugger();
}

}  // namespace assert

LIMES_END_NAMESPACE
