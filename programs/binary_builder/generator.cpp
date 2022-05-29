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
	stream << "#include <string>\n";
	stream << "#include <string_view>\n";
	stream << "#include <limes_core.h>\n";
	stream << "\n";

	if (! options.namespaceName.empty())
		stream << "namespace " << options.namespaceName << "\n{\n";

	static constexpr char s_headerContent[] = R"raw(
struct FileInfo final
{
	const char* const fileName;
	const char* const fileData;

	const std::size_t fileDataSize;

	[[nodiscard]] std::string name() const;

	[[nodiscard]] std::string string() const;

	[[nodiscard]] limes::memory::RawData data() const;
};

extern const std::size_t fileInfoListSize;

extern const FileInfo fileInfoList[];

struct FileInfoList final
{
	[[nodiscard]] const FileInfo* begin() const noexcept;

	[[nodiscard]] const FileInfo* end() const noexcept;

	[[nodiscard]] const std::size_t size() const noexcept;

	[[nodiscard]] const FileInfo* findFile (const std::string_view& fileName) const noexcept;
};

[[nodiscard]] FileInfoList fileList() noexcept;

[[nodiscard]] limes::memory::RawData loadFile (const std::string_view& fileName);
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
		{
			throw std::runtime_error { std::string ("Failed to open file ") + fileName.getAbsolutePath().string() };
		}

		limes::misc::ScopedStreamFlags flags { stream };

		const auto fileLen = static_cast<unsigned int> (fileName.sizeInBytes());

		stream << "\tconst char * " << fileId << "_name = \"" << fileName.getFilename (true) << "\";\n";
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
	stream << "#include <string>\n";
	stream << "#include <string_view>\n";
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

	stream << "\tconst std::size_t fileInfoListSize = " << fileIds.size() << ";\n";
	stream << "\tconst FileInfo fileInfoList[fileInfoListSize] = {\n";

	for (const auto& id : fileIds)
	{
		stream << "\t\t{ " << id << "_name, reinterpret_cast<const char*>(" << id << "_data), " << id << "_data_size },\n";
	}

	stream << "\t};\n";

	stream << R"raw(
	std::string FileInfo::name() const
	{
		return { fileName };
	}

	std::string FileInfo::string() const
	{
		return std::string { fileData, fileDataSize };
	}

	limes::memory::RawData FileInfo::data() const
	{
		return limes::memory::RawData { string() };
	}

	const FileInfo* FileInfoList::begin() const noexcept
	{
		return &fileInfoList[0];
	}

	const FileInfo* FileInfoList::end() const noexcept
	{
		return begin() + fileInfoListSize;
	}

	const std::size_t FileInfoList::size() const noexcept
	{
		return fileInfoListSize;
	}

	const FileInfo* FileInfoList::findFile (const std::string_view& fileName) const noexcept
	{
		for (const auto& info : fileInfoList)
			if (info.fileName == fileName)
				return &info;

		return nullptr;
	}

	FileInfoList fileList() noexcept
	{
		return FileInfoList{};
	}

	limes::memory::RawData loadFile (const std::string_view& fileName)
	{
		if (const auto* file = fileList().findFile(fileName))
			return file->data();

		return {};
	}
	)raw";

	if (! options.namespaceName.empty())
	{
		stream << "}\n";
	}
}

}  // namespace binary_builder
