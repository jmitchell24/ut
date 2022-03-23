#pragma once

#include <vector>
#include <memory>
#include <istream>
#include <stdexcept>
#include <string_view>

#include <cstdio>
#include <cassert>

namespace ut
{
    template <typename Char, typename Traits=std::char_traits<Char>>
    struct basic_string_metic;

    template <typename Char, typename Traits>
    struct basic_string_metic
    {
        using char_type         = Char;
        using traits_type       = Traits;
        using size_type         = std::size_t;
        using stringview_type   = std::basic_string_view<char_type, traits_type>;


        // Levenshtein distance
        // https://en.wikipedia.org/wiki/Levenshtein_distance

        static size_type editDistance(stringview_type source, stringview_type target,
                                     size_type d_insert  =1,
                                     size_type d_delete  =1,
                                     size_type d_replace =1)
        {
            if (source.size() > target.size())
            {
                std::swap(source, target);
                std::swap(d_insert, d_delete);
            }

            size_type sz_source = size_type(source.size());
            size_type sz_target = size_type(target.size());

            std::vector<size_type> v(sz_source + 1);

            v[0] = 0;
            for (size_type i = 1; i <= sz_source; ++i)
            {
                v[i] = v[i - 1] + d_delete;
            }

            for (size_type y = 1; y <= sz_target; ++y)
            {
                size_type cell_xy = v[0];

                v[0] += d_insert;
                for (size_type x = 1; x <= sz_source; ++x)
                {
                    //       x-1       x
                    //     ┌─────────┬────────┐
                    // y-1 │ cell_xy │ cell_y │
                    //     ├─────────┼────────┤
                    // y   │ cell_x  │ cell   │
                    //     └─────────┴────────┘

                    size_type& cell   = v[x];           // cell to be computed
                    size_type  cell_y = cell;           // computed cell from the previous row

                    if (source[x-1] == target[y-1])
                    {
                        cell = cell_xy;                 // no added distance if chars are equal
                    }
                    else
                    {
                        size_type cell_x = v[x-1];      // computed cell from the previous column

                        cell = std::min(std::min(
                            cell_x  + d_delete,
                            cell_y  + d_insert),
                            cell_xy + d_replace
                        );                              // compute current cell
                    }

                    cell_xy = cell_y;                   // cell_y becomes cell_yx in the next column
                }
            }

            return v[sz_source];
        }
    };

    using string_metric = basic_string_metic<char>;




}
