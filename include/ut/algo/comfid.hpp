//
// Created by james on 04/12/25.
//


#include "ut/bit.hpp"

namespace ut
{

    /// \brief Comfy IDs: Unique identifiers that are easy to read.
    /// Stored as an unsigned 32-bit int, mapped 1-to-1 with a unique string
    /// Format (braced element replaced with a word): [adjective1]-[noun1]-[adjective2]-[noun2]
    struct ComfID
    {
        b32 value=0;

        ComfID()=default;
        ComfID(b32 value)
            : value(value)
        { }

        operator b64() const
        { return value; }


        std::string toString() const;
        static ComfID fromString(std::string const& id);

        static void printDetBits();

    };
}