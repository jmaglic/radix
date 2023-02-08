
# Node_handle

```c++
namespace xsm::detail {
  template <class T, class Compare>
  class Node_handle;
}
```

Node_handle declares a friend relation with radix.

```c++
friend radix<T,Compare>;
```

## Public interface

Node_handle provides the user a safe way to interact with a Node pointer. Node_handle is accessible
to the user as the typedef xsm::radix::node_type. Node_handle objects are provided upon calling
xsm::radix::extract() and are retrieved when the user calls xsm::radix::insert(). In addition, the
user may construct empty Node_handle objects.

For now, Node_handle is only used as a pointer handle for the user. Pointer operations are managed
by the Node class.

Node_handle uses some of radix's typedefs. 

```c++
using key_type = typename radix<T,Compare>::key_type;
using mapped_type = typename radix<T,Compare>::mapped_type;
using value_type = typename radix<T,Compare>::value_type;
```

Node_handle provides a default constructor that initialises its pointer with `nullptr`. In addtion,
Node_handle provides a move constructor and a move assignment operator. The copy constructor is
implicitly deleted. Therefore, once the user obtains a Node_handle, its pointer is unique and another
object containing the same pointer cannot be created.

```c++
constexpr Node_handle() noexcept = default;
Node_handle(Node_handle&&) noexcept;
Node_handle& operator=(Node_handle&&);
~Node_handle();
```

```c++
[[nodiscard]] bool empty() const noexcept;
explicit operator bool() const noexcept;
```

```c++
value_type& value() const; 
key_type& key() const;
mapped_type& mapped() const;
```

## Private interface

```c++
Node<T,Compare>* m_node_ptr = nullptr;

Node_handle(Node<T,Compare>*);

size_t CountChildren() const;
```
