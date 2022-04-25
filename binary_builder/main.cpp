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

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include <filesystem>
#include <cassert>
#include <limes_core.h>

namespace binary_builder
{

using namespace limes::files;

struct Options final
{
	// list of files to embed
	std::vector<File> inputFiles;

	// output directory for generated files
	Directory outputDir;

	// output file names
	std::string headerFileName;
	std::string cppFileName;

	// C++ namespace to use (if any)
	std::string namespaceName;
};

static constexpr char s_defaultOutputBase[] = "BinaryData";

// Display help message
inline void displayUsage()
{
	std::cout << "Limes BinaryBuilder: generates C++ source code which embed several external (binary) files.\n";
	std::cout << "Supported options:\n";
	std::cout << " <input>	: path to an input file or directory to embed in C++ code.\n";
	std::cout << "			  If it's a directory, its content will be recursively iterated.\n";
	std::cout << "			  Note: several inputs can be passed on the command line.\n";
	std::cout << " -h		    : this help message.\n";
	std::cout << " -d <path>  : directory where to save the generated files.\n";
	std::cout << " -o <name>  : base name to be used for the generated .h/.cpp files.\n";
	std::cout << "			  => '-o generated' will produce 'generated.h' and 'generated.cpp' files.\n";
	std::cout << "			  Default value is '" << s_defaultOutputBase << "'.\n";
	std::cout << " -ns <name> : name of the namespace to be used in generated code (recommended).\n";
	std::cout << "			  Default is empty (no namespace).\n";
}

inline Options parseCommandLine (int argc, char** argv)
{
	auto parseNamedArgument = [] (const std::string& argName, const std::string& argValue, Options& options)
	{
		assert (argName.front() == '-');
		assert (! argValue.empty());

		if (argName == "-d")
		{
			Directory dir { argValue };

			if (! dir.exists())
				if (! dir.createIfDoesntExist())
					throw std::runtime_error { "Invalid output directory: " + argValue };

			options.outputDir = dir;
		}
		else if (argName == "-o")
		{
			options.headerFileName = argValue + ".h";
			options.cppFileName	   = argValue + ".cpp";
		}
		else if (argName == "-ns")
		{
			options.namespaceName = argValue;
		}
		else
		{
			throw std::runtime_error { "Invalid option name: " + argName };
		}
	};

	auto parsePositionalArgument = [] (const std::string& value, Options& options)
	{
		std::function<void (const FilesystemEntry& entry)> processEntry = [&options, &processEntry] (const FilesystemEntry& entry)
		{
			if (entry.isDirectory())
			{
				entry.getDirectoryObject()->iterateFiles ([&options] (const File& f)
														  { options.inputFiles.emplace_back (f); },
														  true);
			}
			else if (entry.isFile())
			{
				options.inputFiles.emplace_back (entry.getAbsolutePath());
			}
			else if (entry.isSymLink())
			{
				processEntry (entry.getSymLinkObject()->follow (true));
			}
			else
			{
				throw std::runtime_error { "Can't find file or directory named " + entry.getAbsolutePath().string() };
			}
		};

		processEntry (FilesystemEntry { value });
	};

	//

	Options options;

	if (argc == 1)
	{
		displayUsage();
		std::exit (EXIT_SUCCESS);
	}

	for (auto i = 1; i < argc; ++i)
	{
		const std::string arg { argv[i] };

		if (arg.front() == '-')
		{
			if (arg == "-h")
			{
				displayUsage();
				std::exit (EXIT_SUCCESS);
			}
			else if (i == argc - 1)
			{
				throw std::runtime_error { "Missing value for option " + arg };
			}
			else
			{
				parseNamedArgument (arg, argv[i + 1], options);
				i += 1;
			}
		}
		else
		{
			parsePositionalArgument (arg, options);
		}
	}

	if (options.cppFileName.empty() || options.headerFileName.empty())
	{
		options.headerFileName = std::string { s_defaultOutputBase } + ".h";
		options.cppFileName	   = std::string { s_defaultOutputBase } + ".cpp";
	}

	return options;
}

inline void generateHeaderFile (const Options& options)
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

inline void generateBodyFile (const Options& options)
{
	auto convertFileDataToCppSource = [] (const File& fileName, const std::string& fileId, std::ostream& stream)
	{
		assert (fileName.isFile() && fileName.exists());

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

		assert (char_count == fileLen);

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

int main (int argc, char** argv)
{
	try
	{
		const auto options = binary_builder::parseCommandLine (argc, argv);

		if (options.inputFiles.empty())
			std::cerr << "Warning: no input file to process, will generate empty C++ output!\n";
		else
			std::cout << "Ready to process " << options.inputFiles.size() << " file(s).\n";

		binary_builder::generateHeaderFile (options);
		binary_builder::generateBodyFile (options);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
