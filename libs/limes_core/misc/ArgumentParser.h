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

#include "../limes_namespace.h"
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

	This parser accepts the following kinds of argument specifications:

	@par Flags
	Flags are boolean arguments whose presence on the command line indicates that they
	are on/true, and absence indicates that they are off/false. Flags only consume one
	argument from the list of command line arguments, which is the flag itself.

	@par Named arguments
	@parblock
	Named arguments are arguments that consume 1 or more values from the list of command
	line arguments. On the command line, values are assigned to arguments in two ways:

	- By passing the argument's delimiter and then a list of space-separated values. For example:
	@code
	--arg val1 val2 val3
	@endcode
	assigns 3 values to the named argument \c --arg .

	- By passing the argument's delimiter followed immediately by \c =\<value> . For example,
	the following invocation would assign 1 value to the argument \c --arg :
	@code
	--arg=val1
	@endcode
	You can also pass lists in this format, using comma or semicolon characters as separators.
	For example, the following invocations would both assign 2 values to the argument \c --arg :
	@code
	--arg=val1,val2
	'--arg=val1;val2'
	@endcode

	Named arguments may accept a specified number of arguments (in which case a parse error will
	be raised if the user passes the wrong number of values) or a variadic number of arguments (in
	which case the user must pass 1 or more values to avoid a parse error).

	For named arguments that accept 2 or more values, it is not an error to repeat the delimiter.
	For example, an invocation of the form:
	@code
	--arg val1 <possiblyOtherArguments...> --arg val2
	@endcode
	assigns 2 values to the argument \c --arg with no error.

	For named arguments that accept 1 or more values (or a variadic number of values), it is a parse
	error to pass the delimiter but no values.
	@endparblock

	@par Positional arguments
	Positional arguments are "what's left" after parsing all flags and named arguments. If a \c \-- is
	present on the command line, all arguments after it are treated as positional arguments (even if they
	match the delimiters for another flag, argument, or subcommand). Parsers may choose to require positional
	arguments, raise an error if any positional arguments are present, or do neither;
	additionally, parsers may specify a maximum number of positional arguments that are accepted.

	@par Subcommands
	@parblock
	Subcommands are command-line flags that, if present, affect the way the rest of the remaining
	arguments are parsed.

	For example, if \c git were built using this class, \c commit and \c push would be added as subcommands.

	Subcommands can introduce their own sets of flags and named arguments, and will override the parent parser's
	positional arguments specification. Subcommands may not contain nested subcommands.
	@endparblock

	Flags and named arguments may have more than one command-line delimiter -- for example, you
	may specify that either \c --arg or \c -a assign values to the same underlying argument.
	For this reason, flags and arguments have both a list of command line delimiters, and an ID
	string that is used to retrieve their values after parsing.

	Subcommands do not accept multiple delimiters; the name of the subcommand is also used as its command-line
	delimiter and the ID string to identify it after parsing.

	Every command line delimiter must be unique -- an assertion will be raised if you attempt to add an
	argument, flag, or subcommand that duplicates any command line delimiter already present in any flags,
	arguments, or subcommands. When adding a subcommand, all of \b its flags and arguments must also have
	delimiters that are unique from any delimiters present in the parent parser's arguments, flags, and
	subcommands.

	@ingroup misc

	@todo write unit tests
	@todo create parser from JSON file spec
	@todo minimum # of positional arguments
 */
class LIMES_EXPORT ArgumentParser final
{
public:

	struct LIMES_EXPORT PositionalArgumentsSpec final
	{
		[[nodiscard]] static PositionalArgumentsSpec acceptsNone();
		[[nodiscard]] static PositionalArgumentsSpec acceptsExactly (std::size_t num, const std::string_view& id = "positionalArguments");
		[[nodiscard]] static PositionalArgumentsSpec acceptsAny (const std::string_view& id = "positionalArguments");

		bool required { false };
		bool error { false };

		std::size_t min { 0 };
		std::size_t max { 0 };

		std::string identifier {};
	};

	/** This constant is a special value that can be passed to any function that takes a parameter
		specifying a number of arguments, to signify that the number of arguments may be variable
		-- ie, 1 or more.
	 */
	static constexpr auto VARIADIC_ARGUMENTS = static_cast<std::size_t> (-1);

	/** Constructs an ArgumentParser.

		@param handleHelpFlags If this is true, the parser will automatically search for any \c --help ,
		\c -help , or \c -h flags present on the command line, and if any are found, it will stop parsing
		and print a help/usage message.

		@param requiresPositionalArguments If true, it will be considered a parse error for the user not to
		specify any positional arguments. This argument and \c errorOnPositionalArguments cannot both be true.

		@param errorOnPositionalArguments If true, it will be considered a parse error for the user to
		specify any positional arguments. This argument and \c requiresPositionalArguments cannot both be true.

		@param maxNumPositionalArguments The maximum number of positional arguments that the parser should accept.
		If the user specifies more positional arguments than this, a parse error will be raised. You can set this
		to \c VARIADIC_ARGUMENTS to denote that any number of 1 or more positional arguments may be accepted.

		@param posArgIdentifier The placeholder text to be used to denote positional arguments when printing this
		parser's help string. Defaults to \c positionalArguments .
	 */
	ArgumentParser (bool						   handleHelpFlags = true,
					const PositionalArgumentsSpec& posArgSpec	   = PositionalArgumentsSpec::acceptsAny());

	/** @name Specifying accepted arguments */
	///@{

	/** Adds a named argument to the parser.
		Named arguments consume 1 or more arguments from the command line.
		For example, if the user passes:
		@code
		--arg one two three
		@endcode
		then the argument named \c arg will be assigned three values, \c one , \c two , and \c three .

		@param argument The command-line delimiters to be used for this flag. You can separate multiple
		different delimiters using pipe characters -- for example, specifying \c "--arg|-a" allows the
		user to pass either \c --arg or \c -a on the command line to denote that they are specifying
		values for this argument.

		@param helpString A help string associated with this argument.

		@param argumentID A unique string ID by which the value(s) of this argument will be referenced after the arguments are parsed.
		This is the string you will pass to \c ParsedArguments::getArgumentValue() to retrieve its value.
		If this argument is empty, it will be assigned to the first delimiter passed in \c argument , minus any leading \c - or \c \-- characters.

		@param numArgsConsumed The number of arguments this argument will consume from the command line.
		This is the number of \b additional values \b after the argument delimiter that will be consumed.
		For example, if you specify that an argument consumes 1 value, then:
		@code
		--arg value
		@endcode
		is correct, and
		@code
		--arg value1 value2
		@endcode
		will raise a parse error.
		You can also set this to \c VARIADIC_ARGUMENTS to specify that the argument can consume any number of 1 or more values.

		@param required If true, then if this argument is not specified, a parse error will be raised.

		@see addFlag()
	 */
	void addArgument (const std::string_view& argument,
					  const std::string_view& helpString	  = "",
					  const std::string_view& argumentID	  = "",
					  std::size_t			  numArgsConsumed = 1,
					  bool					  required		  = false,
					  const std::string_view& defaultValue	  = "");

	/** Adds a boolean flag to the parser.
		Flags are boolean arguments that consume only a single argument from the command line -
		the presence of the flag denotes that it is true, and its absence denotes false.
		Under the hood, this function calls \c addArgument() with \c numArgsConsumed=0 and \c required=false .

		@param flag The command-line delimiters to be used for this flag. You can separate multiple
		different delimiters using pipe characters -- for example, specifying \c "--help|-h" allows the
		user to pass either \c --help or \c -h on the command line for this flag to be considered true.
		If this argument is empty, it will be assigned to the first delimiter passed in \c flag , minus any leading \c - or \c \-- characters.

		@param helpString A help string associated with this flag.

		@param argumentID A unique string ID by which the value of this flag will be referenced after the arguments are parsed.
		This is the string you will pass to \c ParsedArguments::hasFlag() to retrieve its value.
		If this argument is empty, it will be assigned to the first delimiter passed in \c argument , minus any leading \c - or \c \-- characters.

		@see addArgument()
	 */
	void addFlag (const std::string_view& flag,
				  const std::string_view& helpString = "",
				  const std::string_view& argumentID = "");

	/** Adds a subcommand to the parser.
		Subcommands are flags that, if specified on the command line, affect the way the rest of the remaining arguments are parsed.
		For example, if you specify a subcommand \c subcmd that accepts an argument \c --arg , then:
		@code
		executable subcmd --arg value
		@endcode
		will be a correct usage, but
		@code
		executable --arg value
		@endcode
		will raise a parse error, because \c --arg is an argument for \c subcmd , not the main executable itself.

		@param subcommand The subcommand name. Unlike the \c addArgument() and \c addFlag() commands, you may \i not
		specify multiple delimiters separated by pipes. Subcommands must have a single delimiter. This identifier will
		be both the command line delimiter and the post-parsing ID string for the subcommand.

		@param subcommandOptions The arguments that this subcommand accepts. This parser must not have nested subcommands,
		but may specify flags, arguments, and accept positional arguments.

		@param helpString A help string associated with this subcommand.
	 */
	void addSubcommand (const std::string_view& subcommand,
						std::unique_ptr<ArgumentParser>
												subcommandOptions,
						const std::string_view& helpString = "");
	///@}

	/** @name Help */
	///@{
	/** Returns a help string describing the arguments that this parser accepts.
		@todo describe the output format
		@see printHelp()
	 */
	[[nodiscard]] std::string getHelpString() const;

	/** Convenience function that prints the output of \c getHelpString() to \c std::out .
		@see getHelpString()
	 */
	void printHelp() const;
	///@}

	/** This struct contains the results of parsing a set of command-line arguments:
		a list of which flags were present, the name of the subcommand specified,
		the values of any named arguments, and a list of positional arguments.
		@see ArgumentParser::parse(), ArgumentParser::parseSafe()
	 */
	class LIMES_EXPORT ParsedArguments final
	{
	public:

		/** Returns true if this struct contains no data.
			This returns true if no flags are present, no named arguments have values,
			no subcommand is present, and no positional arguments were provided.
		 */
		bool isEmpty() const;

		/** @name Subcommands */
		///@{
		/** Returns true if a subcommand name was specified.
			@see getSubcommandName()
		 */
		bool hasSubcommand() const;

		/** Returns the name of the subcommand that was specified.
			If no subcommand was specified, this returns an empty string.
			@see hasSubcommand()
		 */
		std::string getSubcommandName() const;
		///@}

		/** Returns true if the specified flag was present on the command line. */
		bool hasFlag (const std::string_view& flagID) const;

		/** @name Named arguments */
		///@{
		/** Returns true if the named argument was present on the command line (ie, has a value).
			@see getArgumentValue()
		 */
		bool hasValueForArgument (const std::string_view& argumentID) const;

		/** Returns the value of the named argument.
			@throws std::out_of_range An exception will be thrown if no value was specified for the named argument.
			@see hasValueForArgument()
		 */
		const std::vector<std::string>& getArgumentValue (const std::string_view& argumentID) const;
		///@}

		/** @name Positional arguments */
		///@{
		/** Returns true if any positional arguments were specified.
			@see getPositionalArguments()
		 */
		bool hasPositionalArguments() const;

		/** Returns the list of positional arguments that were specified.
			@see hasPositionalArguments()
		 */
		const std::vector<std::string>& getPositionalArguments() const noexcept;
		///@}

		/** @name The original arguments */
		///@{
		/** The length of the original list of arguments sent to the \c ArgumentParser::parse() function.
			@see argv
		 */
		int argc { 0 };

		/** The original argument values sent to the \c ArgumentParser::parse() function.
			@see argc
		 */
		char** argv { nullptr };
		///@}

	private:

		std::vector<std::string> flags {};

		std::vector<std::string> positionalArgs {};

		std::map<std::string, std::vector<std::string>> argumentValues {};

		std::string subcommandName {};

		friend class ArgumentParser;
	};

	/** @name Parsing the input arguments */
	///@{
	/** Parses the list of command line arguments and returns a ParsedArguments object.
		@throws ParseError An exception will be thrown if there is an error parsing the input arguments.
		@see ParsedArguments, parseSafe()
	 */
	[[nodiscard]] ParsedArguments parse (int argc, char** argv);

	/** Parses the list of command line arguments and returns a ParsedArguments object.
		Unlike \c parse() , this function will never throw an exception. If an error occurs during parsing,
		an invalid or incomplete ParsedArguments object will be returned, and if \c errorString isn't \c nullptr
		then it will be assigned to a string describing the error.
		@see ParsedArguments, parse()
	 */
	[[nodiscard]] ParsedArguments parseSafe (int argc, char** argv, std::string* errorString) noexcept;
	///@}

	/** @exception ArgumentParser::ParseError This exception is thrown by the \c ArgumentParser::parse() function.
		If the user passed unexpected or incorrect arguments, one of these exceptions will be thrown.
	 */

	/** This exception is thrown by the \c ArgumentParser::parse() function.
		If the user passed unexpected or incorrect arguments, one of these exceptions will be thrown.
	 */
	struct ParseError final : public std::runtime_error
	{
		/** Constructs a parse error with the given message. */
		explicit ParseError (const std::string_view& message)
			: std::runtime_error (message.begin())
		{
		}

		/** Constructs a parse error with the given argument index and message. */
		explicit ParseError (std::size_t indexToUse, const std::string_view& message)
			: std::runtime_error (message.begin()), index (indexToUse)
		{
		}

		/** The index of the argument in the original list where the parse error occurred.
			If the error is not related to a specific argument, this will be 0.
		 */
		std::size_t index { 0 };
	};

private:

	struct Argument final
	{
		Argument (const std::vector<std::string>& delimiters_,
				  const std::string_view&		  help,
				  const std::string_view&		  id_,
				  std::size_t					  numArgs,
				  bool							  required_,
				  const std::string_view&		  defaultVal);

		std::string getHelpString() const;

		bool isFlag() const noexcept;

		bool acceptsVariadicArguments() const noexcept;

		std::vector<std::string> delimiters {};
		std::string				 helpString {};
		std::string				 id {};
		std::size_t				 argsConsumed { 1 };
		bool					 required { false };

		std::string defaultValue {};

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

	Argument* getArgument (const std::string& commandLineArg);

	Subcommand* getSubcommand (const std::string& commandLineArg);

	void parseInternal (ParsedArguments& parsedArgs, int argc, char** argv);

	void parseArgument (Argument& argument,
						int argc, char** argv,
						ParsedArguments& parsedArgs,
						int&			 idx);

	void parsePositionalArgument (ParsedArguments&	 parsedArgs,
								  int				 idx,
								  const std::string& argValue) const;

	void checkRequiredArgs (ParsedArguments& args) const;

	[[noreturn]] void throwError (int idx, const std::string_view& message) const;
	[[noreturn]] void throwError (const std::string_view& message) const;

	std::vector<Argument> arguments;

	std::vector<Subcommand> subcommands;

	PositionalArgumentsSpec posArgsSpec;

	bool shouldHandleHelp { true };

	Subcommand* activeSubcommand { nullptr };
};

}  // namespace misc

LIMES_END_NAMESPACE
