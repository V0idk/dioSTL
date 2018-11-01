
#ifndef _RBTREE_H_
#define _RBTREE_H_

#include "algorithm.h"
#include "functional.h"
#include "iterator.h"
#include "type_traits.h"
#include "pair.h"
#include "utility.h"
#include "memory.h"
#include "allocator.h"
#include "pair.h"
#include <stddef.h>

namespace mmm {

enum RBTreeColor { kRBTreeColorRed, kRBTreeColorBlack };

enum RBTreeSide { kRBTreeSideLeft, kRBTreeSideRight };

struct rbtree_node_base {
  typedef rbtree_node_base this_type;

public:
  this_type *mpNodeRight; 
  this_type *mpNodeLeft;
  this_type *mpNodeParent;
  char mColor;
};

template <typename Value> struct rbtree_node : public rbtree_node_base {
  Value mValue; 
};

rbtree_node_base *RBTreeIncrement(const rbtree_node_base *pNode);
rbtree_node_base *RBTreeDecrement(const rbtree_node_base *pNode);
rbtree_node_base *RBTreeGetMinChild(const rbtree_node_base *pNode);
rbtree_node_base *RBTreeGetMaxChild(const rbtree_node_base *pNode);
size_t RBTreeGetBlackCount(const rbtree_node_base *pNodeTop,
                           const rbtree_node_base *pNodeBottom);
void RBTreeInsert(rbtree_node_base *pNode, rbtree_node_base *pNodeParent,
                  rbtree_node_base *pNodeAnchor, RBTreeSide insertionSide);
void RBTreeErase(rbtree_node_base *pNode, rbtree_node_base *pNodeAnchor);
rbtree_node_base* RBTreeRotateLeft(rbtree_node_base* pNode, rbtree_node_base* pNodeRoot);
rbtree_node_base* RBTreeRotateRight(rbtree_node_base* pNode, rbtree_node_base* pNodeRoot);


//-----------------------------红黑树操作实现---------------------------------
rbtree_node_base* RBTreeIncrement(const rbtree_node_base* pNode)
{
	if(pNode->mpNodeRight) 
	{
		pNode = pNode->mpNodeRight;

		while(pNode->mpNodeLeft)
			pNode = pNode->mpNodeLeft;
	}
	else 
	{
		rbtree_node_base* pNodeTemp = pNode->mpNodeParent;

		while(pNode == pNodeTemp->mpNodeRight) 
		{
			pNode = pNodeTemp;
			pNodeTemp = pNodeTemp->mpNodeParent;
		}

		if(pNode->mpNodeRight != pNodeTemp)
			pNode = pNodeTemp;
	}

	return const_cast<rbtree_node_base*>(pNode);
}


rbtree_node_base* RBTreeDecrement(const rbtree_node_base* pNode)
{
	if((pNode->mpNodeParent->mpNodeParent == pNode) && (pNode->mColor == kRBTreeColorRed))
		return pNode->mpNodeRight;
	else if(pNode->mpNodeLeft)
	{
		rbtree_node_base* pNodeTemp = pNode->mpNodeLeft;

		while(pNodeTemp->mpNodeRight)
			pNodeTemp = pNodeTemp->mpNodeRight;

		return pNodeTemp;
	}

	rbtree_node_base* pNodeTemp = pNode->mpNodeParent;

	while(pNode == pNodeTemp->mpNodeLeft) 
	{
		pNode     = pNodeTemp;
		pNodeTemp = pNodeTemp->mpNodeParent;
	}

	return const_cast<rbtree_node_base*>(pNodeTemp);
}


size_t RBTreeGetBlackCount(const rbtree_node_base* pNodeTop, const rbtree_node_base* pNodeBottom)
{
	size_t nCount = 0;

	for(; pNodeBottom; pNodeBottom = pNodeBottom->mpNodeParent)
	{
		if(pNodeBottom->mColor == kRBTreeColorBlack) 
			++nCount;

		if(pNodeBottom == pNodeTop) 
			break;
	}

	return nCount;
}

rbtree_node_base* RBTreeRotateLeft(rbtree_node_base* pNode, rbtree_node_base* pNodeRoot)
{
	rbtree_node_base* const pNodeTemp = pNode->mpNodeRight;

	pNode->mpNodeRight = pNodeTemp->mpNodeLeft;

	if(pNodeTemp->mpNodeLeft)
		pNodeTemp->mpNodeLeft->mpNodeParent = pNode;
	pNodeTemp->mpNodeParent = pNode->mpNodeParent;
	
	if(pNode == pNodeRoot)
		pNodeRoot = pNodeTemp;
	else if(pNode == pNode->mpNodeParent->mpNodeLeft)
		pNode->mpNodeParent->mpNodeLeft = pNodeTemp;
	else
		pNode->mpNodeParent->mpNodeRight = pNodeTemp;

	pNodeTemp->mpNodeLeft = pNode;
	pNode->mpNodeParent = pNodeTemp;

	return pNodeRoot;
}

rbtree_node_base* RBTreeRotateRight(rbtree_node_base* pNode, rbtree_node_base* pNodeRoot)
{
	rbtree_node_base* const pNodeTemp = pNode->mpNodeLeft;

	pNode->mpNodeLeft = pNodeTemp->mpNodeRight;

	if(pNodeTemp->mpNodeRight)
		pNodeTemp->mpNodeRight->mpNodeParent = pNode;
	pNodeTemp->mpNodeParent = pNode->mpNodeParent;

	if(pNode == pNodeRoot)
		pNodeRoot = pNodeTemp;
	else if(pNode == pNode->mpNodeParent->mpNodeRight)
		pNode->mpNodeParent->mpNodeRight = pNodeTemp;
	else
		pNode->mpNodeParent->mpNodeLeft = pNodeTemp;

	pNodeTemp->mpNodeRight = pNode;
	pNode->mpNodeParent = pNodeTemp;

	return pNodeRoot;
}

void RBTreeInsert(rbtree_node_base* pNode,
							rbtree_node_base* pNodeParent, 
							rbtree_node_base* pNodeAnchor,
							RBTreeSide insertionSide)
{
	rbtree_node_base*& pNodeRootRef = pNodeAnchor->mpNodeParent;

	// Initialize fields in new node to insert.
	pNode->mpNodeParent = pNodeParent;
	pNode->mpNodeRight  = NULL;
	pNode->mpNodeLeft   = NULL;
	pNode->mColor       = kRBTreeColorRed;

	// Insert the node.
	if(insertionSide == kRBTreeSideLeft)
	{
		pNodeParent->mpNodeLeft = pNode; // Also makes (leftmost = pNode) when (pNodeParent == pNodeAnchor)

		if(pNodeParent == pNodeAnchor)
		{
			pNodeAnchor->mpNodeParent = pNode;
			pNodeAnchor->mpNodeRight = pNode;
		}
		else if(pNodeParent == pNodeAnchor->mpNodeLeft)
			pNodeAnchor->mpNodeLeft = pNode; // Maintain leftmost pointing to min node
	}
	else
	{
		pNodeParent->mpNodeRight = pNode;

		if(pNodeParent == pNodeAnchor->mpNodeRight)
			pNodeAnchor->mpNodeRight = pNode; // Maintain rightmost pointing to max node
	}

	// Rebalance the tree.
	while((pNode != pNodeRootRef) && (pNode->mpNodeParent->mColor == kRBTreeColorRed)) 
	{

		rbtree_node_base* const pNodeParentParent = pNode->mpNodeParent->mpNodeParent;

		if(pNode->mpNodeParent == pNodeParentParent->mpNodeLeft) 
		{
			rbtree_node_base* const pNodeTemp = pNodeParentParent->mpNodeRight;

			if(pNodeTemp && (pNodeTemp->mColor == kRBTreeColorRed)) 
			{
				pNode->mpNodeParent->mColor = kRBTreeColorBlack;
				pNodeTemp->mColor = kRBTreeColorBlack;
				pNodeParentParent->mColor = kRBTreeColorRed;
				pNode = pNodeParentParent;
			}
			else 
			{
				if(pNode->mpNodeParent && pNode == pNode->mpNodeParent->mpNodeRight) 
				{
					pNode = pNode->mpNodeParent;
					pNodeRootRef = RBTreeRotateLeft(pNode, pNodeRootRef);
				}


				pNode->mpNodeParent->mColor = kRBTreeColorBlack;
				pNodeParentParent->mColor = kRBTreeColorRed;
				pNodeRootRef = RBTreeRotateRight(pNodeParentParent, pNodeRootRef);
			}
		}
		else 
		{
			rbtree_node_base* const pNodeTemp = pNodeParentParent->mpNodeLeft;

			if(pNodeTemp && (pNodeTemp->mColor == kRBTreeColorRed)) 
			{
				pNode->mpNodeParent->mColor = kRBTreeColorBlack;
				pNodeTemp->mColor = kRBTreeColorBlack;
				pNodeParentParent->mColor = kRBTreeColorRed;
				pNode = pNodeParentParent;
			}
			else 
			{


				if(pNode == pNode->mpNodeParent->mpNodeLeft) 
				{
					pNode = pNode->mpNodeParent;
					pNodeRootRef = RBTreeRotateRight(pNode, pNodeRootRef);
				}

				pNode->mpNodeParent->mColor = kRBTreeColorBlack;
				pNodeParentParent->mColor = kRBTreeColorRed;
				pNodeRootRef = RBTreeRotateLeft(pNodeParentParent, pNodeRootRef);
			}
		}
	}

	pNodeRootRef->mColor = kRBTreeColorBlack;

} // RBTreeInsert


void RBTreeErase(rbtree_node_base* pNode, rbtree_node_base* pNodeAnchor)
{
	rbtree_node_base*& pNodeRootRef      = pNodeAnchor->mpNodeParent;
	rbtree_node_base*& pNodeLeftmostRef  = pNodeAnchor->mpNodeLeft;
	rbtree_node_base*& pNodeRightmostRef = pNodeAnchor->mpNodeRight;
	rbtree_node_base*  pNodeSuccessor    = pNode;
	rbtree_node_base*  pNodeChild        = NULL;
	rbtree_node_base*  pNodeChildParent  = NULL;

	if(pNodeSuccessor->mpNodeLeft == NULL)         // pNode has at most one non-NULL child.
		pNodeChild = pNodeSuccessor->mpNodeRight;  // pNodeChild might be null.
	else if(pNodeSuccessor->mpNodeRight == NULL)   // pNode has exactly one non-NULL child.
		pNodeChild = pNodeSuccessor->mpNodeLeft;   // pNodeChild is not null.
	else 
	{
		// pNode has two non-null children. Set pNodeSuccessor to pNode's successor. pNodeChild might be NULL.
		pNodeSuccessor = pNodeSuccessor->mpNodeRight;

		while(pNodeSuccessor->mpNodeLeft)
			pNodeSuccessor = pNodeSuccessor->mpNodeLeft;

		pNodeChild = pNodeSuccessor->mpNodeRight;
	}

	// Here we remove pNode from the tree and fix up the node pointers appropriately around it.
	if(pNodeSuccessor == pNode) // If pNode was a leaf node (had both NULL children)...
	{
		pNodeChildParent = pNodeSuccessor->mpNodeParent;  // Assign pNodeReplacement's parent.

		if(pNodeChild) 
			pNodeChild->mpNodeParent = pNodeSuccessor->mpNodeParent;

		if(pNode == pNodeRootRef) // If the node being deleted is the root node...
			pNodeRootRef = pNodeChild; // Set the new root node to be the pNodeReplacement.
		else 
		{
			if(pNode == pNode->mpNodeParent->mpNodeLeft) // If pNode is a left node...
				pNode->mpNodeParent->mpNodeLeft  = pNodeChild;  // Make pNode's replacement node be on the same side.
			else
				pNode->mpNodeParent->mpNodeRight = pNodeChild;
			// Now pNode is disconnected from the bottom of the tree (recall that in this pathway pNode was determined to be a leaf).
		}

		if(pNode == pNodeLeftmostRef) // If pNode is the tree begin() node...
		{
			// Because pNode is the tree begin(), pNode->mpNodeLeft must be NULL.
			// Here we assign the new begin() (first node).
			if(pNode->mpNodeRight && pNodeChild)
			{

				pNodeLeftmostRef = RBTreeGetMinChild(pNodeChild); 
			}
			else
				pNodeLeftmostRef = pNode->mpNodeParent; // This  makes (pNodeLeftmostRef == end()) if (pNode == root node)
		}

		if(pNode == pNodeRightmostRef) // If pNode is the tree last (rbegin()) node...
		{
			// Because pNode is the tree rbegin(), pNode->mpNodeRight must be NULL.
			// Here we assign the new rbegin() (last node)
			if(pNode->mpNodeLeft && pNodeChild)
			{

				pNodeRightmostRef = RBTreeGetMaxChild(pNodeChild);
			}
			else // pNodeChild == pNode->mpNodeLeft
				pNodeRightmostRef = pNode->mpNodeParent; // makes pNodeRightmostRef == &mAnchor if pNode == pNodeRootRef
		}
	}
	else // else (pNodeSuccessor != pNode)
	{
		// Relink pNodeSuccessor in place of pNode. pNodeSuccessor is pNode's successor.
		// We specifically set pNodeSuccessor to be on the right child side of pNode, so fix up the left child side.
		pNode->mpNodeLeft->mpNodeParent = pNodeSuccessor; 
		pNodeSuccessor->mpNodeLeft = pNode->mpNodeLeft;

		if(pNodeSuccessor == pNode->mpNodeRight) // If pNode's successor was at the bottom of the tree... (yes that's effectively what this statement means)
			pNodeChildParent = pNodeSuccessor; // Assign pNodeReplacement's parent.
		else
		{
			pNodeChildParent = pNodeSuccessor->mpNodeParent;

			if(pNodeChild)
				pNodeChild->mpNodeParent = pNodeChildParent;

			pNodeChildParent->mpNodeLeft = pNodeChild;

			pNodeSuccessor->mpNodeRight = pNode->mpNodeRight;
			pNode->mpNodeRight->mpNodeParent = pNodeSuccessor;
		}

		if(pNode == pNodeRootRef)
			pNodeRootRef = pNodeSuccessor;
		else if(pNode == pNode->mpNodeParent->mpNodeLeft)
			pNode->mpNodeParent->mpNodeLeft = pNodeSuccessor;
		else 
			pNode->mpNodeParent->mpNodeRight = pNodeSuccessor;

		// Now pNode is disconnected from the tree.

		pNodeSuccessor->mpNodeParent = pNode->mpNodeParent;
		mmm::swap(pNodeSuccessor->mColor, pNode->mColor);
	}

	// Here we do tree balancing as per the conventional red-black tree algorithm.
	if(pNode->mColor == kRBTreeColorBlack) 
	{ 
		while((pNodeChild != pNodeRootRef) && ((pNodeChild == NULL) || (pNodeChild->mColor == kRBTreeColorBlack)))
		{
			if(pNodeChild == pNodeChildParent->mpNodeLeft) 
			{
				rbtree_node_base* pNodeTemp = pNodeChildParent->mpNodeRight;

				if(pNodeTemp->mColor == kRBTreeColorRed) 
				{
					pNodeTemp->mColor = kRBTreeColorBlack;
					pNodeChildParent->mColor = kRBTreeColorRed;
					pNodeRootRef = RBTreeRotateLeft(pNodeChildParent, pNodeRootRef);
					pNodeTemp = pNodeChildParent->mpNodeRight;
				}

				if(((pNodeTemp->mpNodeLeft  == NULL) || (pNodeTemp->mpNodeLeft->mColor  == kRBTreeColorBlack)) &&
					((pNodeTemp->mpNodeRight == NULL) || (pNodeTemp->mpNodeRight->mColor == kRBTreeColorBlack))) 
				{
					pNodeTemp->mColor = kRBTreeColorRed;
					pNodeChild = pNodeChildParent;
					pNodeChildParent = pNodeChildParent->mpNodeParent;
				} 
				else 
				{
					if((pNodeTemp->mpNodeRight == NULL) || (pNodeTemp->mpNodeRight->mColor == kRBTreeColorBlack)) 
					{
						pNodeTemp->mpNodeLeft->mColor = kRBTreeColorBlack;
						pNodeTemp->mColor = kRBTreeColorRed;
						pNodeRootRef = RBTreeRotateRight(pNodeTemp, pNodeRootRef);
						pNodeTemp = pNodeChildParent->mpNodeRight;
					}

					pNodeTemp->mColor = pNodeChildParent->mColor;
					pNodeChildParent->mColor = kRBTreeColorBlack;

					if(pNodeTemp->mpNodeRight) 
						pNodeTemp->mpNodeRight->mColor = kRBTreeColorBlack;

					pNodeRootRef = RBTreeRotateLeft(pNodeChildParent, pNodeRootRef);
					break;
				}
			} 
			else 
			{   
				// The following is the same as above, with mpNodeRight <-> mpNodeLeft.
				rbtree_node_base* pNodeTemp = pNodeChildParent->mpNodeLeft;

				if(pNodeTemp->mColor == kRBTreeColorRed) 
				{
					pNodeTemp->mColor        = kRBTreeColorBlack;
					pNodeChildParent->mColor = kRBTreeColorRed;

					pNodeRootRef = RBTreeRotateRight(pNodeChildParent, pNodeRootRef);
					pNodeTemp = pNodeChildParent->mpNodeLeft;
				}

				if(((pNodeTemp->mpNodeRight == NULL) || (pNodeTemp->mpNodeRight->mColor == kRBTreeColorBlack)) &&
					((pNodeTemp->mpNodeLeft  == NULL) || (pNodeTemp->mpNodeLeft->mColor  == kRBTreeColorBlack))) 
				{
					pNodeTemp->mColor = kRBTreeColorRed;
					pNodeChild       = pNodeChildParent;
					pNodeChildParent = pNodeChildParent->mpNodeParent;
				} 
				else 
				{
					if((pNodeTemp->mpNodeLeft == NULL) || (pNodeTemp->mpNodeLeft->mColor == kRBTreeColorBlack)) 
					{
						pNodeTemp->mpNodeRight->mColor = kRBTreeColorBlack;
						pNodeTemp->mColor              = kRBTreeColorRed;

						pNodeRootRef = RBTreeRotateLeft(pNodeTemp, pNodeRootRef);
						pNodeTemp = pNodeChildParent->mpNodeLeft;
					}

					pNodeTemp->mColor = pNodeChildParent->mColor;
					pNodeChildParent->mColor = kRBTreeColorBlack;

					if(pNodeTemp->mpNodeLeft) 
						pNodeTemp->mpNodeLeft->mColor = kRBTreeColorBlack;

					pNodeRootRef = RBTreeRotateRight(pNodeChildParent, pNodeRootRef);
					break;
				}
			}
		}

		if(pNodeChild)
			pNodeChild->mColor = kRBTreeColorBlack;
	}

} // RBTreeErase


//===================================================迭代器=======================================================

template <typename T, typename Pointer, typename Reference>
struct rbtree_iterator {
  typedef rbtree_iterator<T, Pointer, Reference> this_type;
  typedef rbtree_iterator<T, T *, T &> iterator;
  typedef rbtree_iterator<T, const T *, const T &> const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef rbtree_node_base base_node_type;
  typedef rbtree_node<T> node_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef mmm::bidirectional_iterator_tag iterator_category;

public:
  node_type *mpNode;

public:
  rbtree_iterator() : mpNode(NULL) {}
  explicit rbtree_iterator(const node_type *pNode)
      : mpNode(static_cast<node_type *>(const_cast<node_type *>(pNode))) {}
  rbtree_iterator(const iterator &x) : mpNode(x.mpNode) {}

  reference operator*() const { return mpNode->mValue; }
  pointer operator->() const { return &mpNode->mValue; }

  rbtree_iterator &operator++() {
    mpNode = static_cast<node_type *>(RBTreeIncrement(mpNode));
    return *this;
  }

  rbtree_iterator operator++(int) {
    this_type temp(*this);
    mpNode = static_cast<node_type *>(RBTreeIncrement(mpNode));
    return temp;
  }

  rbtree_iterator &operator--() {
    mpNode = static_cast<node_type *>(RBTreeDecrement(mpNode));
    return *this;
  }

  rbtree_iterator operator--(int) {
    this_type temp(*this);
    mpNode = static_cast<node_type *>(RBTreeDecrement(mpNode));
    return temp;
  }

}; // rbtree_iterator


//===================================================类实现=======================================================


//rb_base封装map, multimap, set, and multiset之间的差距. 如是否允许重复, extract_key的选择.


//初等模板
//适用于set
template <typename Key, typename Value, typename Compare, typename ExtractKey,
          bool bUniqueKeys, typename RBTree>
struct rb_base {
  typedef ExtractKey extract_key;

public:
  Compare mCompare; 

public:
  rb_base() : mCompare() {}
  rb_base(const Compare &compare) : mCompare(compare) {}
};

//multiset
template <typename Key, typename Value, typename Compare, typename ExtractKey,
          typename RBTree>
struct rb_base<Key, Value, Compare, ExtractKey, false, RBTree> {
  typedef ExtractKey extract_key;

public:
  Compare mCompare; 
public:
  rb_base() : mCompare() {}
  rb_base(const Compare &compare) : mCompare(compare) {}
};

//map
template <typename Key, typename Pair, typename Compare, typename RBTree>
struct rb_base<Key, Pair, Compare, mmm::select1st<Pair>, true, RBTree> {
  typedef mmm::select1st<Pair> extract_key;
public:
  Compare mCompare; 
public:
  rb_base() : mCompare() {}
  rb_base(const Compare &compare) : mCompare(compare) {}
};

//multimap

template <typename Key, typename Pair, typename Compare, typename RBTree>
struct rb_base<Key, Pair, Compare, mmm::select1st<Pair>, false, RBTree> {
  typedef mmm::select1st<Pair> extract_key;
public:
  Compare mCompare;
public:
  rb_base() : mCompare() {}
  rb_base(const Compare &compare) : mCompare(compare) {}
};

// rbtree

/// bMutableIterators (bool): true if rbtree::iterator is a mutable
/// iterator, false if iterator and const_iterator are both const iterators. 
/// It will be true for map and multimap and false for set and multiset.

template <typename Key, typename Value, typename Compare, typename Allocator, 
			typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
class rbtree
	: public rb_base<Key, Value, Compare, ExtractKey, bUniqueKeys, 
						rbtree<Key, Value, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys> >
{
public:
	typedef ptrdiff_t                                                                       difference_type;
	typedef size_t                                                                          size_type;
	typedef Key                                                                             key_type;
	typedef Value                                                                           value_type;
	typedef rbtree_node<value_type>                                                         node_type;
	typedef value_type&                                                                     reference;
	typedef const value_type&                                                               const_reference;
	typedef value_type*                                                                     pointer;
	typedef const value_type*                                                               const_pointer;

	typedef typename type_select<bMutableIterators, 
				rbtree_iterator<value_type, value_type*, value_type&>, 
				rbtree_iterator<value_type, const value_type*, const value_type&> >::type   iterator;
	typedef rbtree_iterator<value_type, const value_type*, const value_type&>               const_iterator;
	typedef mmm::reverse_iterator<iterator>                                               reverse_iterator;
	typedef mmm::reverse_iterator<const_iterator>                                         const_reverse_iterator;

	typedef Allocator                                                                       allocator_type;
	typedef Compare                                                                         key_compare;
	typedef typename type_select<bUniqueKeys, mmm::pair<iterator, bool>, iterator>::type    insert_return_type;  // map/set::insert return a pair, multimap/multiset::iterator return an iterator.
	typedef rbtree<Key, Value, Compare, Allocator, 
					ExtractKey, bMutableIterators, bUniqueKeys>                             this_type;
	typedef rb_base<Key, Value, Compare, ExtractKey, bUniqueKeys, this_type>                base_type;
	typedef integral_constant<bool, bUniqueKeys>                                            has_unique_keys_type;
	typedef typename base_type::extract_key                                                 extract_key;

	using base_type::mCompare;

public:
	rbtree_node_base  mAnchor;      ///  作为end()标识, 其left指向begin(), right指向rbegin().
	size_type         mnSize;       /// 存储的数据个数
	allocator_type    mAllocator;

public:
	// ctor/dtor
	rbtree();
	rbtree(const allocator_type& allocator);
	rbtree(const Compare& compare, const allocator_type& allocator = allocator_type());
	rbtree(const this_type& x);
	rbtree(this_type&& x);
	rbtree(this_type&& x, const allocator_type& allocator);

	template <typename InputIterator>
	rbtree(InputIterator first, InputIterator last, const Compare& compare, const allocator_type& allocator = allocator_type());

	~rbtree(){DoNukeSubtree((node_type*)mAnchor.mpNodeParent);}

public:
	// properties
	const allocator_type& get_allocator() const noexcept {return mAllocator;}
	allocator_type&       get_allocator() noexcept { return mAllocator;}
	void                  set_allocator(const allocator_type& allocator) {mAllocator = allocator;}

	const key_compare& key_comp() const { return mCompare; }
	key_compare&       key_comp()       { return mCompare; }

	this_type& operator=(const this_type& x);
	this_type& operator=(std::initializer_list<value_type> ilist);
	this_type& operator=(this_type&& x);

	void swap(this_type& x);

public: 
	// iterators
	iterator        begin() noexcept 		{ return iterator(static_cast<node_type*>(mAnchor.mpNodeLeft)); }
	const_iterator  begin() const noexcept  		{ return const_iterator(static_cast<node_type*>(const_cast<rbtree_node_base*>(mAnchor.mpNodeLeft))); }

	const_iterator  cbegin() const noexcept 		{ return const_iterator(static_cast<node_type*>(const_cast<rbtree_node_base*>(mAnchor.mpNodeLeft))); }


	iterator        end() noexcept 		{ return iterator(static_cast<node_type*>(&mAnchor)); }


	const_iterator  end() const noexcept 		{ return const_iterator(static_cast<node_type*>(const_cast<rbtree_node_base*>(&mAnchor))); }

	const_iterator  cend() const noexcept 		{ return const_iterator(static_cast<node_type*>(const_cast<rbtree_node_base*>(&mAnchor))); }



	reverse_iterator        rbegin() noexcept 		{ return reverse_iterator(end()); }
	const_reverse_iterator  rbegin() const noexcept 		{ return const_reverse_iterator(end()); }
	const_reverse_iterator  crbegin() const noexcept 		{ return const_reverse_iterator(end()); }

	reverse_iterator        rend() noexcept 		{ return reverse_iterator(begin()); }
	const_reverse_iterator  rend() const noexcept 		{ return reverse_iterator(begin()); }
	const_reverse_iterator  crend() const noexcept 		{ return const_reverse_iterator(begin()); }

public:
	bool      empty() const noexcept { return (mnSize == 0); }
	size_type size() const noexcept { return mnSize; }

	template <class... Args>
	insert_return_type emplace(Args&&... args)	{
		return DoInsertValue(has_unique_keys_type(), mmm::forward<Args>(args)...);
	}

	template <class... Args> 
	iterator emplace_hint(const_iterator position, Args&&... args)	{
		return DoInsertValueHint(has_unique_keys_type(), position, mmm::forward<Args>(args)...);
	}

	template <class... Args> mmm::pair<iterator, bool> try_emplace(const key_type& key, Args&&... args)	{
		return DoInsertValue(has_unique_keys_type(), piecewise_construct_t(), forward_as_tuple(key), forward_as_tuple(forward<Args>(args)...));
	}

	template <class... Args> mmm::pair<iterator, bool> try_emplace(key_type&& key, Args&&... args)	{
		return DoInsertValue(has_unique_keys_type(), piecewise_construct_t(), forward_as_tuple(mmm::move(key)), forward_as_tuple(forward<Args>(args)...));
	}

	template <class... Args> iterator                    
	try_emplace(const_iterator position, const key_type& key, Args&&... args)	{
	return DoInsertValueHint(
			has_unique_keys_type(), position,
			value_type(piecewise_construct_t(), forward_as_tuple(key), forward_as_tuple(forward<Args>(args)...)));
	}

	template <class... Args> iterator                   
	try_emplace(const_iterator position, key_type&& key, Args&&... args)	{
	return DoInsertValueHint(
			has_unique_keys_type(), position,
			value_type(piecewise_construct_t(), forward_as_tuple(key), forward_as_tuple(forward<Args>(args)...)));
	}



	// // Standard conversion overload to avoid the overhead of mismatched 'pair<const Key, Value>' types.
	// template <class P, class = typename mmm::enable_if<mmm::is_constructible<value_type, P&&>::value>::type> 
	// insert_return_type insert(P&& otherValue) 	{ 
	// // Need to use forward instead of move because P&& is a "universal reference" instead of an rvalue reference.
	// 	return emplace(mmm::forward<P>(otherValue));
	// }


	insert_return_type insert(value_type&& otherValue) 	{ 
		return emplace(mmm::forward<value_type>(otherValue));
	}


	// Currently limited to value_type instead of P because it collides with insert(InputIterator, InputIterator).
	// To allow this to work with templated P we need to implement a compile-time specialization for the
	// case that P&& is const_iterator and have that specialization handle insert(InputIterator, InputIterator)
	// instead of insert(InputIterator, InputIterator). Curiously, neither libstdc++ nor libc++
	// implement this function either, which suggests they ran into the same problem I did here
	// and haven't yet resolved it (at least as of March 2014, GCC 4.8.1).
	iterator insert(const_iterator hint, value_type&& value) 	{
		return DoInsertValueHint(has_unique_keys_type(), hint, value_type(mmm::move(value)));
	}


	/// map::insert and set::insert return a pair, while multimap::insert and
	/// multiset::insert return an iterator.
	insert_return_type insert(const value_type& value) 	{
		return DoInsertValue(has_unique_keys_type(), value);
	}



	// C++ standard: inserts value if and only if there is no element with 
	// key equivalent to the key of t in containers with unique keys; always 
	// inserts value in containers with equivalent keys. Always returns the 
	// iterator pointing to the element with key equivalent to the key of value. 
	// iterator position is a hint pointing to where the insert should start
	// to search. However, there is a potential defect/improvement report on this behaviour:
	// LWG issue #233 (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1780.html)
	// We follow the same approach as SGI STL/STLPort and use the position as
	// a forced insertion position for the value when possible.
	iterator insert(const_iterator position, const value_type& value)	{
		return DoInsertValueHint(has_unique_keys_type(), position, value);
	}


	void insert(std::initializer_list<value_type> ilist);

	template <typename InputIterator>
	void insert(InputIterator first, InputIterator last);

	// TODO(rparolin):
	// insert_return_type insert(node_type&& nh);
	// iterator insert(const_iterator hint, node_type&& nh);

	template <class M> pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
	template <class M> pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
	template <class M> iterator             insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
	template <class M> iterator             insert_or_assign(const_iterator hint, key_type&& k, M&& obj);

	iterator         erase(const_iterator position);
	iterator         erase(const_iterator first, const_iterator last);
	reverse_iterator erase(const_reverse_iterator position);
	reverse_iterator erase(const_reverse_iterator first, const_reverse_iterator last);

	// For some reason, multiple STL versions make a specialization 
	// for erasing an array of key_types. I'm pretty sure we don't
	// need this, but just to be safe we will follow suit. 
	// The implementation is trivial. Returns void because the values
	// could well be randomly distributed throughout the tree and thus
	// a return value would be nearly meaningless.
	void erase(const key_type* first, const key_type* last);

	void clear();
	void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

	iterator       find(const key_type& key);
	const_iterator find(const key_type& key) const;

	/// Implements a find whereby the user supplies a comparison of a different type
	/// than the tree's value_type. A useful case of this is one whereby you have
	/// a container of string objects but want to do searches via passing in char pointers.
	/// The problem is that without this kind of find, you need to do the expensive operation
	/// of converting the char pointer to a string so it can be used as the argument to the
	/// find function.
	///
	/// Example usage (note that the compare uses string as first type and char* as second):
	///     set<string> strings;
	///     strings.find_as("hello", less_2<string, const char*>());
	///
	template <typename U, typename Compare2> iterator       find_as(const U& u, Compare2 compare2);
	template <typename U, typename Compare2> const_iterator find_as(const U& u, Compare2 compare2) const;

	iterator       lower_bound(const key_type& key);
	const_iterator lower_bound(const key_type& key) const;

	iterator       upper_bound(const key_type& key);
	const_iterator upper_bound(const key_type& key) const;

	bool validate() const;

protected:
	node_type* DoAllocateNode();
	void       DoFreeNode(node_type* pNode);

	node_type* DoCreateNodeFromKey(const key_type& key);

	template<class... Args>
	node_type* DoCreateNode(Args&&... args);
	node_type* DoCreateNode(const value_type& value);
	node_type* DoCreateNode(value_type&& value);
	node_type* DoCreateNode(const node_type* pNodeSource, node_type* pNodeParent);

	node_type* DoCopySubtree(const node_type* pNodeSource, node_type* pNodeDest);
	void       DoNukeSubtree(node_type* pNode);

	template <class... Args>
	mmm::pair<iterator, bool> DoInsertValue(true_type, Args&&... args);

	template <class... Args>
	iterator DoInsertValue(false_type, Args&&... args);

	mmm::pair<iterator, bool> DoInsertValue(true_type, value_type&& value);
	iterator DoInsertValue(false_type, value_type&& value);

	template <class... Args>
	iterator DoInsertValueImpl(node_type* pNodeParent, bool bForceToLeft, const key_type& key, Args&&... args);
	iterator DoInsertValueImpl(node_type* pNodeParent, bool bForceToLeft, const key_type& key, node_type* pNodeNew);

	mmm::pair<iterator, bool> DoInsertKey(true_type, const key_type& key);
	iterator                    DoInsertKey(false_type, const key_type& key);

	iterator DoInsertValueHint(true_type, const_iterator position, const value_type& value);
	iterator DoInsertValueHint(false_type, const_iterator position, const value_type& value);

	iterator DoInsertKey(true_type, const_iterator position, const key_type& key);  // By design we return iterator and not a pair.
	iterator DoInsertKey(false_type, const_iterator position, const key_type& key);
	iterator DoInsertKeyImpl(node_type* pNodeParent, bool bForceToLeft, const key_type& key);

	node_type* DoGetKeyInsertionPositionUniqueKeys(bool& canInsert, const key_type& key);
	node_type* DoGetKeyInsertionPositionNonuniqueKeys(const key_type& key);

	node_type* DoGetKeyInsertionPositionUniqueKeysHint(const_iterator position, bool& bForceToLeft, const key_type& key);
	node_type* DoGetKeyInsertionPositionNonuniqueKeysHint(const_iterator position, bool& bForceToLeft, const key_type& key);

}; // rbtree



///////////////////////////////////////////////////////////////////////
// rbtree_node_base functions
///////////////////////////////////////////////////////////////////////

inline rbtree_node_base* RBTreeGetMinChild(const rbtree_node_base* pNodeBase)
{
	while(pNodeBase->mpNodeLeft) 
		pNodeBase = pNodeBase->mpNodeLeft;
	return const_cast<rbtree_node_base*>(pNodeBase);
}

inline rbtree_node_base* RBTreeGetMaxChild(const rbtree_node_base* pNodeBase)
{
	while(pNodeBase->mpNodeRight) 
		pNodeBase = pNodeBase->mpNodeRight;
	return const_cast<rbtree_node_base*>(pNodeBase);
}


// The C++ defect report #179 requires that we support comparisons between const and non-const iterators.
// Thus we provide additional template paremeters here to support this. The defect report does not
// require us to support comparisons between reverse_iterators and const_reverse_iterators.
template <typename T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
inline bool operator==(const rbtree_iterator<T, PointerA, ReferenceA>& a, 
						const rbtree_iterator<T, PointerB, ReferenceB>& b)
{
	return a.mpNode == b.mpNode;
}


template <typename T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
inline bool operator!=(const rbtree_iterator<T, PointerA, ReferenceA>& a, 
						const rbtree_iterator<T, PointerB, ReferenceB>& b)
{
	return a.mpNode != b.mpNode;
}


// We provide a version of operator!= for the case where the iterators are of the 
// same type. This helps prevent ambiguity errors in the presence of rel_ops.
template <typename T, typename Pointer, typename Reference>
inline bool operator!=(const rbtree_iterator<T, Pointer, Reference>& a, 
						const rbtree_iterator<T, Pointer, Reference>& b)
{
	return a.mpNode != b.mpNode;
}




///////////////////////////////////////////////////////////////////////
// rbtree functions
///////////////////////////////////////////////////////////////////////

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline rbtree<K, V, C, A, E, bM, bU>::rbtree()
	: mAnchor(),
		mnSize(0),
		mAllocator()
{
	reset_lose_memory();
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline rbtree<K, V, C, A, E, bM, bU>::rbtree(const allocator_type& allocator)
	: mAnchor(),
		mnSize(0),
		mAllocator(allocator)
{
	reset_lose_memory();
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline rbtree<K, V, C, A, E, bM, bU>::rbtree(const C& compare, const allocator_type& allocator)
	: base_type(compare),
		mAnchor(),
		mnSize(0),
		mAllocator(allocator)
{
	reset_lose_memory();
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline rbtree<K, V, C, A, E, bM, bU>::rbtree(const this_type& x)
	: base_type(x.mCompare),
		mAnchor(),
		mnSize(0),
		mAllocator(x.mAllocator)
{
	reset_lose_memory();

	if(x.mAnchor.mpNodeParent) // mAnchor.mpNodeParent is the rb_tree root node.
	{
		mAnchor.mpNodeParent = DoCopySubtree((const node_type*)x.mAnchor.mpNodeParent, (node_type*)&mAnchor);
		mAnchor.mpNodeRight  = RBTreeGetMaxChild(mAnchor.mpNodeParent);
		mAnchor.mpNodeLeft   = RBTreeGetMinChild(mAnchor.mpNodeParent);
		mnSize               = x.mnSize;
	}
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline rbtree<K, V, C, A, E, bM, bU>::rbtree(this_type&& x)
	: base_type(x.mCompare),
		mAnchor(),
		mnSize(0),
		mAllocator(x.mAllocator)
{
	reset_lose_memory();
	swap(x);
}

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline rbtree<K, V, C, A, E, bM, bU>::rbtree(this_type&& x, const allocator_type& allocator)
	: base_type(x.mCompare),
		mAnchor(),
		mnSize(0),
		mAllocator(allocator)
{
	reset_lose_memory();
	swap(x); // swap will directly or indirectly handle the possibility that mAllocator != x.mAllocator.
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <typename InputIterator>
inline rbtree<K, V, C, A, E, bM, bU>::rbtree(InputIterator first, InputIterator last, const C& compare, const allocator_type& allocator)
	: base_type(compare),
		mAnchor(),
		mnSize(0),
		mAllocator(allocator)
{
	reset_lose_memory();
	for(; first != last; ++first)
		insert(*first);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::this_type&
rbtree<K, V, C, A, E, bM, bU>::operator=(const this_type& x)
{
	if(this != &x)
	{
		clear();

		base_type::mCompare = x.mCompare;

		if(x.mAnchor.mpNodeParent) // mAnchor.mpNodeParent is the rb_tree root node.
		{
			mAnchor.mpNodeParent = DoCopySubtree((const node_type*)x.mAnchor.mpNodeParent, (node_type*)&mAnchor);
			mAnchor.mpNodeRight  = RBTreeGetMaxChild(mAnchor.mpNodeParent);
			mAnchor.mpNodeLeft   = RBTreeGetMinChild(mAnchor.mpNodeParent);
			mnSize               = x.mnSize;
		}
	}
	return *this;
}

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::this_type&
rbtree<K, V, C, A, E, bM, bU>::operator=(this_type&& x)
{
	if(this != &x)
	{
		clear();        // To consider: Are we really required to clear here? x is going away soon and will clear itself in its dtor.
		swap(x);        // member swap handles the case that x has a different allocator than our allocator by doing a copy.
	}
	return *this; 
}

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::this_type&
rbtree<K, V, C, A, E, bM, bU>::operator=(std::initializer_list<value_type> ilist)
{
	// The simplest means of doing this is to clear and insert. There probably isn't a generic
	// solution that's any more efficient without having prior knowledge of the ilist contents.
	clear();

	for(typename std::initializer_list<value_type>::iterator it = ilist.begin(), itEnd = ilist.end(); it != itEnd; ++it)
		DoInsertValue(has_unique_keys_type(), mmm::move(*it));

	return *this;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
void rbtree<K, V, C, A, E, bM, bU>::swap(this_type& x)
{
	if(mAllocator == x.mAllocator) // If allocators are equivalent...
	{
		// Most of our members can be exchaged by a basic swap:
		// We leave mAllocator as-is.
		mmm::swap(mnSize,              x.mnSize);
		mmm::swap(base_type::mCompare, x.mCompare);

		// However, because our anchor node is a part of our class instance and not 
		// dynamically allocated, we can't do a swap of it but must do a more elaborate
		// procedure. This is the downside to having the mAnchor be like this, but 
		// otherwise we consider it a good idea to avoid allocating memory for a 
		// nominal container instance.

		// We optimize for the expected most common case: both pointers being non-null.
		if(mAnchor.mpNodeParent && x.mAnchor.mpNodeParent) // If both pointers are non-null...
		{
			mmm::swap(mAnchor.mpNodeRight,  x.mAnchor.mpNodeRight);
			mmm::swap(mAnchor.mpNodeLeft,   x.mAnchor.mpNodeLeft);
			mmm::swap(mAnchor.mpNodeParent, x.mAnchor.mpNodeParent);

			// We need to fix up the anchors to point to themselves (we can't just swap them).
			mAnchor.mpNodeParent->mpNodeParent   = &mAnchor;
			x.mAnchor.mpNodeParent->mpNodeParent = &x.mAnchor;
		}
		else if(mAnchor.mpNodeParent)
		{
			x.mAnchor.mpNodeRight  = mAnchor.mpNodeRight;
			x.mAnchor.mpNodeLeft   = mAnchor.mpNodeLeft;
			x.mAnchor.mpNodeParent = mAnchor.mpNodeParent;
			x.mAnchor.mpNodeParent->mpNodeParent = &x.mAnchor;

			// We need to fix up our anchor to point it itself (we can't have it swap with x).
			mAnchor.mpNodeRight  = &mAnchor;
			mAnchor.mpNodeLeft   = &mAnchor;
			mAnchor.mpNodeParent = NULL;
		}
		else if(x.mAnchor.mpNodeParent)
		{
			mAnchor.mpNodeRight  = x.mAnchor.mpNodeRight;
			mAnchor.mpNodeLeft   = x.mAnchor.mpNodeLeft;
			mAnchor.mpNodeParent = x.mAnchor.mpNodeParent;
			mAnchor.mpNodeParent->mpNodeParent = &mAnchor;

			// We need to fix up x's anchor to point it itself (we can't have it swap with us).
			x.mAnchor.mpNodeRight  = &x.mAnchor;
			x.mAnchor.mpNodeLeft   = &x.mAnchor;
			x.mAnchor.mpNodeParent = NULL;
		} // Else both are NULL and there is nothing to do.
	}
	else
	{
		const this_type temp(*this); // Can't call mmm::swap because that would
		*this = x;                   // itself call this member swap function.
		x     = temp;
	}
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <class M>
mmm::pair<typename rbtree<K, V, C, A, E, bM, bU>::iterator, bool>
rbtree<K, V, C, A, E, bM, bU>::insert_or_assign(const key_type& k, M&& obj)
{
	auto iter = find(k);

	if(iter == end())
	{
		return insert(value_type(piecewise_construct_t(), forward_as_tuple(k), forward_as_tuple(mmm::forward<M>(obj))));
	}
	else
	{
		iter->second = mmm::forward<M>(obj);
		return {iter, false};
	}
}

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <class M>
mmm::pair<typename rbtree<K, V, C, A, E, bM, bU>::iterator, bool>
rbtree<K, V, C, A, E, bM, bU>::insert_or_assign(key_type&& k, M&& obj)
{
	auto iter = find(k);

	if(iter == end())
	{
		return insert(value_type(piecewise_construct_t(), forward_as_tuple(mmm::move(k)), forward_as_tuple(mmm::forward<M>(obj))));
	}
	else
	{
		iter->second = mmm::forward<M>(obj);
		return {iter, false};
	}
}

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <class M>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::insert_or_assign(const_iterator hint, const key_type& k, M&& obj)
{
	auto iter = find(k);

	if(iter == end())
	{
		return insert(hint, value_type(piecewise_construct_t(), forward_as_tuple(k), forward_as_tuple(mmm::forward<M>(obj))));
	}
	else
	{
		iter->second = mmm::forward<M>(obj);
		return iter;
	}
}

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <class M>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::insert_or_assign(const_iterator hint, key_type&& k, M&& obj)
{
	auto iter = find(k);

	if(iter == end())
	{
		return insert(hint, value_type(piecewise_construct_t(), forward_as_tuple(mmm::move(k)), forward_as_tuple(mmm::forward<M>(obj))));
	}
	else
	{
		iter->second = mmm::forward<M>(obj);
		return iter;
	}
}

template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoGetKeyInsertionPositionUniqueKeys(bool& canInsert, const key_type& key)
{
	// This code is essentially a slightly modified copy of the the rbtree::insert 
	// function whereby this version takes a key and not a full value_type.
	extract_key extractKey;

	node_type* pCurrent    = (node_type*)mAnchor.mpNodeParent; // Start with the root node.
	node_type* pLowerBound = (node_type*)&mAnchor;             // Set it to the container end for now.
	node_type* pParent;                                        // This will be where we insert the new node.

	bool bValueLessThanNode = true; // If the tree is empty, this will result in an insertion at the front.

	// Find insertion position of the value. This will either be a position which 
	// already contains the value, a position which is greater than the value or
	// end(), which we treat like a position which is greater than the value.
	while(pCurrent) // Do a walk down the tree.
	{
		bValueLessThanNode = mCompare(key, extractKey(pCurrent->mValue));
		pLowerBound        = pCurrent;

		if(bValueLessThanNode)
		{
			pCurrent = (node_type*)pCurrent->mpNodeLeft;
		}
		else
			pCurrent = (node_type*)pCurrent->mpNodeRight;
	}

	pParent = pLowerBound; // pLowerBound is actually upper bound right now (i.e. it is > value instead of <=), but we will make it the lower bound below.

	if(bValueLessThanNode) // If we ended up on the left side of the last parent node...
	{
		if(pLowerBound != (node_type*)mAnchor.mpNodeLeft) // If the tree was empty or if we otherwise need to insert at the very front of the tree...
		{
			// At this point, pLowerBound points to a node which is > than value.
			// Move it back by one, so that it points to a node which is <= value.
			pLowerBound = (node_type*)RBTreeDecrement(pLowerBound);
		}
		else
		{
			canInsert = true;
			return pLowerBound;
		}
	}

	// Since here we require values to be unique, we will do nothing if the value already exists.
	if(mCompare(extractKey(pLowerBound->mValue), key)) // If the node is < the value (i.e. if value is >= the node)...
	{
		canInsert = true;
		return pParent;
	}

	// The item already exists (as found by the compare directly above), so return false.
	canInsert = false;
	return pLowerBound;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoGetKeyInsertionPositionNonuniqueKeys(const key_type& key)
{
	// This is the pathway for insertion of non-unique keys (multimap and multiset, but not map and set).
	node_type* pCurrent  = (node_type*)mAnchor.mpNodeParent; // Start with the root node.
	node_type* pRangeEnd = (node_type*)&mAnchor;             // Set it to the container end for now.
	extract_key extractKey;

	while(pCurrent)
	{
		pRangeEnd = pCurrent;

		if(mCompare(key, extractKey(pCurrent->mValue)))
		{
			mmm_VALIDATE_COMPARE(!mCompare(extractKey(pCurrent->mValue), key)); // Validate that the compare function is sane.
			pCurrent = (node_type*)pCurrent->mpNodeLeft;
		}
		else
			pCurrent = (node_type*)pCurrent->mpNodeRight;
	}

	return pRangeEnd;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
mmm::pair<typename rbtree<K, V, C, A, E, bM, bU>::iterator, bool> 
rbtree<K, V, C, A, E, bM, bU>::DoInsertValue(true_type, value_type&& value)
{
	extract_key extractKey;
	key_type    key(extractKey(value));
	bool        canInsert;
	node_type*  pPosition = DoGetKeyInsertionPositionUniqueKeys(canInsert, key);

	if(canInsert)
	{
		const iterator itResult(DoInsertValueImpl(pPosition, false, key, mmm::move(value)));
		return pair<iterator, bool>(itResult, true);
	}

	return pair<iterator, bool>(iterator(pPosition), false);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator 
rbtree<K, V, C, A, E, bM, bU>::DoInsertValue(false_type, value_type&& value)
{
	extract_key extractKey;
	key_type    key(extractKey(value));
	node_type*  pPosition = DoGetKeyInsertionPositionNonuniqueKeys(key);

	return DoInsertValueImpl(pPosition, false, key, mmm::move(value));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <class... Args>
mmm::pair<typename rbtree<K, V, C, A, E, bM, bU>::iterator, bool>
rbtree<K, V, C, A, E, bM, bU>::DoInsertValue(true_type, Args&&... args) // true_type means keys are unique.
{
	// This is the pathway for insertion of unique keys (map and set, but not multimap and multiset).
	// Note that we return a pair and not an iterator. This is because the C++ standard for map
	// and set is to return a pair and not just an iterator.

	node_type* pNodeNew = DoCreateNode(mmm::forward<Args>(args)...); // Note that pNodeNew->mpLeft, mpRight, mpParent, will be uninitialized.
	const key_type& key = extract_key{}(pNodeNew->mValue);

	bool        canInsert;
	node_type*  pPosition = DoGetKeyInsertionPositionUniqueKeys(canInsert, key);

	if(canInsert)
	{
		iterator itResult(DoInsertValueImpl(pPosition, false, key, pNodeNew));
		return pair<iterator, bool>(itResult, true);
	}

	DoFreeNode(pNodeNew);
	return pair<iterator, bool>(iterator(pPosition), false);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <class... Args>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertValue(false_type, Args&&... args) // false_type means keys are not unique.
{
	// We have a problem here if sizeof(value_type) is too big for the stack. We may want to consider having a specialization for large value_types.
	// To do: Change this so that we call DoCreateNode(mmm::forward<Args>(args)...) here and use the value from the resulting pNode to get the 
	// key, and make DoInsertValueImpl take that node as an argument. That way there is no value created on the stack.

	node_type* const pNodeNew = DoCreateNode(mmm::forward<Args>(args)...); // Note that pNodeNew->mpLeft, mpRight, mpParent, will be uninitialized.
	const key_type& key = extract_key{}(pNodeNew->mValue);

	node_type* pPosition = DoGetKeyInsertionPositionNonuniqueKeys(key);

	return DoInsertValueImpl(pPosition, false, key, pNodeNew);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <class... Args>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertValueImpl(node_type* pNodeParent, bool bForceToLeft, const key_type& key, Args&&... args)
{
	node_type* const pNodeNew = DoCreateNode(mmm::forward<Args>(args)...); // Note that pNodeNew->mpLeft, mpRight, mpParent, will be uninitialized.

	return DoInsertValueImpl(pNodeParent, bForceToLeft, key, pNodeNew);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertValueImpl(node_type* pNodeParent, bool bForceToLeft, const key_type& key, node_type* pNodeNew)
{

	RBTreeSide  side;
	extract_key extractKey;

	// The reason we may want to have bForceToLeft == true is that pNodeParent->mValue and value may be equal.
	// In that case it doesn't matter what side we insert on, except that the C++ LWG #233 improvement report
	// suggests that we should use the insert hint position to force an ordering. So that's what we do.
	if(bForceToLeft || (pNodeParent == &mAnchor) || mCompare(key, extractKey(pNodeParent->mValue)))
		side = kRBTreeSideLeft;
	else
		side = kRBTreeSideRight;

	RBTreeInsert(pNodeNew, pNodeParent, &mAnchor, side);
	mnSize++;

	return iterator(pNodeNew);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
mmm::pair<typename rbtree<K, V, C, A, E, bM, bU>::iterator, bool>
rbtree<K, V, C, A, E, bM, bU>::DoInsertKey(true_type, const key_type& key) // true_type means keys are unique.
{
	// This is the pathway for insertion of unique keys (map and set, but not multimap and multiset).
	// Note that we return a pair and not an iterator. This is because the C++ standard for map
	// and set is to return a pair and not just an iterator.
	bool       canInsert;
	node_type* pPosition = DoGetKeyInsertionPositionUniqueKeys(canInsert, key);

	if(canInsert)
	{
		const iterator itResult(DoInsertKeyImpl(pPosition, false, key));
		return pair<iterator, bool>(itResult, true);
	}

	return pair<iterator, bool>(iterator(pPosition), false);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertKey(false_type, const key_type& key) // false_type means keys are not unique.
{
	node_type* pPosition = DoGetKeyInsertionPositionNonuniqueKeys(key);

	return DoInsertKeyImpl(pPosition, false, key);
}



template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoGetKeyInsertionPositionUniqueKeysHint(const_iterator position, bool& bForceToLeft, const key_type& key)
{
	extract_key extractKey;

	if((position.mpNode != mAnchor.mpNodeRight) && (position.mpNode != &mAnchor)) // If the user specified a specific insertion position...
	{
		iterator itNext(position.mpNode);
		++itNext;

		// To consider: Change this so that 'position' specifies the position after 
		// where the insertion goes and not the position before where the insertion goes.
		// Doing so would make this more in line with user expectations and with LWG #233.
		const bool bPositionLessThanValue = mCompare(extractKey(position.mpNode->mValue), key);

		if(bPositionLessThanValue) // If (value > *position)...
		{
			const bool bValueLessThanNext = mCompare(key, extractKey(itNext.mpNode->mValue));

			if(bValueLessThanNext) // If value < *itNext...
			{
				if(position.mpNode->mpNodeRight)
				{
					bForceToLeft = true; // Specifically insert in front of (to the left of) itNext (and thus after 'position').
					return itNext.mpNode;
				}

				bForceToLeft = false;
				return position.mpNode;
			}
		}

		bForceToLeft = false;
		return NULL;  // The above specified hint was not useful, then we do a regular insertion.
	}

	if(mnSize && mCompare(extractKey(((node_type*)mAnchor.mpNodeRight)->mValue), key))
	{
		bForceToLeft = false;
		return (node_type*)mAnchor.mpNodeRight;
	}

	bForceToLeft = false;
	return NULL; // The caller can do a default insert.
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoGetKeyInsertionPositionNonuniqueKeysHint(const_iterator position, bool& bForceToLeft, const key_type& key)
{
	extract_key extractKey;

	if((position.mpNode != mAnchor.mpNodeRight) && (position.mpNode != &mAnchor)) // If the user specified a specific insertion position...
	{
		iterator itNext(position.mpNode);
		++itNext;

		// To consider: Change this so that 'position' specifies the position after 
		// where the insertion goes and not the position before where the insertion goes.
		// Doing so would make this more in line with user expectations and with LWG #233.
		if(!mCompare(key, extractKey(position.mpNode->mValue)) && // If value >= *position && 
			!mCompare(extractKey(itNext.mpNode->mValue), key))     // if value <= *itNext...
		{
			if(position.mpNode->mpNodeRight) // If there are any nodes to the right... [this expression will always be true as long as we aren't at the end()]
			{
				bForceToLeft = true; // Specifically insert in front of (to the left of) itNext (and thus after 'position').
				return itNext.mpNode;
			}

			bForceToLeft = false;
			return position.mpNode;
		}

		bForceToLeft = false;
		return NULL; // The above specified hint was not useful, then we do a regular insertion.
	}

	// This pathway shouldn't be commonly executed, as the user shouldn't be calling 
	// this hinted version of insert if the user isn't providing a useful hint.
	if(mnSize && !mCompare(key, extractKey(((node_type*)mAnchor.mpNodeRight)->mValue))) // If we are non-empty and the value is >= the last node...
	{
		bForceToLeft =false;
		return (node_type*)mAnchor.mpNodeRight;
	}

	bForceToLeft = false;
	return NULL;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertValueHint(true_type, const_iterator position, const value_type& value) // true_type means keys are unique.
{
	// This is the pathway for insertion of unique keys (map and set, but not multimap and multiset).
	//
	// We follow the same approach as SGI STL/STLPort and use the position as
	// a forced insertion position for the value when possible.

	extract_key extractKey;
	key_type    key(extractKey(value));
	bool        bForceToLeft;
	node_type*  pPosition = DoGetKeyInsertionPositionUniqueKeysHint(position, bForceToLeft, key);

	if(pPosition)
		return DoInsertValueImpl(pPosition, bForceToLeft, key, value);
	else
		return DoInsertValue(has_unique_keys_type(), value).first;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertValueHint(false_type, const_iterator position, const value_type& value) // false_type means keys are not unique.
{
	// This is the pathway for insertion of non-unique keys (multimap and multiset, but not map and set).
	//
	// We follow the same approach as SGI STL/STLPort and use the position as
	// a forced insertion position for the value when possible.
	extract_key extractKey;
	key_type    key(extractKey(value));
	bool        bForceToLeft;
	node_type*  pPosition = DoGetKeyInsertionPositionNonuniqueKeysHint(position, bForceToLeft, key);

	if(pPosition)
		return DoInsertValueImpl(pPosition, bForceToLeft, key, value);
	else
		return DoInsertValue(has_unique_keys_type(), value);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertKey(true_type, const_iterator position, const key_type& key) // true_type means keys are unique.
{
	bool       bForceToLeft;
	node_type* pPosition = DoGetKeyInsertionPositionUniqueKeysHint(position, bForceToLeft, key);

	if(pPosition)
		return DoInsertKeyImpl(pPosition, bForceToLeft, key);
	else
		return DoInsertKey(has_unique_keys_type(), key).first;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertKey(false_type, const_iterator position, const key_type& key) // false_type means keys are not unique.
{
	// This is the pathway for insertion of non-unique keys (multimap and multiset, but not map and set).
	//
	// We follow the same approach as SGI STL/STLPort and use the position as
	// a forced insertion position for the value when possible.
	bool       bForceToLeft;
	node_type* pPosition = DoGetKeyInsertionPositionNonuniqueKeysHint(position, bForceToLeft, key);

	if(pPosition)
		return DoInsertKeyImpl(pPosition, bForceToLeft, key);
	else
		return DoInsertKey(has_unique_keys_type(), key); // We are empty or we are inserting at the end.
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::DoInsertKeyImpl(node_type* pNodeParent, bool bForceToLeft, const key_type& key)
{
	RBTreeSide  side;
	extract_key extractKey;

	// The reason we may want to have bForceToLeft == true is that pNodeParent->mValue and value may be equal.
	// In that case it doesn't matter what side we insert on, except that the C++ LWG #233 improvement report
	// suggests that we should use the insert hint position to force an ordering. So that's what we do.
	if(bForceToLeft || (pNodeParent == &mAnchor) || mCompare(key, extractKey(pNodeParent->mValue)))
		side = kRBTreeSideLeft;
	else
		side = kRBTreeSideRight;

	node_type* const pNodeNew = DoCreateNodeFromKey(key); // Note that pNodeNew->mpLeft, mpRight, mpParent, will be uninitialized.
	RBTreeInsert(pNodeNew, pNodeParent, &mAnchor, side);
	mnSize++;

	return iterator(pNodeNew);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
void rbtree<K, V, C, A, E, bM, bU>::insert(std::initializer_list<value_type> ilist)
{
	for(typename std::initializer_list<value_type>::iterator it = ilist.begin(), itEnd = ilist.end(); it != itEnd; ++it)
		DoInsertValue(has_unique_keys_type(), mmm::move(*it));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <typename InputIterator>
void rbtree<K, V, C, A, E, bM, bU>::insert(InputIterator first, InputIterator last)
{
	for( ; first != last; ++first)
		DoInsertValue(has_unique_keys_type(), *first); // Or maybe we should call 'insert(end(), *first)' instead. If the first-last range was sorted then this might make some sense.
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline void rbtree<K, V, C, A, E, bM, bU>::clear()
{
	// Erase the entire tree. DoNukeSubtree is not a 
	// conventional erase function, as it does no rebalancing.
	DoNukeSubtree((node_type*)mAnchor.mpNodeParent);
	reset_lose_memory();
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline void rbtree<K, V, C, A, E, bM, bU>::reset_lose_memory()
{
	// The reset_lose_memory function is a special extension function which unilaterally 
	// resets the container to an empty state without freeing the memory of 
	// the contained objects. This is useful for very quickly tearing down a 
	// container built into scratch memory.
	mAnchor.mpNodeRight  = &mAnchor;
	mAnchor.mpNodeLeft   = &mAnchor;
	mAnchor.mpNodeParent = NULL;
	mAnchor.mColor       = kRBTreeColorRed;
	mnSize               = 0;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::erase(const_iterator position)
{
	const iterator iErase(position.mpNode);
	--mnSize; // Interleave this between the two references to itNext. We expect no exceptions to occur during the code below.
	++position;
	RBTreeErase(iErase.mpNode, &mAnchor);
	DoFreeNode(iErase.mpNode);
	return iterator(position.mpNode);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::erase(const_iterator first, const_iterator last)
{
	// We expect that if the user means to clear the container, they will call clear.
	if(((first.mpNode != mAnchor.mpNodeLeft) || (last.mpNode != &mAnchor))) // If (first != begin or last != end) ...
	{
		// Basic implementation:
		while(first != last)
			first = erase(first);
		return iterator(first.mpNode);

		// Inlined implementation:
		//size_type n = 0;
		//while(first != last)
		//{
		//    const iterator itErase(first);
		//    ++n;
		//    ++first;
		//    RBTreeErase(itErase.mpNode, &mAnchor);
		//    DoFreeNode(itErase.mpNode);
		//}
		//mnSize -= n;
		//return first;
	}

	clear();
	return iterator((node_type*)&mAnchor); // Same as: return end();
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::reverse_iterator
rbtree<K, V, C, A, E, bM, bU>::erase(const_reverse_iterator position)
{
	return reverse_iterator(erase((++position).base()));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::reverse_iterator
rbtree<K, V, C, A, E, bM, bU>::erase(const_reverse_iterator first, const_reverse_iterator last)
{
	// Version which erases in order from first to last.
	// difference_type i(first.base() - last.base());
	// while(i--)
	//     first = erase(first);
	// return first;

	// Version which erases in order from last to first, but is slightly more efficient:
	return reverse_iterator(erase((++last).base(), (++first).base()));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline void rbtree<K, V, C, A, E, bM, bU>::erase(const key_type* first, const key_type* last)
{
	// We have no choice but to run a loop like this, as the first/last range could
	// have values that are discontiguously located in the tree. And some may not 
	// even be in the tree.
	while(first != last)
		erase(*first++);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::find(const key_type& key)
{
	// To consider: Implement this instead via calling lower_bound and 
	// inspecting the result. The following is an implementation of this:
	//    const iterator it(lower_bound(key));
	//    return ((it.mpNode == &mAnchor) || mCompare(key, extractKey(it.mpNode->mValue))) ? iterator(&mAnchor) : it;
	// We don't currently implement the above because in practice people tend to call 
	// find a lot with trees, but very uncommonly call lower_bound.
	extract_key extractKey;

	node_type* pCurrent  = (node_type*)mAnchor.mpNodeParent; // Start with the root node.
	node_type* pRangeEnd = (node_type*)&mAnchor;             // Set it to the container end for now.

	while(pCurrent) // Do a walk down the tree.
	{
		if(!mCompare(extractKey(pCurrent->mValue), key)) // If pCurrent is >= key...
		{
			pRangeEnd = pCurrent;
			pCurrent  = (node_type*)pCurrent->mpNodeLeft;
		}
		else
		{
			pCurrent  = (node_type*)pCurrent->mpNodeRight;
		}
	}

	if((pRangeEnd != &mAnchor) && !mCompare(key, extractKey(pRangeEnd->mValue)))
		return iterator(pRangeEnd);
	return iterator((node_type*)&mAnchor);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::const_iterator
rbtree<K, V, C, A, E, bM, bU>::find(const key_type& key) const
{
	typedef rbtree<K, V, C, A, E, bM, bU> rbtree_type;
	return const_iterator(const_cast<rbtree_type*>(this)->find(key));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <typename U, typename Compare2>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::find_as(const U& u, Compare2 compare2)
{
	extract_key extractKey;

	node_type* pCurrent  = (node_type*)mAnchor.mpNodeParent; // Start with the root node.
	node_type* pRangeEnd = (node_type*)&mAnchor;             // Set it to the container end for now.

	while((pCurrent)) // Do a walk down the tree.
	{
		if((!compare2(extractKey(pCurrent->mValue), u))) // If pCurrent is >= u...
		{
			pRangeEnd = pCurrent;
			pCurrent  = (node_type*)pCurrent->mpNodeLeft;
		}
		else
		{
			mmm_VALIDATE_COMPARE(!compare2(u, extractKey(pCurrent->mValue))); // Validate that the compare function is sane.
			pCurrent  = (node_type*)pCurrent->mpNodeRight;
		}
	}

	if(((pRangeEnd != &mAnchor) && !compare2(u, extractKey(pRangeEnd->mValue))))
		return iterator(pRangeEnd);
	return iterator((node_type*)&mAnchor);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template <typename U, typename Compare2>
inline typename rbtree<K, V, C, A, E, bM, bU>::const_iterator
rbtree<K, V, C, A, E, bM, bU>::find_as(const U& u, Compare2 compare2) const
{
	typedef rbtree<K, V, C, A, E, bM, bU> rbtree_type;
	return const_iterator(const_cast<rbtree_type*>(this)->find_as(u, compare2));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::lower_bound(const key_type& key)
{
	extract_key extractKey;

	node_type* pCurrent  = (node_type*)mAnchor.mpNodeParent; // Start with the root node.
	node_type* pRangeEnd = (node_type*)&mAnchor;             // Set it to the container end for now.

	while((pCurrent)) // Do a walk down the tree.
	{
		if((!mCompare(extractKey(pCurrent->mValue), key))) // If pCurrent is >= key...
		{
			pRangeEnd = pCurrent;
			pCurrent  = (node_type*)pCurrent->mpNodeLeft;
		}
		else
		{
			pCurrent  = (node_type*)pCurrent->mpNodeRight;
		}
	}

	return iterator(pRangeEnd);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::const_iterator
rbtree<K, V, C, A, E, bM, bU>::lower_bound(const key_type& key) const
{
	typedef rbtree<K, V, C, A, E, bM, bU> rbtree_type;
	return const_iterator(const_cast<rbtree_type*>(this)->lower_bound(key));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::iterator
rbtree<K, V, C, A, E, bM, bU>::upper_bound(const key_type& key)
{
	extract_key extractKey;

	node_type* pCurrent  = (node_type*)mAnchor.mpNodeParent; // Start with the root node.
	node_type* pRangeEnd = (node_type*)&mAnchor;             // Set it to the container end for now.

	while((pCurrent)) // Do a walk down the tree.
	{
		if((mCompare(key, extractKey(pCurrent->mValue)))) // If key is < pCurrent...
		{
			mmm_VALIDATE_COMPARE(!mCompare(extractKey(pCurrent->mValue), key)); // Validate that the compare function is sane.
			pRangeEnd = pCurrent;
			pCurrent  = (node_type*)pCurrent->mpNodeLeft;
		}
		else
			pCurrent  = (node_type*)pCurrent->mpNodeRight;
	}

	return iterator(pRangeEnd);
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::const_iterator
rbtree<K, V, C, A, E, bM, bU>::upper_bound(const key_type& key) const
{
	typedef rbtree<K, V, C, A, E, bM, bU> rbtree_type;
	return const_iterator(const_cast<rbtree_type*>(this)->upper_bound(key));
}


// To do: Move this validate function entirely to a template-less implementation.
template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
bool rbtree<K, V, C, A, E, bM, bU>::validate() const
{
	// Red-black trees have the following canonical properties which we validate here:
	//   1 Every node is either red or black.
	//   2 Every leaf (NULL) is black by defintion. Any number of black nodes may appear in a sequence. 
	//   3 If a node is red, then both its children are black. Thus, on any path from 
	//     the root to a leaf, red nodes must not be adjacent.
	//   4 Every simple path from a node to a descendant leaf contains the same number of black nodes.
	//   5 The mnSize member of the tree must equal the number of nodes in the tree.
	//   6 The tree is sorted as per a conventional binary tree.
	//   7 The comparison function is sane; it obeys strict weak ordering. If mCompare(a,b) is true, then mCompare(b,a) must be false. Both cannot be true.

	extract_key extractKey;

	if(mnSize)
	{
		// Verify basic integrity.
		//if(!mAnchor.mpNodeParent || (mAnchor.mpNodeLeft == mAnchor.mpNodeRight))
		//    return false;             // Fix this for case of empty tree.

		if(mAnchor.mpNodeLeft != RBTreeGetMinChild(mAnchor.mpNodeParent))
			return false;

		if(mAnchor.mpNodeRight != RBTreeGetMaxChild(mAnchor.mpNodeParent))
			return false;

		const size_t nBlackCount   = RBTreeGetBlackCount(mAnchor.mpNodeParent, mAnchor.mpNodeLeft);
		size_type    nIteratedSize = 0;

		for(const_iterator it = begin(); it != end(); ++it, ++nIteratedSize)
		{
			const node_type* const pNode      = (const node_type*)it.mpNode;
			const node_type* const pNodeRight = (const node_type*)pNode->mpNodeRight;
			const node_type* const pNodeLeft  = (const node_type*)pNode->mpNodeLeft;

			// Verify #7 above.
			if(pNodeRight && mCompare(extractKey(pNodeRight->mValue), extractKey(pNode->mValue)) && mCompare(extractKey(pNode->mValue), extractKey(pNodeRight->mValue))) // Validate that the compare function is sane.
				return false;

			// Verify #7 above.
			if(pNodeLeft && mCompare(extractKey(pNodeLeft->mValue), extractKey(pNode->mValue)) && mCompare(extractKey(pNode->mValue), extractKey(pNodeLeft->mValue))) // Validate that the compare function is sane.
				return false;

			// Verify item #1 above.
			if((pNode->mColor != kRBTreeColorRed) && (pNode->mColor != kRBTreeColorBlack))
				return false;

			// Verify item #3 above.
			if(pNode->mColor == kRBTreeColorRed)
			{
				if((pNodeRight && (pNodeRight->mColor == kRBTreeColorRed)) ||
					(pNodeLeft  && (pNodeLeft->mColor  == kRBTreeColorRed)))
					return false;
			}

			// Verify item #6 above.
			if(pNodeRight && mCompare(extractKey(pNodeRight->mValue), extractKey(pNode->mValue)))
				return false;

			if(pNodeLeft && mCompare(extractKey(pNode->mValue), extractKey(pNodeLeft->mValue)))
				return false;

			if(!pNodeRight && !pNodeLeft) // If we are at a bottom node of the tree...
			{
				// Verify item #4 above.
				if(RBTreeGetBlackCount(mAnchor.mpNodeParent, pNode) != nBlackCount)
					return false;
			}
		}

		// Verify item #5 above.
		if(nIteratedSize != mnSize)
			return false;

		return true;
	}
	else
	{
		if((mAnchor.mpNodeLeft != &mAnchor) || (mAnchor.mpNodeRight != &mAnchor))
			return false;
	}

	return true;
}



template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoAllocateNode()
{
	auto* pNode = (node_type*)allocator_type::allocate();
	return pNode;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
inline void rbtree<K, V, C, A, E, bM, bU>::DoFreeNode(node_type* pNode)
{
	pNode->~node_type();
	allocator_type::deallocate(pNode, sizeof(node_type));
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoCreateNodeFromKey(const key_type& key)
{
	// Note that this function intentionally leaves the node pointers uninitialized.
	// The caller would otherwise just turn right around and modify them, so there's
	// no point in us initializing them to anything (except in a debug build).
	node_type* const pNode = DoAllocateNode();
	::new (mmm::addressof(pNode->mValue)) value_type(pair_first_construct_t(), key);
	return pNode;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoCreateNode(const value_type& value)
{
	// Note that this function intentionally leaves the node pointers uninitialized.
	// The caller would otherwise just turn right around and modify them, so there's
	// no point in us initializing them to anything (except in a debug build).
	node_type* const pNode = DoAllocateNode();
	::new(mmm::addressof(pNode->mValue)) value_type(value);
	return pNode;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoCreateNode(value_type&& value)
{
	// Note that this function intentionally leaves the node pointers uninitialized.
	// The caller would otherwise just turn right around and modify them, so there's
	// no point in us initializing them to anything (except in a debug build).
	node_type* const pNode = DoAllocateNode();

	::new(mmm::addressof(pNode->mValue)) value_type(mmm::move(value));


	return pNode;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
template<class... Args>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoCreateNode(Args&&... args)
{
	// Note that this function intentionally leaves the node pointers uninitialized.
	// The caller would otherwise just turn right around and modify them, so there's
	// no point in us initializing them to anything (except in a debug build).
	node_type* const pNode = DoAllocateNode();


	::new(mmm::addressof(pNode->mValue)) value_type(mmm::forward<Args>(args)...);
	return pNode;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoCreateNode(const node_type* pNodeSource, node_type* pNodeParent)
{
	node_type* const pNode = DoCreateNode(pNodeSource->mValue);

	pNode->mpNodeRight  = NULL;
	pNode->mpNodeLeft   = NULL;
	pNode->mpNodeParent = pNodeParent;
	pNode->mColor       = pNodeSource->mColor;

	return pNode;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
typename rbtree<K, V, C, A, E, bM, bU>::node_type*
rbtree<K, V, C, A, E, bM, bU>::DoCopySubtree(const node_type* pNodeSource, node_type* pNodeDest)
{
	node_type* const pNewNodeRoot = DoCreateNode(pNodeSource, pNodeDest);

	// Copy the right side of the tree recursively.
	if(pNodeSource->mpNodeRight)
		pNewNodeRoot->mpNodeRight = DoCopySubtree((const node_type*)pNodeSource->mpNodeRight, pNewNodeRoot);

	node_type* pNewNodeLeft;

	for(pNodeSource = (node_type*)pNodeSource->mpNodeLeft, pNodeDest = pNewNodeRoot; 
		pNodeSource;
		pNodeSource = (node_type*)pNodeSource->mpNodeLeft, pNodeDest = pNewNodeLeft)
	{
		pNewNodeLeft = DoCreateNode(pNodeSource, pNodeDest);

		pNodeDest->mpNodeLeft = pNewNodeLeft;

		// Copy the right side of the tree recursively.
		if(pNodeSource->mpNodeRight)
			pNewNodeLeft->mpNodeRight = DoCopySubtree((const node_type*)pNodeSource->mpNodeRight, pNewNodeLeft);
	}


	return pNewNodeRoot;
}


template <typename K, typename V, typename C, typename A, typename E, bool bM, bool bU>
void rbtree<K, V, C, A, E, bM, bU>::DoNukeSubtree(node_type* pNode)
{
	while(pNode) // Recursively traverse the tree and destroy items as we go.
	{
		DoNukeSubtree((node_type*)pNode->mpNodeRight);

		node_type* const pNodeLeft = (node_type*)pNode->mpNodeLeft;
		DoFreeNode(pNode);
		pNode = pNodeLeft;
	}
}



///////////////////////////////////////////////////////////////////////
// global operators
///////////////////////////////////////////////////////////////////////

template <typename K, typename V, typename A, typename C, typename E, bool bM, bool bU>
inline bool operator==(const rbtree<K, V, C, A, E, bM, bU>& a, const rbtree<K, V, C, A, E, bM, bU>& b)
{
	return (a.size() == b.size()) && mmm::equal(a.begin(), a.end(), b.begin());
}


// Note that in operator< we do comparisons based on the tree value_type with operator<() of the
// value_type instead of the tree's Compare function. For set/multiset, the value_type is T, while
// for map/multimap the value_type is a pair<Key, T>. operator< for pair can be seen by looking
// utility.h, but it basically is uses the operator< for pair.first and pair.second. The C++ standard
// appears to require this behaviour, whether intentionally or not. If anything, a good reason to do
// this is for consistency. A map and a vector that contain the same items should compare the same.
template <typename K, typename V, typename A, typename C, typename E, bool bM, bool bU>
inline bool operator<(const rbtree<K, V, C, A, E, bM, bU>& a, const rbtree<K, V, C, A, E, bM, bU>& b)
{
	return mmm::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}


template <typename K, typename V, typename A, typename C, typename E, bool bM, bool bU>
inline bool operator!=(const rbtree<K, V, C, A, E, bM, bU>& a, const rbtree<K, V, C, A, E, bM, bU>& b)
{
	return !(a == b);
}


template <typename K, typename V, typename A, typename C, typename E, bool bM, bool bU>
inline bool operator>(const rbtree<K, V, C, A, E, bM, bU>& a, const rbtree<K, V, C, A, E, bM, bU>& b)
{
	return b < a;
}


template <typename K, typename V, typename A, typename C, typename E, bool bM, bool bU>
inline bool operator<=(const rbtree<K, V, C, A, E, bM, bU>& a, const rbtree<K, V, C, A, E, bM, bU>& b)
{
	return !(b < a);
}


template <typename K, typename V, typename A, typename C, typename E, bool bM, bool bU>
inline bool operator>=(const rbtree<K, V, C, A, E, bM, bU>& a, const rbtree<K, V, C, A, E, bM, bU>& b)
{
	return !(a < b);
}


template <typename K, typename V, typename A, typename C, typename E, bool bM, bool bU>
inline void swap(rbtree<K, V, C, A, E, bM, bU>& a, rbtree<K, V, C, A, E, bM, bU>& b)
{
	a.swap(b);
}


} // namespace mmm




#endif