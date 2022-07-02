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

#include "./ArgumentParser.h"
#include "../limes_namespace.h"
#include <iostream>
#include "../text/StringUtils.h"
#include "../system/limes_assert.h"
#include "./Algorithm.h"
#include <sstream>
#include <algorithm>

#if LIMES_RELEASE
#	include "../misc/IgnoreUnused.h"
#endif

LIMES_BEGIN_NAMESPACE

namespace misc
{

ArgumentParser::PositionalArgumentsSpec ArgumentParser::PositionalArgumentsSpec::acceptsNone()
{
	PositionalArgumentsSpec spec;

	spec.required = false;
	spec.error	  = true;
	spec.min	  = 0;
	spec.max	  = 0;

	return spec;
}

ArgumentParser::PositionalArgumentsSpec ArgumentParser::PositionalArgumentsSpec::acceptsExactly (std::size_t num, const std::string_view& id)
{
	PositionalArgumentsSpec spec;

	spec.required	= true;
	spec.error		= false;
	spec.min		= num;
	spec.max		= num;
	spec.identifier = id;

	return spec;
}

ArgumentParser::PositionalArgumentsSpec ArgumentParser::PositionalArgumentsSpec::acceptsAny (const std::string_view& id)
{
	return acceptsExactly (ArgumentParser::VARIADIC_ARGUMENTS, id);
}

/*--------------------------------------------------------------------------------------------------*/

ArgumentParser::ArgumentParser (bool						   handleHelpFlags,
								const PositionalArgumentsSpec& posArgSpec)
	: posArgsSpec (posArgSpec),
	  shouldHandleHelp (handleHelpFlags)
{
	// it makes no sense for these to both be true
	LIMES_ASSERT (! (posArgsSpec.required && posArgsSpec.error));

	if (posArgsSpec.error)
	{
		posArgsSpec.max = 0UL;
		posArgsSpec.min = 0UL;
	}
	else if (posArgsSpec.required)
	{
		LIMES_ASSERT (posArgsSpec.min == VARIADIC_ARGUMENTS || posArgsSpec.min > 0);
		LIMES_ASSERT (posArgsSpec.max == VARIADIC_ARGUMENTS || posArgsSpec.max > 0);
		LIMES_ASSERT (posArgsSpec.max >= posArgsSpec.min);
	}

	if (posArgsSpec.identifier.empty())
	{
		posArgsSpec.identifier = "positionalArguments";

		if (posArgsSpec.min == VARIADIC_ARGUMENTS || posArgsSpec.min > 1)
			posArgsSpec.identifier += "...";
	}
}

void ArgumentParser::validateArgument (const std::vector<std::string>& delimiters,
									   const std::string&			   newID) const
{
#if LIMES_DEBUG
	// '--' is an invalid command line delimiter
	LIMES_ASSERT (! alg::contains (delimiters, std::string { "--" }));

	if (shouldHandleHelp)
	{
		// if the parser is handling help flags for you, then you can't use these delimiters for any arguments
		LIMES_ASSERT (! alg::contains (delimiters, std::string { "--help" }));
		LIMES_ASSERT (! alg::contains (delimiters, std::string { "-h" }));
	}

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
#else
	misc::ignore_unused (delimiters, newID);
#endif
}

static inline std::string createID (const std::string& delimiter)
{
	if (delimiter.starts_with ("--"))
		return delimiter.substr (2, std::string_view::npos);

	if (delimiter.starts_with ("-"))
		return delimiter.substr (1, std::string_view::npos);

	return delimiter;
}

void ArgumentParser::addArgument (const std::string_view& argument,
								  const std::string_view& helpString,
								  const std::string_view& argumentID,
								  std::size_t			  numArgsConsumed,
								  bool					  required,
								  const std::string_view& defaultValue)
{
	const auto delims = text::split (argument, "|", false);

	const auto newID = argumentID.empty() ? createID (delims.front()) : std::string { argumentID };

	LIMES_ASSERT (! newID.empty());

	validateArgument (delims, newID);

	arguments.emplace_back (delims,
							std::string { helpString },
							newID,
							numArgsConsumed,
							required,
							defaultValue);
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
	LIMES_ASSERT (subcommandOptions.get() != nullptr);

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
				<< arg.getHelpString() << '\n';

		if (shouldHandleHelp)
			stream << "[--help|-help|-h] - Print this message and exit\n";

		stream << '\n';
	}

	if (! subcommands.empty())
	{
		stream << "Subcommands:\n\n";

		for (const auto& subcommand : subcommands)
			stream
				<< subcommand.getHelpString() << '\n';
	}

	if (posArgsSpec.required || ! posArgsSpec.error)
	{
		if (! posArgsSpec.required)
			stream << '[';

		LIMES_ASSERT (! posArgsSpec.identifier.empty());

		stream << posArgsSpec.identifier;

		if (posArgsSpec.min == VARIADIC_ARGUMENTS || posArgsSpec.min > 1)
			stream << "...";

		// stream << " {" << numPosArgs << " accepted}";

		if (! posArgsSpec.required)
			stream << ']';
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

	parseInternal (parsedArgs, argc, argv);

	return parsedArgs;
}

ArgumentParser::ParsedArguments ArgumentParser::parseSafe (int argc, char** argv, std::string* errorString) noexcept
{
	ParsedArguments parsedArgs;

	try
	{
		parseInternal (parsedArgs, argc, argv);

		return parsedArgs;
	}
	catch (std::exception& e)
	{
		if (auto* str = errorString)
			*str = e.what();

		return parsedArgs;
	}
}

void ArgumentParser::parseInternal (ParsedArguments& parsedArgs, int argc, char** argv)
{
	parsedArgs.argc = argc;
	parsedArgs.argv = argv;

	for (auto idx = 0; idx < argc; ++idx)
	{
		const auto inputString = text::unquoted (std::string_view { argv[idx] });

		if (shouldHandleHelp)
		{
			if (inputString.ends_with ("help") || inputString == "-h")
			{
				printHelp();
				parsedArgs.flags.emplace_back ("help");
				return;
			}
		}

		if (inputString == "--")
		{
			for (auto i = idx + 1; i < argc; ++i)
				parsePositionalArgument (parsedArgs, i, inputString);

			break;
		}

		// check if the input string is in the form --arg=<value>
		if (inputString.find ('=') != std::string::npos)
		{
			const auto delimiter = text::upToFirstOccurrenceOf (inputString, "=");
			const auto argValue	 = text::fromFirstOccurrenceOf (inputString, "=");

			if (auto* argument = getArgument (delimiter))
			{
				if (argument->isFlag())
					throwError (idx, "Flag '" + argument->id + "' cannot be assigned a value");

				argument->alreadyParsed = true;

				parsedArgs.argumentValues[argument->id].emplace_back (argValue);

				continue;
			}
		}

		if (auto* argument = getArgument (inputString))
		{
			parseArgument (*argument, argc, argv, parsedArgs, idx);

			continue;
		}

		if (auto* subcommand = getSubcommand (inputString))
		{
			if (parsedArgs.hasSubcommand())
				throwError (idx, "Only one subcommand can be specified!");

			LIMES_ASSERT (activeSubcommand == nullptr);

			parsedArgs.subcommandName = inputString;

			activeSubcommand = subcommand;

			posArgsSpec = subcommand->parser->posArgsSpec;

			continue;
		}

		if (posArgsSpec.error)
			throwError (idx, "Unknown argument '" + inputString + '\'');
		else
			parsePositionalArgument (parsedArgs, idx, inputString);
	}

	checkRequiredArgs (parsedArgs);

	if (const auto* ac = activeSubcommand)
		ac->parser->checkRequiredArgs (parsedArgs);

	if (shouldHandleHelp)
		if (parsedArgs.isEmpty())
			printHelp();
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

	argument.alreadyParsed = true;

	for (; idx < argc; ++idx)
	{
		const auto arg = std::string { argv[idx] };

		if (getSubcommand (arg) != nullptr
			|| getArgument (arg) != nullptr
			|| getArgument (text::upToFirstOccurrenceOf (arg, "=")) != nullptr)
			return;

		auto& argValues = parsedArgs.argumentValues[argument.id];

		if (argument.acceptsVariadicArguments())
		{
			if (arg.starts_with ('-'))
				return;
		}
		else
		{
			if (argValues.size() >= argument.argsConsumed)
				return;
		}

		argValues.emplace_back (arg);
	}
}

void ArgumentParser::parsePositionalArgument (ParsedArguments&	 parsedArgs,
											  int				 idx,
											  const std::string& argValue) const
{
	if (posArgsSpec.error)
		throwError (idx, "Positional arguments not accepted!");

	parsedArgs.positionalArgs.emplace_back (argValue);
}

void ArgumentParser::checkRequiredArgs (ParsedArguments& args) const
{
	for (const auto& arg : arguments)
	{
		auto& argValues = args.argumentValues.at (arg.id);

		const auto numValues = argValues.size();

		if (arg.required && numValues == 0)
			throwError ("Missing required argument '" + arg.id + '\'');

		if (! arg.acceptsVariadicArguments())
		{
			if (numValues != arg.argsConsumed)
			{
				std::stringstream s;

				s << "Argument '" << arg.id << "' has incorrect number of values - expected "
				  << arg.argsConsumed << ", got " << numValues;

				throwError (s.str());
			}
		}

		if (numValues == 0 && ! arg.defaultValue.empty())
			argValues.emplace_back (arg.defaultValue);
	}

	if (args.hasPositionalArguments())
	{
		std::stringstream s;

		if (posArgsSpec.error)
		{
			s << "Unexpected positional arguments: " << text::join (args.getPositionalArguments(), " ");
			throwError (s.str());
		}

		const auto numPosArgs = args.getPositionalArguments().size();

		if (posArgsSpec.min != VARIADIC_ARGUMENTS && numPosArgs < posArgsSpec.min)
		{
			s << "Too few positional arguments: expected at least " << posArgsSpec.min << ", got " << numPosArgs;
			throwError (s.str());
		}

		if (posArgsSpec.max != VARIADIC_ARGUMENTS && numPosArgs > posArgsSpec.max)
		{
			s << "Too many positional arguments: expected at most " << posArgsSpec.max << ", got " << numPosArgs;
			throwError (s.str());
		}
	}
	else
	{
		if (posArgsSpec.required)
			throwError ("Missing required positional arguments!");
	}
}

ArgumentParser::Argument* ArgumentParser::getArgument (const std::string& commandLineArg)
{
	for (auto& arg : arguments)
		if (alg::contains (arg.delimiters, commandLineArg))	 // cppcheck-suppress useStlAlgorithm
			return &arg;

	if (auto* ac = activeSubcommand)
		for (auto& arg : ac->parser->arguments)
			if (alg::contains (arg.delimiters, commandLineArg))	 // cppcheck-suppress useStlAlgorithm
				return &arg;

	return nullptr;
}

ArgumentParser::Subcommand* ArgumentParser::getSubcommand (const std::string& commandLineArg)
{
	for (auto& subcommand : subcommands)
		if (subcommand.command == commandLineArg)  // cppcheck-suppress useStlAlgorithm
			return &subcommand;

	return nullptr;
}

void ArgumentParser::throwError (int idx, const std::string_view& message) const
{
	LIMES_ASSERT (idx >= 0);

	std::stringstream stream;

	stream << "Error at argument " << idx << ": " << message;

	throw ParseError { static_cast<std::size_t> (idx), stream.str() };
}

void ArgumentParser::throwError (const std::string_view& message) const
{
	throw ParseError { message };
}

/*--------------------------------------------------------------------------------------------------*/

ArgumentParser::Argument::Argument (const std::vector<std::string>& delimiters_,
									const std::string_view&			help,
									const std::string_view&			id_,
									std::size_t						numArgs,
									bool							required_,
									const std::string_view&			defaultVal)
	: delimiters (delimiters_), helpString (help), id (id_), argsConsumed (numArgs), required (required_), defaultValue (defaultVal)
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

	if (! helpString.empty())
		stream << " - " << helpString;

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

	stream << '\n';
	stream << helpString << '\n';
	stream << command << parser->getHelpString();

	return stream.str();
}

/*--------------------------------------------------------------------------------------------------*/

bool ArgumentParser::ParsedArguments::isEmpty() const
{
	return subcommandName.empty()
		&& flags.empty()
		&& argumentValues.empty()
		&& positionalArgs.empty();
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

bool ArgumentParser::ParsedArguments::hasPositionalArguments() const
{
	return ! positionalArgs.empty();
}

const std::vector<std::string>& ArgumentParser::ParsedArguments::getPositionalArguments() const noexcept
{
	return positionalArgs;
}

}  // namespace misc

LIMES_END_NAMESPACE
