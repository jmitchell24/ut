#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <set>

#define UT_GC_DEBUG

namespace ut
{
namespace gc
{
    class object;
    class collector;

    using byte      = std::uint8_t;
    using objset_t  = std::set<object*>;
    using objlist_t = std::vector<object*>;

#ifdef UT_GC_DEBUG
    struct debug_counters
    {
        size_t live=0,dead=0,total=0;
        inline void reset() { live=dead=total=0; }
    };
#endif

    class collector
    {
    public:
#ifdef UT_GC_DEBUG
        debug_counters counters;
#endif

        static collector& instance();

        void addObj(object* o);
        void removeObj(object* o);

        void addRoot(object* o) ;
        void removeRoot(object* o);

        size_t live() const;

        void collect();

    private:
        objset_t    m_heap;
        objset_t    m_roots;

        void sweep();
    };

    class object
    {
    public:
        friend class collector;

        virtual ~object(){}

        inline bool marked() const { return m_marked; }
        void mark();

    protected:
        object(collector& c = collector::instance());
        virtual void onMark(){}

    private:
        bool m_marked;
    };

    class memory : object
    {
    public:
        memory(size_t size);
        ~memory();

        inline size_t       size() const    { return m_size; }
        inline byte const*  data() const    { return (byte*)m_data; }
        inline byte*        data()          { return (byte*)m_data; }

//        inline byte& operator* () { return *data(); }
//        inline byte const& operator* () const { return *data(); }

//        inline byte* operator-> () { return data(); }
//        inline byte const* operator-> () const { return data(); }


    private:
        size_t  m_size;
        void*   m_data;
    };


}
}
