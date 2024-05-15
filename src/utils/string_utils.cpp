#include "utils/string_utils.hpp"
#include <string>
#include "core/assert.hpp"

std::vector<std::string> split_string(const std::string &input, const std::string &spliter)
{
    FW_ASSERT(!spliter.empty());
    std::vector<std::string> result;
    
    std::string::size_type pos = 0, next = pos;

    while((pos = input.find(spliter, next)) != std::string::npos) {
        if (pos - next > 0)
            result.emplace_back(input.substr(next, pos - next));
        next = pos + spliter.length();
    }

    if (next < input.length())
        result.emplace_back(input.substr(next));

    return result;
}

std::string to_lower (const std::string &str)
{
    auto                                                                              result = str;
    std::transform (result.begin (), result.end (), result.begin (), [](unsigned char c) -> unsigned char {
        return static_cast<unsigned char> (std::tolower (c));
    });

    return result;
}

std::string to_upper (const std::string &str)
{
    auto                                                                              result = str;
    std::transform (result.begin (), result.end (), result.begin (), [](unsigned char c) -> unsigned char {
        return static_cast<unsigned char> (std::toupper (c));
    });

    return result;
}

std::string capitalize (const std::string &str)
{
    auto result = str;
    if (!result.empty ())
    {
        result.front () = static_cast<char> (std::toupper (result.front ()));
    }

    return result;
}

std::string capitalize_first_char (const std::string &str)
{
    auto result = to_lower (str);
    if (!result.empty ())
    {
        result.front () = static_cast<char> (std::toupper (result.front ()));
    }

    return result;
}

bool contains (const std::string &str, const std::string &substring)
{
    return str.find (substring) != std::string::npos;
}

bool contains (const std::string &str, const char character)
{
    return contains (str, std::string (1, character));
}

bool compare_ignore_case (const std::string &str1, const std::string &str2)
{
    return to_lower (str1) == to_lower (str2);
}

void trim_left (std::string &str)
{
    str.erase (str.begin (), std::find_if (str.begin (), str.end (), [](int ch) {
        return !std::isspace (ch);
    }));
}

void trim_right (std::string &str)
{
    str.erase (std::find_if (str.rbegin (), str.rend (), [](int ch) {
        return !std::isspace (ch);
    }).base (), str.end ());
}

void trim (std::string &str)
{
    trim_left (str);
    trim_right (str);
}

std::string trim_left_copy (std::string str)
{
    trim_left (str);
    return str;
}

std::string trim_right_copy (std::string str)
{
    trim_right (str);
    return str;
}

std::string trim_copy (std::string str)
{
    trim (str);
    return str;
}

bool replace_first (std::string &str, const std::string &target, const std::string &replacement)
{
    const size_t start_pos = str.find (target);
    if (start_pos == std::string::npos)
    {
        return false;
    }

    str.replace (start_pos, target.length (), replacement);
    return true;
}

bool replace_last (std::string &str, const std::string &target, const std::string &replacement)
{
    size_t start_pos = str.rfind (target);
    if (start_pos == std::string::npos)
    {
        return false;
    }

    str.replace (start_pos, target.length (), replacement);
    return true;
}

bool replace_all (std::string &str, const std::string &target, const std::string &replacement)
{
    if (target.empty ())
    {
        return false;
    }

    size_t     start_pos       = 0;
    const bool found_substring = str.find (target, start_pos) != std::string::npos;

    while ((start_pos = str.find (target, start_pos)) != std::string::npos)
    {
        str.replace (start_pos, target.length (), replacement);
        start_pos += replacement.length ();
    }

    return found_substring;
}

bool ends_with (const std::string &str, const std::string &suffix)
{
    const auto suffix_start = str.size () - suffix.size ();
    const auto result       = str.find (suffix, suffix_start);
    return (result == suffix_start) && (result != std::string::npos);
}

bool ends_with (const std::string &str, const char suffix)
{
    return !str.empty () && (str.back () == suffix);
}

bool starts_with (const std::string &str, const std::string &prefix)
{
    return str.rfind (prefix, 0) == 0;
}

bool starts_with (const std::string &str, const char prefix)
{
    return !str.empty () && (str.front () == prefix);
}

std::vector<std::string> split (const std::string &str, const char delim)
{
    std::vector<std::string> tokens;
    std::stringstream        ss (str);

    std::string token;
    while (std::getline (ss, token, delim))
    {
        tokens.push_back (token);
    }

    // Match semantics of split(str,str)
    if (str.empty () || ends_with (str, delim))
    {
        tokens.emplace_back ();
    }

    return tokens;
}

std::vector<std::string> split (const std::string &str, const std::string &delim)
{
    size_t                   pos_start = 0, pos_end, delim_len = delim.length ();
    std::string              token;
    std::vector<std::string> tokens;

    while ((pos_end = str.find (delim, pos_start)) != std::string::npos)
    {
        token     = str.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        tokens.push_back (token);
    }

    tokens.push_back (str.substr (pos_start));
    return tokens;
}

std::vector<std::string> regex_split (const std::string &src, const std::string &rgx_str)
{
    std::vector<std::string>   elems;
    const std::regex           rgx (rgx_str);
    std::sregex_token_iterator iter (src.begin (), src.end (), rgx, -1);
    std::sregex_token_iterator end;
    while (iter != end)
    {
        elems.push_back (*iter);
        ++iter;
    }
    return elems;
}

std::map<std::string, std::string> regex_split_map (const std::string &src, const std::string &rgx_str)
{
    std::map<std::string, std::string> dest;
    std::string                        tstr = src + " ";
    std::regex                         rgx (rgx_str);
    std::sregex_token_iterator         niter (tstr.begin (), tstr.end (), rgx);
    std::sregex_token_iterator         viter (tstr.begin (), tstr.end (), rgx, -1);
    std::sregex_token_iterator         end;
    ++viter;
    while (niter != end)
    {
        dest[*niter] = *viter;
        ++niter;
        ++viter;
    }

    return dest;
}

std::vector<std::string> split_any (const std::string &str, const std::string &delims)
{
    std::string              token;
    std::vector<std::string> tokens;

    size_t pos_start = 0;
    for (size_t pos_end = 0; pos_end < str.length (); ++pos_end)
    {
        if (contains (delims, str[pos_end]))
        {
            token = str.substr (pos_start, pos_end - pos_start);
            tokens.push_back (token);
            pos_start = pos_end + 1;
        }
    }

    tokens.push_back (str.substr (pos_start));
    return tokens;
}

void drop_empty (std::vector<std::string> &tokens)
{
    auto last = std::remove_if (tokens.begin (), tokens.end (), [](const std::string &s) {
        return s.empty ();
    });
    tokens.erase (last, tokens.end ());
}

std::vector<std::string> drop_empty_copy (std::vector<std::string> tokens)
{
    drop_empty (tokens);
    return tokens;
}

void drop_duplicate (std::vector<std::string> &tokens)
{
    std::sort (tokens.begin (), tokens.end ());
    auto end_unique = std::unique (tokens.begin (), tokens.end ());
    tokens.erase (end_unique, tokens.end ());
}

std::vector<std::string> drop_duplicate_copy (std::vector<std::string> tokens)
{
    std::sort (tokens.begin (), tokens.end ());
    auto end_unique = std::unique (tokens.begin (), tokens.end ());
    tokens.erase (end_unique, tokens.end ());
    return tokens;
}

std::string repeat (const std::string &str, unsigned n)
{
    std::string result;

    for (unsigned i = 0; i < n; ++i)
    {
        result += str;
    }

    return result;
}

std::string repeat (char c, unsigned n)
{
    return std::string (n, c);
}

bool matches (const std::string &str, const std::regex &regex)
{
    return std::regex_match (str, regex);
}

template <typename T>
void sorting_descending (std::vector<T> &strs)
{
    std::sort (strs.begin (), strs.end (), std::greater<T> ());
}


