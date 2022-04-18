//
//  radix.hpp
//
//  Created by Jasmin B. Maglic on 01.01.22.
//

#ifndef radix_hpp
#define radix_hpp

//////////////////
// DECLARATIONS //
//////////////////

#include <map>
#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <cassert>
#include <iterator>

// Forward declaration for friend relation
namespace xsm{
  template <class T> class radix;
}

namespace xsm::detail{
  // Forward declaration for friend relation
  template <class T, class ItType=T> class Iterator_impl;

  //////////
  // NODE //
  //////////
  // Node represents a key-value pair
  template <class T> class Node{
    friend typename radix<T>::iterator;
    friend typename radix<T>::const_iterator;
    friend xsm::radix<T>;

    using node_type = typename radix<T>::node_type;
    using key_type = typename radix<T>::key_type;
    using mapped_type = typename radix<T>::mapped_type;
    using value_type = typename radix<T>::value_type;
    public:
      Node();
      Node(node_type, value_type&&, const bool=false);
      template <class... Args> Node(node_type, const bool, Args&&...);
      ~Node();
      
      // Display
      void print();
    private:
      // Members
      value_type m_value_pair;
      node_type m_parent;
      std::map<key_type,node_type> m_children;
      bool m_is_leaf;

      // Container operations
      void Remove();
      const Node<T>* Retrieve(const key_type&) const;
      node_type Retrieve(const key_type&);

      // Methods used during container manipulation
      void MakeLeaf(const mapped_type&);
      node_type AddChild(const key_type&, node_type);
      node_type AddChild(const key_type&);
      
      // For iterator operations
      bool IsChildless() const;
      bool IsLeaf() const;
      Node* GetParent() const;
      void SetParent(node_type);
      Node* GetFirstChild() const;
      const std::map<key_type,node_type>& GetChildren() const;
  };

  // Forward declarations to allow for overloaded comparison operators
  template <class T, class ItType, class ItType2=ItType>
  bool operator==(const Iterator_impl<T,ItType>&, const Iterator_impl<T,ItType2>&);
  template <class T, class ItType, class ItType2=ItType>
  bool operator!=(const Iterator_impl<T,ItType>&, const Iterator_impl<T,ItType2>&);

  //////////////
  // ITERATOR //
  //////////////
  // Custom iterator class
  template <class T, class ItType>
  class Iterator_impl : public std::iterator<std::bidirectional_iterator_tag, typename radix<T>::value_type> {
    friend radix<T>;
    // If ItType is T: ItType2 is const T
    // If ItType is const T: ItType2 is T
    typedef typename std::conditional<std::is_const_v<ItType>, T, const T>::type ItType2;
    friend Iterator_impl<T,ItType2>;

    using node_type = typename radix<T>::node_type;
    using const_iterator = typename radix<T>::const_iterator;
    using key_type = typename radix<T>::key_type;
    using reverse_iterator = typename radix<T>::reverse_iterator;
    using const_reverse_iterator = typename radix<T>::const_reverse_iterator;
    // These typenames cause an error:
    // using reference = typename radix<T>::reference;
    // using pointer = typename radix<T>::pointer;
    public:
      // Constructor
      Iterator_impl(node_type);

      // Iterator operations
      Iterator_impl& operator++();
      Iterator_impl operator++(int);
      Iterator_impl& operator--();
      Iterator_impl operator--(int);
      //Iterator_impl operator++(int);
      typename radix<T>::reference operator*() const;
      typename radix<T>::pointer operator->() const;

      // Conversion from iterator to const_iterator
      operator const_iterator() const;
      
      // Explicit instantiation for template type
      friend bool operator== <> (const Iterator_impl<T,ItType>&, const Iterator_impl<T,ItType>&);
      friend bool operator== <> (const Iterator_impl<T,ItType>&, const Iterator_impl<T,ItType2>&);
      friend bool operator!= <> (const Iterator_impl<T,ItType>&, const Iterator_impl<T,ItType>&);
      friend bool operator!= <> (const Iterator_impl<T,ItType>&, const Iterator_impl<T,ItType2>&);
    private:
      node_type m_node;

      bool Advance();
      bool Regress();
  };
}  

namespace xsm{
  ///////////
  // RADIX //
  ///////////
  // Publicly accessible container class
  template <class T> class radix{
    public:
      radix();
      radix(const radix&);
      radix(radix&&) noexcept;
      ~radix();

      radix<T>& operator=(const radix<T>&);
      radix<T>& operator=(radix<T>&&) noexcept;

      // Aliases
      typedef std::string key_type;
      typedef T mapped_type;
      typedef std::pair<const key_type, mapped_type> value_type;
      typedef std::size_t size_type;
      typedef value_type& reference;
      typedef const value_type& const_reference;
      typedef value_type* pointer;
      typedef const value_type* const_pointer;
      typedef detail::Iterator_impl<T> iterator;
      typedef detail::Iterator_impl<T,const T> const_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef detail::Node<mapped_type>* node_type;

      // Capacity
      bool empty() const;
      size_type size() const noexcept;
  
      // Modifiers
      std::pair<iterator,bool> insert(const value_type&);
      std::pair<iterator,bool> insert(value_type&&);
      std::pair<iterator,bool> insert(const key_type&, const mapped_type&);
      std::pair<bool,bool> insert(const std::vector<std::string>&, const mapped_type&);
      // insert_or_assign
      template <class... Args>
      std::pair<iterator,bool> emplace(Args&&...);
      // try_emplace
      iterator erase(iterator);
      iterator erase(const_iterator);
      iterator erase(const_iterator, const_iterator);
      size_type erase(const key_type&);
      void swap(radix<T>&);
      void clear();
      // extract
      // merge

      // Element access
      mapped_type& at(const key_type&);
      const mapped_type& at(const key_type&) const;
      iterator find(const key_type&);
      const_iterator find(const key_type&) const;
      mapped_type& operator[](const key_type&);

      // Lookup
      // count
      bool contains(const key_type&) const;

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

      void print();

    private:
      // Pointer to root node of radix tree
      node_type m_root; 
      size_type m_size;
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
  template <class T>
  radix<T>::radix() : m_root(new detail::Node<T>()), m_size(0) {}
  
  // Copy constructor
  template <class T>
  radix<T>::radix(const radix& rdx){
    m_root = new detail::Node<T>();
    for (auto it = rdx.begin(); it != rdx.end(); ++it){
      insert(it->first, it->second);
    }
    m_size = rdx.size();
  }

  // Move constructor
  template <class T>
  radix<T>::radix(radix&& rdx) noexcept {
    m_root = rdx.m_root;
    m_size = rdx.size();
    rdx.m_root = new detail::Node<T>();
    rdx.m_size = 0;
  }
  
  // Destructor
  template <class T>
  radix<T>::~radix(){
    delete m_root;
  }
  
  // Copy assignment
  template <class T>
  radix<T>& radix<T>::operator=(const radix<T>& rdx){
    radix<T> cpy = rdx;
    this->swap(cpy);
    return *this;
  }

  // Move assignment
  template <class T>
  radix<T>& radix<T>::operator=(radix<T>&& rdx) noexcept {
    delete m_root;
    m_root = rdx.m_root;
    m_size = rdx.m_size;
    rdx.m_root = new detail::Node<T>();
    rdx.m_size = 0;
    return *this;
  }

  template <class T>
  bool radix<T>::empty() const {
    return begin() == end();
  }
  
  template <class T>
  typename radix<T>::size_type radix<T>::size() const noexcept {
    return m_size;
  }

  template <class T>
  std::pair<detail::Iterator_impl<T>,bool> radix<T>::insert(const value_type& key_value){ 
    return emplace(std::move(key_value));
  }

  template <class T>
  std::pair<detail::Iterator_impl<T>,bool> radix<T>::insert(value_type&& key_value){ 
    return emplace(key_value);
  }
  
  template <class T>
  std::pair<detail::Iterator_impl<T>,bool> radix<T>::insert(const key_type& key, const mapped_type& value){
    return emplace(std::move(std::make_pair(key, value)));
  }
  
  template <class T>
  std::pair<bool,bool> radix<T>::insert(const std::vector<key_type>& key_list, const mapped_type& value){
    bool all = true;
    bool any = false;
    for (const key_type& key : key_list){
      bool success = insert(key, value).second;
      all &= success;
      any |= success;
    }
    return std::make_pair(all,any);
  }

  template <class T> template <class... Args>
  std::pair<detail::Iterator_impl<T>,bool> radix<T>::emplace(Args&&... args){
    node_type node_ptr = new detail::Node<T>(nullptr, true, std::forward<Args>(args)...);
    iterator parent = iterator(m_root);

    auto key_start = node_ptr->m_value_pair.first.begin();
    auto key_end = node_ptr->m_value_pair.first.end();

    bool next_child;
    // This loop only repeats if we descend one level in the tree
    do {
      next_child = false;

      // Loop through all children in iterator, until you find a match within the keys
      for (auto& entry : parent.m_node->m_children){
        auto entrykey_start = entry.first.begin();
        auto entrykey_end = entry.first.end();
        auto last_match = std::mismatch(key_start, key_end, entrykey_start, entrykey_end);
        
        // 1. Complete mismatch -> Check next child
        if (key_start == last_match.first){continue;}

        // 2. New key and entry key are identical -> Key already exists, or leads to a non-leaf node
        if (key_end == last_match.first && entrykey_end == last_match.second){

          bool new_entry = !entry.second->IsLeaf();
          if (new_entry){

            entry.second->MakeLeaf(node_ptr->m_value_pair.second);
            delete node_ptr;

            m_size++;
          }
          return std::make_pair(iterator(entry.second), new_entry);
        }

        // 3. One key is prefix of the other -> The prefix key becomes the parent of the other entry
        else if (key_end == last_match.first || entrykey_end == last_match.second){
          // New keyword is prefix of the existing nodekey
          if (key_end == last_match.first){
            // Add a new node with the prefix to the current node
            parent.m_node->AddChild(std::string(key_start, key_end), node_ptr);
            // Old child node becomes child of the new node
            node_ptr->AddChild(std::string(last_match.second, entrykey_end), entry.second);
            // Remove old node from current node
            parent.m_node->m_children.erase(entry.first);

            m_size++;
            return std::make_pair(iterator(node_ptr),true);
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
          std::string common_prefix(key_start, last_match.first);

          // New parent
          node_type parent_ptr = parent.m_node->AddChild(std::string(key_start, last_match.first));
          // Old entry
          parent_ptr->AddChild(std::string(last_match.second,entrykey_end), entry.second);
          entry.second->SetParent(parent_ptr);
          parent.m_node->m_children.erase(entry.first); 
          // New entry
          parent_ptr->AddChild(std::string(last_match.first,key_end), node_ptr);

          m_size++;
          return std::make_pair(iterator(node_ptr),true);
        }
      }
    } while (next_child);
    
    // 5. No common prefix has been found in any children -> keyword becomes a new entry
    parent.m_node->AddChild(std::string(key_start, key_end), node_ptr);
    m_size++;
    return std::make_pair(iterator(node_ptr), true);
  }

  template <class T>
  typename radix<T>::iterator radix<T>::erase(iterator it){
    return erase(const_iterator(it));
  }

  template <class T>
  typename radix<T>::iterator radix<T>::erase(const_iterator it){
    detail::Node<T>* node = it.m_node;
    ++it;
    node->Remove();
    --m_size;
    return iterator(it.m_node); 
  }

  template <class T>
  typename radix<T>::iterator radix<T>::erase(const_iterator first, const_iterator last){
    while (first != last){
      first = erase(first);
    }
    return iterator(last.m_node);
  }

  template <class T>
  typename radix<T>::size_type radix<T>::erase(const key_type& key){
    if (contains(key)){
      erase(find(key));
      return 1;
    }
    return 0;
  }

  template <class T>
  void radix<T>::swap(radix<T>& rdx){
    // swap m_root
    {
      detail::Node<T>* temp = m_root;
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

  template <class T>
  void radix<T>::clear(){
    delete m_root;
    m_root = new detail::Node<T>();
    m_size = 0;
  }

  template <class T>
  T& radix<T>::at(const std::string& key){
    return const_cast<T&>(std::as_const(*this).at(key));
  }

  template <class T>
  const T& radix<T>::at(const std::string& key) const {
    const detail::Node<T>* ptr = m_root->Retrieve(key);
    if (ptr == nullptr){
      throw std::out_of_range("radix::at:  key not found");
    }
    return m_root->Retrieve(key)->m_value_pair.second;
  }
  
  template <class T>
  typename radix<T>::iterator radix<T>::find(const key_type& key){
    return iterator(m_root->Retrieve(key));
  }

  template <class T>
  typename radix<T>::const_iterator radix<T>::find(const key_type& key) const {
    return const_iterator(m_root->Retrieve(key));
  }

  template <class T>
  T& radix<T>::operator[](const std::string& key){
    return insert(std::make_pair(key, T())).first->second;
  }

  template <class T>
  bool radix<T>::contains(const std::string& key) const {
    const detail::Node<T>* ptr = m_root->Retrieve(key);
    return (ptr != nullptr && ptr->m_is_leaf);
  }
  
  template <class T>
  detail::Iterator_impl<T> radix<T>::begin() noexcept {
    // need to increment because the root node is not a leaf node
    return ++iterator(m_root);
  }
  
  template <class T>
  detail::Iterator_impl<T,const T> radix<T>::begin() const noexcept {
    return cbegin();
  }
  
  template <class T>
  detail::Iterator_impl<T> radix<T>::end() noexcept {
    return iterator(m_root);
  }
  
  template <class T>
  detail::Iterator_impl<T,const T> radix<T>::end() const noexcept {
    return cend();
  }
  
  template <class T>
  typename radix<T>::reverse_iterator radix<T>::rbegin() noexcept {
    return reverse_iterator(end());
  }
  
  template <class T>
  typename radix<T>::const_reverse_iterator radix<T>::rbegin() const noexcept {
    return crbegin();
  }
  
  template <class T>
  typename radix<T>::reverse_iterator radix<T>::rend() noexcept {
    return reverse_iterator(begin());
  }
  
  template <class T>
  typename radix<T>::const_reverse_iterator radix<T>::rend() const noexcept {
    return crend();
  }

  template <class T>
  detail::Iterator_impl<T,const T> radix<T>::cbegin() const noexcept {
    return ++const_iterator(m_root);
  }
  
  template <class T>
  detail::Iterator_impl<T,const T> radix<T>::cend() const noexcept {
    return const_iterator(m_root);
  }
  
  template <class T>
  typename radix<T>::const_reverse_iterator radix<T>::crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  
  template <class T>
  typename radix<T>::const_reverse_iterator radix<T>::crend() const noexcept {
    return const_reverse_iterator(begin());
  }
  
  template <class T>
  void radix<T>::print(){
    m_root->print();
    std::cout << std::endl;
  }
}

namespace xsm::detail{ 
  //////////////
  // ITERATOR //
  //////////////
  template <class T, class ItType>
  Iterator_impl<T,ItType>::Iterator_impl(node_type ptr) : m_node(ptr) {}
 
  template <class T, class ItType>
  Iterator_impl<T,ItType>& Iterator_impl<T,ItType>::operator++(){
    // Advance iterator until you reach a leaf node
    while (Advance()) {}
    return *this;
  }

  template <class T, class ItType>
  Iterator_impl<T,ItType> Iterator_impl<T,ItType>::operator++(int){
    Iterator_impl<T,ItType> temp = *this;
    ++*this;
    return temp;
  }

  template <class T, class ItType>
  Iterator_impl<T,ItType>& Iterator_impl<T,ItType>::operator--(){
    // Regress iterator until you reach a leaf node
    while (Regress()) {}
    return *this;
  }

  template <class T, class ItType>
  Iterator_impl<T,ItType> Iterator_impl<T,ItType>::operator--(int){
    Iterator_impl<T,ItType> temp = *this;
    --*this;
    return temp;
  }

  template <class T, class ItType>
  typename radix<T>::reference Iterator_impl<T,ItType>::operator*() const {
    return m_node->m_value_pair;
  }

  template <class T, class ItType>
  typename radix<T>::pointer Iterator_impl<T,ItType>::operator->() const {
    return &m_node->m_value_pair;
  }

  // conversion iterator to const_iterator
  template<class T, class ItType>
  Iterator_impl<T,ItType>::operator const_iterator() const {
    return Iterator_impl<T,const T>(m_node);
  }

  std::string StrDiff(const std::string& fullstr, const std::string& substr){
    auto last_match = std::mismatch(fullstr.begin(), fullstr.end(), substr.begin(), substr.end());
    return std::string(last_match.first, fullstr.end());
  }

  // Advances iterator forward by one. Returns true if the iterator lands on a non-leaf node
  template <class T, class ItType>
  bool Iterator_impl<T,ItType>::Advance(){

    // If node has children, go to first child in sequence
    if (!m_node->IsChildless()){
      m_node = m_node->GetFirstChild();
      return !m_node->IsLeaf();
    }

    // Until we reach the root node
    while (!m_node->m_value_pair.first.empty()){
      // If node has no children, then go up to parent and remember child
      std::string prev_child = m_node->m_value_pair.first;
      m_node = m_node->GetParent();

      // Find next child 
      std::string key = StrDiff(prev_child, m_node->m_value_pair.first);
      auto it = m_node->m_children.find(key);
      ++it;
      if (it != m_node->m_children.end()){
        m_node = it->second;
        return !m_node->IsLeaf();
      }
    }
    // Cannot advance anymore, nullptr reached
    return false;
  }

  template <class T, class ItType>
  bool Iterator_impl<T,ItType>::Regress(){
    if (m_node->m_parent){
      // Check whether this node has younger siblings
      // younger == sorted before
      key_type start_key = StrDiff(this->operator->()->first, m_node->m_parent->m_value_pair.first);
      auto it = m_node->m_parent->m_children.find(start_key);
      // This node has no younger siblings
      if (it == m_node->m_parent->m_children.begin()){
        m_node = m_node->m_parent;
        return !m_node->IsLeaf();
      }
      m_node = (--it)->second;
    }
    while (!m_node->m_children.empty()){
      m_node = m_node->m_children.rbegin()->second;
    }
    return !m_node->IsLeaf();
  }
  
  template <class T, class ItType, class ItType2>
  bool operator==(const Iterator_impl<T,ItType>& lhs, const Iterator_impl<T,ItType2>& rhs){
    return lhs.m_node == rhs.m_node;
  }

  template <class T, class ItType, class ItType2>
  bool operator!=(const Iterator_impl<T,ItType>& lhs, const Iterator_impl<T,ItType2>& rhs){
    return !(lhs == rhs);
  }
  
  //////////
  // NODE //
  //////////
  // CONSTRUCTORS
  template <class T>
  Node<T>::Node()
    : m_is_leaf(false), 
      m_value_pair(std::make_pair("",T())),
      m_parent(nullptr),
      m_children(std::map<std::string,Node<T>*>()) {}

  template <class T>
  Node<T>::Node(node_type parent, value_type&& value_pair, const bool is_leaf)
    : m_is_leaf(is_leaf),
      m_value_pair(value_pair),
      m_parent(parent),
      m_children(std::map<key_type,node_type>()) {}
 
  template <class T> template <class... Args>
  Node<T>::Node(node_type parent, const bool is_leaf, Args&&... args)
    : m_is_leaf(is_leaf),
      m_value_pair(value_type(std::forward<Args>(args)...)),
      m_parent(parent),
      m_children(std::map<key_type,node_type>()) {}

  // Each Node is responsible for deleting its children
  template <class T>
  Node<T>::~Node(){
    for (auto& entry : m_children){
      delete entry.second;
    }
  }

  // TODO: Copy and move?
  
  template <class T>
  void Node<T>::Remove(){
    if (!IsChildless()){
      m_is_leaf = false;
    }
    else {
      const std::string key = StrDiff(m_value_pair.first, m_parent->m_value_pair.first);
      m_parent->m_children.erase(key);
      // Recursively delete ancestor, if they
      //  - do not have other children, and
      //  - are not a leaf node, and
      //  - are not the root node
      if (m_parent->IsChildless() && !m_parent->m_is_leaf && !m_parent->m_value_pair.first.empty()){
        m_parent->Remove();
      }
      delete this;
    }
  }

  template <class T>
  const Node<T>* Node<T>::Retrieve(const key_type& key) const {
    if (key.empty()){
      return this;
    }
    auto begin = key.begin();
    auto end = key.end();
    for (auto pos = begin; pos <= end; ++pos){
      std::string substr(begin,pos);
      if (m_children.contains(substr)){
        return m_children.at(substr)->Retrieve(std::string(pos,end));
      }
    }
    return nullptr;
  }

  template <class T>
  typename radix<T>::node_type Node<T>::Retrieve(const key_type& key) {
    return const_cast<node_type>(std::as_const(*this).Retrieve(key)); 
  }
  
  template <class T>
  void Node<T>::MakeLeaf(const mapped_type& value){
    m_value_pair.second = value;
    m_is_leaf = true;
  }

  template <class T>
  typename radix<T>::node_type Node<T>::AddChild(const key_type& word, node_type node){
    node->SetParent(this);
    return m_children.emplace(word, node).first->second;
  }

  template <class T>
  typename radix<T>::node_type Node<T>::AddChild(const key_type& part){
    return AddChild(part, new Node(this, false, m_value_pair.first + part, T()));
  }

  template <class T>
  bool Node<T>::IsChildless() const {
    return !m_children.size();
  }

  template <class T>
  bool Node<T>::IsLeaf() const {
    return m_is_leaf;
  }
  
  template <class T>
  Node<T>* Node<T>::GetParent() const {
    return m_parent;
  }
  
  template <class T>
  void Node<T>::SetParent(node_type node){
    m_parent = node;
  }
  
  template <class T>
  Node<T>* Node<T>::GetFirstChild() const {
    return m_children.begin()->second;
  }
  
  template <class T>
  const std::map<typename radix<T>::key_type,typename radix<T>::node_type>& Node<T>::GetChildren() const {
    return m_children;
  }
  
  template <class T>
  void Node<T>::print(){
    for (auto& entry : m_children){
      std::cout << entry.first << " <";
      entry.second->print();
      std::cout << "> ";
    }
    std::cout << (m_is_leaf? "+" : "-");
  }
}

#endif /* radix_hpp */
