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

	if (stream)
	{
		stream << "#pragma once\n";
		stream << "\n";
		stream << "#include <string>\n";
		stream << "\n";

		if (! options.namespaceName.empty())
		{
			stream << "namespace\n"
				   << options.namespaceName << " {\n";
			stream << "\n";
		}

		static constexpr char s_headerContent[] = R"raw(
	struct FileInfo final
	{
		const char * const fileName;
		const char * const fileData;

		const unsigned int fileDataSize;

		[[nodiscard]] std::string name() const
		{
			return { fileName };
		}

		[[nodiscard]] std::string content() const
		{
			return std::string { fileData, fileDataSize };
		}
	};

	extern const unsigned int fileInfoListSize;

	extern const FileInfo fileInfoList[];

	struct FileInfoRange final
	{
		[[nodiscard]] const FileInfo * begin() const
		{
			return &fileInfoList[0];
		}

		[[nodiscard]] const FileInfo * end() const
		{
			return begin() + fileInfoListSize;
		}

		[[nodiscard]] const std::size_t size() const
		{
			return fileInfoListSize;
		}
	};

	[[nodiscard]] inline FileInfoRange fileList()
	{
		return FileInfoRange{};
	}
)raw";

		stream << s_headerContent;

		if (! options.namespaceName.empty())
		{
			stream << "\n";
			stream << "}\n";
		}
	}
	else
	{
		throw std::runtime_error { "Failed to create header file!" };
	}
}


void generateBodyFile (const Options& options)
{
	auto convertFileDataToCppSource = [] (const File& fileName, const std::string& fileId, std::ostream& stream)
	{
		LIMES_ASSERT (fileName.isFile() && fileName.exists());

		std::ifstream inputFile { fileName.getAbsolutePath(), std::ios_base::in | std::ios_base::binary };

		if (! inputFile)
		{
			throw std::runtime_error { std::string ("Failed to open file ") + fileName.getAbsolutePath().string() };
		}

		limes::ScopedStreamFlags flags { stream };

		const auto fileLen = static_cast<unsigned int> (fileName.sizeInBytes());

		stream << "\tconst char * " << fileId << "_name = \"" << fileName.getFilename() << "\";\n";
		stream << "\tconst unsigned int " << fileId << "_data_size = " << fileLen << ";\n";
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

	if (stream)
	{
		stream << "#include \"" << options.headerFileName << "\"\n";
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

		stream << "}\n";
		stream << "\n";

		if (! options.namespaceName.empty())
		{
			stream << "namespace " << options.namespaceName << " {\n";
		}

		stream << "\tconst unsigned int fileInfoListSize = " << fileIds.size() << ";\n";
		stream << "\tconst FileInfo fileInfoList[fileInfoListSize] = {\n";

		for (const auto& id : fileIds)
		{
			stream << "\t\t{ " << id << "_name, reinterpret_cast<const char*>(" << id << "_data), " << id << "_data_size },\n";
		}

		stream << "\t};\n";

		if (! options.namespaceName.empty())
		{
			stream << "}\n";
		}
	}
	else
	{
		throw std::runtime_error { "Failed to create cpp file!" };
	}
}

}  // namespace binary_builder
