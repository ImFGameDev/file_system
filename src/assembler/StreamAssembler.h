#pragma once

#include <fstream>
#include <string>
#include <boost/json.hpp>

#include "StreamAssembler.h"
#include "src/core/debug/DebugSystem.h"

namespace main_player::core::stream
{
	class StreamAssembler
	{
	private:
		template<typename T>
		static bool load(const std::string& filePath, T& output)
		{
			try
			{
				std::ifstream file(filePath);

				if (!file.is_open()) return false;

				boost::json::value jv = boost::json::parse(file);
				auto result = boost::json::try_value_to<T>(jv);

				if (result.has_value())
				{
					output = result.value();
					return true;
				}
			}
			catch (const std::exception& e)
			{
				debug::DebugSystem::error("stream_assembler", "JSON load error: " + std::string(e.what()));
				return false;
			}

			debug::DebugSystem::log("stream_assembler", "JSON value null");
			return false;
		}

	public:
		StreamAssembler() = delete;

		~StreamAssembler() = delete;

		template<typename T>
		static bool parce_to_object(const std::string& stream_data, T& output)
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

				debug::DebugSystem::error("stream_assembler", "JSON parsing false");
				return false;
			}
			catch (const std::exception& e)
			{
				debug::DebugSystem::error("stream_assembler", "JSON parsing error: " + std::string(e.what()));
				return false;
			}
		}

		template<typename T>
		static std::string to_json(const T& data)
		{
			try
			{
				boost::json::value jv = boost::json::value_from(data);
				return boost::json::serialize(jv);
			}
			catch (const std::exception& e)
			{
				debug::DebugSystem::error("stream_assembler", "JSON serialization error: " + std::string(e.what()));
				return "{}";
			}
		}

		template<typename T>
		static bool read_file(const std::string& filePath, T& output)
		{
			try
			{
				std::ifstream file(filePath);
				if (!file.is_open())
				{
					debug::DebugSystem::error("stream_assembler", "Cannot open file: " + filePath);
					return false;
				}

				boost::json::value jv = boost::json::parse(file);
				auto result = boost::json::try_value_to<T>(jv);

				if (result.has_value())
				{
					output = result.value();
					return true;
				}

				debug::DebugSystem::error("stream_assembler", "has_value false");
				return false;
			}
			catch (const std::exception& e)
			{
				debug::DebugSystem::error("stream_assembler", "Error: " + std::string(e.what()) + "\n" + filePath);
				return false;
			}
		}

		template<typename T>
		static bool find_read_file(const std::string filePath[], const std::size_t& length, const std::string& name,
		                           T& output
		)
		{
			for (int i = 0; i < length; i++) if (load(filePath[i] + name, output)) return true;

			std::string paths{};

			for (int i = 0; i < length; i++) paths += filePath[i] + name + "\n";

			debug::DebugSystem::error("stream_assembler", "Cannot found file:\n" + paths);
			return false;
		}

		template<typename T>
		static bool find_read_file(const std::vector<std::string>& filePath, const std::string& name, T& output)
		{
			for (const auto& path: filePath) if (load(path + name, output)) return true;

			std::string paths{};

			for (const auto& path: filePath) paths += path + name + "\n";

			debug::DebugSystem::error("stream_assembler", "Cannot found file:\n" + paths);
			return false;
		}

		template<typename T>
		static void write_file(const std::string& filePath, const T& data)
		{
			try
			{
				std::ofstream file(filePath);
				boost::json::value jv = boost::json::value_from(data);

				file << boost::json::serialize(jv);
			}
			catch (const std::exception& e)
			{
				debug::DebugSystem::error("stream_assembler", "File write error: " + std::string(e.what()));
			}
		}
	};
}
