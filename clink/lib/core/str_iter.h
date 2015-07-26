// Copyright (c) 2015 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#pragma once

#include "str.h"

//------------------------------------------------------------------------------
template <typename TYPE>
class str_iter_impl
{
public:
                str_iter_impl(const TYPE* s) : m_ptr(s) {}
                str_iter_impl(const str_impl<TYPE>& s) : m_ptr(s.c_str()) {}
    int         next();

private:
    const TYPE* m_ptr;
};

//------------------------------------------------------------------------------
template <> inline int
str_iter_impl<char>::next()
{
    if (*m_ptr == '\0')
        return 0;

    int c;
    int ax = 0;
    int encode_length = 0;
    while (c = *m_ptr++)
    {
        ax = (ax << 6) | (c & 0x7f);
        if (encode_length)
        {
            --encode_length;
            continue;
        }

        if ((c & 0xc0) < 0xc0)
            break;

        if (encode_length = !!(c & 0x20))
            encode_length += !!(c & 0x10);

        ax &= (0x1f >> encode_length);
    }

    return ax;
}

//------------------------------------------------------------------------------
template <> inline int
str_iter_impl<wchar_t>::next()
{
    if (*m_ptr == '\0')
        return 0;

    int c = *m_ptr++;

    // Decode surrogate pairs.
    if ((c & 0xfc00) == 0xd800)
    {
        unsigned short d = *m_ptr;
        if ((d & 0xfc00) == 0xdc00)
        {
            c = (c << 10) + d - 0x35fdc00;
            ++m_ptr;
        }
    }

    return c;
}



//------------------------------------------------------------------------------
typedef str_iter_impl<char>     str_iter;
typedef str_iter_impl<wchar_t>  wstr_iter;
