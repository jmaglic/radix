
# Nodes\_handle

```c++
namespace xsm::detail {
  template <class T, class Compare>
  class Nodes\_handle;
}
```

Nodes\_handle declares a friend relation with radix.

```c++
friend radix<T,Compare>;
```

## Public interface

Nodes\_handle provides the user a safe way to interact with a Node pointer. Nodes\_handle is accessible
to the user as the typedef xsm::radix::node\_type. Nodes\_handle objects are provided upon calling
xsm::radix::extract and are retrieved when the user calls xsm::radix::insert. In addition, the
user may construct empty Nodes\_handle objects.

For now, Nodes\_handle is only used as a pointer handle for the user. Pointer operations are managed
by the Node class.

Nodes\_handle uses some of radix's typedefs. 

```c++
using key_type = typename radix<T,Compare>::key_type;
using mapped_type = typename radix<T,Compare>::mapped_type;
using value_type = typename radix<T,Compare>::value_type;
```

Nodes\_handle provides a default constructor that initialises its pointer with `nullptr`. In addtion,
Nodes\_handle provides a move constructor and a move assignment operator. The copy constructor is
implicitly deleted. Therefore, once the user obtains a Nodes\_handle, its pointer is unique and another
object containing the same pointer cannot be created.

```c++
constexpr Nodes\_handle() noexcept = default;
Nodes\_handle(Nodes\_handle&&) noexcept;
Nodes\_handle& operator=(Nodes\_handle&&);
~Nodes\_handle();
```

```c++
[[nodiscard]] bool empty() const noexcept;
explicit operator bool() const noexcept;
```

In std::map, std::map::node\_type::key returns a non-const reference. Thereby, std::map::extract allows
manipulating the key of a mapped object without moving or reallocating the mapped object.
However, the underlying type for storing the value pair is std::pair<const key_type,mapped_type>,
i.e. the key is immutable. Changing the key seems impossible without introducing undefined behaviour.
Therefore Nodes\_handle does not allow changing the key.

```c++
const key_type& key() const;
```

```c++
mapped_type& mapped() const;
```

## Private interface

```c++
Node<T,Compare>* m_node_ptr = nullptr;
```

Node\_handle's private interface is purposefully limited to avoid overloading the class' purpose. As it is
written now, it serves entirely as a return type for xsm::radix::extract and not as a general purpose node
handle. It provides a private constructor that can be used internally to construct a handle from a node 
pointer.

```c++
Nodes\_handle(Node<T,Compare>*);
```
