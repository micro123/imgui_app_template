#ifndef HTTP_TOOLS_HPP
#define HTTP_TOOLS_HPP

#include <concepts>
#include <optional>
#include <string>
#include <utility>

#include "config.h"
#include "utils/json.hpp"

namespace Http
{

    using Json      = nlohmann::json;
    using HeaderMap = std::map<std::string, std::string>;
    using QueryMap  = std::map<std::string, std::string>;

    template <typename T>
    concept JsonSerializable = requires (T &t, Json &j) {
        {
            j.get_to (t)
        } -> std::same_as<T &>;
        {
            j = t
        } -> std::same_as<Json &>;
    };

    namespace internal
    {
        std::optional<std::string> FW_API ExecuteHttpGet (const std::string &url, const HeaderMap &headers, const QueryMap &queries);
    }

    template <typename Response>
    std::optional<Response> Get (const std::string &url, const HeaderMap &headers, const QueryMap &queries)
        requires JsonSerializable<Response>
    {
        std::optional<Response> res {};

        auto res_text = internal::ExecuteHttpGet (url, headers, queries);
        if (res_text.has_value ())
        {
            Json     j = Json::parse (res_text.value ());
            Response r {};
            j.get_to (r);
            res = std::move (r);
        }

        return res;
    }

}  // namespace Http
#endif /* HTTP_TOOLS_HPP */
