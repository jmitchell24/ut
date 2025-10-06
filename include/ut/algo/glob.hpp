//
// Created by james on 08/05/25.
//

#pragma once

//
// std
//
#include <string>
#include <vector>

namespace ut
{
    /**
     * @brief Match a string against a glob pattern using Bash-style conventions
     *
     * Supports:
     * - '*': Matches any sequence of characters (including empty sequence)
     * - '?': Matches any single character
     *
     * @param pattern The glob pattern to match against
     * @param str The string to check
     * @return bool True if the string matches the pattern, false otherwise
     */
    bool globMatch(std::string const& pattern, std::string const& str)
    {
        // Dynamic programming solution using 1D array
        // We only need to keep track of the current row of the DP table
        std::vector<int> dp(pattern.length() + 1, false);

        // Empty pattern matches empty string
        dp[0] = true;

        // Empty pattern can match non-empty string only if pattern starts with '*'
        for (size_t j = 1; j <= pattern.length(); j++)
        {
            if (pattern[j - 1] == '*')
            {
                dp[j] = dp[j - 1];
            }
        }

        // Process each character of the string
        for (size_t i = 1; i <= str.length(); i++)
        {
            // Keep track of the diagonal element (dp[i-1][j-1] in the 2D version)
            bool prev = dp[0];
            // Empty string can't match non-empty pattern unless the pattern consists only of '*'
            dp[0] = false;

            for (size_t j = 1; j <= pattern.length(); j++)
            {
                // Store the current dp[j] before we update it (will be diagonal in next iteration)
                bool temp = dp[j];
                char p = pattern[j - 1];
                char s = str[i - 1];

                if (p == '*')
                {
                    // '*' can match empty sequence or any sequence
                    // Either ignore the '*' (dp[j-1]) or use it to match the current character (previous dp[j])
                    dp[j] = dp[j - 1] || dp[j];
                }
                else if (p == '?' || p == s)
                {
                    // Single character match or '?' wildcard
                    dp[j] = prev;
                }
                else
                {
                    // No match
                    dp[j] = false;
                }

                // Update diagonal for next iteration
                prev = temp;
            }
        }

        return dp[pattern.length()];
    }
}