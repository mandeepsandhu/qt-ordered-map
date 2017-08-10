# Qt Ordered Map

[![Github Issues](https://img.shields.io/github/issues/mandeepsandhu/qt-ordered-map.svg)](https://github.com/mandeepsandhu/qt-ordered-map/issues)
[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/mandeepsandhu/qt-ordered-map.svg)](http://isitmaintained.com/project/mandeepsandhu/qt-ordered-map
"Average time to resolve an issue")

A **Qt** based implementation of an *ordered map*.

An ordered map stores key-value pairs according to the *insertion order* of the keys. It supports a sub-set of the **QMap** API, as certain APIs like <code>QMap::insertMulti()</code> etc do not make sense for an ordered map.

<code>OrderedMap</code> stores keys according to their insertion order, so if you add a key that already exists, it's order is changed to being the last key in the map. Eg:

```C++
#include "orderedmap.h"
...
OrderedMap<int, int> om;
om.insert(1,1);
om.insert(2,2);
// Order of keys is [1, 2]

om.insert(1,1);
// Order of keys is [2, 1]
```

Requirements
============
- The key type for the <code>OrderedMap</code> **must** provide <code>operator==()</code> and a global hash function called <code>qHash()</code>.

Limitations
===========
- OrderedMap currently does NOT support implicit sharing like other Qt containers. A deep-copy will be made whenever copying the container with either copy constructor or assignment operator.

Performance
===========
<code>OrderedMap</code> uses a hashtable (<code>QHash</code>) for storing the data and a map (<code>QLinkedList</code>) for storing the order of the keys.

<table border=2 cellspacing="2" cellpadding="5%">
<tr>
    <th rowspan=2></th>
    <th colspan=2>Key Lookup</th>
    <th colspan=2>Insertion</th>
    <th colspan=2>Removal</th>
</tr>
<tr>
    <th>Average</th>
    <th>Worst Case</th>
    <th>Average</th>
    <th>Worst Case</th>
    <th>Average</th>
    <th>Worst Case</th>
</tr>
<tr>
    <td align="center">OrderedMap</td>
    <td align="center">Amortized <b>O(1)<b></td>
    <td align="center"><b>O(n)</b></td>
    <td align="center">Amortized <b>O(1)</b></td>
    <td align="center"><b>O(n)</b></td>
    <td align="center">Amortized <b>O(1)</b></td>
    <td align="center"><b>O(n)</b></td>
</tr>
</table>

