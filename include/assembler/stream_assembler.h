#pragma once

#include <string>
#include <fstream>
#include <debug_system.h>
#include <boost/json.hpp>

namespace main_player::core::stream
{
	using logger = main_player::core::debug::debug_system;
	using str = std::string;
	using value = boost::json::value;
	using object = boost::json::object;

	class stream_assembler
	{
	private:
		template<typename T>
		static bool load(const str& path, T& output)
		{
			try
			{
				std::ifstream file(path);

				if (!file.is_open()) return false;

				value jv = boost::json::parse(file);
				auto result = boost::json::try_value_to<T>(jv);

				if (result.has_value())
				{
					output = result.value();
					return true;
				}
			}
			catch (const std::exception& e)
			{
				logger::error("stream_assembler", "JSON load error: " + std::string(e.what()));
				return false;
			}

			logger::log("stream_assembler", "JSON value null");
			return false;
		}

	public:
		stream_assembler() = delete;

		~stream_assembler() = delete;

		template<typename T>
		static void write_file(const str& path, const T& data)
		{
			try
			{
				std::ofstream file(path);
				boost::json::value jv = boost::json::value_from(data);

				file << boost::json::serialize(jv);
			}
			catch (const std::exception& e)
			{
				logger::error("stream_assembler", "file write error: " + std::string(e.what()));
			}
		}

		template<typename T>
		static bool parce_to_object(const str& stream_data, T& output)
		{
			try
			{
				boost::json::value jv = boost::json::parse(stream_data);
				auto result = boost::json::try_value_to<T>(jv);

				if (result.has_value())
				{
					output = result.value();
					return true;
				}

				logger::error("stream_assembler", "json parsing false");
				return false;
			}
			catch (const std::exception& e)
			{
				logger::error("stream_assembler", "json parsing error: " + std::string(e.what()));
				return false;
			}
		}

		template<typename T>
		static str to_json(const T& data)
		{
			try
			{
				value jv = boost::json::value_from(data);
				return boost::json::serialize(jv);
			}
			catch (const std::exception& e)
			{
				logger::error("stream_assembler", "json serialization error: " + std::string(e.what()));
				return "{}";
			}
		}

		template<typename T>
		static bool read_file(const str& path, T& output)
		{
			try
			{
				std::ifstream file(path);
				if (!file.is_open())
				{
					logger::error("stream_assembler", "cannot open file: " + path);
					return false;
				}

				boost::json::value jv = boost::json::parse(file);
				auto result = boost::json::try_value_to<T>(jv);

				if (result.has_value())
				{
					output = result.value();
					return true;
				}

				logger::error("stream_assembler", "has_value false");
				return false;
			}
			catch (const std::exception& e)
			{
				logger::error("stream_assembler", "error: " + str(e.what()) + "\n" + path);
				return false;
			}
		}

		template<typename T>
		static bool find_read_file(const str paths[], const std::size_t& length, const str& name, T& output)
		{
			for (int i = 0; i < length; i++) if (load(paths[i] + name, output)) return true;

			str log_paths{};

			for (int i = 0; i < length; i++) log_paths += paths[i] + name + "\n";

			logger::error("stream_assembler", "cannot found file:\n" + log_paths);
			return false;
		}

		template<typename T>
		static bool find_read_file(const std::vector<str>& paths, const str& name, T& output)
		{
			for (const auto& path: paths) if (load(path + name, output)) return true;

			str log_paths{};

			for (const auto& path: paths) log_paths += path + name + "\n";

			logger::error("stream_assembler", "cannot found file:\n" + log_paths);
			return false;
		}

		static bool find_load_str(const std::vector<str>& paths, const str& name, str& output)
		{
			for (const auto& path: paths)
			{
				std::ifstream file(path + name);

				if (file.is_open())
				{
					output = str(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

					file.close();
					return true;
				}
			}

			return false;
		}
	};
}
