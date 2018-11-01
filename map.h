#ifndef _MAP_H_
#define _MAP_H_

#include "allocator.h"
#include "functional.h"
#include "pair.h"
#include "rbtree.h"
#include "exception.h"

namespace mmm {
template <typename Key, typename T, typename Compare = mmm::less<Key>,
          typename Allocator = mmm::allocator<rbtree_node<mmm::pair<const Key, T>>>>
class map : public rbtree<Key, mmm::pair<const Key, T>, Compare, Allocator,
                          mmm::select1st<mmm::pair<const Key, T>>, true, true> {
public:
  typedef rbtree<Key, mmm::pair<const Key, T>, Compare, Allocator,
                 mmm::select1st<mmm::pair<const Key, T>>, true, true>
      base_type;
  typedef map<Key, T, Compare, Allocator> this_type;
  typedef typename base_type::size_type size_type;
  typedef typename base_type::key_type key_type;
  typedef T mapped_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::node_type node_type;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::allocator_type allocator_type;
  typedef typename base_type::insert_return_type insert_return_type;
  typedef typename base_type::extract_key extract_key;

  using base_type::begin;
  using base_type::end;
  using base_type::erase;
  using base_type::find;
  using base_type::insert;
  using base_type::lower_bound;
  using base_type::mCompare;
  using base_type::upper_bound;

  class value_compare {
  protected:
    friend class map;
    Compare compare;
    value_compare(Compare c) : compare(c) {}

  public:
    typedef bool result_type;
    typedef value_type first_argument_type;
    typedef value_type second_argument_type;

    bool operator()(const value_type &x, const value_type &y) const {
      return compare(x.first, y.first);
    }
  };

public:
  map(const allocator_type &allocator = allocator_type())
      : base_type(allocator) {}
  map(const Compare &compare,
      const allocator_type &allocator = allocator_type())
      : base_type(compare, allocator) {}
  map(const this_type &x) : base_type(x) {}
  map(this_type &&x) : base_type(mmm::move(x)) {}
  map(this_type &&x, const allocator_type &allocator)
      : base_type(mmm::move(x), allocator) {}

  map(std::initializer_list<value_type> ilist,
      const Compare &compare = Compare(),
      const allocator_type &allocator = allocator_type())
      : base_type(ilist.begin(), ilist.end(), compare, allocator) {}

  template <typename Iterator>
  map(Iterator itBegin, Iterator itEnd)
      : base_type(itBegin, itEnd, Compare(), allocator_type()) {}

  this_type &operator=(const this_type &x) {
    return (this_type &)base_type::operator=(x);
  }
  this_type &operator=(std::initializer_list<value_type> ilist) {
    return (this_type &)base_type::operator=(ilist);
  }
  this_type &operator=(this_type &&x) {
    return (this_type &)base_type::operator=(mmm::move(x));
  }

public:
  insert_return_type insert(const Key &key) {
    return base_type::DoInsertKey(true_type(), key);
  }

  value_compare value_comp() const { return value_compare(mCompare); }

  size_type erase(const Key &key) {
    const iterator it(find(key));

    if (it != end()) // If it exists...
    {
      base_type::erase(it);
      return 1;
    }
    return 0;
  }

  size_type count(const Key &key) const {
    const const_iterator it(find(key));
    return (it != end()) ? 1 : 0;
  }

  mmm::pair<iterator, iterator> equal_range(const Key &key);
  mmm::pair<const_iterator, const_iterator> equal_range(const Key &key) const;

  T &operator[](const Key &key); // Of map, multimap, set, and multimap, only
                                 // map has operator[].
  T &operator[](Key &&key);

  T &at(const Key &key);
  const T &at(const Key &key) const;

}; // map

/// multimap

template <typename Key, typename T, typename Compare = mmm::less<Key>,
          typename Allocator = mmm::allocator<rbtree_node<mmm::pair<const Key, T>>>>
class multimap
    : public rbtree<Key, mmm::pair<const Key, T>, Compare, Allocator,
                    mmm::select1st<mmm::pair<const Key, T>>, true, false> {
public:
  typedef rbtree<Key, mmm::pair<const Key, T>, Compare, Allocator,
                 mmm::select1st<mmm::pair<const Key, T>>, true, false>
      base_type;
  typedef multimap<Key, T, Compare, Allocator> this_type;
  typedef typename base_type::size_type size_type;
  typedef typename base_type::key_type key_type;
  typedef T mapped_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::node_type node_type;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::allocator_type allocator_type;
  typedef typename base_type::insert_return_type insert_return_type;
  typedef typename base_type::extract_key extract_key;
  // Other types are inherited from the base class.

  using base_type::begin;
  using base_type::end;
  using base_type::erase;
  using base_type::find;
  using base_type::insert;
  using base_type::lower_bound;
  using base_type::mCompare;
  using base_type::upper_bound;

  class value_compare {
  protected:
    friend class multimap;
    Compare compare;
    value_compare(Compare c) : compare(c) {}

  public:
    typedef bool result_type;
    typedef value_type first_argument_type;
    typedef value_type second_argument_type;

    bool operator()(const value_type &x, const value_type &y) const {
      return compare(x.first, y.first);
    }
  };

public:
  multimap(const allocator_type &allocator = allocator_type())
      : base_type(allocator) {}

  multimap(const Compare &compare,
           const allocator_type &allocator = allocator_type())
      : base_type(compare, allocator) {}

  multimap(const this_type &x) : base_type(x) {}

  multimap(this_type &&x) : base_type(mmm::move(x)) {}
  multimap(this_type &&x, const allocator_type &allocator)
      : base_type(mmm::move(x), allocator) {}
  multimap(std::initializer_list<value_type> ilist,
           const Compare &compare = Compare(),
           const allocator_type &allocator = allocator_type())
      : base_type(ilist.begin(), ilist.end(), compare, allocator) {}

  template <typename Iterator>
  multimap(Iterator itBegin, Iterator itEnd)
      : base_type(itBegin, itEnd, Compare(), allocator_type()) {}
  this_type &operator=(const this_type &x) {
    return (this_type &)base_type::operator=(x);
  }
  this_type &operator=(std::initializer_list<value_type> ilist) {
    return (this_type &)base_type::operator=(ilist);
  }
  this_type &operator=(this_type &&x) {
    return (this_type &)base_type::operator=(mmm::move(x));
  }

public:
  insert_return_type insert(const Key &key) {
    return base_type::DoInsertKey(false_type(), key);
  }

  value_compare value_comp() const { return value_compare(mCompare); }

  size_type erase(const Key &key) {
    const mmm::pair<iterator, iterator> range(equal_range(key));
    const size_type n = (size_type)mmm::distance(range.first, range.second);
    base_type::erase(range.first, range.second);
    return n;
  }

  size_type count(const Key &key) const {
    const mmm::pair<const_iterator, const_iterator> range(equal_range(key));
    return (size_type)mmm::distance(range.first, range.second);
  }

  mmm::pair<iterator, iterator> equal_range(const Key &key) {
    return mmm::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
  }
  mmm::pair<const_iterator, const_iterator> equal_range(const Key &key) const {
    // See comments above in the non-const version of equal_range.
    return mmm::pair<const_iterator, const_iterator>(lower_bound(key),
                                                     upper_bound(key));
  }

  mmm::pair<iterator, iterator> equal_range_small(const Key &key);
  mmm::pair<const_iterator, const_iterator>
  equal_range_small(const Key &key) const;

private:
  using base_type::insert_or_assign;
  using base_type::try_emplace;
}; // multimap

// map

template <typename Key, typename T, typename Compare, typename Allocator>
inline mmm::pair<typename map<Key, T, Compare, Allocator>::iterator,
                 typename map<Key, T, Compare, Allocator>::iterator>
map<Key, T, Compare, Allocator>::equal_range(const Key &key) {

  const iterator itLower(lower_bound(key));

  if ((itLower == end()) ||
      mCompare(key, itLower.mpNode->mValue
                        .first)) // If at the end or if (key is < itLower)...
    return mmm::pair<iterator, iterator>(itLower, itLower);

  iterator itUpper(itLower);
  return mmm::pair<iterator, iterator>(itLower, ++itUpper);
}

template <typename Key, typename T, typename Compare, typename Allocator>
inline mmm::pair<typename map<Key, T, Compare, Allocator>::const_iterator,
                 typename map<Key, T, Compare, Allocator>::const_iterator>
map<Key, T, Compare, Allocator>::equal_range(const Key &key) const {
  const const_iterator itLower(lower_bound(key));

  if ((itLower == end()) ||
      mCompare(key, itLower.mpNode->mValue
                        .first)) // If at the end or if (key is < itLower)...
    return mmm::pair<const_iterator, const_iterator>(itLower, itLower);

  const_iterator itUpper(itLower);
  return mmm::pair<const_iterator, const_iterator>(itLower, ++itUpper);
}

template <typename Key, typename T, typename Compare, typename Allocator>
inline T &map<Key, T, Compare, Allocator>::operator[](const Key &key) {
  iterator itLower(lower_bound(key)); // itLower->first is >= key.
  
  if ((itLower == end()) || mCompare(key, (*itLower).first)) {
    itLower = base_type::DoInsertKey(true_type(), itLower, key);
  }

  return (*itLower).second;

  // Reference implementation of this function, which may not be as fast:
  // iterator it(base_type::insert(mmm::pair<iterator, iterator>(key,
  // T())).first); return it->second;
}

template <typename Key, typename T, typename Compare, typename Allocator>
inline T &map<Key, T, Compare, Allocator>::operator[](Key &&key) {
  iterator itLower(lower_bound(key)); // itLower->first is >= key.

  if ((itLower == end()) || mCompare(key, (*itLower).first)) {
    itLower = base_type::DoInsertKey(true_type(), itLower, mmm::move(key));
  }

  return (*itLower).second;
}

template <typename Key, typename T, typename Compare, typename Allocator>
inline T &map<Key, T, Compare, Allocator>::at(const Key &key) {
  iterator itLower(lower_bound(key)); // itLower->first is >= key.

  if (itLower == end()) {
    throw mmm::out_of_range("map::at key does not exist");
  }

  return (*itLower).second;
}

template <typename Key, typename T, typename Compare, typename Allocator>
inline const T &map<Key, T, Compare, Allocator>::at(const Key &key) const {
  const_iterator itLower(lower_bound(key)); // itLower->first is >= key.

  if (itLower == end()) {
    throw mmm::out_of_range("map::at key does not exist");
  }

  return (*itLower).second;
}

// multimap
template <typename Key, typename T, typename Compare, typename Allocator>
inline mmm::pair<typename multimap<Key, T, Compare, Allocator>::iterator,
                 typename multimap<Key, T, Compare, Allocator>::iterator>
multimap<Key, T, Compare, Allocator>::equal_range_small(const Key &key) {
  const iterator itLower(lower_bound(key));
  iterator itUpper(itLower);

  while ((itUpper != end()) && !mCompare(key, itUpper.mpNode->mValue.first))
    ++itUpper;

  return mmm::pair<iterator, iterator>(itLower, itUpper);
}

template <typename Key, typename T, typename Compare, typename Allocator>
inline mmm::pair<typename multimap<Key, T, Compare, Allocator>::const_iterator,
                 typename multimap<Key, T, Compare, Allocator>::const_iterator>
multimap<Key, T, Compare, Allocator>::equal_range_small(const Key &key) const {
  const const_iterator itLower(lower_bound(key));
  const_iterator itUpper(itLower);

  while ((itUpper != end()) && !mCompare(key, itUpper.mpNode->mValue.first))
    ++itUpper;

  return mmm::pair<const_iterator, const_iterator>(itLower, itUpper);
}

} // namespace mmm

#endif
