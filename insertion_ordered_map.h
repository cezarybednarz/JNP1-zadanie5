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

private:
    typedef std::list<std::pair<K, V> > List;
    typedef std::unordered_map<K, typename List::iterator, Hash> Map;

    CowPtr<List> list_ptr;
    CowPtr<Map> map_ptr;
    bool refered = false;

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

    using iterator = typename List::const_iterator;

    iterator begin() const;
    iterator end() const;
};

class lookup_error : public std::exception {
public:
    const char* what() const throw() {
    	return "lookup_error";
    }
};

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() {
    list_ptr = CowPtr<List>(new List);
    map_ptr = CowPtr<Map>(new Map);
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map const &other) {
    if(!other->refered) {
        list_ptr = other.list_ptr;
        map_ptr = other.map_ptr;
    } else {
        list_ptr = CowPtr(new List(*other.list_ptr));
        map_ptr = CowPtr(new Map(*other.map_ptr));

        for(typename List::iterator it = list_ptr->begin(); it != list_ptr->end(); ++it) {
            map_ptr->at(it->first) = it;
        }
    }
}
template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) :
    list_ptr(std::move(other.list_ptr)),
    map_ptr(std::move(other.map_ptr)) {}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map other) {
    map_ptr(other.map_ptr);
    list_ptr(other.list_ptr);
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::insert(K const &k, V const &v) {
    if(map_ptr->find(k) == map_ptr->end()) {
        map_ptr->insert(k, list_ptr->push_back(std::make_pair(k, v)));
        return true;
    }
    else {
        V curr_v = map_ptr->at(k)->second;
        list_ptr->erase(map_ptr->at(k));
        map_ptr->at(k) = list_ptr->insert(list_ptr->push_back(std::make_pair(k, curr_v)));
        return false;
    }
}

template <class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::erase(K const &k){
    list_ptr->erase(map_ptr->at(k));
    map_ptr->erase(k);
}

template <class K, class V, class Hash>
void merge(insertion_ordered_map<K, V, Hash> const &other) {
    for(const auto &it : *other.list_ptr) {
        insert(it->first, it->second);
    }
}

template <class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::at(K const &k) {
    refered = true;
    return list_ptr->find(map_ptr->at(k))->second;
}

template <class K, class V, class Hash>
V const &insertion_ordered_map<K, V, Hash>::at(K const &k) const {
    return list_ptr->find(map_ptr->at(k))->second;
}

template <class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::operator[](K const &k) {
    refered = true;

    if(map_ptr->find(k) == map_ptr->end())
        map_ptr->insert(k, V());

    return list_ptr->find(map_ptr->at(k))->second;
}

template <class K, class V, class Hash>
size_t insertion_ordered_map<K, V, Hash>::size() const {
    return list_ptr->size();
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::empty() const {
    return list_ptr->empty();
}

template <class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::clear() {
    list_ptr->clear();
    map_ptr->clear();
    refered = false;
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::contains(K const &k) const {
    return map_ptr->find(k) != map_ptr->end();
}


template <class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::begin() const {
    return list_ptr->begin();
}


template <class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::end() const {
    return list_ptr->end();
}

#endif // __INSERTION_ORDERED_MAP__
