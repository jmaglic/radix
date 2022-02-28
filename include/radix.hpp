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
      Node(Node*, const T&, const bool=false);
      ~Node();
      
      // Display
      void print();
    private:
      // Members
      T m_value;
      bool m_is_leaf;
      Node* m_parent;
      std::map<std::string,Node*> m_children;

      // Container operations
      bool Insert(const std::string&, const T&);
      const T& At(const std::string&) const;

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
      std::string GetKey() const;
      const T& GetValue() const;
      
      // Explicit instantiation for template type
      friend bool operator== <> (const Iterator_impl<T>&, const Iterator_impl<T>&);
      friend bool operator!= <> (const Iterator_impl<T>&, const Iterator_impl<T>&);
    private:
      bool Advance();
      // Value is accessible through node
      Node<T>* m_node;
      std::string m_key;
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
      typedef std::string key_type;
      typedef T mapped_type;
      typedef std::pair<const std::string, mapped_type> value_type;
  
      bool insert(const std::string&, const mapped_type&);
      bool insert(const value_type&);
      std::pair<bool,bool> insert(const std::vector<std::string>&, const mapped_type&);

      mapped_type& at(const key_type&);
      const mapped_type& at(const key_type&) const;

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
    m_root = new detail::Node<T>(NULL, T());
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
      bool success = m_root->Insert(key, value);
      all &= success;
      any |= success;
    }
    return std::make_pair(all,any);
  }
  
  template <class T>
  bool radix<T>::insert(const std::string& key, const T& value){
    return m_root->Insert(key, value);
  }

  template <class T>
  bool radix<T>::insert(const value_type& key_value){ 
    return m_root->Insert(key_value.first, key_value.second);
  }

  template <class T>
  T& radix<T>::at(const std::string& key){
    return const_cast<T&>(std::as_const(*this).at(key));
  }

  template <class T>
  const T& radix<T>::at(const std::string& key) const {
    try{
      return m_root->At(key);
    }
    catch(const std::out_of_range& e){
      throw e;
    }
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
  Iterator_impl<T>::Iterator_impl(Node<T>* ptr, const std::string& key) : m_key(key), m_node(ptr) {}
 
  template <class T>
  Iterator_impl<T>& Iterator_impl<T>::operator++(){
    // Advance iterator until you reach a leaf node
    while (Advance()) {}
    return *this;
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
      m_key += suffix;
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
          m_key += elem.first;
          m_node = elem.second;
          return !m_node->IsLeaf();
        }
        if (prev_child == elem.second){
          prev_child_found = true;
          m_key.erase(m_key.length()-elem.first.length());
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
    return m_key;
  }
  
  template <class T>
  const T& Iterator_impl<T>::GetValue() const {
    return m_node->m_value;
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
  Node<T>::Node(Node* parent, const T& value, const bool is_leaf) 
    : m_is_leaf(is_leaf), m_value(value), m_parent(parent), m_children(std::map<std::string,Node<T>*>()) {}
  
  // Each Node is responsible for deleting its children
  template <class T>
  Node<T>::~Node(){
    for (auto& entry : m_children){
      delete entry.second;
    }
  }
  
  // METHODS
  // Recursively insert a single word into the radix tree
  template <class T>
  bool Node<T>::Insert(const std::string& word, const T& value){
    bool success = false;
    for (auto& entry : m_children){
  
      auto last_match = std::mismatch(word.begin(), word.end(), entry.first.begin(), entry.first.end());
      
      // Complete mismatch
      if (word.begin() == last_match.first){continue;}
  
      // Word and prefix are identical
      if (word.end() == last_match.first && entry.first.end() == last_match.second){
        // If the target node is already a leaf node do not continue
        if (entry.second->IsLeaf()){
          return false;
        }
        else {
          entry.second->MakeLeaf(value);
          return true;
        }
      }
      // Either string is prefix of the other string
      else if (word.end() == last_match.first || entry.first.end() == last_match.second){
        // Inserted word is prefix of the existing key
        if (word.end() == last_match.first){
          // Add a new node with the prefix to the current node
          success = AddChild(word, value);
          // Old child node becomes child of the new node
          m_children[word]->AddChild(std::string(last_match.second, entry.first.end()), entry.second);
          // Remove old node from current node
          m_children.erase(entry.first);
        }
        else {
          // Insert new word as child of its prefix
          success = entry.second->Insert(std::string(last_match.first, word.end()), value);
        }
        return success;
      }
      // Words partially match but diverge
      else {
        std::string common_prefix(word.begin(), last_match.first);
        // Add a new node with the common prefix 
        AddChild(common_prefix);
        // Add new and old node
        success = m_children[common_prefix]->AddChild(std::string(last_match.first,word.end()), value);
        m_children[common_prefix]->AddChild(std::string(last_match.second,entry.first.end()), entry.second);
        // Remove old node from current node's children
        m_children.erase(entry.first);
        return success;
      }
    }
    // If no common prefix has been found then word becomes a new prefix
    success = AddChild(word, value);
    return success;
  }

  template <class T>
  const T& Node<T>::At(const std::string& key) const {
    if (key.empty()){
      return m_value;
    }
    auto begin = key.begin();
    auto end = key.end();
    for (auto pos = begin; pos <= end; ++pos){
      std::string substr(begin,pos);
      if (m_children.contains(substr)){
        return m_children.at(substr)->At(std::string(pos,end));
      }
    }
    throw std::out_of_range("radix::at:  key not found");
  }
  
  template <class T>
  void Node<T>::MakeLeaf(const T& value){
    m_value = value;
    m_is_leaf = true;
  }
  
  // Adds a new node.  
  template <class T>
  bool Node<T>::AddChild(const std::string& word, const T& value, const bool is_leaf){
    bool child_exists = m_children.contains(word);
    if (!child_exists){
      m_children[word] = new Node(this, value, is_leaf);
    }
    return !child_exists;
  }
  
  // Adds a new node.  
  template <class T>
  void Node<T>::AddChild(const std::string& word){
    AddChild(word, T(), false);
  }
  
  // Adds an existing node
  template <class T>
  void Node<T>::AddChild(const std::string& word, Node* node){
    if (!m_children.contains(word)){
      m_children[word] = node;
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
