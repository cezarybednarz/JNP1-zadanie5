#ifndef __INSERTION_ORDERED_MAP__
#define __INSERTION_ORDERED_MAP__

#include <functional>
#include <unordered_set>
#include <iterator>
#include <memory>
#include <list>
#include <unordered_map>

#include <bits/stdc++.h>
using namespace std;

#define PII pair<int, int>
#define VI vector<int>
#define VPII vector<PII>
#define LL long long
#define LD long double
#define f first
#define s second
#define MP make_pair
#define PB push_back
#define endl '\n'
#define ALL(c) (c).begin(), (c).end()
#define SIZ(c) (int)(c).size()
#define REP(i, n) for (int i = 0; i < (int)(n); i++)
#define FOR(i, b, e) for (int i = (b); i <= (int)(e); i++)
#define FORD(i, b, e) for (int i = (b); i >= (int)(e); i--)
#define ll long long
#define st f
#define nd s
#define pb PB
#define eb emplace_back
#define mp make_pair
#define siz(c) SIZ(c)
const int inf = 1e9 + 7;
const LL INF = 1e18L + 7;

#define sim template<class n
sim, class s> ostream & operator << (ostream &p, pair<n, s> x)
{return p << "<" << x.f << ", " << x.s << ">";}
sim> auto operator << (ostream &p, n y) ->
typename enable_if<!is_same<n, string>::value, decltype(y.begin(), p)>::type
{int o = 0; p << "{"; for (auto c : y) {if (o++) p << ", "; p << c;} return p << "}";}
void dor() {cerr << endl;}
sim, class...s> void dor(n p, s...y) {cerr << p << " "; dor(y...);}
sim, class s> void mini(n &p, s y) {if (p > y) p = y;}
sim, class s> void maxi(n &p, s y) {if (p < y) p = y;}

#define DEB

#ifdef DEB
#define debug(...) dor(__FUNCTION__, ":", __LINE__, ": ", __VA_ARGS__)
#else
#define debug(...)
#endif

#define I(x) #x " = ", (x), " "
#define A(a, i) #a "[" #i " = ", i, "] = ", a[i], " "

// skopiowany, mozna pozmieniac
/*template <class T>
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
    CowPtr() : m_sp(nullptr) {}
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
};*/

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
private:
    using List = std::list<std::pair<K, V> >;
    using Map = std::unordered_map<K, typename List::iterator, Hash>;

    std::shared_ptr<List> list_ptr;
    std::shared_ptr<Map> map_ptr;
    void detach();
    bool refered = false;

public:
    insertion_ordered_map();
    ~insertion_ordered_map() noexcept;
    insertion_ordered_map(insertion_ordered_map const &other);
    insertion_ordered_map(insertion_ordered_map &&other) noexcept;
    insertion_ordered_map &operator=(insertion_ordered_map other);
    bool insert(K const &k, V const &v);
    void erase(K const &k);
    void merge(insertion_ordered_map const &other); // tu skonczylem dodawanie wyjatkow
    V &at(K const &k); // lookup_error
    V const &at(K const &k) const; // lookup_error
    V &operator[](K const &k); // push back i emplace rzuca wyjatki
    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;
    bool contains(K const &k) const noexcept;

    using iterator = typename List::const_iterator;
    iterator begin() const noexcept;
    iterator end() const noexcept;
};

class lookup_error : public std::exception {
public:
    const char* what() const throw() {
    	return "lookup_error";
    }
};

template <class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::detach() {
    if(!list_ptr.unique()) {

        try {
            auto ptr_copy = list_ptr;
            list_ptr = std::make_shared<List>(*list_ptr);
            try {
                map_ptr = std::make_shared<Map>(*map_ptr);
            }
            catch(std::exception &e) {
                list_ptr = ptr_copy;
                throw e;
            }
        }
        catch(std::exception &e) {
            throw e;
        }

        for(typename List::iterator it = list_ptr->begin(); it != list_ptr->end(); ++it) {
            map_ptr->at(it->first) = it;
        }

        refered = false;
    }
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() {
    try {
        list_ptr = std::make_shared<List>();
        map_ptr = std::make_shared<Map>();
    }
    catch(exception &e) {
        throw e;
    }
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::~insertion_ordered_map() noexcept = default;

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map const &other) {
    if(!other.refered) {
        list_ptr = other.list_ptr;
        map_ptr = other.map_ptr;
    } else {
        try {
            auto ptr_copy = list_ptr;
            list_ptr = std::make_shared<List>(*other.list_ptr);
            try {
                map_ptr = std::make_shared<Map>(*other.map_ptr);
            }
            catch(std::exception &e) {
                list_ptr = ptr_copy;
                throw e;
            }
        }
        catch(std::exception &e) {
            throw e;
        }


        for(typename List::iterator it = list_ptr->begin(); it != list_ptr->end(); ++it) {
            map_ptr->at(it->first) = it;
        }
    }
}
template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) noexcept :
    list_ptr(std::move(other.list_ptr)),
    map_ptr(std::move(other.map_ptr)) {}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map other) {
    if(!other.refered) {
        list_ptr = other.list_ptr;
        map_ptr = other.map_ptr;
        refered = other.refered;
    } else {
        try {
            auto ptr_copy = list_ptr;
            list_ptr = std::make_shared<List>(*other.list_ptr);
            try {
                map_ptr = std::make_shared<Map>(*other.map_ptr);
            }
            catch(std::exception &e) {
                list_ptr = ptr_copy;
                throw e;
            }
        }
        catch(std::exception &e) {
            throw e;
        }

        for(typename List::iterator it = list_ptr->begin(); it != list_ptr->end(); ++it) {
            map_ptr->at(it->first) = it;
       }

       refered = false;
    }

    return *this;
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::insert(K const &k, V const &v) {
    try {
        detach();
    }
    catch(std::exception &e) {
        throw e;
    }

    if(map_ptr->find(k) == map_ptr->end()) {
        try {
            list_ptr->push_back(std::make_pair(k, v));
            try {
                map_ptr->emplace(k, prev(list_ptr->end()));
            }
            catch(std::exception &e) {
                list_ptr->pop_back();
                throw e;
            }
        }
        catch(std::exception& e) {
            throw e;
        }
        return true;
    } else {
        try {
            V curr_v = map_ptr->at(k)->second;
            try {
                try {
                    list_ptr->push_back(std::make_pair(k, curr_v));
                    list_ptr->erase(map_ptr->at(k));
                    map_ptr->at(k) = prev(list_ptr->end());
                }
                catch(std::exception &e) {
                    list_ptr->pop_back();
                    throw e;
                }
            }
            catch(std::exception &e) {
                throw e;
            }
        }
        catch(std::exception &e) {
            throw e;
        }
        return false;
    }
}

template <class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::erase(K const &k) {
    try {
        detach();
    }
    catch(std::exception &e) {
        throw e;
    }

    list_ptr->erase(map_ptr->at(k));
    map_ptr->erase(k);


    if(list_ptr->empty())
        refered = false;
}

template <class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::merge(insertion_ordered_map<K, V, Hash> const &other) {
    try {
        insertion_ordered_map<K, V, Hash> copy(*this);
        for(const auto &it : *other.list_ptr) {
            copy.insert(it.first, it.second);
        }
        *this = copy;
    }
    catch(std::exception &e) {
        throw e;
    }
}

template <class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::at(K const &k) {
    try {
        detach();
    }
    catch(std::exception &e) {
        throw e;
    }

    if(map_ptr->find(k) == map_ptr->end()) {
        throw lookup_error();
    }

    refered = true;
    return map_ptr->at(k)->second;
}

template <class K, class V, class Hash>
V const &insertion_ordered_map<K, V, Hash>::at(K const &k) const {
    if(map_ptr->find(k) == map_ptr->end()) {
        throw lookup_error();
    }

    return map_ptr->at(k)->second;
}

template <class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::operator[](K const &k) {
    try {
        detach();
    }
    catch(std::exception &e) {
        throw e;
    }

    if(map_ptr->find(k) == map_ptr->end()) {
        try {
            list_ptr->push_back(std::make_pair(k, V()));
            try {
                map_ptr->emplace(k, prev(list_ptr->end()));
            }
            catch(std::exception &e) {
                list_ptr->pop_back();
                throw e;
            }
        }
        catch(std::exception &e) {
            throw e;
        }
    }

    refered = true;
    return map_ptr->at(k)->second;
}

template <class K, class V, class Hash>
size_t insertion_ordered_map<K, V, Hash>::size() const noexcept {
    return list_ptr->size();
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::empty() const noexcept {
    return list_ptr->empty();
}

template <class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::clear() noexcept {
    list_ptr->clear();
    map_ptr->clear();
    refered = false;
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::contains(K const &k) const noexcept {
    return map_ptr->find(k) != map_ptr->end();
}


template <class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::begin() const noexcept{
    return list_ptr->begin();
}


template <class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::end() const noexcept {
    return list_ptr->end();
}

#endif // __INSERTION_ORDERED_MAP__
