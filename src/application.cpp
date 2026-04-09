#include <filesystem>
#include <linux/limits.h>
#include <unistd.h>
#include <debug_system.h>

#include "../include/application.h"
#include "../include/assembler/stream_assembler.h"

namespace main_player::core::stream
{
	using str = std::string;
	using logger = main_player::core::debug::debug_system;

	//Public:
	[[nodiscard]] str application::path_to_application()
	{
		str path;

#ifdef _WIN32
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			path = buffer;
#else
		char buffer[PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

		if (len != -1)
		{
			buffer[len] = '\0';
			path = buffer;
		}
#endif

		std::size_t found = path.find_last_of("/\\");

		if (found != str::npos) path = path.substr(0, found);

		return path;
	}

	[[nodiscard]] std::vector<str> application::get_directory_arr(const str& path, const str& file)
	{
		std::vector<str> directory_config;
		std::vector<str> directory;

		auto full_path = path_to_application() + "/" + path + file;
		if (stream_assembler::read_file(full_path, directory_config))
		{
			directory.reserve(directory_config.size());

			for (const auto& dir: directory_config) directory.push_back(path + dir);

			return directory;
		}

		return directory;
	}

	std::vector<str> application::get_json_files(const str& path, const str& extension)
	{
		std::vector<str> json_files;

		try
		{
			if (!std::filesystem::exists(path)) logger::error("application", "path not found: " + path);
			if (!std::filesystem::is_directory(path)) logger::error("application", "path not directory: " + path);

			for (const auto& entry: std::filesystem::directory_iterator(path))
				if (std::filesystem::is_regular_file(entry.status()))
				{
					str file_extension = entry.path().extension().string();

					if (std::equal(extension.begin(), extension.end(), file_extension.begin(), file_extension.end(),
					               [](char a, char b)
					               {
						               return std::tolower(a) == std::tolower(b);
					               }))
					{
						json_files.push_back(entry.path().filename().string());
					}
				}
		}
		catch (const std::filesystem::filesystem_error& ex)
		{
			logger::error("application", "error file system: " + str(ex.what()));
		}

		return json_files;
	}
}
