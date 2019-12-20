#ifndef __INSERTION_ORDERED_MAP__
#define __INSERTION_ORDERED_MAP__

#include <functional>
#include <unordered_set>
#include <iterator>
#include <memory>
#include <list>
#include <unordered_map>

// skopiowany, mozna pozmieniac
template <class T>
class CowPtr {
public:
    typedef std::shared_ptr<T> RefPtr;

private:
    RefPtr m_sp;

    void detach() {
        T* tmp = m_sp.get();
        if(!( tmp == 0 || m_sp.unique())) {
            m_sp = RefPtr(new T(*tmp));
        }
    }

public:
    CowPtr(T* t) : m_sp(t) {}

    CowPtr(const RefPtr& refptr) : m_sp(refptr) {}

    const T& operator*() const {
        return *m_sp;
    }

    T& operator*() {
        detach();
        return *m_sp;
    }

    const T* operator->() const {
        return m_sp.operator->();
    }

    T* operator->() {
        detach();
        return m_sp.operator->();
    }
};

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {

public:
    insertion_ordered_map();
    insertion_ordered_map(insertion_ordered_map const &other);
    insertion_ordered_map(insertion_ordered_map &&other);
    insertion_ordered_map &operator=(insertion_ordered_map other);
    bool insert(K const &k, V const &v);
    void erase(K const &k);
    void merge(insertion_ordered_map const &other);
    V &at(K const &k);
    V const &at(K const &k) const;
    V &operator[](K const &k);
    size_t size() const;
    bool empty() const;
    void clear();
    bool contains(K const &k) const;

    class iterator {
        iterator();
        iterator(const iterator &it);
        iterator& operator++();
        bool operator==(const iterator& it);
        bool operator!=(const iterator& it);
        std::pair<const K&, const V&> operator*();
    };

private:
    typedef std::list<std::pair<K, V> > List;
    typedef std::unordered_map<K, typename List::iterator, Hash> Map;

    std::shared_ptr<List> list_ptr;
    std::shared_ptr<Map> map_ptr;
};

class lookup_error : public std::exception {

public:

};


#endif // __INSERTION_ORDERED_MAP__
