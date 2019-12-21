#include "insertion_ordered_map.h"
#include <functional>
#include <unordered_set>
#include <iterator>
#include <memory>
#include <list>
#include <unordered_map>

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() {
	list_ptr = CowPtr<List>(new List);
	map_ptr = CowPtr<Map>(new Map);
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(CowPtr<List> list_ptr, CowPtr<Map> map_ptr) :
list_ptr(list_ptr), map_ptr(map_ptr) {}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map const &other) :
list_ptr(other.list_ptr), map_ptr(other.map_ptr) {}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) {
	//
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map other) {
	map_ptr = other.map_ptr;
	list_ptr = other.list_ptr;
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::insert(K const &k, V const &v) {
	if(map_ptr->find(k) == map_ptr->end()) {
		map_ptr->insert(k, list_ptr->push_back(std::make_pair(v, k)));
	}
	else {
		list_ptr->erase(map_ptr->at(k));
		map_ptr->at(k) = list_ptr->insert(list_ptr->push_back(std::make_pair(v, k)));
	}
}

template <class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::erase(K const &k){
	list_ptr->erase(map_ptr->at(k));
	map_ptr->erase(k);
}

template <class K, class V, class Hash>
void merge(insertion_ordered_map<K, V, Hash> const &other) {
	//
}

template <class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::at(K const &k) {
	return list_ptr->find(map_ptr->at(k))->second;
}

template <class K, class V, class Hash>
V const &insertion_ordered_map<K, V, Hash>::at(K const &k) const {
	return list_ptr->find(map_ptr->at(k))->second;
}

template <class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::operator[](K const &k) {
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
}

template <class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::contains(K const &k) const {
	return map_ptr->find(k) != map_ptr->end();
}

iterator<K, V, Hash> &insertion_ordered_map<K, V, Hash>::begin() {
    return iterator(list_ptr->begin());
}

iterator<K, V, Hash> &insertion_ordered_map<K, V, Hash>::end() {
    return iterator(list_ptr->begin());
}

    /*V &at(K const &k);
    V const &at(K const &k) const;
    V &operator[](K const &k);
    size_t size() const;
    bool empty() const;
    void clear();
    bool contains(K const &k) const;*/
