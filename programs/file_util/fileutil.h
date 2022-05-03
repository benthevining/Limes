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

#include <string>
#include <vector>

namespace fileutil
{

void append (const std::string& fileName, const std::string& content);

void cat (const std::vector<std::string>& items);

void cd (const std::string& name);

void copy (const std::vector<std::string>& items, const std::string& destName);

void exists (const std::vector<std::string>& items);

void follow_symlink (const std::string& linkName, bool recurse);

void ln (const std::string& linkName, const std::string& targetName);

void ls (const std::string& dir);

void mkdir (const std::string& dirName);

void modtime (const std::string& name);

void pwd();

void relative (const std::string& path, const std::string& basePath);

void rename (const std::string& oldName, const std::string& newName);

void rm (const std::vector<std::string>& items);

void size (const std::string& name);

void space();

void touch (const std::vector<std::string>& items, bool allowCreation);

void type (const std::string& name);

void write (const std::string& fileName, const std::string& content);

}  // namespace fileutil
