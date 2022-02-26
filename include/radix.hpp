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

namespace xsm::detail{
  // Forward declaraion for friend relation
  template <class T> class Iterator_impl;
  // Node represents a key-value pair but the key is not stored explicitly
  template <class T> class Node{
    friend xsm::detail::Iterator_impl<T>;
    public:
      Node(Node*, const T&, const bool=false);
      ~Node();
     
      // Insert
      void insert(const std::string&, const T&);
      // TODO: At(), Erase(), Find()
      
      // Display
      void print();
    private:
      T m_value;
      bool m_is_leaf;
      Node* m_parent;
      std::map<std::string,Node*> m_children;
      // For iterator
      bool IsChildless() const;
      bool IsLeaf() const;
      Node* GetParent() const;
      void SetParent(Node*);
      std::pair<std::string,Node*> GetFirstChild() const;
      const std::map<std::string,Node*>& GetChildren() const;
  
      // Methods used during container manipulation
      void SetEnd(bool);
      void AddChild(const std::string&, const T&, const bool=false);
      void AddChild(const std::string&, Node*);
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
  template <class T>
  class radix{
    public:
      radix();
      ~radix();

      // Typedef allows iterator class to be accessed through xsm::radix::iterator
      typedef detail::Iterator_impl<T> iterator;
  
      void insert(const std::string&, const T&);
      void insert(const std::pair<std::string,T>&);
      void insert(const std::vector<std::string>&, const T&);

      iterator begin();
      iterator end();
      void print();

    private:
      // Pointer to root node of radix tree
      detail::Node<T>* m_root; 
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
  void radix<T>::insert(const std::vector<std::string>& key_list, const T& value){
    for (const std::string& key : key_list){
      m_root->insert(key, value);
    }
  }
  
  template <class T>
  void radix<T>::insert(const std::string& key, const T& value){
    m_root->insert(key, value);
  }

  template <class T>
  void radix<T>::insert(const std::pair<std::string,T>& key_value){ 
    m_root->insert(key_value.first, key_value.second);
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
    Node<T>* prev_child = NULL; // TODO: make this a unique ptr
    /*
    auto GoToParent = [&, prev_child]() mutable {
      // Go up one node
      prev_child = m_node;
      m_node = m_node->GetParent();
    };*/
    
    // If node has children, go to first child in sequence
    if (!m_node->IsChildless()){
      std::string suffix;
      std::tie(suffix, m_node) = m_node->GetFirstChild();
      m_key += suffix;
      return !m_node->IsLeaf();
    }
    // If node has no children, then go up to parent and remember child
    else {
      //GoToParent(_node, prev_child);
      //GoToParent();
      prev_child = m_node;
      m_node = m_node->GetParent();
    }
    
    while (m_node != NULL){
      // Search for the previous child
      bool prev_child_found = false;
    
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
      //GoToParent();
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
  
  // Recursively insert a single word into the radix tree
  template <class T>
  void Node<T>::insert(const std::string& word, const T& value){
    for (auto& entry : m_children){
  
      auto last_match = std::mismatch(word.begin(), word.end(), entry.first.begin(), entry.first.end());
      
      // Complete mismatch
      if (word.begin() == last_match.first){continue;}
  
      // Word and prefix are identical
      if (word.end() == last_match.first && entry.first.end() == last_match.second){
        entry.second->SetEnd(true);
        return;
      }
      // Either string is prefix of the other string
      else if (word.end() == last_match.first || entry.first.end() == last_match.second){
        if (word.end() == last_match.first){
          AddChild(word, value, true);
          m_children[word]->AddChild(std::string(last_match.second, entry.first.end()), entry.second);
  
          m_children.erase(entry.first);
        }
        else {
          entry.second->insert(std::string(last_match.first, word.end()), value);
        }
        return;
      }
      // Words partially match but diverge
      else {
        std::string common_prefix(word.begin(), last_match.first);
        
        AddChild(common_prefix, value);
        m_children[common_prefix]->AddChild(std::string(last_match.first,word.end()), value, true);
        m_children[common_prefix]->AddChild(std::string(last_match.second,entry.first.end()), entry.second);
  
        m_children.erase(entry.first);
        return;
      }
    }
    // If no common prefix has been found then word becomes a new prefix
    AddChild(word, value, true);
  }
  
  template <class T>
  void Node<T>::SetEnd(bool end){
    m_is_leaf = end;
  }
  
  // Adds a new node
  template <class T>
  void Node<T>::AddChild(const std::string& word, const T& value, const bool is_leaf){
    if (!m_children.contains(word)){
      m_children[word] = new Node(this, value, is_leaf);
    }
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
