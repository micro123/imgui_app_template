#include <list>

#include <net/http_tools.hpp>
#include "core/log.hpp"
#include "fmt/format.h"
#include "utils/json.hpp"

struct WallhavenResult {
    struct WallhavenImage {
        std::string id;
        std::string url;
        std::string short_url;
        long dimension_x;
        long dimension_y;
        long file_size;
        std::string path;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WallhavenImage, id, url, short_url, dimension_x, dimension_y, file_size, path)
    };

    std::list<WallhavenImage> data;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WallhavenResult, data);
};

void TestHttpGet()
{
    auto r = Http::Get<WallhavenResult>("https://wallhaven.cc/api/v1/search", {}, {{"sorting","random"}});
    if (r.has_value()) {
        auto &v = r.value();
        L_INFO("result count = %u", v.data.size());
        for (auto const &x: v.data) {
            auto text = fmt::format("  id: {}, url: {}, size: {}x{},{}B, file_url: {}", x.id, x.short_url, x.dimension_x, x.dimension_y, x.file_size, x.path);
            L_INFO("%s", text.c_str());
        }
    }
    else
    {
        L_ERROR("Failed!");
    }
}
