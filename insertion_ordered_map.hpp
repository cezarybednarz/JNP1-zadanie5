#ifndef __INSERTION_ORDERED_MAP__
#define __INSERTION_ORDERED_MAP__

#include <functional>
#include <unordered_set>
#include <iterator>
#include <memory>

// skopiowany, mozna pozmieniac
template <class T>
class CowPtr {
public:
    typedef std::shared_ptr<T> RefPtr;

private:
    RefPtr m_sp;

    void detach() {
        T* tmp = m_sp.get();
        if( !( tmp == 0 || m_sp.unique() ) ) {
            m_sp = RefPtr( new T( *tmp ) );
        }
    }

public:
    CowPtr(T* t)
        :   m_sp(t)
    {}
    CowPtr(const RefPtr& refptr)
        :   m_sp(refptr)
    {}
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

};


template<typename K, typename V>
class iterator : public std::iterator<std::forward_iterator_tag, V> {

public:

    



};


#endif // __INSERTION_ORDERED_MAP__
