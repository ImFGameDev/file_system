#pragma once

#include <string>
#include <vector>

namespace main_player::core::stream
{
    class Application
    {
    public:
        Application() = delete;

        ~Application() = delete;

        [[nodiscard]]
        static std::string path_to_application();

        [[nodiscard]]
        static std::vector<std::string> get_directory_arr(const std::string& path, const std::string& file = "directory.json");

        static std::vector<std::string> get_json_files(const std::string& path, const std::string& extension = ".json");
    };
}
