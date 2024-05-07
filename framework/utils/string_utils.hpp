#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include "config.h"
#include <vector>
#include <string>
#include <regex>
#include <sstream>

std::vector<std::string>  split_string (const std::string &input, const std::string &spliter);

/**
 * @brief Converts std::string to lower case.
 * @param str - std::string that needs to be converted.
 * @return Lower case input std::string.
 */
 std::string to_lower (const std::string &str);

/**
 * @brief Converts std::string to upper case.
 * @param str - std::string that needs to be converted.
 * @return Upper case input std::string.
 */
 std::string to_upper (const std::string &str);

/**
 * @brief Converts the first character of a string to uppercase letter and lowercases all other characters, if any.
 * @param str - input string to be capitalized.
 * @return A string with the first letter capitalized and all other characters lowercased. It doesn't modify the input string.
 */
 std::string capitalize (const std::string &str);

/**
 * @brief Converts only the first character of a string to uppercase letter, all other characters stay unchanged.
 * @param str - input string to be modified.
 * @return A string with the first letter capitalized. All other characters stay unchanged. It doesn't modify the input string.
 */
 std::string capitalize_first_char (const std::string &str);

/**
 * @brief Checks if input std::string str contains specified substring.
 * @param str - std::string to be checked.
 * @param substring - searched substring.
 * @return True if substring was found in str, false otherwise.
 */
 bool contains (const std::string &str, const std::string &substring);

/**
 * @brief Checks if input std::string str contains specified character.
 * @param str - std::string to be checked.
 * @param character - searched character.
 * @return True if character was found in str, false otherwise.
 */
 bool contains (const std::string &str, const char character);

/**
 * @brief Compares two std::strings ignoring their case (lower/upper).
 * @param str1 - std::string to compare
 * @param str2 - std::string to compare
 * @return True if str1 and str2 are equal, false otherwise.
 */
 bool compare_ignore_case (const std::string &str1, const std::string &str2);

/**
 * @brief Trims (in-place) white spaces from the left side of std::string.
 *        Taken from: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring.
 * @param str - input std::string to remove white spaces from.
 */
 void trim_left (std::string &str);

/**
 * @brief Trims (in-place) white spaces from the right side of std::string.
 *        Taken from: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring.
 * @param str - input std::string to remove white spaces from.
 */
 void trim_right (std::string &str);

/**
 * @brief Trims (in-place) white spaces from the both sides of std::string.
 *        Taken from: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring.
 * @param str - input std::string to remove white spaces from.
 */
 void trim (std::string &str);

/**
 * @brief Trims white spaces from the left side of std::string.
 *        Taken from: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring.
 * @param str - input std::string to remove white spaces from.
 * @return Copy of input str with trimmed white spaces.
 */
 std::string trim_left_copy (std::string str);

/**
  * @brief Trims white spaces from the right side of std::string.
  *        Taken from: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring.
  * @param str - input std::string to remove white spaces from.
  * @return Copy of input str with trimmed white spaces.
  */
 std::string trim_right_copy (std::string str);

/**
  * @brief Trims white spaces from the both sides of std::string.
  *        Taken from: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring.
  * @param str - input std::string to remove white spaces from.
  * @return Copy of input str with trimmed white spaces.
  */
 std::string trim_copy (std::string str);

/**
 * @brief Replaces (in-place) the first occurrence of target with replacement.
 *        Taken from: http://stackoverflow.com/questions/3418231/c-replace-part-of-a-string-with-another-string.
 * @param str - input std::string that will be modified.
 * @param target - substring that will be replaced with replacement.
 * @param replacement - substring that will replace target.
 * @return True if replacement was successfull, false otherwise.
 */
 bool replace_first (std::string &str, const std::string &target, const std::string &replacement);

/**
 * @brief Replaces (in-place) last occurrence of target with replacement.
 *        Taken from: http://stackoverflow.com/questions/3418231/c-replace-part-of-a-string-with-another-string.
 * @param str - input std::string that will be modified.
 * @param target - substring that will be replaced with replacement.
 * @param replacement - substring that will replace target.
 * @return True if replacement was successfull, false otherwise.
 */
 bool replace_last (std::string &str, const std::string &target, const std::string &replacement);

/**
 * @brief Replaces (in-place) all occurrences of target with replacement.
 *        Taken from: http://stackoverflow.com/questions/3418231/c-replace-part-of-a-string-with-another-string.
 * @param str - input std::string that will be modified.
 * @param target - substring that will be replaced with replacement.
 * @param replacement - substring that will replace target.
 * @return True if replacement was successfull, false otherwise.
 */
 bool replace_all (std::string &str, const std::string &target, const std::string &replacement);

/**
 * @brief Checks if std::string str ends with specified suffix.
 * @param str - input std::string that will be checked.
 * @param suffix - searched suffix in str.
 * @return True if suffix was found, false otherwise.
 */
 bool ends_with (const std::string &str, const std::string &suffix);

/**
 * @brief Checks if std::string str ends with specified character.
 * @param str - input std::string that will be checked.
 * @param suffix - searched character in str.
 * @return True if ends with character, false otherwise.
 */
 bool ends_with (const std::string &str, const char suffix);

/**
 * @brief Checks if std::string str starts with specified prefix.
 * @param str - input std::string that will be checked.
 * @param prefix - searched prefix in str.
 * @return True if prefix was found, false otherwise.
 */
 bool starts_with (const std::string &str, const std::string &prefix);

/**
 * @brief Checks if std::string str starts with specified character.
 * @param str - input std::string that will be checked.
 * @param prefix - searched character in str.
 * @return True if starts with character, false otherwise.
 */
 bool starts_with (const std::string &str, const char prefix);

/**
 * @brief Splits input std::string str according to input delim.
 * @param str - std::string that will be splitted.
 * @param delim - the delimiter.
 * @return std::vector<std::string> that contains all splitted tokens.
 */
 std::vector<std::string> split (const std::string &str, const char delim);

/**
 * @brief Splits input std::string str according to input std::string delim.
 *        Taken from: https://stackoverflow.com/a/46931770/1892346.
 * @param str - std::string that will be split.
 * @param delim - the delimiter.
 * @return std::vector<std::string> that contains all splitted tokens.
 */
 std::vector<std::string> split (const std::string &str, const std::string &delim);

/**
 * @brief Splits input string using regex as a delimiter.
 * @param src - std::string that will be split.
 * @param rgx_str - the set of delimiter characters.
 * @return vector of resulting tokens.
 */
 std::vector<std::string> regex_split (const std::string &src, const std::string &rgx_str);

/**
 * @brief Splits input string using regex as a delimiter.
 * @param src - std::string that will be split.
 * @param dest - map of matched delimiter and those being splitted.
 * @param rgx_str - the set of delimiter characters.
 * @return True if the parsing is successfully done.
 */
 std::map<std::string, std::string> regex_split_map (const std::string &src, const std::string &rgx_str);

/**
 * @brief Splits input string using any delimiter in the given set.
 * @param str - std::string that will be split.
 * @param delims - the set of delimiter characters.
 * @return vector of resulting tokens.
 */
 std::vector<std::string> split_any (const std::string &str, const std::string &delims);

/**
 * @brief Joins all elements of std::vector tokens of arbitrary datatypes
 *        into one std::string with delimiter delim.
 * @tparam T - arbitrary datatype.
 * @param tokens - vector of tokens.
 * @param delim - the delimiter.
 * @return std::string with joined elements of vector tokens with delimiter delim.
 */
template <typename T>
static inline std::string join (const std::vector<T> &tokens, const std::string &delim)
{
    std::ostringstream result;
    for (auto it = tokens.begin (); it != tokens.end (); ++it)
    {
        if (it != tokens.begin ())
        {
            result << delim;
        }

        result << *it;
    }

    return result.str ();
}

/**
 * @brief Inplace removal of all empty strings in a vector<string>
 * @param tokens - vector of strings.
 */
 void drop_empty (std::vector<std::string> &tokens);

/**
 * @brief Inplace removal of all empty strings in a vector<string>
 * @param tokens - vector of strings.
 * @return vector of non-empty tokens.
 */
 std::vector<std::string> drop_empty_copy (std::vector<std::string> tokens);

/**
 * @brief Inplace removal of all duplicate strings in a vector<string> where order is not to be maintained
 *        Taken from: C++ Primer V5
 * @param tokens - vector of strings.
 * @return vector of non-duplicate tokens.
 */
 void drop_duplicate (std::vector<std::string> &tokens);

/**
 * @brief Removal of all duplicate strings in a vector<string> where order is not to be maintained
 *        Taken from: C++ Primer V5
 * @param tokens - vector of strings.
 * @return vector of non-duplicate tokens.
 */
 std::vector<std::string> drop_duplicate_copy (std::vector<std::string> tokens);

/**
 * @brief Creates new std::string with repeated n times substring str.
 * @param str - substring that needs to be repeated.
 * @param n - number of iterations.
 * @return std::string with repeated substring str.
 */
 std::string repeat (const std::string &str, unsigned n);

/**
 * @brief Creates new std::string with repeated n times char c.
 * @param c - char that needs to be repeated.
 * @param n - number of iterations.
 * @return std::string with repeated char c.
 */
 std::string repeat (char c, unsigned n);

/**
 * @brief Checks if input std::string str matches specified reular expression regex.
 * @param str - std::string to be checked.
 * @param regex - the std::regex regular expression.
 * @return True if regex matches str, false otherwise.
 */
 bool matches (const std::string &str, const std::regex &regex);

/**
 * @brief Sort input std::vector<std::string> strs in ascending order.
 * @param strs - std::vector<std::string> to be checked.
 */
template <typename T>
static inline void sorting_ascending (std::vector<T> &strs)
{
    std::sort (strs.begin (), strs.end ());
}

/**
 * @brief Sorted input std::vector<std::string> strs in descending order.
 * @param strs - std::vector<std::string> to be checked.
 */
template <typename T>
static inline void sorting_descending (std::vector<T> &strs);

/**
 * @brief Reverse input std::vector<std::string> strs.
 * @param strs - std::vector<std::string> to be checked.
 */
template <typename T>
static inline void reverse_inplace (std::vector<T> &strs)
{
    std::reverse (strs.begin (), strs.end ());
}

/**
 * @brief Reverse input std::vector<std::string> strs.
 * @param strs - std::vector<std::string> to be checked.
 */
template <typename T>
static inline std::vector<T> reverse_copy (std::vector<T> strs)
{
    std::reverse (strs.begin (), strs.end ());
    return strs;
}


#endif // STRING_UTILS_HPP
