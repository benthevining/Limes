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

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include <limes_platform.h>

#if defined(__linux__)
#	include <linux/limits.h>
#else
#	include <limits.h>
#endif

#include <inttypes.h>
#include <limes_namespace.h>
#include "exec_location.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

std::string getExecutablePath()
{
	char buffer[PATH_MAX];

#if defined(__sun)
	static constexpr auto proc_self_exe = "/proc/self/path/a.out";
#else
	static constexpr auto proc_self_exe = "/proc/self/exe";
#endif

	if (const auto* resolved = realpath (proc_self_exe, buffer))
		return { resolved, static_cast<std::string::size_type> (std::strlen (resolved)) };

	return {};
}

#if LIMES_ANDROID
#	include <fcntl.h>
#	include <sys/mman.h>
#	include <unistd.h>
#endif

std::string getModulePath()
{
#if LIMES_MSVC
#	define WAI_RETURN_ADDRESS() _ReturnAddress()
#elif defined(__GNUC__)
#	define WAI_RETURN_ADDRESS() __builtin_extract_return_addr (__builtin_return_address (0))
#endif

#ifdef WAI_RETURN_ADDRESS

#	if defined(__sun)
	static constexpr auto proc_self_maps = "/proc/self/map";
#	else
	static constexpr auto proc_self_maps = "/proc/self/maps";
#	endif

	for (auto r = 0; r < 5; ++r)
	{
		if (auto* maps = std::fopen (proc_self_maps, "r"))
		{
			do
			{
				char buffer[std::max (1024, PATH_MAX)];	 // NOLINT

				if (! std::fgets (buffer, sizeof (buffer), maps))
					break;

				char	 perms[5];
				uint64_t low, high, offset;
				uint32_t major, minor, inode;

				char path[PATH_MAX];

				if (std::sscanf (buffer, "%" PRIx64 "-%" PRIx64 " %s %" PRIx64 " %x:%x %u %s\n", &low, &high, perms, &offset, &major, &minor, &inode, path) != 8)
					break;

				const auto addr = reinterpret_cast<uintptr_t> (WAI_RETURN_ADDRESS());

				if (low <= addr && addr <= high)
				{
					if (auto* resolved = realpath (path, buffer))
					{
						auto length = static_cast<int> (std::strlen (resolved));

#	if LIMES_ANDROID
						if (length > 4
							&& buffer[length - 1] == 'k'
							&& buffer[length - 2] == 'p'
							&& buffer[length - 3] == 'a'
							&& buffer[length - 4] == '.')
						{
							const auto fd = open (path, O_RDONLY);

							if (fd == -1)
								break;

							auto* begin = static_cast<char*> (mmap (0, offset, PROT_READ, MAP_SHARED, fd, 0));

							if (begin == MAP_FAILED)
							{
								close (fd);
								break;
							}

							auto* p = begin + offset - 30;	// minimum size of local file header

							while (p >= begin)	// scan backwards
							{
								if (*reinterpret_cast<uint32_t*> (p) == 0x04034b50UL)  // local file header signature found
								{
									const auto length_ = *(reinterpret_cast<uint32_t*> (p + 26));

									if (length + 2 + length_ < static_cast<uint32_t> (sizeof (buffer)))
									{
										std::memcpy (&buffer[length], "!/", 2);
										std::memcpy (&buffer[length + 2], p + 30, length_);
										length += 2 + length_;
									}

									break;
								}

								--p;
							}

							munmap (begin, offset);
							close (fd);
						}
#	endif

						std::string result { resolved, static_cast<std::string::size_type> (length) };

						std::fclose (maps);

						return result;
					}
				}
			} while (false);

			std::fclose (maps);
		}
	}

	return {};

#	undef WAI_RETURN_ADDRESS

#else
	return {};
#endif
}
}  // namespace files

LIMES_END_NAMESPACE