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

#pragma once

#include <limes_namespace.h>
#include <limes_export.h>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <stdexcept>
#include <memory>

/** @file
	This file defines the ArgumentParser class.
	@ingroup misc
 */

LIMES_BEGIN_NAMESPACE

namespace misc
{

/** This class is a command-line argument parser, inspired by Python's ArgParse.
	This parser supports flags, arguments that consume single, multiple, or a variadic number of arguments,
	and subcommands that can have their own sets of argument specifications.
	@ingroup misc
 */
class LIMES_EXPORT ArgumentParser final
{
public:

	static constexpr auto VARIADIC_ARGUMENTS = static_cast<std::size_t> (-1);

	void addArgument (const std::string_view& argument,
					  const std::string_view& helpString	  = "",
					  const std::string_view& argumentID	  = "",
					  std::size_t			  numArgsConsumed = 1,
					  bool					  required		  = false);

	void addFlag (const std::string_view& flag,
				  const std::string_view& helpString = "",
				  const std::string_view& argumentID = "");

	void addSubcommand (const std::string_view& subcommand,
						std::unique_ptr<ArgumentParser>
												subcommandOptions,
						const std::string_view& helpString = "");

	[[nodiscard]] std::string getHelpString() const;

	void printHelp() const;

	class LIMES_EXPORT ParsedArguments final
	{
	public:

		bool isEmpty() const;

		bool hasSubcommand() const;

		std::string getSubcommandName() const;

		bool hasFlag (const std::string_view& flagID) const;

		bool hasValueForArgument (const std::string_view& argumentID) const;

		const std::vector<std::string>& getArgumentValue (const std::string_view& argumentID) const;

		int argc { 0 };

		char** argv { nullptr };

	private:

		std::vector<std::string> flags {};

		std::map<std::string, std::vector<std::string>> argumentValues {};

		std::string subcommandName {};

		friend class ArgumentParser;
	};

	[[nodiscard]] ParsedArguments parse (int argc, char** argv);

	struct ParseError final : public std::runtime_error
	{
		explicit ParseError (const std::string_view& message)
			: std::runtime_error (message.begin())
		{
		}
	};

private:

	struct Argument final
	{
		Argument (const std::vector<std::string>& delimiters_,
				  const std::string_view&		  help,
				  const std::string_view&		  id_,
				  std::size_t					  numArgs,
				  bool							  required_);

		std::string getHelpString() const;

		bool isFlag() const noexcept;

		bool acceptsVariadicArguments() const noexcept;

		std::vector<std::string> delimiters {};
		std::string				 helpString {};
		std::string				 id {};
		std::size_t				 argsConsumed { 1 };
		bool					 required { false };

		bool alreadyParsed { false };
	};

	struct Subcommand final
	{
		Subcommand (const std::string_view& command_,
					const std::string_view& help,
					std::unique_ptr<ArgumentParser>
						parser_);

		std::string getHelpString() const;

		std::string command {};
		std::string helpString {};

		std::unique_ptr<ArgumentParser> parser;
	};

	void validateArgument (const std::vector<std::string>& delimiters,
						   const std::string&			   newID) const;

	Argument* getArgument (const std::string_view& commandLineArg);

	Subcommand* getSubcommand (const std::string_view& commandLineArg);

	void parseArgument (Argument& argument,
						int argc, char** argv,
						ParsedArguments& parsedArgs,
						int&			 idx);

	void checkRequiredArgs();

	[[noreturn]] void throwError (int idx, const std::string_view& message);
	[[noreturn]] void throwError (const std::string_view& message);

	std::vector<Argument> arguments;

	std::vector<Subcommand> subcommands;
};

}  // namespace misc

LIMES_END_NAMESPACE
