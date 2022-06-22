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

#include "ArgumentParser.h"
#include <limes_namespace.h>
#include <iostream>
#include "../text/StringUtils.h"
#include "../system/limes_assert.h"
#include "Algorithm.h"
#include <sstream>

LIMES_BEGIN_NAMESPACE

namespace misc
{

void ArgumentParser::validateArgument (const std::vector<std::string>& delimiters,
									   const std::string&			   newID) const
{
#if LIMES_DEBUG
	for (const auto& arg : arguments)
	{
		// if you hit this, then you've added multiple arguments with the same ID!
		LIMES_ASSERT (arg.id != newID);

		for (const auto& delim : delimiters)
		{
			// if you hit this, then you've added multiple arguments with identical command-line delimiters!
			LIMES_ASSERT (! alg::contains (arg.delimiters, delim));
		}
	}

	for (const auto& subcommand : subcommands)
	{
		// if you hit this, you've added an argument with the same ID as a subcommand's name!
		LIMES_ASSERT (subcommand.command != newID);

		for (const auto& arg : subcommand.parser->arguments)
		{
			for (const auto& delim : delimiters)
			{
				// if you hit this, you've added an argument with identical command-line delimiters to a subcommand's arguments!
				LIMES_ASSERT (! alg::contains (arg.delimiters, delim));
			}

			// if you hit this, you've added an argument with the same ID as a sub-command's argument!
			LIMES_ASSERT (newID != arg.id);
		}
	}
#endif
}

void ArgumentParser::addArgument (const std::string_view& argument,
								  const std::string_view& helpString,
								  const std::string_view& argumentID,
								  std::size_t			  numArgsConsumed,
								  bool					  required)
{
	const auto delims = text::split (argument, "|", false);

	const auto newID = argumentID.empty() ? delims.front() : std::string { argumentID };

	LIMES_ASSERT (! newID.empty());

	validateArgument (delims, newID);

	arguments.emplace_back (delims,
							std::string { helpString },
							newID,
							numArgsConsumed,
							required);
}

void ArgumentParser::addFlag (const std::string_view& flag,
							  const std::string_view& helpString,
							  const std::string_view& argumentID)
{
	addArgument (flag,
				 helpString,
				 argumentID,
				 0,
				 false);
}

void ArgumentParser::addSubcommand (const std::string_view& subcommand,
									std::unique_ptr<ArgumentParser>
															subcommandOptions,
									const std::string_view& helpString)
{
	LIMES_ASSERT (! subcommand.empty());

	// subcommands cannot have nested subcommands
	LIMES_ASSERT (subcommandOptions->subcommands.empty());

#if LIMES_DEBUG
	for (const auto& subcmd : subcommands)
	{
		// if you hit this, you've added multiple subcommands with the same command name!
		LIMES_ASSERT (subcmd.command != subcommand);
	}

	for (const auto& arg : subcommandOptions->arguments)
		validateArgument (arg.delimiters, arg.id);
#endif

	subcommands.emplace_back (subcommand,
							  helpString,
							  std::move (subcommandOptions));
}

std::string ArgumentParser::getHelpString() const
{
	std::stringstream stream;

	if (! arguments.empty())
	{
		stream << "Options:\n\n";

		for (const auto& arg : arguments)
			stream
				<< arg.getHelpString() << ' ';

		stream << '\n';
	}

	if (! subcommands.empty())
	{
		stream << "Subcommands:\n\n";

		for (const auto& subcommand : subcommands)
			stream
				<< subcommand.getHelpString() << '\n';
	}

	stream << '\n';

	return stream.str();
}

void ArgumentParser::printHelp() const
{
	std::cout << getHelpString() << std::endl;
}

ArgumentParser::ParsedArguments ArgumentParser::parse (int argc, char** argv)
{
	ParsedArguments parsedArgs;

	parsedArgs.argc = argc;
	parsedArgs.argv = argv;

	for (auto idx = 0; idx < argc; ++idx)
	{
		const auto inputString = std::string { argv[idx] };

		if (auto* argument = getArgument (inputString))
		{
			parseArgument (*argument, argc, argv, parsedArgs, idx);

			continue;
		}

		if (auto* subcommand = getSubcommand (inputString))
		{
			if (parsedArgs.hasSubcommand())
				throwError (idx, "Only one subcommand can be specified!");

			parsedArgs.subcommandName = inputString;

			for (auto& arg : subcommand->parser->arguments)
				parseArgument (arg, argc, argv, parsedArgs, idx);

			subcommand->parser->checkRequiredArgs();

			continue;
		}

		throwError (idx, "Unknown argument '" + inputString + '\'');
	}

	checkRequiredArgs();

	return parsedArgs;
}

void ArgumentParser::parseArgument (Argument& argument,
									int argc, char** argv,
									ParsedArguments& parsedArgs,
									int&			 idx)
{
	if (argument.isFlag())
	{
		if (argument.alreadyParsed)
			throwError (idx, "Flag repeated: '" + argument.id + '\'');

		argument.alreadyParsed = true;

		parsedArgs.flags.emplace_back (argument.id);

		return;
	}

	if (argument.alreadyParsed)
		if (argument.argsConsumed == 1)
			throwError (idx, "Invalid repetition of argument '" + argument.id + '\'');

	if (const auto numRequired = argument.acceptsVariadicArguments() ? 1UL : argument.argsConsumed;
		idx + static_cast<int> (numRequired) >= argc)
	{
		std::stringstream stream;

		stream << "Argument '" << argument.id << "' requires " << numRequired << " value";

		if (numRequired > 1)
			stream << 's';

		stream << '!';

		throwError (idx, stream.str());
	}

	argument.alreadyParsed = true;

	for (auto j = idx + 1, i = 0;
		 j < argc;
		 ++j, ++i, ++idx)
	{
		if (! argument.acceptsVariadicArguments())
			if (i == static_cast<decltype (i)> (argument.argsConsumed))
				return;

		const auto arg = std::string { argv[j] };

		if (argument.acceptsVariadicArguments())
			if (arg.starts_with ('-') || getSubcommand (arg) != nullptr)
				return;

		parsedArgs.argumentValues[argument.id].emplace_back (arg);
	}
}

void ArgumentParser::checkRequiredArgs()
{
	for (const auto& arg : arguments)
		if (arg.required && ! arg.alreadyParsed)
			throwError ("Missing required argument '" + arg.id + '\'');
}

ArgumentParser::Argument* ArgumentParser::getArgument (const std::string_view& commandLineArg)
{
	for (auto& arg : arguments)
		if (alg::contains (arg.delimiters, std::string { commandLineArg }))	 // cppcheck-suppress useStlAlgorithm
			return &arg;

	return nullptr;
}

ArgumentParser::Subcommand* ArgumentParser::getSubcommand (const std::string_view& commandLineArg)
{
	for (auto& subcommand : subcommands)
		if (subcommand.command == commandLineArg)  // cppcheck-suppress useStlAlgorithm
			return &subcommand;

	return nullptr;
}

void ArgumentParser::throwError (int idx, const std::string_view& message)
{
	std::stringstream stream;

	stream << "Error at argument " << idx << ": " << message;

	throwError (stream.str());
}

void ArgumentParser::throwError (const std::string_view& message)
{
	throw ParseError { message };
}

/*--------------------------------------------------------------------------------------------------*/

ArgumentParser::Argument::Argument (const std::vector<std::string>& delimiters_,
									const std::string_view&			help,
									const std::string_view&			id_,
									std::size_t						numArgs,
									bool							required_)
	: delimiters (delimiters_), helpString (help), id (id_), argsConsumed (numArgs), required (required_)
{
}

std::string ArgumentParser::Argument::getHelpString() const
{
	std::stringstream stream;

	if (! required)
		stream << '[';

	stream
		<< text::join (delimiters, "|");

	if (acceptsVariadicArguments() || argsConsumed > 0)
	{
		stream << '<' << id;

		if (acceptsVariadicArguments() || argsConsumed > 1)
			stream << "...";

		stream << '>';
	}

	if (! required)
		stream << ']';

	return stream.str();
}

bool ArgumentParser::Argument::isFlag() const noexcept
{
	return ! acceptsVariadicArguments() && argsConsumed > 0;
}

bool ArgumentParser::Argument::acceptsVariadicArguments() const noexcept
{
	return argsConsumed == ArgumentParser::VARIADIC_ARGUMENTS;
}

/*--------------------------------------------------------------------------------------------------*/

ArgumentParser::Subcommand::Subcommand (const std::string_view& command_,
										const std::string_view& help,
										std::unique_ptr<ArgumentParser>
											parser_)
	: command (command_), helpString (help), parser (std::move (parser_))
{
}

std::string ArgumentParser::Subcommand::getHelpString() const
{
	std::stringstream stream;

	stream << command << '\n';
	stream << helpString << '\n';
	stream << parser->getHelpString();

	return stream.str();
}

/*--------------------------------------------------------------------------------------------------*/

bool ArgumentParser::ParsedArguments::isEmpty() const
{
	return subcommandName.empty()
		&& flags.empty()
		&& argumentValues.empty();
}

bool ArgumentParser::ParsedArguments::hasSubcommand() const
{
	return ! subcommandName.empty();
}

std::string ArgumentParser::ParsedArguments::getSubcommandName() const
{
	return subcommandName;
}

bool ArgumentParser::ParsedArguments::hasFlag (const std::string_view& flagID) const
{
	return alg::contains (flags, std::string { flagID });
}

bool ArgumentParser::ParsedArguments::hasValueForArgument (const std::string_view& argumentID) const
{
	return argumentValues.contains (std::string { argumentID });
}

const std::vector<std::string>& ArgumentParser::ParsedArguments::getArgumentValue (const std::string_view& argumentID) const
{
	return argumentValues.at (std::string { argumentID });
}

}  // namespace misc

LIMES_END_NAMESPACE