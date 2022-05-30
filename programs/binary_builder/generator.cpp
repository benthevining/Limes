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

#include "generator.h"
#include "commandLine.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include <limes_core.h>

namespace binary_builder
{

void generateHeaderFile (const Options& options)
{
	const auto fileName = [&options]() -> Path
	{
		if (! options.outputDir.isValid())
			return options.headerFileName;

		return options.outputDir.getAbsolutePath() / options.headerFileName;
	}();

	std::cout << "Generating " << fileName.generic_string() << "...\n";
	std::ofstream stream { fileName };

	if (! stream)
		throw std::runtime_error { "Failed to create header file!" };

	stream << "#pragma once\n";
	stream << "\n";
	stream << "#include <limes_core.h>\n";
	stream << "\n";

	if (! options.namespaceName.empty())
		stream << "namespace " << options.namespaceName << "\n{\n";

	static constexpr char s_headerContent[] = R"raw(
extern const std::size_t fileInfoListSize;

extern const ::limes::binaries::FileInfo fileInfoList[];

[[nodiscard]] constexpr ::limes::binaries::FileInfoList fileList() noexcept
{
	return ::limes::binaries::FileInfoList { &fileInfoList[0], fileInfoListSize };
}
)raw";

	stream << s_headerContent;

	if (! options.namespaceName.empty())
		stream << "\n} // namespace " << options.namespaceName << "\n";
}


void generateBodyFile (const Options& options)
{
	auto convertFileDataToCppSource = [] (const File& fileName, const std::string& fileId, std::ostream& stream)
	{
		LIMES_ASSERT (fileName.isFile() && fileName.exists());

		std::ifstream inputFile { fileName.getAbsolutePath(), std::ios_base::in | std::ios_base::binary };

		if (! inputFile)
			throw std::runtime_error { std::string ("Failed to open file ") + fileName.getAbsolutePath().string() };

		limes::misc::ScopedStreamFlags flags { stream };

		const auto fileLen = static_cast<unsigned int> (fileName.sizeInBytes());

		stream << "\tconst char * " << fileId << "_name = \"" << fileName.getFilename (false) << "\";\n";
		stream << "\tconst char * " << fileId << "_xtn = \"" << fileName.getFileExtension() << "\";\n";
		stream << "\tconst std::size_t " << fileId << "_data_size = " << fileLen << ";\n";
		stream << "\tconst unsigned char " << fileId << "_data[" << fileId << "_data_size] = {";

		size_t char_count { 0 };
		char   c;
		while (inputFile.get (c))
		{
			if (char_count % 20 == 0)
				stream << "\n\t\t";

			++char_count;

			stream << "0x" << std::hex << (static_cast<int> (c) & 0xFF) << ",";
		}

		LIMES_ASSERT (char_count == fileLen);

		stream << "\n\t};\n";
	};

	//

	const auto fileName = [&options]() -> Path
	{
		if (! options.outputDir.isValid())
			return options.cppFileName;

		return options.outputDir.getAbsolutePath() / options.cppFileName;
	}();

	std::cout << "Generating " << fileName.generic_string() << "...\n";

	std::ofstream stream { fileName };

	if (! stream)
		throw std::runtime_error { "Failed to create cpp file!" };

	stream << "#include \"" << options.headerFileName << "\"\n";
	stream << "#include <limes_core.h>\n";
	stream << "\n";

	stream << "namespace /* anonymous */ {\n";

	std::vector<std::string> fileIds;

	for (const auto& path : options.inputFiles)
	{
		const std::string fileId = "file" + std::to_string (fileIds.size());

		std::cout << "  " << path.getAbsolutePath().string() << "\n";
		convertFileDataToCppSource (path, fileId, stream);
		fileIds.emplace_back (fileId);
	}

	stream << "} // anonymous namespace\n";
	stream << "\n";

	if (! options.namespaceName.empty())
		stream << "namespace " << options.namespaceName << " {\n";

	stream << "\tconst std::size_t fileInfoListSize = " << fileIds.size() << ";\n";
	stream << "\tconst FileInfo fileInfoList[fileInfoListSize] = {\n";

	for (const auto& id : fileIds)
		stream << "\t\t{ " << id << "_name, " << id << "_xtn, reinterpret_cast<const char*>(" << id << "_data), " << id << "_data_size },\n";

	stream << "\t};\n";

	if (! options.namespaceName.empty())
		stream << "}\n";
}

}  // namespace binary_builder
