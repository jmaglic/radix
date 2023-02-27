
# Node\_handle

```c++
namespace xsm::detail {
  template <class T, class Compare>
  class Node_handle;
}
```

Node\_handle declares a friend relation with radix. This grants radix access to the private
constructor.

```c++
friend radix<T,Compare>;
```

## Public interface

Node\_handle provides the user a safe way to interact with a Node pointer. Node\_handle is accessible
to the user as the typedef xsm::radix::node\_type. Node\_handle objects are provided upon calling
xsm::radix::extract and are retrieved when the user calls xsm::radix::insert. In addition, the
user may construct empty Node\_handle objects.

For now, Node\_handle is only used as a pointer handle for the user. Pointer operations are managed
by the Node class.

Node\_handle uses some of radix's typedefs. 

```c++
using key_type = typename radix<T,Compare>::key_type;
using mapped_type = typename radix<T,Compare>::mapped_type;
using value_type = typename radix<T,Compare>::value_type;
```

Node\_handle provides a default constructor that initialises its pointer with `nullptr`. In addtion,
Node\_handle provides a move constructor and a move assignment operator. The copy constructor is
implicitly deleted. Therefore, once the user obtains a Node\_handle, its pointer is unique and another
object containing the same pointer cannot be created.

Node\_handle is furthermore responsible for deleting isolated Nodes, i.e. Nodes without parent or children.
When the live-time of the Node handle runs out, the destructor is called if the managed Node is isolated.

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

In std::map, std::map::node\_type::key returns a non-const reference. Thereby, std::map::extract allows
manipulating the key of a mapped object without moving or reallocating the mapped object.
However, the underlying type for storing the value pair is std::pair<const key_type,mapped_type>,
i.e. the key is immutable. Changing the key seems impossible without introducing undefined behaviour.
Therefore Node\_handle does not allow changing the key.

```c++
const key_type& key() const;
```

```c++
mapped_type& mapped() const;
```

## Private interface

Private typedefs.

Node\_handle's private interface is purposefully limited to avoid overloading the class' purpose. As it is
written now, it serves entirely as a return type for xsm::radix::extract and not as a general purpose node
handle. It provides a private constructor that can be used internally to construct a handle from a node 
pointer.

```c++
using child_map = typename Node<T,Compare>::child_map;
using node_type = typename radix<T,Compare>::node_type;
```

```c++
node_ptr m_node_ptr = nullptr;
```

```c++
Node_handle(Node<T,Compare>*);
```

