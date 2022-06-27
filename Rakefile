require "fileutils"
#require "etc"

#

limes_root = __dir__()

cache = File.join(limes_root, "Cache")

builds = File.join(limes_root, "Builds")

doc = File.join(limes_root, "doc")

config = "Release"

#

def set_env_if_unset(envvar, value)
	if not ENV.has_key?(envvar) then
		ENV[envvar] = value
	end
end

set_env_if_unset("FETCHCONTENT_BASE_DIR", cache)
set_env_if_unset("CMAKE_BUILD_TYPE", config)
set_env_if_unset("CMAKE_CONFIG_TYPE", config)
set_env_if_unset("CMAKE_EXPORT_COMPILE_COMMANDS", "1")
set_env_if_unset("VERBOSE", "1")
#set_env_if_unset("CMAKE_BUILD_PARALLEL_LEVEL", etc.nprocessors)

module OS
	def OS.windows?
		(/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil
	end

	def OS.mac?
		(/darwin/ =~ RUBY_PLATFORM) != nil
	end
end

if OS.windows? then
	set_env_if_unset("CMAKE_GENERATOR", "Visual Studio 17 2022")
elsif OS.mac? then
	set_env_if_unset("CMAKE_GENERATOR", "Xcode")
else
	set_env_if_unset("CMAKE_GENERATOR", "Ninja Multi-Config")
end

#

task default: [:help]

desc "Show this message"
task :help do
	exec "cd %s && rake --tasks" % [limes_root]
end

desc "Initialize the workspace"
task :init do
	script_1 = File.join(limes_root, "scripts", "build_all.sh")
	script_2 = File.join(limes_root, "scripts", "build_all_vecops_variants.sh")
	python_reqs = File.join(limes_root, "requirements.txt")

	exec "chmod +x %s %s && python3 -m pip install -r %s && cd %s && pre-commit install --install-hooks --overwrite && pre-commit install --install-hooks --overwrite --hook-type commit-msg" % [script_1, script_2, python_reqs, limes_root]
end

desc "Run CMake configuration"
task :config do
	exec "cd %s && cmake --preset maintainer" % [limes_root]
end

desc "Open the IDE project"
task :open => :config do
	exec "cmake --open %s" % [builds]
end

#

desc "Run the build"
task :build => :config do
	exec "cd %s && cmake --build --preset maintainer --config %s" % [limes_root, config]
end

#

desc "Run CMake install"
task :install => :build do
	exec "cmake --install %s --config %s" % [builds, config]
end

desc "Run the uninstall script"
task :uninstall do
	script = File.join(builds, "uninstall.cmake")
	exec "cmake -P %s" % [script]
end

#

desc "Generate the dependency graph image"
task :deps_graph => :config do
	exec "cmake --build %s --target LimesDependencyGraph" % [builds]
end

desc "Build the docs"
task :docs => :config do
	exec "cmake --build %s --target LimesDocs" % [builds]
end

#

def safe_dir_delete(dir)
	if File.directory?(dir) then
		FileUtils.remove_dir(dir)
	end
end

desc "Clean the builds directory"
task :clean do
	safe_dir_delete(builds)
	safe_dir_delete(doc)

	exec "pre-commit gc"
end

desc "Wipe the cache of downloaded dependencies"
task :wipe => :clean do
	safe_dir_delete(cache)

	if ENV.has_key?("FETCHCONTENT_BASE_DIR") then
		safe_dir_delete(ENV["FETCHCONTENT_BASE_DIR"])
	end
end

#

desc "Run pre-commit over all files"
task :pc do
	exec "cd %s && git add . && pre-commit run --all-files" % [limes_root]
end
