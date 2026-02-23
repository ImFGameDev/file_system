#include <map>
#include <vector>
#include <string>
#include <boost/json.hpp>

namespace boost::json
{
    void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, const std::map<std::string, std::string>& map)
    {
        boost::json::object obj;

        for (auto const& [key, value] : map) obj[key] = value;

        jv = obj;
    }

    std::map<std::string, std::string> tag_invoke(boost::json::value_to_tag<std::map<std::string, std::string>>,
                                                  const boost::json::value& jv)
    {
        std::map<std::string, std::string> result;
        auto const& obj = jv.as_object();

        for (auto const& [key, value] : obj) result[key] = value.as_string().c_str();

        return result;
    }

    void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, const std::vector<std::string>& vec)
    {
        boost::json::array arr;
        for (auto const& str : vec) arr.push_back(str.c_str());

        jv = arr;
    }

    std::vector<std::string> tag_invoke(boost::json::value_to_tag<std::vector<std::string>>,
                                        const boost::json::value& jv)
    {
        std::vector<std::string> result;
        auto const& arr = jv.as_array();

        for (auto const& item : arr) result.push_back(item.as_string().c_str());

        return result;
    }

    void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, const std::vector<uint64_t>& vec)
    {
        boost::json::array arr;
        for (auto const& value : vec) arr.push_back(value);

        jv = arr;
    }

    std::vector<std::uint64_t> tag_invoke(boost::json::value_to_tag<std::vector<std::int64_t>>,
                                          const boost::json::value& jv)
    {
        std::vector<uint64_t> result;
        auto const& arr = jv.as_array();

        for (auto const& item : arr) result.push_back(item.as_uint64());

        return result;
    }

    void tag_invoke(boost::json::value_from_tag, value& jv, const std::map<std::string, boost::json::object>& voidMap)
    {
        boost::json::object obj;

        for (auto const& [key, voidPtr] : voidMap) obj[key] = value_from(voidPtr);

        jv = obj;
    }

    std::map<std::string, boost::json::object> tag_invoke(
        boost::json::value_to_tag<std::map<std::string, boost::json::value>>, const boost::json::value& jv)
    {
        std::map<std::string, boost::json::object> result;
        auto const& obj = jv.as_object();

        for (auto const& [key, value] : obj) result[key] = value_to<boost::json::object>(value);

        return result;
    }
}
