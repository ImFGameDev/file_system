#include <filesystem>
#include <linux/limits.h>
#include <unistd.h>
#include <debug_system.h>

#include "../include/application.h"
#include "../include/assembler/StreamAssembler.h"

namespace main_player::core::stream
{
    //Public:
    [[nodiscard]]
    std::string application::path_to_application()
    {
        std::string path;

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

        if (found != std::string::npos) path = path.substr(0, found);

        return path;
    }

    [[nodiscard]]
    std::vector<std::string> application::get_directory_arr(const std::string& path, const std::string& file)
    {
        std::vector<std::string> directory_config;

        auto full_path = path_to_application() + "/" + path + file;
        if (stream_assembler::read_file(full_path, directory_config))
        {
            std::vector<std::string> directory;
            directory.reserve(directory_config.size());

            for (const auto& dir : directory_config) directory.push_back(path + dir);

            return directory;
        }

        return std::vector<std::string>();
    }

    std::vector<std::string> application::get_json_files(const std::string& path, const std::string& extension)
    {
        std::vector<std::string> json_files;

        try
        {
            if (!std::filesystem::exists(path))
                main_player::core::debug::debug_system::error("application", "Path not found: " + path);

            if (!std::filesystem::is_directory(path))
                main_player::core::debug::debug_system::error("application", "Path not directory: " + path);

            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (std::filesystem::is_regular_file(entry.status()))
                {
                    std::string file_extension = entry.path().extension().string();

                    if (std::equal(extension.begin(), extension.end(),
                                   file_extension.begin(), file_extension.end(),
                                   [](char a, char b)
                                   {
                                       return std::tolower(a) == std::tolower(b);
                                   }))
                    {
                        json_files.push_back(entry.path().filename().string());
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& ex)
        {
            main_player::core::debug::debug_system::error("application", "Error file system: " + std::string(ex.what()));
        }

        return json_files;
    }
}
