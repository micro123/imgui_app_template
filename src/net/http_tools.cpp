#include "http_tools.hpp"
#include <cpr/cpr.h>
#include <optional>
#include "cpr/api.h"
#include "cpr/connect_timeout.h"
#include "cpr/cprtypes.h"
#include "cpr/parameters.h"
#include "cpr/ssl_options.h"
#include "cpr/timeout.h"


namespace Http
{
    namespace internal {
        std::optional<std::string> ExecuteHttpGet (const std::string &url, const HeaderMap &headers, const QueryMap &queries)
        {
            cpr::Parameters params;
            for (auto &x: queries)
            {
                params.Add({x.first, x.second});
            }

            cpr::Header header;
            for (auto &x: headers)
            {
                header[x.first] = x.second;
            }

            auto res = cpr::Get(cpr::Url(url), header, params, cpr::VerifySsl(false), cpr::Timeout(3000), cpr::ConnectTimeout(3000));
            if (res.status_code == 200) // OK
            {
                return res.text;
            }
            else
            {
                return std::nullopt;
            }
        }
    }
}
