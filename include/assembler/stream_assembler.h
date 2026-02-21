#pragma once

#include <string>

namespace main_player::core::stream
{
	class stream_assembler
	{
	private:
		template<typename T>
		static bool load(const std::string& filePath, T& output);

	public:
		stream_assembler() = delete;

		~stream_assembler() = delete;

		template<typename T>
		static bool parce_to_object(const std::string& stream_data, T& output);

		template<typename T>
		static std::string to_json(const T& data);

		template<typename T>
		static bool read_file(const std::string& filePath, T& output);

		template<typename T>
		static bool find_read_file(const std::string filePath[], const std::size_t& length, const std::string& name, T& output);

		template<typename T>
		static bool find_read_file(const std::vector<std::string>& filePath, const std::string& name, T& output);

		template<typename T>
		static void write_file(const std::string& filePath, const T& data);
	};
}
