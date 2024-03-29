//
//  radix.hpp
//
//  (c) Jasmin B. Maglic, 2023
//

#ifndef radix_hpp
#define radix_hpp

//////////////
// INCLUDES //
//////////////

#include <map>
#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <cassert>
#include <iterator>
#include <functional>
#include <utility>

//////////////////
// DECLARATIONS //
//////////////////

// Forward declaration for friend relation
namespace xsm{
  template <class T, class Compare> class radix;
}

namespace xsm::detail{
  // Aux function declaration
  std::string StrDiff(const std::string&, const std::string&);

  // Forward declaration for friend relation
  template <class T, class Compare, class ItType=T> class Iterator_impl;
  template <class T, class Compare> class Node;
  template <class T, class Compare> class Node_handle;
  template <class Iter, class NodeType> struct Node_insert_bundle;

  //////////
  // NODE //
  //////////
  // Node is the class that stores the key-value pair, when an element is added
  // to the radix tree. Each node represents one pair and connects to other nodes
  // via pointers. Each node has one parent and a variable number of children that
  // are stored inside a std::map. Due to the nature of radix trees, there are
  // nodes that do not themselves store meaningful data but only exist to link to
  // children. These are called non-leaf nodes.
  // The node class implementation is entirely private and only accessible through
  // its friend relations. Nodes are only accessed via pointers, so the class does
  // not provide move and copy constructors/operators.
  // Node deletion follows the following rules:
  //  - the radix instance deletes its root node
  //  - parent nodes delete their children
  //  - Node_handle manages and deletes orphan nodes
  template <class T, class Compare>
  class Node{

    // Friends
    friend typename radix<T,Compare>::iterator;
    friend typename radix<T,Compare>::const_iterator;
    friend typename radix<T,Compare>::node_type;
    friend xsm::radix<T,Compare>;

    // Aliases
    using key_type = typename radix<T,Compare>::key_type;
    using mapped_type = typename radix<T,Compare>::mapped_type;
    using value_type = typename radix<T,Compare>::value_type;
    using key_compare = typename radix<T,Compare>::key_compare;
    typedef Node<T,Compare>* node_ptr;
    typedef const Node<T,Compare>* const_node_ptr;
    typedef std::map<key_type,node_ptr,key_compare> child_map;

    // Members
    value_type m_value_pair;
    node_ptr m_parent;
    child_map m_children;
    const bool m_is_leaf;

    // Constructors and related
    Node();
    Node(node_ptr, value_type&&, const bool=false);
    template <class... Args> Node(node_ptr, const bool, Args&&...);
    ~Node();

    // Node state
    bool IsChildless() const;
    bool IsLeaf() const;
    size_t CountChildren() const;

    // Getters and setters
    node_ptr GetParent() const;
    void SetParent(node_ptr);
    node_ptr GetFirstChild() const;
    node_ptr GetLastDescendant();
    child_map& GetChildren();
    const child_map& GetChildren() const;
    const key_type& GetKey() const;

    // Node ownership
    node_ptr AddChild(const key_type&, node_ptr);
    node_ptr AddChild(const key_type&);
    node_ptr Extract();
    node_ptr GiveUpChild();
    void SubstituteWith(node_ptr);
    void RemoveParent();
    void Adopt(node_ptr);
    void Emancipate();

    // Container operations
    node_ptr Retrieve(const key_type&);
    template <class K> node_ptr FindCondition(bool(*)(const key_type&, const K&), const K&);
    template <class K> node_ptr FindConditionNonLeaf(bool(*)(const key_type&, const K&), const K&);

    // Display
    void print() const; // TODO just for testing
  };

  // Forward declarations to allow for overloaded comparison operators
  template <class T, class Compare, class ItType, class ItType2=ItType>
  bool operator==(const Iterator_impl<T,Compare,ItType>&, const Iterator_impl<T,Compare,ItType2>&);
  template <class T, class Compare, class ItType, class ItType2=ItType>
  bool operator!=(const Iterator_impl<T,Compare,ItType>&, const Iterator_impl<T,Compare,ItType2>&);

  /////////////////
  // NODE HANDLE //
  /////////////////
  // Node handle is the return type for xsm::radix::extract, and also serves as the class, that
  // is allowed to manage and delete isolated nodes, i.e. nodes without children or a parent.
  // In this latter function it behaves like unique_ptr, where it deletes the object
  // once node handle outlives its life-time.
  template <class T, class Compare>
  class Node_handle{
    // Friend
    friend radix<T,Compare>;

    public:
      using key_type = typename radix<T,Compare>::key_type;
      using mapped_type = typename radix<T,Compare>::mapped_type;
      using node_ptr = typename Node<T,Compare>::node_ptr;
      //using allocator_type // TODO
      
      constexpr Node_handle() noexcept = default;
      Node_handle(Node_handle<T,Compare>&&) noexcept;
      Node_handle& operator=(Node_handle<T,Compare>&&);
      ~Node_handle();

      [[nodiscard]] bool empty() const noexcept;
      explicit operator bool() const noexcept;
      //allocator_type get_allocator() const; 
     
      const key_type& key() const;
      mapped_type& mapped() const;

      /*void swap(Node_handle&) noexcept(
          std::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
          std::allocator_traits<allocator_type>::is_always_equal::value);
      */

    private:
      using child_map = typename Node<T,Compare>::child_map;
      using node_type = typename radix<T,Compare>::node_type;

      node_ptr m_node_ptr = nullptr;

      // Constructor from raw pointer
      Node_handle(node_ptr);

  };

  //friend void swap(Node_handle& x, Node_handle& y) noexcept(noexcept(x.swap(y)));
  
  ////////////////////////
  // INSERT RETURN TYPE //
  ////////////////////////
  // Small structure that serves as return type for an xsm::radix::insert overload.
  template <class Iter, class NodeType>
  struct Node_insert_bundle{
    Iter position;
    NodeType node;
    bool inserted;
  };

  //////////////
  // ITERATOR //
  //////////////
  // Bidiectional iterator class
  template <class T, class Compare, class ItType>
  class Iterator_impl {

    friend radix<T,Compare>;
    // If ItType is T: ItType2 is const T
    // If ItType is const T: ItType2 is T
    typedef typename std::conditional<std::is_const_v<ItType>, T, const T>::type ItType2;
    friend Iterator_impl<T,Compare,ItType2>;

    using const_iterator = typename radix<T,Compare>::const_iterator;
    using key_type = typename radix<T,Compare>::key_type;
    using reverse_iterator = typename radix<T,Compare>::reverse_iterator;
    using const_reverse_iterator = typename radix<T,Compare>::const_reverse_iterator;
    using node_ptr = typename Node<T,Compare>::node_ptr;
    using const_node_ptr = typename Node<T,Compare>::const_node_ptr;
    
    public:
      // These aliases must be public for std::reverse_iterator to work
      using value_type = typename radix<T,Compare>::value_type;
      using difference_type = typename radix<T,Compare>::difference_type;
      typedef typename std::conditional<
        std::is_const_v<ItType>, typename radix<T,Compare>::const_reference, typename radix<T,Compare>::reference
        >::type reference;
      typedef typename std::conditional<
        std::is_const_v<ItType>, typename radix<T,Compare>::const_pointer, typename radix<T,Compare>::pointer
        >::type pointer;

      // Constructor
      Iterator_impl();

      // Iterator operations
      Iterator_impl& operator++();
      Iterator_impl operator++(int);
      Iterator_impl& operator--();
      Iterator_impl operator--(int);

      reference operator*() const;
      pointer operator->() const;

      // Conversion from iterator to const_iterator
      operator const_iterator() const;
      
      // Explicit instantiation for template type
      friend bool operator== <> (const Iterator_impl<T,Compare,ItType>&, const Iterator_impl<T,Compare,ItType>&);
      friend bool operator== <> (const Iterator_impl<T,Compare,ItType>&, const Iterator_impl<T,Compare,ItType2>&);
      friend bool operator!= <> (const Iterator_impl<T,Compare,ItType>&, const Iterator_impl<T,Compare,ItType>&);
      friend bool operator!= <> (const Iterator_impl<T,Compare,ItType>&, const Iterator_impl<T,Compare,ItType2>&);
    private:
      node_ptr m_node;

      Iterator_impl(node_ptr);

      bool Advance();
      bool Regress();
  };
}

namespace xsm{
  ///////////
  // RADIX //
  ///////////
  //
  //template <class T> operator<=>(const radix<T,Compare>, const radix<T,Compare>);
  //template <class T, class Pred> radix<T,Compare>::size_type erase_if(radix<T,Compare>, Pred);
  //
  // Publicly accessible container class
  template <class T, class Compare=std::less<std::string>> class radix{
    public:
      // Aliases
      typedef std::string key_type;
      typedef T mapped_type;
      typedef std::pair<const key_type, mapped_type> value_type;
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;
      typedef Compare key_compare;
      typedef value_type& reference;
      typedef const value_type& const_reference;
      typedef value_type* pointer;
      typedef const value_type* const_pointer;
      typedef detail::Iterator_impl<mapped_type,key_compare> iterator;
      typedef detail::Iterator_impl<mapped_type,key_compare,const mapped_type> const_iterator;
      typedef std::pair<iterator,iterator> iterator_pair;
      typedef std::pair<const_iterator,const_iterator> const_iterator_pair;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef detail::Node_handle<mapped_type,key_compare> node_type;
      typedef detail::Node_insert_bundle<iterator,node_type> insert_return_type;

      // Constructors and related
      radix();
      radix(const radix&);
      radix(radix&&) noexcept;
      ~radix();
      radix(std::initializer_list<value_type>);

      radix<T,Compare>& operator=(const radix<T,Compare>&);
      radix<T,Compare>& operator=(radix<T,Compare>&&) noexcept;

      // Special
      template <class K> iterator_pair complete(const K&);
      template <class K> const_iterator_pair complete(const K&) const;

      // Capacity
      [[nodiscard]] bool empty() const noexcept;
      size_type size() const noexcept;
      // size_type max_size() const noexcept;
  
      // Modifiers
      std::pair<iterator,bool> insert(const value_type&);
      template <class P> std::pair<iterator, bool> insert(P&&);
      std::pair<iterator,bool> insert(value_type&&);
      iterator insert(const_iterator, const value_type&);
      template <class P> iterator insert(const_iterator, P&&);
      iterator insert(const_iterator, value_type&&);
      template <class InputIt> void insert(InputIt, InputIt);
      void insert(std::initializer_list<value_type>);
      insert_return_type insert(node_type&&);
      iterator insert(const_iterator, node_type&&);
      // template< class M > std::pair<iterator, bool> insert_or_assign( const Key& k, M&& obj );
      // template< class M > std::pair<iterator, bool> insert_or_assign( Key&& k, M&& obj );
      // template< class M > iterator insert_or_assign( const_iterator hint, const Key& k, M&& obj );
      // template< class M > iterator insert_or_assign( const_iterator hint, Key&& k, M&& obj );
      template <class... Args> std::pair<iterator,bool> emplace(Args&&...);
      template <class... Args> iterator emplace_hint(const_iterator, Args&&...);
      //template <class... Args> pair<iterator, bool> try_emplace(const key_type&, Args&&...);
      //template <class... Args> pair<iterator, bool> try_emplace(key_type&&, Args&&...);
      //template <class... Args> pair<iterator, bool> try_emplace(const iterator, const key_type&, Args&&...);
      //template <class... Args> pair<iterator, bool> try_emplace(const_iterator, key_type&&, Args&&...);
      iterator erase(iterator);
      iterator erase(const_iterator);
      iterator erase(const_iterator, const_iterator);
      size_type erase(const key_type&); 
      //template <class K> size_type erase(K&&); c++23
      void swap(radix<T,Compare>&);
      node_type extract(const_iterator);
      node_type extract(const key_type&);
      //void merge(radix<T,Compare>&);
      //void merge(radix<T,Compare>&&);
      void clear();

      // Element access
      mapped_type& at(const key_type&);
      const mapped_type& at(const key_type&) const;
      mapped_type& operator[](const key_type&);
      //mapped_type& operator[](key_type&&);

      // Lookup
      size_type count(const key_type&) const;
      template <class K> size_type count(const K&) const;
      bool contains(const key_type&) const;
      template <class K> bool contains(const K&) const;
      iterator find(const key_type&);
      const_iterator find(const key_type&) const;
      template <class K> iterator find(const K&);
      template <class K> const_iterator find(const K&) const;
      iterator_pair equal_range(const key_type&);
      const_iterator_pair equal_range(const key_type&) const;
      template <class K> iterator_pair equal_range(const K&);
      template <class K> const_iterator_pair equal_range(const K&) const;
      iterator lower_bound(const key_type&);
      const_iterator lower_bound(const key_type&) const;
      template <class K> iterator lower_bound(const K&);
      template <class K> const_iterator lower_bound(const K&) const;
      iterator upper_bound(const key_type&);
      const_iterator upper_bound(const key_type&) const;
      template <class K> iterator upper_bound(const K&);
      template <class K> const_iterator upper_bound(const K&) const;
      
      // Observers
      // key_compare key_comp() const;
      // std::map::value_compare value_comp() const;
      // allocator_type get_allocator() const noexcept;

      // Iterator
      iterator begin() noexcept;
      const_iterator begin() const noexcept;
      iterator end() noexcept;
      const_iterator end() const noexcept;

      reverse_iterator rbegin() noexcept;
      const_reverse_iterator rbegin() const noexcept;
      reverse_iterator rend() noexcept;
      const_reverse_iterator rend() const noexcept;

      const_iterator cbegin() const noexcept;
      const_iterator cend() const noexcept;
      const_reverse_iterator crbegin() const noexcept;
      const_reverse_iterator crend() const noexcept;

      void print() const;

    private:
      using node_ptr = typename detail::Node<mapped_type,key_compare>::node_ptr;
      using const_node_ptr = typename detail::Node<mapped_type,key_compare>::const_node_ptr;
      
      node_ptr m_root; // Pointer to root node of radix tree
      size_type m_size;

      key_type::const_iterator ProcessHint(const_iterator&, node_ptr);
      std::pair<iterator,bool> NodeInTree(node_type&&, const_iterator, key_type::const_iterator);

      template <class K> static bool conditionLower(const key_type&, const K&);
      template <class K> static bool conditionUpper(const key_type&, const K&);
  };
}
 
/////////////////
// DEFINITIONS //
/////////////////

#include <iostream>
#include <utility>

namespace xsm{
  ///////////
  // RADIX //
  ///////////
  // Constructor
  template <class T, class Compare>
  radix<T,Compare>::radix() : m_root(new detail::Node<T,Compare>()), m_size(0) {}

  // Constructor from initaliser list
  template <class T, class Compare>
  radix<T,Compare>::radix(std::initializer_list<typename radix<T,Compare>::value_type> init) : 
    m_root(new detail::Node<T,Compare>()), m_size(0){
    insert(init);
  }
  
  // Copy constructor
  template <class T, class Compare>
  radix<T,Compare>::radix(const radix& rdx){
    m_root = new detail::Node<T,Compare>();
    for (auto it = rdx.begin(); it != rdx.end(); ++it){
      emplace(it->first, it->second);
    }
    m_size = rdx.size();
  }

  // Move constructor
  template <class T, class Compare>
  radix<T,Compare>::radix(radix&& rdx) noexcept {
    m_root = rdx.m_root;
    m_size = rdx.size();
    rdx.m_root = new detail::Node<T,Compare>();
    rdx.m_size = 0;
  }
  
  // Destructor
  template <class T, class Compare>
  radix<T,Compare>::~radix(){
    delete m_root;
  }
  
  // Copy assignment
  template <class T, class Compare>
  radix<T,Compare>& radix<T,Compare>::operator=(const radix<T,Compare>& rdx){
    radix<T,Compare> cpy = rdx;
    this->swap(cpy);
    return *this;
  }

  // Move assignment
  template <class T, class Compare>
  radix<T,Compare>& radix<T,Compare>::operator=(radix<T,Compare>&& rdx) noexcept {
    delete m_root;
    m_root = rdx.m_root;
    m_size = rdx.m_size;
    rdx.m_root = new detail::Node<T,Compare>();
    rdx.m_size = 0;
    return *this;
  }

  //////////////
  // COMPLETE //
  //////////////
  // The two return iterators are the begin and end iterators for the set of
  // all descendants of the input key
  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::iterator_pair radix<T,Compare>::complete(const K& key){
    
    node_ptr b_node = m_root->FindConditionNonLeaf(radix::conditionLower<K>, key);
    iterator e_it(b_node->GetLastDescendant());
    ++e_it;

    iterator b_it(b_node);
    if (!b_node->IsLeaf()){
      ++b_it;
    }

    return std::make_pair(b_it, e_it);
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::const_iterator_pair radix<T,Compare>::complete(const K& key) const {
    
    node_ptr b_node = m_root->FindConditionNonLeaf(radix::conditionLower<K>, key);
    const_iterator e_it(b_node->GetLastDescendant());
    ++e_it;

    const_iterator b_it(b_node);
    if (!b_node->IsLeaf()){
      ++b_it;
    }

    return std::make_pair(b_it, e_it);
  }

  template <class T, class Compare>
  [[nodiscard]] bool radix<T,Compare>::empty() const noexcept {
    return begin() == end();
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::size_type radix<T,Compare>::size() const noexcept {
    return m_size;
  }

  template <class T, class Compare>
  std::pair<detail::Iterator_impl<T,Compare>,bool> radix<T,Compare>::insert(const value_type& key_value){ 
    return emplace(std::move(key_value));
  }
  
  template <class T, class Compare> template <class P>
  std::pair<detail::Iterator_impl<T,Compare>,bool> radix<T,Compare>::insert(P&& value){
    return emplace(std::forward<P>(value));
  }

  template <class T, class Compare>
  std::pair<detail::Iterator_impl<T,Compare>,bool> radix<T,Compare>::insert(value_type&& key_value){ 
    return emplace(std::forward<value_type>(key_value));
  }

  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::insert(const_iterator pos, const value_type& key_value){
    return emplace_hint(pos, key_value);
  }

  template <class T, class Compare> template <class P> 
  typename radix<T,Compare>::iterator radix<T,Compare>::insert(const_iterator pos, P&& value){
    return emplace_hint(pos, std::forward<P>(value));
  }

  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::insert(const_iterator pos, value_type&& key_value){
    return emplace_hint(pos, std::forward<value_type>(key_value));
  }
  
  template <class T, class Compare> template <class InputIt>
  void radix<T,Compare>::insert(InputIt it, InputIt end){
    while (it != end){
      insert(*it);
      ++it;
    }
  }

  template <class T, class Compare>
  void radix<T,Compare>::insert(std::initializer_list<value_type> init_list){
    for (auto e : init_list){
      insert(e);
    }
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::insert_return_type radix<T,Compare>::insert(node_type&& node){
    insert_return_type retval;
    if (node.empty()){
      retval.inserted = false;
      retval.node = std::move(node);
      retval.position = end();
    }
    else {
      std::tie(retval.position,retval.inserted) 
        = NodeInTree(std::move(node), iterator(m_root), node.m_node_ptr->GetKey().begin());
      retval.node = std::move(node);
      if (retval.inserted) {
        retval.node = node_type();
      }
    }
    return retval;
  }

  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::insert(const_iterator pos, node_type&& node){
    if (node.empty()){
      return end();
    }
    iterator it;
    std::tie(it,std::ignore) = NodeInTree(std::move(node), pos, ProcessHint(pos,node.m_node_ptr));
    // Move assignment
    node = node_type();
    return it;
  }

  template <class T, class Compare>
  typename radix<T,Compare>::key_type::const_iterator radix<T,Compare>::ProcessHint(const_iterator& parent, node_ptr node){
    
    auto key_start = node->GetKey().begin();
    auto key_end = node->GetKey().end();

    bool correct_parent_found = false;

    while(!correct_parent_found){

      auto parentkey_start = parent.m_node->GetKey().begin();
      auto parentkey_end = parent.m_node->GetKey().end();
  
      auto last_match = std::mismatch(parentkey_start, parentkey_end, key_start, key_end);

      // Check if parent's key is a prefix of node's key
      correct_parent_found = last_match.first == parentkey_end;
  
      if (!correct_parent_found){
        // Go up one parent
        parent = iterator(parent.m_node->GetParent());
      }
      else {
        // Node should become a descendent of parent (not necessarily direct child)
        key_start = last_match.second;
      }
    }
    return key_start;
  }

  template <class T, class Compare>
  std::pair<typename radix<T,Compare>::iterator,bool> radix<T,Compare>::NodeInTree(
      node_type&& nh, const_iterator parent, key_type::const_iterator key_start){

    auto key_end = nh.key().end();

    bool next_child;
    // This loop only repeats if we descend one level in the tree
    do {
      next_child = false;

      // Loop through all children in iterator, until you find a match within the keys
      for (const std::pair<const key_type,node_ptr>& entry : parent.m_node->GetChildren()){

        auto entrykey_start = entry.first.begin();
        auto entrykey_end = entry.first.end();
        auto last_match = std::mismatch(key_start, key_end, entrykey_start, entrykey_end);
        
        // 1. Complete mismatch -> Check next child
        if (key_start == last_match.first){continue;}

        // 2. New key and entry key are identical -> Key already exists, or leads to a non-leaf node
        if (key_end == last_match.first && entrykey_end == last_match.second){
          
          if (!entry.second->IsLeaf()){
            // Unlink non-leaf node and substitute with leaf node
           
            // Need to copy the pointer, because entry.second will be changed by SubstituteWith()
            node_type non_leaf_handle(entry.second);
            non_leaf_handle.m_node_ptr->SubstituteWith(nh.m_node_ptr);

            m_size++;
            return std::make_pair(iterator(nh.m_node_ptr), true);
          }
          else {
            // Insertion unsuccessful, return iterator to existing element
            return std::make_pair(iterator(entry.second), false);
          }
        }

        // 3. One key is prefix of the other -> The prefix key becomes the parent of the other entry
        else if (key_end == last_match.first || entrykey_end == last_match.second){
          // New keyword is prefix of the existing nodekey
          if (key_end == last_match.first){
            // Add a new node with the prefix to the current node
            parent.m_node->AddChild(key_type(key_start, key_end), nh.m_node_ptr);
            // Old child node becomes child of the new node
            nh.m_node_ptr->AddChild(key_type(last_match.second, entrykey_end), entry.second);
            // Remove old node from current node
            parent.m_node->GetChildren().erase(entry.first);

            m_size++;
            return std::make_pair(iterator(nh.m_node_ptr),true);
          }
          else {
            // Insert new keyword as child of its prefix
            key_start = last_match.first;
            parent = iterator(entry.second);
            next_child = true;
            break;
          }
        }

        // 4. Words partially match but diverge
        // -> A new node needs to be created whose key is the common prefix of the words.
        // This node has two children, one representing the new key, the other representing the
        // entry key
        else {
          key_type common_prefix(key_start, last_match.first);

          // New parent
          node_ptr parent_ptr = parent.m_node->AddChild(key_type(key_start, last_match.first));
          // Old entry
          parent_ptr->AddChild(key_type(last_match.second,entrykey_end), entry.second);
          entry.second->SetParent(parent_ptr);
          parent.m_node->GetChildren().erase(entry.first); 
          // New entry
          parent_ptr->AddChild(key_type(last_match.first,key_end), nh.m_node_ptr);

          m_size++;
          return std::make_pair(iterator(nh.m_node_ptr),true);
        }
      }
    } while (next_child);
    
    // 5. No common prefix has been found in any children -> keyword becomes a new entry
    parent.m_node->AddChild(key_type(key_start, key_end), nh.m_node_ptr);
    m_size++;
    return std::make_pair(iterator(nh.m_node_ptr), true);
  }
  
  template <class T, class Compare> template <class... Args>
  std::pair<detail::Iterator_impl<T,Compare>,bool> radix<T,Compare>::emplace(Args&&... args){
    
    node_ptr node = new detail::Node<T,Compare>(nullptr, true, std::forward<Args>(args)...);
    return NodeInTree(node_type(node), iterator(m_root), node->GetKey().begin());
  }

  template <class T, class Compare> template <class... Args>
  typename radix<T,Compare>::iterator radix<T,Compare>::emplace_hint(const_iterator pos, Args&&... args){
    
    node_ptr node = new detail::Node<T,Compare>(nullptr, true, std::forward<Args>(args)...);

    const_iterator parent = --pos;
    key_type::const_iterator key_start = ProcessHint(parent, node);

    return NodeInTree(node_type(node), parent, key_start).first;
  }

  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::erase(iterator it){
    return erase(const_iterator(it));
  }

  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::erase(const_iterator it){
    // Passing and iterator that equals end() will result in an error (Seg fault)
    // This is the same behaviour as in std::map, so there is no need to check for
    // this case.
    auto ret_it = it;
    ++ret_it;
    extract(it);
    return iterator(ret_it.m_node);
  }

  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::erase(const_iterator first, const_iterator last){
    while (first != last){
      first = erase(first);
    }
    return iterator(last.m_node);
  }

  template <class T, class Compare>
  typename radix<T,Compare>::size_type radix<T,Compare>::erase(const key_type& key){
    if (contains(key)){
      erase(find(key));
      return 1;
    }
    return 0;
  }

  template <class T, class Compare>
  void radix<T,Compare>::swap(radix<T,Compare>& rdx){
    // swap m_root
    {
      node_ptr temp = m_root;
      m_root = rdx.m_root;
      rdx.m_root = temp;
    }
    // swap m_size
    {
      size_t temp = m_size;
      m_size = rdx.m_size;
      rdx.m_size = temp;
    }
  }

  template <class T, class Compare>
  typename radix<T,Compare>::node_type radix<T,Compare>::extract(const_iterator it){

    node_ptr extracted = it.m_node->Extract();

    // Decrementing the counter causes an issue when the iterator doesn't point to an element in this radix
    // but this is the same behaviour as in std::map
    --m_size;

    return node_type(extracted);
  }

  template <class T, class Compare>
  typename radix<T,Compare>::node_type radix<T,Compare>::extract(const key_type& key){
    if (contains(key)){
      return extract(find(key));
    }
    else {
      return node_type();
    }
  }

  template <class T, class Compare>
  void radix<T,Compare>::clear(){
    delete m_root;
    m_root = new detail::Node<T,Compare>();
    m_size = 0;
  }

  template <class T, class Compare>
  T& radix<T,Compare>::at(const key_type& key){
    return const_cast<T&>(std::as_const(*this).at(key));
  }

  template <class T, class Compare>
  const T& radix<T,Compare>::at(const key_type& key) const {
    const_node_ptr node = m_root->Retrieve(key);
    if (node == nullptr || !node->IsLeaf()){
      throw std::out_of_range("radix::at:  key not found");
    }
    return m_root->Retrieve(key)->m_value_pair.second;
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::find(const key_type& key){
    node_ptr node = m_root->Retrieve(key);
    return (node->IsLeaf()? iterator(node) : end());
  }

  template <class T, class Compare>
  typename radix<T,Compare>::const_iterator radix<T,Compare>::find(const key_type& key) const {
    node_ptr node = m_root->Retrieve(key);
    return (node->IsLeaf()? const_iterator(node) : cend());
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::iterator radix<T,Compare>::find(const K& key){
    iterator it = lower_bound(key);

    const Compare comp;
    return (it != end() && !comp(it->first, key) && !comp(key, it->first))? it : end();
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::const_iterator radix<T,Compare>::find(const K& key) const {
    const_iterator it = lower_bound(key);

    Compare comp;
    return (it != cend() && !comp(it->first, key) && !comp(key, it->first))? it : cend();
  }

  /////////////////
  // LOWER BOUND //
  /////////////////

  // Condition for finding first item that is not smaller than key
  // Used in lower_bound
  template <class T, class Compare> template <class K>
  bool radix<T,Compare>::conditionLower(const key_type& tree_key, const K& key){
    const Compare comp;
    return !comp(tree_key,key);
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::lower_bound(const key_type& key){
    return iterator(m_root->FindCondition(radix::conditionLower<key_type>, key));
  }

  template <class T, class Compare>
  typename radix<T,Compare>::const_iterator radix<T,Compare>::lower_bound(const key_type& key) const {
    return const_iterator(m_root->FindCondition(radix::conditionLower<key_type>, key));
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::iterator radix<T,Compare>::lower_bound(const K& key){
    return iterator(m_root->FindCondition(radix::conditionLower, key));
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::const_iterator radix<T,Compare>::lower_bound(const K& key) const {
    return const_iterator(m_root->FindCondition(radix::conditionLower, key));
  }

  /////////////////
  // UPPER BOUND //
  /////////////////
  
  // Condition for finding first item that is greater than key
  // Used in upper_bound
  template <class T, class Compare> template <class K>
  bool radix<T,Compare>::conditionUpper(const key_type& tree_key, const K& key){
    Compare comp;
    return comp(key, tree_key);
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::iterator radix<T,Compare>::upper_bound(const key_type& key){
    return iterator(m_root->FindCondition(radix::conditionUpper<key_type>, key));
  }

  template <class T, class Compare>
  typename radix<T,Compare>::const_iterator radix<T,Compare>::upper_bound(const key_type& key) const {
    return const_iterator(m_root->FindCondition(radix::conditionUpper<key_type>, key));
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::iterator radix<T,Compare>::upper_bound(const K& key){
    return iterator(m_root->FindCondition(radix::conditionUpper, key));
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::const_iterator radix<T,Compare>::upper_bound(const K& key) const {
    return const_iterator(m_root->FindCondition(radix::conditionUpper, key));
  }

  /////////////////
  // EQUAL RANGE //
  /////////////////

  template <class T, class Compare>
  typename radix<T,Compare>::iterator_pair radix<T,Compare>::equal_range(const key_type& key)
  {
    iterator lb = lower_bound(key);
    iterator ub;
    for (ub = lb; ub != cend() && !radix::conditionUpper(ub->first,key); ++ub){}

    return std::make_pair(lb, ub);
  }

  template <class T, class Compare>
  typename radix<T,Compare>::const_iterator_pair radix<T,Compare>::equal_range(const key_type& key) const
  {
    const_iterator lb = lower_bound(key);
    const_iterator ub;
    for (ub = lb; ub != cend() && !radix::conditionUpper(ub->first,key); ++ub){}

    return std::make_pair(lb, ub);
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::iterator_pair radix<T,Compare>::equal_range(const K& key)
  {
    iterator lb = lower_bound(key);
    iterator ub;
    for (ub = lb; ub != cend() && !radix::conditionUpper(ub->first,key); ++ub){}

    return std::make_pair(lb, ub);
  }

  template <class T, class Compare> template <class K>
  typename radix<T,Compare>::const_iterator_pair radix<T,Compare>::equal_range(const K& key) const
  {
    const_iterator lb = lower_bound(key);
    const_iterator ub;
    for (ub = lb; ub != cend() && !radix::conditionUpper(ub->first,key); ++ub){}

    return std::make_pair(lb, ub);
  }

  template <class T, class Compare>
  T& radix<T,Compare>::operator[](const key_type& key){
    return insert(std::make_pair(key, T())).first->second;
  }

  template <class T, class Compare>
  typename xsm::radix<T,Compare>::size_type xsm::radix<T,Compare>::count(const key_type& key) const {
    return contains(key)? 1 : 0;
  }
  
  template <class T, class Compare> template <class K>
  typename xsm::radix<T,Compare>::size_type xsm::radix<T,Compare>::count(const K& key) const {

    size_type i = 0;
    for (auto it = lower_bound(key); it != cend() && !radix::conditionUpper(it->first,key); ++it){
      ++i;
    }
    return i;

  }

  template <class T, class Compare>
  bool radix<T,Compare>::contains(const key_type& key) const {
    const_node_ptr node = m_root->Retrieve(key);
    return (node != nullptr && node->IsLeaf());
  }

  template <class T, class Compare> template <class K>
  bool radix<T,Compare>::contains(const K& key) const {
    return (find(key) != cend());
  }
  
  template <class T, class Compare>
  detail::Iterator_impl<T,Compare> radix<T,Compare>::begin() noexcept {
    // need to increment because the root node is not a leaf node
    return ++iterator(m_root);
  }
  
  template <class T, class Compare>
  detail::Iterator_impl<T,Compare,const T> radix<T,Compare>::begin() const noexcept {
    return cbegin();
  }
  
  template <class T, class Compare>
  detail::Iterator_impl<T,Compare> radix<T,Compare>::end() noexcept {
    return iterator(m_root);
  }
  
  template <class T, class Compare>
  detail::Iterator_impl<T,Compare,const T> radix<T,Compare>::end() const noexcept {
    return cend();
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::reverse_iterator radix<T,Compare>::rbegin() noexcept {
    return reverse_iterator(end());
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::const_reverse_iterator radix<T,Compare>::rbegin() const noexcept {
    return crbegin();
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::reverse_iterator radix<T,Compare>::rend() noexcept {
    return reverse_iterator(begin());
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::const_reverse_iterator radix<T,Compare>::rend() const noexcept {
    return crend();
  }

  template <class T, class Compare>
  detail::Iterator_impl<T,Compare,const T> radix<T,Compare>::cbegin() const noexcept {
    return ++const_iterator(m_root);
  }
  
  template <class T, class Compare>
  detail::Iterator_impl<T,Compare,const T> radix<T,Compare>::cend() const noexcept {
    return const_iterator(m_root);
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::const_reverse_iterator radix<T,Compare>::crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  
  template <class T, class Compare>
  typename radix<T,Compare>::const_reverse_iterator radix<T,Compare>::crend() const noexcept {
    return const_reverse_iterator(begin());
  }
  
  template <class T, class Compare>
  void radix<T,Compare>::print() const {
    std::cout << "ROOT";
    m_root->print();
    std::cout << "\n" << std::flush;
  }
}

namespace xsm::detail{ 
  //////////////
  // ITERATOR //
  //////////////
  template <class T, class Compare, class ItType>
  Iterator_impl<T,Compare,ItType>::Iterator_impl() : m_node(nullptr) {}
  
  template <class T, class Compare, class ItType>
  Iterator_impl<T,Compare,ItType>::Iterator_impl(node_ptr ptr) : m_node(ptr) {}
 
  template <class T, class Compare, class ItType>
  Iterator_impl<T,Compare,ItType>& Iterator_impl<T,Compare,ItType>::operator++(){
    // Advance iterator until you reach a leaf node
    while (Advance()) {}
    return *this;
  }

  template <class T, class Compare, class ItType>
  Iterator_impl<T,Compare,ItType> Iterator_impl<T,Compare,ItType>::operator++(int){
    Iterator_impl<T,Compare,ItType> temp = *this;
    ++*this;
    return temp;
  }

  template <class T, class Compare, class ItType>
  Iterator_impl<T,Compare,ItType>& Iterator_impl<T,Compare,ItType>::operator--(){
    // Regress iterator until you reach a leaf node
    while (Regress()) {}
    return *this;
  }

  template <class T, class Compare, class ItType>
  Iterator_impl<T,Compare,ItType> Iterator_impl<T,Compare,ItType>::operator--(int){
    Iterator_impl<T,Compare,ItType> temp = *this;
    --*this;
    return temp;
  }

  template <class T, class Compare, class ItType>
  typename Iterator_impl<T,Compare,ItType>::reference Iterator_impl<T,Compare,ItType>::operator*() const {
    return m_node->m_value_pair;
  }

  template <class T, class Compare, class ItType>
  typename Iterator_impl<T,Compare,ItType>::pointer Iterator_impl<T,Compare,ItType>::operator->() const {
    return &m_node->m_value_pair;
  }

  // conversion iterator to const_iterator
  template <class T, class Compare, class ItType>
  Iterator_impl<T,Compare,ItType>::operator const_iterator() const {
    return Iterator_impl<T,Compare,const T>(m_node);
  }

  std::string StrDiff(const std::string& fullstr, const std::string& substr){
    auto last_match = std::mismatch(fullstr.begin(), fullstr.end(), substr.begin(), substr.end());
    return std::string(last_match.first, fullstr.end());
  }

  // Advances iterator forward by one. Returns true if the iterator lands on a non-leaf node
  template <class T, class Compare, class ItType>
  bool Iterator_impl<T,Compare,ItType>::Advance(){

    // If node has children, go to first child in sequence
    if (!m_node->IsChildless()){
      m_node = m_node->GetFirstChild();
      return !m_node->IsLeaf();
    }

    // Until we reach the root node
    while (!m_node->GetKey().empty()){
      // If node has no children, then go up to parent and remember child
      key_type prev_child = m_node->GetKey();
      m_node = m_node->GetParent();

      // Find next child 
      key_type key = StrDiff(prev_child, m_node->GetKey());
      auto it = m_node->GetChildren().find(key);
      ++it;
      if (it != m_node->GetChildren().end()){
        m_node = it->second;
        return !m_node->IsLeaf();
      }
    }
    // Cannot advance anymore, nullptr reached
    return false;
  }

  template <class T, class Compare, class ItType>
  bool Iterator_impl<T,Compare,ItType>::Regress(){
    if (m_node->GetParent()){
      // Check whether this node has younger siblings
      // younger == sorted before
      key_type start_key = StrDiff(this->operator->()->first, m_node->GetParent()->GetKey());
      auto it = m_node->GetParent()->GetChildren().find(start_key);
      // This node has no younger siblings
      if (it == m_node->GetParent()->GetChildren().begin()){
        m_node = m_node->GetParent();
        return !m_node->IsLeaf();
      }
      m_node = (--it)->second;
    }
    while (!m_node->GetChildren().empty()){
      m_node = m_node->GetChildren().rbegin()->second;
    }
    return !m_node->IsLeaf();
  }
  
  template <class T, class Compare, class ItType, class ItType2>
  bool operator==(const Iterator_impl<T,Compare,ItType>& lhs, const Iterator_impl<T,Compare,ItType2>& rhs){
    return lhs.m_node == rhs.m_node;
  }

  template <class T, class Compare, class ItType, class ItType2>
  bool operator!=(const Iterator_impl<T,Compare,ItType>& lhs, const Iterator_impl<T,Compare,ItType2>& rhs){
    return !(lhs == rhs);
  }
  
  //////////
  // NODE //
  //////////
  // Constructors
  template <class T, class Compare>
  Node<T,Compare>::Node()
    : m_is_leaf(false), 
      m_value_pair(std::make_pair("",T())),
      m_parent(nullptr),
      m_children(child_map()) {}

  template <class T, class Compare>
  Node<T,Compare>::Node(node_ptr parent, value_type&& value_pair, const bool is_leaf)
    : m_is_leaf(is_leaf),
      m_value_pair(value_pair),
      m_parent(parent),
      m_children(child_map()) {}
 
  template <class T, class Compare> template <class... Args>
  Node<T,Compare>::Node(node_ptr parent, const bool is_leaf, Args&&... args)
    : m_is_leaf(is_leaf),
      m_value_pair(value_type(std::forward<Args>(args)...)),
      m_parent(parent),
      m_children(child_map()) {}

  // Destructor
  // Each Node is responsible for deleting its children
  template <class T, class Compare>
  Node<T,Compare>::~Node(){
    for (auto& entry : GetChildren()){
      delete entry.second;
    }
  }

  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::Retrieve(const key_type& key) {
    if (key.empty()){
      return this;
    }
    auto first = key.begin();
    auto key_end = key.end();
    auto pos = first;
    const child_map* children = &(this->GetChildren());

    do {
      ++pos;
      key_type subkey(first,pos);
      auto it = children->find(subkey);

      if (it != children->end()){
        if (pos == key_end){
          return it->second;
        }

        first = pos;
        children = &(it->second->GetChildren());
      }
    } while (pos != key_end);

    return this;
  }

  template <class T, class Compare>
  size_t Node<T,Compare>::CountChildren() const {
    return GetChildren().size();
  }
  
  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::AddChild(const key_type& word, node_ptr node){
    node->SetParent(this);
    return GetChildren().emplace(word, node).first->second;
  }

  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::AddChild(const key_type& part){
    return AddChild(part, new Node(this, false, GetKey() + part, T()));
  }


  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::Extract(){

    if (CountChildren() == 0){
      // Store parent ptr so that this node can emancipate
      node_ptr parent = GetParent();
      // Cut ties with parent
      Emancipate();
      // Remove parent if they: 
      //  Are not a leaf node
      //  & Are childless
      //  & Are not the root node
      if (!parent->IsLeaf() && parent->IsChildless() && !parent->GetKey().empty()){
        delete parent->Extract();
      }
    }
    else if (CountChildren() == 1){
      // Store parent ptr so that this node can emancipate
      node_ptr parent = GetParent();
      // Cut ties with parent
      Emancipate();
      
      // Child is adopted by parent
      node_ptr orphan = GiveUpChild();
      parent->Adopt(orphan); 
    }
    else {
      // Create new node with same key as target but empty element
      node_ptr empty_node = new Node(nullptr, false, GetKey(), mapped_type());
      
      SubstituteWith(empty_node);
    }
    return this;
  }

  template <class T, class Compare>
  void Node<T,Compare>::SubstituteWith(node_ptr empty_node){
    // Store parent ptr so that this node can emancipate
    node_ptr parent = GetParent();
    // Cut ties with parent
    Emancipate();

    // Let parent adopt new node
    parent->Adopt(empty_node);

    // Transfer children from target to new node
    empty_node->GetChildren().swap(GetChildren());

    for (auto it : empty_node->GetChildren()){
      it.second->SetParent(empty_node);
    }
  }
  
  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::GiveUpChild(){
    // Extracts the first child of this node, severs the relation between child and this node,
    // and returns the orphaned node pointer
    auto first_child_it = GetChildren().begin();
    auto nh = GetChildren().extract(first_child_it);

    node_ptr orphan = nh.mapped();

    orphan->RemoveParent();
    return orphan;
  }

  template <class T, class Compare>
  void Node<T,Compare>::RemoveParent(){
    SetParent(nullptr);
  }

  template <class T, class Compare>
  void Node<T,Compare>::Adopt(node_ptr child){
    const key_type str = StrDiff(child->GetKey(), GetKey());
    AddChild(str, child);
  }

  template <class T, class Compare>
  void Node<T,Compare>::Emancipate(){
    // Cut ties with parent
    const key_type keydiff = StrDiff(GetKey(), GetParent()->GetKey());
    GetParent()->GetChildren().erase(GetParent()->GetChildren().find(keydiff));
    RemoveParent();
  }

  ////////////////////
  // FIND CONDITION //
  ////////////////////
  // This function is used for both lower_bound() and upper_bound(). Each function specifies a
  // condition for finding the desired element as a function and passes it to FindCondition() as
  // a function pointer. The condition must strictly specify a key order, not just key equality.
  template <class T, class Compare> template <class K>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::FindCondition(
      bool(*condition)(const key_type&, const K&), const K& key){

    node_ptr candidate_node = FindConditionNonLeaf(condition, key);

    // Find first non-leaf node (candidate is not root)
    if (!candidate_node->GetKey().empty()){
      while (!candidate_node->IsLeaf()){
        candidate_node = candidate_node->GetFirstChild();
      }
    }

    return candidate_node;
  }

  template <class T, class Compare> template <class K>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::FindConditionNonLeaf(
      bool(*condition)(const key_type&, const K&), const K& key){

    node_ptr candidate_node = this;
    child_map* children = &(this->GetChildren());
    bool match_found = false;

    while(!match_found){

      // Search for first child that matches the condition in search node
      auto it = children->cbegin();
      auto endit = children->cend();
      while (it != endit && !condition(it->second->GetKey(), key)){
        ++it;
      }
      // Search done. Iterator may now point to map element or to end iterator

      if (it != endit) {
        candidate_node = it->second;
      }
      
      // If match is the first in among the children, then there cannot be a match
      // preceding the candidate
      if (it == children->begin()){
        match_found = true;
      }
      else {
        --it;
        children = &(it->second->GetChildren());
      }
      
    } // while
    return candidate_node;
  }

  template <class T, class Compare>
  bool Node<T,Compare>::IsChildless() const {
    return !GetChildren().size();
  }

  template <class T, class Compare>
  bool Node<T,Compare>::IsLeaf() const {
    return m_is_leaf;
  }
  
  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::GetParent() const {
    return m_parent;
  }
  
  template <class T, class Compare>
  void Node<T,Compare>::SetParent(node_ptr node){
    m_parent = node;
  }
  
  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::GetFirstChild() const {
    return GetChildren().begin()->second;
  }

  template <class T, class Compare>
  typename Node<T,Compare>::node_ptr Node<T,Compare>::GetLastDescendant() {
    node_ptr candidate = this;
    while (!candidate->GetChildren().empty()){
      candidate = candidate->GetChildren().rbegin()->second;
    }
    return candidate;
  }
  
  template <class T, class Compare>
  typename Node<T,Compare>::child_map& Node<T,Compare>::GetChildren(){
    return m_children;
  }

  template <class T, class Compare>
  const typename Node<T,Compare>::child_map& Node<T,Compare>::GetChildren() const {
    return m_children;
  }

  template <class T, class Compare>
  const typename Node<T,Compare>::key_type& Node<T,Compare>::GetKey() const {
    return m_value_pair.first;
  }
  
  template <class T, class Compare>
  void Node<T,Compare>::print() const {
    std::cout << " ("<< (IsLeaf()? "+" : "-") << ")";
    std::cout << " <";
    for (auto& entry : GetChildren()){
      std::cout << entry.first;
      entry.second->print();
    }
    std::cout << "> ";
  }

  /////////////////
  // NODE HANDLE //
  /////////////////
  template <class T, class Compare>
  Node_handle<T,Compare>::~Node_handle(){
    // If node is orphan and has no children, then delete
    // Otherwise, node will be deleted by its parent
    if (m_node_ptr && !m_node_ptr->GetParent() && !m_node_ptr->CountChildren()){
      delete m_node_ptr;
    }
    else {
      m_node_ptr = nullptr;
    }
  }

  template <class T, class Compare>
  Node_handle<T,Compare>::Node_handle(Node_handle<T,Compare>&& node) noexcept {
    m_node_ptr = node.m_node_ptr;
    node.m_node_ptr = nullptr;
  }
  
  template <class T, class Compare>
  Node_handle<T,Compare>& Node_handle<T,Compare>::operator=(Node_handle<T,Compare>&& node){
    if (m_node_ptr && !m_node_ptr->GetParent() && !m_node_ptr->CountChildren()){
      delete m_node_ptr;
    }
    m_node_ptr = node.m_node_ptr;
    node.m_node_ptr = nullptr;
    return *this;    
  }

  template <class T, class Compare>
  Node_handle<T,Compare>::Node_handle(node_ptr node) : m_node_ptr(node) {}

  template <class T, class Compare>
  [[nodiscard]] bool Node_handle<T,Compare>::empty() const noexcept {
    return !m_node_ptr;
  }
      
  template <class T, class Compare>
  Node_handle<T,Compare>::operator bool() const noexcept {
    return m_node_ptr;
  }
  
  template <class T, class Compare>
  const typename Node_handle<T,Compare>::key_type& Node_handle<T,Compare>::key() const {
    return m_node_ptr->GetKey();
  }
                                                                               
  template <class T, class Compare>
  typename Node_handle<T,Compare>::mapped_type& Node_handle<T,Compare>::mapped() const {
    return m_node_ptr->m_value_pair.second;
  }

}

#endif /* radix_hpp */
