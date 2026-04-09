#include <map>
#include <vector>
#include <string>
#include <boost/json.hpp>

namespace boost::json
{
	using str = std::string;
	using map_str = std::map<str, str>;
	using value = boost::json::value;
	using object = boost::json::object;
	using u64 = std::uint64_t;

	void tag_invoke(boost::json::value_from_tag, value& jv, const map_str& map)
	{
		object obj;

		for (auto const& [key, value]: map) obj[key] = value;

		jv = obj;
	}

	std::map<str, str> tag_invoke(boost::json::value_to_tag<map_str>, const value& jv)
	{
		std::map<str, str> result;
		auto const& obj = jv.as_object();

		for (auto const& [key, value]: obj) result[key] = value.as_string().c_str();

		return result;
	}

	void tag_invoke(boost::json::value_from_tag, value& jv, const std::vector<str>& vec)
	{
		boost::json::array arr;
		for (auto const& str: vec) arr.push_back(str.c_str());

		jv = arr;
	}

	std::vector<str> tag_invoke(boost::json::value_to_tag<std::vector<str>>, const value& jv)
	{
		std::vector<str> result;
		auto const& arr = jv.as_array();

		for (auto const& item: arr) result.push_back(item.as_string().c_str());

		return result;
	}

	void tag_invoke(boost::json::value_from_tag, value& jv, const std::vector<u64>& vec)
	{
		boost::json::array arr;
		for (auto const& value: vec) arr.push_back(value);

		jv = arr;
	}

	std::vector<u64> tag_invoke(boost::json::value_to_tag<std::vector<u64>>, const value& jv)
	{
		std::vector<u64> result;
		auto const& arr = jv.as_array();

		for (auto const& item: arr) result.push_back(item.as_uint64());

		return result;
	}

	void tag_invoke(boost::json::value_from_tag, value& jv, const std::map<str, object>& voidMap)
	{
		object obj;

		for (auto const& [key, voidPtr]: voidMap) obj[key] = value_from(voidPtr);

		jv = obj;
	}

	std::map<str, object> tag_invoke(boost::json::value_to_tag<std::map<str, value>>, const value& jv)
	{
		std::map<str, object> result;
		auto const& obj = jv.as_object();

		for (auto const& [key, value]: obj) result[key] = value_to<object>(value);

		return result;
	}
}
