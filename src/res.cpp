
// Created by james on 24/09/25.
//

#include "ut/res.hpp"
#include "ut/time.hpp"
#include "ut/algo/gulp.hpp"
using namespace ut;

//
// std
//
#include <cstring>
#include <cstddef>
#include <vector>
#include <cstdint>
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <format>
#include <unordered_set>
#include <fstream>
using namespace std;

// stb_compress* from stb.h - declaration
typedef unsigned int stb_uint;
typedef unsigned char stb_uchar;
stb_uint stb_compress(stb_uchar* out, stb_uchar* in, stb_uint len);

// stb_compress* from stb.h - definition

////////////////////           compressor         ///////////////////////

static stb_uint stb_adler32(stb_uint adler32, stb_uchar *buffer, stb_uint buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen, i;

    blocklen = buflen % 5552;
    while (buflen) {
        for (i=0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (s2 << 16) + s1;
}

static unsigned int stb_matchlen(stb_uchar *m1, stb_uchar *m2, stb_uint maxlen)
{
    stb_uint i;
    for (i=0; i < maxlen; ++i)
        if (m1[i] != m2[i]) return i;
    return i;
}

// simple implementation that just takes the source data in a big block

static stb_uchar *stb__out;
static FILE      *stb__outfile;
static stb_uint   stb__outbytes;

static void stb__write(unsigned char v)
{
    fputc(v, stb__outfile);
    ++stb__outbytes;
}

//#define stb_out(v)    (stb__out ? *stb__out++ = (stb_uchar) (v) : stb__write((stb_uchar) (v)))
#define stb_out(v)    do { if (stb__out) *stb__out++ = (stb_uchar) (v); else stb__write((stb_uchar) (v)); } while (0)

static void stb_out2(stb_uint v) { stb_out(v >> 8); stb_out(v); }
static void stb_out3(stb_uint v) { stb_out(v >> 16); stb_out(v >> 8); stb_out(v); }
static void stb_out4(stb_uint v) { stb_out(v >> 24); stb_out(v >> 16); stb_out(v >> 8 ); stb_out(v); }

static void outliterals(stb_uchar *in, int numlit)
{
    while (numlit > 65536) {
        outliterals(in,65536);
        in     += 65536;
        numlit -= 65536;
    }

    if      (numlit ==     0)    ;
    else if (numlit <=    32)    stb_out (0x000020 + numlit-1);
    else if (numlit <=  2048)    stb_out2(0x000800 + numlit-1);
    else /*  numlit <= 65536) */ stb_out3(0x070000 + numlit-1);

    if (stb__out) {
        memcpy(stb__out,in,numlit);
        stb__out += numlit;
    } else
        fwrite(in, 1, numlit, stb__outfile);
}

static int stb__window = 0x40000; // 256K

static int stb_not_crap(int best, int dist)
{
    return   ((best > 2  &&  dist <= 0x00100)
        || (best > 5  &&  dist <= 0x04000)
        || (best > 7  &&  dist <= 0x80000));
}

static  stb_uint stb__hashsize = 32768;

// note that you can play with the hashing functions all you
// want without needing to change the decompressor
#define stb__hc(q,h,c)      (((h) << 7) + ((h) >> 25) + q[c])
#define stb__hc2(q,h,c,d)   (((h) << 14) + ((h) >> 18) + (q[c] << 7) + q[d])
#define stb__hc3(q,c,d,e)   ((q[c] << 14) + (q[d] << 7) + q[e])

static unsigned int stb__running_adler;

static int stb_compress_chunk(stb_uchar *history,
    stb_uchar *start,
    stb_uchar *end,
    int length,
    int *pending_literals,
    stb_uchar **chash,
    stb_uint mask)
{
    (void)history;
    int window = stb__window;
    stb_uint match_max;
    stb_uchar *lit_start = start - *pending_literals;
    stb_uchar *q = start;

#define STB__SCRAMBLE(h)   (((h) + ((h) >> 16)) & mask)

    // stop short of the end so we don't scan off the end doing
    // the hashing; this means we won't compress the last few bytes
    // unless they were part of something longer
    while (q < start+length && q+12 < end) {
        int m;
        stb_uint h1,h2,h3,h4, h;
        stb_uchar *t;
        int best = 2, dist=0;

        if (q+65536 > end)
            match_max = (stb_uint)(end-q);
        else
            match_max = 65536;

#define stb__nc(b,d)  ((d) <= window && ((b) > 9 || stb_not_crap((int)(b),(int)(d))))

#define STB__TRY(t,p)  /* avoid retrying a match we already tried */ \
    if (p ? dist != (int)(q-t) : 1)                             \
    if ((m = stb_matchlen(t, q, match_max)) > best)     \
    if (stb__nc(m,q-(t)))                                \
    best = m, dist = (int)(q - (t))

        // rather than search for all matches, only try 4 candidate locations,
        // chosen based on 4 different hash functions of different lengths.
        // this strategy is inspired by LZO; hashing is unrolled here using the
        // 'hc' macro
        h = stb__hc3(q,0, 1, 2); h1 = STB__SCRAMBLE(h);
        t = chash[h1]; if (t) STB__TRY(t,0);
        h = stb__hc2(q,h, 3, 4); h2 = STB__SCRAMBLE(h);
        h = stb__hc2(q,h, 5, 6);        t = chash[h2]; if (t) STB__TRY(t,1);
        h = stb__hc2(q,h, 7, 8); h3 = STB__SCRAMBLE(h);
        h = stb__hc2(q,h, 9,10);        t = chash[h3]; if (t) STB__TRY(t,1);
        h = stb__hc2(q,h,11,12); h4 = STB__SCRAMBLE(h);
        t = chash[h4]; if (t) STB__TRY(t,1);

        // because we use a shared hash table, can only update it
        // _after_ we've probed all of them
        chash[h1] = chash[h2] = chash[h3] = chash[h4] = q;

        if (best > 2)
            assert(dist > 0);

        // see if our best match qualifies
        if (best < 3) { // fast path literals
            ++q;
        } else if (best > 2  &&  best <= 0x80    &&  dist <= 0x100) {
            outliterals(lit_start, (int)(q-lit_start)); lit_start = (q += best);
            stb_out(0x80 + best-1);
            stb_out(dist-1);
        } else if (best > 5  &&  best <= 0x100   &&  dist <= 0x4000) {
            outliterals(lit_start, (int)(q-lit_start)); lit_start = (q += best);
            stb_out2(0x4000 + dist-1);
            stb_out(best-1);
        } else if (best > 7  &&  best <= 0x100   &&  dist <= 0x80000) {
            outliterals(lit_start, (int)(q-lit_start)); lit_start = (q += best);
            stb_out3(0x180000 + dist-1);
            stb_out(best-1);
        } else if (best > 8  &&  best <= 0x10000 &&  dist <= 0x80000) {
            outliterals(lit_start, (int)(q-lit_start)); lit_start = (q += best);
            stb_out3(0x100000 + dist-1);
            stb_out2(best-1);
        } else if (best > 9                      &&  dist <= 0x1000000) {
            if (best > 65536) best = 65536;
            outliterals(lit_start, (int)(q-lit_start)); lit_start = (q += best);
            if (best <= 0x100) {
                stb_out(0x06);
                stb_out3(dist-1);
                stb_out(best-1);
            } else {
                stb_out(0x04);
                stb_out3(dist-1);
                stb_out2(best-1);
            }
        } else {  // fallback literals if no match was a balanced tradeoff
            ++q;
        }
    }

    // if we didn't get all the way, add the rest to literals
    if (q-start < length)
        q = start+length;

    // the literals are everything from lit_start to q
    *pending_literals = (int)(q - lit_start);

    stb__running_adler = stb_adler32(stb__running_adler, start, (stb_uint)(q - start));
    return (int)(q - start);
}

static int stb_compress_inner(stb_uchar *input, stb_uint length)
{
    int literals = 0;
    stb_uint i;

    stb_uchar **chash;
    chash = (stb_uchar**) malloc(stb__hashsize * sizeof(stb_uchar*));
    if (chash == nullptr) return 0; // failure
    for (i=0; i < stb__hashsize; ++i)
        chash[i] = nullptr;

    // stream signature
    stb_out(0x57); stb_out(0xbc);
    stb_out2(0);

    stb_out4(0);       // 64-bit length requires 32-bit leading 0
    stb_out4(length);
    stb_out4(stb__window);

    stb__running_adler = 1;

#ifdef NDEBUG
    stb_uint len = stb_compress_chunk(input, input, input+length, length, &literals, chash, stb__hashsize-1);
    assert(len == length);
#else
    stb_compress_chunk(input, input, input+length, length, &literals, chash, stb__hashsize-1);
#endif


    outliterals(input+length - literals, literals);

    free(chash);

    stb_out2(0x05fa); // end opcode

    stb_out4(stb__running_adler);

    return 1; // success
}

stb_uint stb_compress(stb_uchar *out, stb_uchar *input, stb_uint length)
{
    stb__out = out;
    stb__outfile = nullptr;

    stb_compress_inner(input, length);

    return (stb_uint)(stb__out - out);
}

//-----------------------------------------------------------------------------
// [SECTION] Decompression code
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array and encoded as base85.
// Use the program in misc/fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------

static unsigned int stb_decompress_length(const unsigned char *input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char *stb__barrier_out_e, *stb__barrier_out_b;
static const unsigned char *stb__barrier_in_b;
static unsigned char *stb__dout;
static void stb__match(const unsigned char *data, unsigned int length)
{
    // INVERSE of memmove... write each byte before copying the next...
    assert(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_out_b) { stb__dout = stb__barrier_out_e+1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char *data, unsigned int length)
{
    assert(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_in_b) { stb__dout = stb__barrier_out_e+1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static const unsigned char *stb_decompress_token(const unsigned char *i)
{
    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       stb__match(stb__dout-i[1]-1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  stb__match(stb__dout-(stb__in2(0) - 0x4000 + 1), i[2]+1), i += 3;
        else /* *i >= 0x20 */ stb__lit(i+1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    } else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       stb__match(stb__dout-(stb__in3(0) - 0x180000 + 1), i[3]+1), i += 4;
        else if (*i >= 0x10)  stb__match(stb__dout-(stb__in3(0) - 0x100000 + 1), stb__in2(3)+1), i += 5;
        else if (*i >= 0x08)  stb__lit(i+2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  stb__lit(i+3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)  stb__match(stb__dout-(stb__in3(1)+1), i[4]+1), i += 5;
        else if (*i == 0x04)  stb__match(stb__dout-(stb__in3(1)+1), stb__in2(4)+1), i += 6;
    }
    return i;
}

static unsigned int stb_decompress(unsigned char *output, const unsigned char *i, unsigned int /*length*/)
{
    if (stb__in4(0) != 0x57bC0000) return 0;
    if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
    const unsigned int olen = stb_decompress_length(i);
    stb__barrier_in_b = i;
    stb__barrier_out_e = output + olen;
    stb__barrier_out_b = output;
    i += 16;

    stb__dout = output;
    for (;;) {
        const unsigned char *old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                assert(stb__dout == output + olen);
                if (stb__dout != output + olen) return 0;
                if (stb_adler32(1, output, olen) != (unsigned int) stb__in4(2))
                    return 0;
                return olen;
            } else {
                assert(0); /* NOTREACHED */
                return 0;
            }
        }
        assert(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

static char Encode85Byte(unsigned int x)
{
    x = (x % 85) + 35;
    return (char)((x >= '\\') ? x + 1 : x);
}

static unsigned int Decode85Byte(char c)
{
    return c >= '\\' ? c-36 : c-35;
}

static void Decode85(const unsigned char* src, unsigned char* dst)
{
    while (*src)
    {
        unsigned int tmp = Decode85Byte(src[0]) + 85 * (Decode85Byte(src[1]) + 85 * (Decode85Byte(src[2]) + 85 * (Decode85Byte(src[3]) + 85 * Decode85Byte(src[4]))));
        dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
        src += 5;
        dst += 4;
    }
}

static bool isValidCPPName(strparam s)
{
    if (s.empty())
        return false;

    // First character must be a letter or underscore
    if (!isalpha(s[0]) && s[0] != '_')
        return false;

    // Remaining characters must be letters, digits, or underscores
    for (size_t i = 1; i < s.size(); ++i)
        if (!isalnum(s[i]) && s[i] != '_')
            return false;

    // Check if it's a C++ keyword
    static const unordered_set<strparam> keywords = {
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
        "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
        "class", "compl", "concept", "const", "consteval", "constexpr", "constinit",
        "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype",
        "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
        "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
        "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
        "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
        "public", "register", "reinterpret_cast", "requires", "return", "short",
        "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
        "switch", "template", "this", "thread_local", "throw", "true", "try",
        "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
        "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
    };

    if (keywords.contains(s))
        return false;

    return true;
}

static bool isValidCPPNamespace(strparam s)
{
    for (auto&& it: strview(s).split("::"))
        if (!isValidCPPName(it))
            return false;
    return true;
}

SrcRes::SrcRes(string name, string ns, size_t wrap)
    : m_name{std::move(name)}, m_namespace{std::move(ns)}, m_wrap{wrap}
{
    if (!isValidCPPName(m_name))
        throw runtime_error{"Invalid CPP Name: " + m_name};

    if (!m_namespace.empty() && !isValidCPPNamespace(m_namespace))
        throw runtime_error{"Invalid CPP Namespace: " + m_namespace};
}

SrcRes::Src SrcRes::encode(void const* data_in, size_t data_size) const
{
    if (data_in == nullptr)
        return {};

    if (data_size == 0)
        return {};

    int data_sz = data_size;
    char const* data = reinterpret_cast<char const*>(data_in);

    // Compress
    int maxlen = data_sz + 512 + (data_sz >> 2) + sizeof(int); // total guess
    vector<uint8_t> compressed;
    compressed.resize(maxlen);

    int compressed_sz = stb_compress(compressed.data(), (stb_uchar*)data, data_sz);


    ostringstream oss;

    oss << "\"";

    char prev_c = 0;
    for (int src_i = 0; src_i < compressed_sz; src_i += 4)
    {
        // This is made a little more complicated by the fact that ??X sequences are interpreted as trigraphs by old C/C++ compilers. So we need to escape pairs of ??.
        unsigned int d = *(unsigned int*)(compressed.data() + src_i);
        for (unsigned int n5 = 0; n5 < 5; n5++, d /= 85)
        {
            char c = Encode85Byte(d);

            if (c == '?' && prev_c == '?')
                oss << "\\";
            oss << c;

            prev_c = c;
        }

        if ((src_i % m_wrap) == m_wrap - 4)
            oss << "\"\n    \"";
    }
    oss << "\"_sv";


    Src src;

    if (m_namespace.empty())
    {
        src.decl = format(R"(
            |// encoded with <ut/res.hpp> ({0})
            |struct {1}
            |{{
            |    constexpr static size_t DECODED_SIZE = {2};
            |    const static ut::cstrview ENCODED_DATA;
            |}};
            )", local_datetime::now().str("%Y-%m-%d, %I:%M %p"), m_name, data_size);

        src.impl = format(R"(
            |cstrview const {0}::ENCODED_DATA =
            |{1};
            )", m_name, oss.str());
    }
    else
    {
        src.decl = format(R"(
            |// encoded with <ut/res.hpp> ({0})
            |namespace {1}
            |{{
            |    struct {2}
            |    {{
            |        constexpr static size_t DECODED_SIZE = {3};
            |        const static ut::cstrview ENCODED_DATA;
            |    }};
            |}}
            )", local_datetime::now().str("%Y-%m-%d, %I:%M %p"), m_namespace, m_name, data_size);

        src.impl = format(R"(
            |cstrview const {0}::{1}::ENCODED_DATA =
            |{2};
            )", m_namespace, m_name, oss.str());
    }
    src.impl = strview(src.impl).trim().trimMargin();
    src.decl = strview(src.decl).trim().trimMargin();
    return src;
}

size_t SrcRes::decode(cstrparam str_encoded, void* bin_out, size_t bin_out_size)
{
    vector<uint8_t> bin_encoded;
    bin_encoded.resize((str_encoded.size() + 4) / 5 * 4);

    Decode85((unsigned char const*)str_encoded.data(), bin_encoded.data());

    //size_t bin_decoded_size = stb_decompress_length(bin_encoded.data());
    return stb_decompress((unsigned char*)bin_out, bin_encoded.data(), bin_out_size);
}

namespace fs = std::filesystem;

// Helper function to sanitize a filename/directory name into a valid C++ identifier
static std::string sanitizeName(std::string const& name)
{
    std::string result;
    result.reserve(name.size());

    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            result += c;
        } else if (c == '.' || c == '-' || c == ' ') {
            result += '_';
        }
        // Skip other invalid characters
    }

    // Ensure it doesn't start with a digit
    if (!result.empty() && std::isdigit(static_cast<unsigned char>(result[0]))) {
        result = "_" + result;
    }

    return result.empty() ? "unnamed" : result;
}

// Helper function to read file contents into a vector
static std::vector<std::uint8_t> readFileBytes(fs::path const& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> buffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + path.string());
    }

    return buffer;
}

namespace fs = std::filesystem;

static string sanitize(std::string const& s)
{
    string r;
    r.reserve(s.size());
    for (char c : s)
        if (isalnum(c)) r += c;
        else if (c == '.' || c == '-' || c == ' ') r += '_';
    if (!r.empty() && isdigit(r[0])) r = "_" + r;
    return r.empty() ? "unnamed" : r;
}

static vector<std::uint8_t> readFile(fs::path const& p)
{
    std::ifstream f(p, std::ios::binary | std::ios::ate);
    if (!f) throw std::runtime_error("Failed to open: " + p.string());
    auto sz = f.tellg();
    f.seekg(0);
    std::vector<std::uint8_t> buf(sz);
    if (!f.read(reinterpret_cast<char*>(buf.data()), sz))
        throw std::runtime_error("Failed to read: " + p.string());
    return buf;
}

SrcRes::Src SrcRes::encodeFile(fs::path const& p)
{
    if (!is_regular_file(p))
        throw std::runtime_error("Not a file: " + p.string());
    return SrcRes(sanitize(p.filename().string())).encode(readFile(p));
}

SrcRes::Src SrcRes::encodeDirectory(fs::path const& p, bool rec)
{
    if (!is_directory(p))
        throw std::runtime_error("Not a directory: " + p.string());

    ostringstream decl, impl;

    auto process = [&](auto& self, fs::path const& dir, std::string const& ns) -> void
    {
        for (auto const& e : fs::directory_iterator(dir))
        {
            if (e.is_regular_file())
            {
                auto name = sanitize(e.path().filename().string());
                try
                {
                    auto src = SrcRes(name, ns).encode(readFile(e.path()));
                    decl << src.decl << "\n";
                    impl << src.impl << "\n";
                } catch (...) {}
            }
            else if (rec && e.is_directory())
            {
                auto sub = sanitize(e.path().filename().string());
                self(self, e.path(), ns.empty() ? sub : ns + "::" + sub);
            }
        }
    };

    process(process, p, sanitize(p.filename().string()));

    return {decl.str(), impl.str()};
}

void SrcRes::writeSrcTofile(fs::path const& path, string const& filename, Src const& src)
{
    if (!exists(path)) return;
    if (!is_directory(path)) return;

    string filename_hpp = filename + ".hpp";
    string filename_cpp = filename + ".cpp";

    if (auto file_hpp = ofstream{path / filename_hpp})
    {
        if (auto file_cpp = ofstream{path / filename_cpp})
        {
            file_hpp << "#pragma once\n\n#include <ut/string.hpp>\n\n";
            file_hpp << src.decl;

            file_cpp << "#include \"" << filename_hpp << "\"\nusing namespace ut;\n\n";
            file_cpp << src.impl;
        }
    }
}