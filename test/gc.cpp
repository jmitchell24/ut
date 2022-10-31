#include "gc.hpp"

using namespace ut;
using namespace ut::gc;

#include <cassert>
#include <cstdlib>
#include <vector>
using namespace std;

//
// implementation -> collector
//

collector& collector::instance()
{
    static collector x;
    return x;
}

void collector::addObj      (object* o) { m_heap.insert(o);     }
void collector::removeObj   (object* o) { m_heap.erase(o);      }
void collector::addRoot     (object* o) { m_roots.insert(o);    }
void collector::removeRoot  (object* o) { m_roots.erase(o);     }

size_t collector::live() const { return m_heap.size(); }

void collector::collect()
{
    for (auto&& it : m_roots)
        it->mark();
    sweep();
}

void collector::sweep()
{
    objlist_t erase;

#ifdef UT_GC_DEBUG
    counters.reset();
    for (auto&& it : m_heap)
    {
        ++counters.total;
        if (it->m_marked)
        {
            ++counters.live;
            it->m_marked = false;
        }
        else
        {
            erase.push_back(it);
        }
    }
    counters.dead = erase.size();
#else
    for (auto&& it : m_heap)
    {
        if (it->m_marked)
            it->m_marked = false;
        else
            erase.push_back(it);
    }
#endif

    for (auto&& it : erase)
    {
        m_heap.erase(it);
        delete it;
    }
}

//
// implementation -> object
//

object::object(collector& c)
    : m_marked{false}
{ c.addObj(this); }

void object::mark()
{
    if (!m_marked)
    {
        m_marked = true;
        onMark();
    }
}

//
// implementation -> memory
//

memory::memory(size_t size)
    : m_size{size}, m_data{malloc(sizeof(byte)*size)}
{ assert(m_data != nullptr); }

memory::~memory() { free((void*)m_data); }
