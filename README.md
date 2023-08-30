![](https://github.com/jmaglic/radix/blob/master/banner.png)

This project implements a [radix tree](https://en.wikipedia.org/wiki/Radix_tree)
written in the modern C++20 standard.

## What is a radix tree?

A radix tree (also known as a "radix trie", "compact prefix tree" or "compressed trie") is a tree data structure
used to search (usually string) keys among a set of keys. Within the tree, each node represents a key while the
edges connecting the nodes represent individual sections of the whole key.

For instance, given two keys `water` and `waste`, each key is represented by a node but because they share a common
prefix `wa` they share a common parent node that represents the string `wa`.

```
                 ┌────► waste
ROOT ─────► wa ──┤
                 └────► water
```

Radix trees are [k-ary](https://en.wikipedia.org/wiki/M-ary_tree) trees because each node can have a maximum
number of children. This number is equal to
the number of allowed characters in the key, for instance, 26 if all keys are lower-case english words.

## Motivation

Radix trees can be a practical way to store and look up (key, value)-pairs 
are useful when implementing an inverted index. Due to their design it is easy to access related keys such as
all keys that share a common prefix. This can be used to autocomplete keys.

My goal with this project was at first to implement a basic template library and a custom iterator using 
iterator concepts introduced in C++20 standard. The scope eventually expanded and this became an attempt to
write a library that feels like it's part of the standard library. Specifically, `xsm::radix` can in many ways
be used exactly like `std::map`.

## Installation

To use this library, simply place the `radix.hpp` file in a directory that you include during compilation. Since
it's a template library, all code is contained in the header file.

## Copyright and License

All C++ code was written by me (Jasmin B. Maglic) and first published in 2023. The code is licensed under version
3 of the GNU General Public License. If you would like to use the code in your non open source project you can
contact me to discuss a commercial license.
