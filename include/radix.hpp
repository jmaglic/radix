//
//  radix.hpp
//  wonderland
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
#include <stdexcept>

// Forward declaration for friend relation
namespace xsm{template <class T> class radix;}
namespace xsm::detail{
  // Forward declaration for friend relation
  template <class T> class Iterator_impl;
  // Node represents a key-value pair but the key is not stored explicitly
  template <class T> class Node{
    friend Iterator_impl<T>;
    friend xsm::radix<T>;
    public:
      Node(Node*, const std::string&, const T&, const bool=false);
      ~Node();
      
      // Display
      void print();
    private:
      // Members
      std::pair<const std::string,T> m_value_pair;
      bool m_is_leaf;
      Node* m_parent;
      std::map<std::string,Node*> m_children;

      // Container operations
      std::pair<Node<T>*,bool> Insert(const std::string&, const T&);
      const Node<T>* Retrieve(const std::string&) const;

      // Methods used during container manipulation
      void MakeLeaf(const T&);
      bool AddChild(const std::string&, const T&, const bool=true);
      void AddChild(const std::string&);
      void AddChild(const std::string&, Node*);
      
      // For iterator operations
      bool IsChildless() const;
      bool IsLeaf() const;
      Node* GetParent() const;
      void SetParent(Node*);
      std::pair<std::string,Node*> GetFirstChild() const;
      const std::map<std::string,Node*>& GetChildren() const;
  };

  // Forward declarations to allow for overloaded comparison operators
  template <class T> bool operator==(const Iterator_impl<T>&, const Iterator_impl<T>&);
  template <class T> bool operator!=(const Iterator_impl<T>&, const Iterator_impl<T>&);

  // Custom iterator class
  template <class T> class Iterator_impl {
    public:
      Iterator_impl(Node<T>*, const std::string&);

      Iterator_impl& operator++();
      std::pair<std::string,T>& operator*() const;
      std::pair<std::string,T>* operator->() const;
      std::string GetKey() const;
      const T& GetValue() const;
      
      // Explicit instantiation for template type
      friend bool operator== <> (const Iterator_impl<T>&, const Iterator_impl<T>&);
      friend bool operator!= <> (const Iterator_impl<T>&, const Iterator_impl<T>&);
    private:
      bool Advance();
      std::pair<std::string,T> m_key_value;
      Node<T>* m_node;
  };
}  

namespace xsm{
  // Publicly accessible container class
  template <class T> class radix{
    public:
      radix();
      ~radix();

      // Aliases
      typedef detail::Iterator_impl<T> iterator;
      typedef detail::Iterator_impl<const T> const_iterator;
      typedef std::string key_type;
      typedef T mapped_type;
      typedef std::pair<const key_type, mapped_type> value_type;
  
      // Modifiers
      std::pair<iterator,bool> insert(const std::string&, const mapped_type&);
      std::pair<iterator,bool> insert(const value_type&);
      std::pair<bool,bool> insert(const std::vector<std::string>&, const mapped_type&);

      // Element access
      mapped_type& at(const key_type&);
      const mapped_type& at(const key_type&) const;
      T& operator[](const key_type&);

      // Lookup
      bool contains(const key_type&) const;

      iterator begin();
      iterator end();
      void print();

    private:
      // Pointer to root node of radix tree
      detail::Node<mapped_type>* m_root; 
  };
}
 
/////////////////
// DEFINITIONS //
/////////////////

#include <iostream>
#include <utility>

namespace xsm{
  ///////////////
  // RADIXTREE //
  ///////////////
  template <class T>
  radix<T>::radix(){
    m_root = new detail::Node<T>(NULL, "", T());
  }
  
  template <class T>
  radix<T>::~radix(){
    delete m_root;
  }
  
  template <class T>
  std::pair<bool,bool> radix<T>::insert(const std::vector<std::string>& key_list, const T& value){
    bool all = true;
    bool any = false;
    for (const std::string& key : key_list){
      bool success = m_root->Insert(key, value).second;
      all &= success;
      any |= success;
    }
    return std::make_pair(all,any);
  }
  
  template <class T>
  std::pair<detail::Iterator_impl<T>,bool> radix<T>::insert(const std::string& key, const T& value){
    std::pair<detail::Node<T>*,bool> node_success = m_root->Insert(key, value);
    return std::make_pair(detail::Iterator_impl(node_success.first, key), node_success.second);
  }

  template <class T>
  std::pair<detail::Iterator_impl<T>,bool> radix<T>::insert(const value_type& key_value){ 
    std::pair<detail::Node<T>*,bool> node_success = m_root->Insert(key_value.first, key_value.second);
    return std::make_pair(detail::Iterator_impl(node_success.first, key_value.first), node_success.second);
  }

  template <class T>
  T& radix<T>::at(const std::string& key){
    return const_cast<T&>(std::as_const(*this).at(key));
  }

  template <class T>
  const T& radix<T>::at(const std::string& key) const {
    const detail::Node<T>* ptr = m_root->Retrieve(key);
    if (ptr == NULL){
      throw std::out_of_range("radix::at:  key not found");
    }
    return m_root->Retrieve(key)->m_value_pair.second;
  }

  template <class T>
  T& radix<T>::operator[](const std::string& key){
    return NULL;
    //return insert(std::make_pair(key, T())).first->second;
  }

  template <class T>
  bool radix<T>::contains(const std::string& key) const {
    const detail::Node<T>* ptr = m_root->Retrieve(key);
    return (ptr != NULL);
  }
  
  template <class T>
  detail::Iterator_impl<T> radix<T>::begin(){
    // need to increment because the root node is not a leaf node
    return ++iterator(m_root, "");
  }
  
  template <class T>
  detail::Iterator_impl<T> radix<T>::end(){
    return iterator(NULL, "");
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
  template <class T>
  Iterator_impl<T>::Iterator_impl(Node<T>* ptr, const std::string& key) : 
    m_key_value(std::make_pair(key,ptr? ptr->m_value_pair.second : T())), m_node(ptr) {}
 
  template <class T>
  Iterator_impl<T>& Iterator_impl<T>::operator++(){
    // Advance iterator until you reach a leaf node
    while (Advance()) {}
    m_key_value.second = m_node? m_node->m_value_pair.second : T();
    return *this;
  }

  template <class T>
  std::pair<std::string,T>& Iterator_impl<T>::operator*() const {
    return m_key_value;
  }

  template <class T>
  std::pair<std::string,T>* Iterator_impl<T>::operator->() const {
    return m_key_value;
  }

  // Advances iterator forward by one. Returns true if the iterator lands on a non-leaf node
  template <class T>
  bool Iterator_impl<T>::Advance(){
    // This pointer does not need to be deleted
    Node<T>* prev_child = NULL;
    
    // If node has children, go to first child in sequence
    if (!m_node->IsChildless()){
      std::string suffix;
      std::tie(suffix, m_node) = m_node->GetFirstChild();
      m_key_value.first += suffix;
      return !m_node->IsLeaf();
    }
    // If node has no children, then go up to parent and remember child
    else {
      prev_child = m_node;
      m_node = m_node->GetParent();
    }
    
    while (m_node != NULL){
      // Search for the previous child
      bool prev_child_found = false;
    
      // Going through all children and comparing the pointer is inefficient, O(n) where
      // n is the number of elements in the map. It may be better if the iterator kept track 
      // of the key that way used to access this node from its parent
      for (const auto& elem : m_node->GetChildren()){
        // Go to the next child in sequence, located after the previously visited child
        if (prev_child_found){
          m_key_value.first += elem.first;
          m_node = elem.second;
          return !m_node->IsLeaf();
        }
        if (prev_child == elem.second){
          prev_child_found = true;
          m_key_value.first.erase(m_key_value.first.length()-elem.first.length());
        }
      }
      // If this section is reached, then the previous child was the last in sequence
      // Therefore, go up to parent
      prev_child = m_node;
      m_node = m_node->GetParent();
    }
    // Iterator has reached end
    return false;
  }
  
  template <class T>
  std::string Iterator_impl<T>::GetKey() const {
    return m_key_value.first;
  }
  
  template <class T>
  const T& Iterator_impl<T>::GetValue() const {
    return m_key_value.second;
  }

  template <class T>
  bool operator==(const Iterator_impl<T>& lhs, const Iterator_impl<T>& rhs){
    return lhs.m_node == rhs.m_node;
  }

  template <class T>
  bool operator!=(const Iterator_impl<T>& lhs, const Iterator_impl<T>& rhs){
    return !(lhs == rhs);
  }
  
  //////////
  // NODE //
  //////////
  // CONSTRUCTORS
  template <class T>
  Node<T>::Node(Node* parent, const std::string& key, const T& value, const bool is_leaf) 
    : m_is_leaf(is_leaf), 
      m_value_pair(std::make_pair(key,value)),
      m_parent(parent), 
      m_children(std::map<std::string,Node<T>*>()) {}
  
  // Each Node is responsible for deleting its children
  template <class T>
  Node<T>::~Node(){
    for (auto& entry : m_children){
      delete entry.second;
    }
  }
  
  // METHODS
  // Recursively insert a single keyword into the radix tree
  template <class T>
  std::pair<Node<T>*,bool> Node<T>::Insert(const std::string& keyword, const T& value){
    std::pair<Node<T>*,bool> retval = std::make_pair<Node<T>*,bool>(NULL,false);
    // Go through all child nodes until you find a node for which either
    // - the new keyword and the existing nodekey share a common prefix, or 
    // - the new keyword and the existing nodekey are identical, or
    // - the keyword is a prefix to the existing nodekey, or
    // - the existing nodekey is a prefix to the keyword
    // If you don't find a matching nodekey, then the keyword is completely unique and can be
    // added to this node
    for (auto& entry : m_children){
  
      auto last_match = std::mismatch(keyword.begin(), keyword.end(), entry.first.begin(), entry.first.end());
      
      // Complete mismatch
      if (keyword.begin() == last_match.first){continue;}
  
      // Word and prefix are identical
      if (keyword.end() == last_match.first && entry.first.end() == last_match.second){
        retval.first = entry.second;
        if (!entry.second->IsLeaf()){
          // Convert a node to a leaf
          entry.second->MakeLeaf(value);
          retval.second = true;
        }
        return retval;
      }
      // One string is prefix of the other
      else if (keyword.end() == last_match.first || entry.first.end() == last_match.second){
        // New keyword is prefix of the existing nodekey
        if (keyword.end() == last_match.first){
          // Add a new node with the prefix to the current node
          retval.second = AddChild(keyword, value);
          // Old child node becomes child of the new node
          m_children[keyword]->AddChild(std::string(last_match.second, entry.first.end()), entry.second);
          // Remove old node from current node
          m_children.erase(entry.first);
          retval.first = m_children[keyword];
        }
        else {
          // Insert new keyword as child of its prefix
          retval = entry.second->Insert(std::string(last_match.first, keyword.end()), value);
        }
        return retval;
      }
      // Words partially match but diverge
      // A new node needs to be created whose key is the common suffix of the words.
      // This node has two children, one representing the new keyword, the other representing the
      // old nodekey
      else {
        std::string common_prefix(keyword.begin(), last_match.first);
        
        AddChild(common_prefix);
        
        retval.second = m_children[common_prefix]->AddChild(std::string(last_match.first,keyword.end()), value);
        m_children[common_prefix]->AddChild(std::string(last_match.second,entry.first.end()), entry.second);
        
        m_children.erase(entry.first);
        retval.first = m_children[common_prefix]->m_children[std::string(last_match.first,keyword.end())];
        return retval;
      }
    }
    // If no common prefix has been found then keyword becomes a new prefix
    retval.second = AddChild(keyword, value);
    retval.first = m_children[keyword];
    return retval;
  }

  template <class T>
  const Node<T>* Node<T>::Retrieve(const std::string& key) const {
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
    return NULL;
  }
  
  template <class T>
  void Node<T>::MakeLeaf(const T& value){
    m_value_pair.second = value;
    m_is_leaf = true;
  }
  
  // Adds a new node
  template <class T>
  bool Node<T>::AddChild(const std::string& suffix, const T& value, const bool is_leaf){
    // TODO: Try replacing the check with an assert
    if (!m_children.contains(suffix)){
      std::string fullkey = (m_parent? m_parent->m_value_pair.first : "") + suffix;
      m_children[suffix] = new Node(this, fullkey, value, is_leaf);
      return false;
    }
    return true;
  }
  
  // Adds a new node 
  template <class T>
  void Node<T>::AddChild(const std::string& suffix){
    AddChild(suffix, T(), false);
  }
  
  // Adds an existing node
  template <class T>
  void Node<T>::AddChild(const std::string& suffix, Node* node){
    if (!m_children.contains(suffix)){
      m_children[suffix] = node;
      node->SetParent(this);
    }
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
  void Node<T>::SetParent(Node* node){
    m_parent = node;
  }
  
  template <class T>
  std::pair<std::string,Node<T>*> Node<T>::GetFirstChild() const {
    return std::make_pair(m_children.begin()->first,m_children.begin()->second);
  }
  
  template <class T>
  const std::map<std::string,Node<T>*>& Node<T>::GetChildren() const {
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
