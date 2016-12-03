/*
 * prjsv 2015
 * 2013, 2014
 * Marco Antognini
 */

#include <algorithm>

template <typename C, typename D>
void append(C const& src, D& dest)
{
    append(src.begin(), src.end(), dest);
}

template <typename C, typename InputIt>
void append(InputIt first, InputIt last, C& dest)
{
    std::copy(first, last, std::back_inserter(dest));
}

template <typename Map, typename F>
void map_erase_if(Map& m, F pred)
{
    for (typename Map::iterator i = m.begin();
         (i = std::find_if(i, m.end(), pred)) != m.end();
         m.erase(i++)) {}
}
