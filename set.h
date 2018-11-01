

#ifndef _SET_H_
#define _SET_H_

#include "allocator.h"
#include "functional.h"
#include "rbtree.h"

namespace mmm {

template <typename Key, typename Compare = mmm::less<Key>,
          typename Allocator = mmm::allocator<rbtree_node<Key>>>
class set : public rbtree<Key, Key, Compare, Allocator, mmm::identity<Key>,
                          false, true> {
public:
  typedef rbtree<Key, Key, Compare, Allocator, mmm::identity<Key>, false, true>
      base_type;
  typedef set<Key, Compare, Allocator> this_type;
  typedef typename base_type::size_type size_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::reverse_iterator reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::allocator_type allocator_type;
  typedef Compare value_compare;
  // Other types are inherited from the base class.

  using base_type::begin;
  using base_type::end;
  using base_type::find;
  using base_type::lower_bound;
  using base_type::mCompare;
  using base_type::upper_bound;

public:
  set(const allocator_type &allocator = allocator_type())
      : base_type(allocator) {}
  set(const Compare &compare,
      const allocator_type &allocator = allocator_type())
      : base_type(compare, allocator) {}

  set(const this_type &x) : base_type(mmm::move(x)) {}

  set(this_type &&x) : base_type(mmm::move(x)) {}

  set(this_type &&x, const allocator_type &allocator)
      : base_type(mmm::move(x), allocator) {}
  set(std::initializer_list<value_type> ilist,
      const Compare &compare = Compare(),
      const allocator_type &allocator = allocator_type())
      : base_type(ilist.begin(), ilist.end(), compare, allocator) {}

  template <typename Iterator>
  set(Iterator itBegin, Iterator itEnd)
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
  value_compare value_comp() const { return mCompare; }

  size_type erase(const Key &k) {
    const iterator it(find(k));

    if (it != end()) {
      base_type::erase(it);
      return 1;
    }
    return 0;
  }

  iterator erase(const_iterator position) { return base_type::erase(position); }
  iterator erase(const_iterator first, const_iterator last) {
    return base_type::erase(first, last);
  }

  reverse_iterator erase(const_reverse_iterator position) {
    return reverse_iterator(erase((++position).base()));
  }

  reverse_iterator erase(const_reverse_iterator first,
                         const_reverse_iterator last) {
    return reverse_iterator(erase((++last).base(), (++first).base()));
  }

  size_type count(const Key &k) const {
    const const_iterator it(find(k));
    return (it != end()) ? (size_type)1 : (size_type)0;
  }

  mmm::pair<iterator, iterator> equal_range(const Key &k);
  mmm::pair<const_iterator, const_iterator> equal_range(const Key &k) const;

}; // set

template <typename Key, typename Compare = mmm::less<Key>,
          typename Allocator = mmm::allocator<rbtree_node<Key>>>
class multiset : public rbtree<Key, Key, Compare, Allocator, mmm::identity<Key>,
                               false, false> {
public:
  typedef rbtree<Key, Key, Compare, Allocator, mmm::identity<Key>, false, false>
      base_type;
  typedef multiset<Key, Compare, Allocator> this_type;
  typedef typename base_type::size_type size_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::reverse_iterator reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::allocator_type allocator_type;
  typedef Compare value_compare;

  using base_type::begin;
  using base_type::end;
  using base_type::find;
  using base_type::lower_bound;
  using base_type::mCompare;
  using base_type::upper_bound;

public:
  multiset(const allocator_type &allocator = allocator_type())
      : base_type(allocator) {}
  multiset(const Compare &compare,
           const allocator_type &allocator = allocator_type())
      : base_type(compare, allocator) {}

  multiset(const this_type &x) : base_type(x) {}

  multiset(this_type &&x) : base_type(mmm::move(x)) {}
  multiset(this_type &&x, const allocator_type &allocator)
      : base_type(mmm::move(x), allocator) {}

  multiset(std::initializer_list<value_type> ilist,
           const Compare &compare = Compare(),
           const allocator_type &allocator = allocator_type())
      : base_type(ilist.begin(), ilist.end(), compare, allocator) {}

  template <typename Iterator>
  multiset(Iterator itBegin, Iterator itEnd)
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
  value_compare value_comp() const { return mCompare; }

  size_type erase(const Key &k) {
    const mmm::pair<iterator, iterator> range(equal_range(k));
    const size_type n = (size_type)mmm::distance(range.first, range.second);
    base_type::erase(range.first, range.second);
    return n;
  }
  iterator erase(const_iterator position) { return base_type::erase(position); }

  iterator erase(const_iterator first, const_iterator last) {
    return base_type::erase(first, last);
  }

  reverse_iterator erase(const_reverse_iterator position) {
    return reverse_iterator(erase((++position).base()));
  }
  reverse_iterator erase(const_reverse_iterator first,
                         const_reverse_iterator last) {
    return reverse_iterator(erase((++last).base(), (++first).base()));
  }

  size_type count(const Key &k) const {
    const mmm::pair<const_iterator, const_iterator> range(equal_range(k));
    return (size_type)mmm::distance(range.first, range.second);
  }

  mmm::pair<iterator, iterator> equal_range(const Key &k) {
    return mmm::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
  }

  mmm::pair<const_iterator, const_iterator> equal_range(const Key &k) const {

    return mmm::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
  }

  /// equal_range_small
  /// This is a special version of equal_range which is optimized for the
  /// case of there being few or no duplicated keys in the tree.
  mmm::pair<iterator, iterator> equal_range_small(const Key &k);
  mmm::pair<const_iterator, const_iterator>
  equal_range_small(const Key &k) const;

}; // multiset

// set

template <typename Key, typename Compare, typename Allocator>
inline mmm::pair<typename set<Key, Compare, Allocator>::iterator,
                 typename set<Key, Compare, Allocator>::iterator>
set<Key, Compare, Allocator>::equal_range(const Key &k) {
  const iterator itLower(lower_bound(k));

  if ((itLower == end()) ||
      mCompare(k, *itLower)) // If at the end or if (k is < itLower)...
    return mmm::pair<iterator, iterator>(itLower, itLower);

  iterator itUpper(itLower);
  return mmm::pair<iterator, iterator>(itLower, ++itUpper);
}

template <typename Key, typename Compare, typename Allocator>
inline mmm::pair<typename set<Key, Compare, Allocator>::const_iterator,
                 typename set<Key, Compare, Allocator>::const_iterator>
set<Key, Compare, Allocator>::equal_range(const Key &k) const {

  const const_iterator itLower(lower_bound(k));

  if ((itLower == end()) ||
      mCompare(k, *itLower)) // If at the end or if (k is < itLower)...
    return mmm::pair<const_iterator, const_iterator>(itLower, itLower);

  const_iterator itUpper(itLower);
  return mmm::pair<const_iterator, const_iterator>(itLower, ++itUpper);
}

// multiset

template <typename Key, typename Compare, typename Allocator>
inline mmm::pair<typename multiset<Key, Compare, Allocator>::iterator,
                 typename multiset<Key, Compare, Allocator>::iterator>
multiset<Key, Compare, Allocator>::equal_range_small(const Key &k) {
  // We provide alternative version of equal_range here which works faster
  // for the case where there are at most small number of potential duplicated
  // keys.
  const iterator itLower(lower_bound(k));
  iterator itUpper(itLower);

  while ((itUpper != end()) && !mCompare(k, itUpper.mpNode->mValue))
    ++itUpper;

  return mmm::pair<iterator, iterator>(itLower, itUpper);
}

template <typename Key, typename Compare, typename Allocator>
inline mmm::pair<typename multiset<Key, Compare, Allocator>::const_iterator,
                 typename multiset<Key, Compare, Allocator>::const_iterator>
multiset<Key, Compare, Allocator>::equal_range_small(const Key &k) const {

  const const_iterator itLower(lower_bound(k));
  const_iterator itUpper(itLower);

  while ((itUpper != end()) && !mCompare(k, *itUpper))
    ++itUpper;

  return mmm::pair<const_iterator, const_iterator>(itLower, itUpper);
}

} // namespace mmm

#endif
