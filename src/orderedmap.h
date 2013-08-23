#ifndef ORDEREDMAP_H
#define ORDEREDMAP_H

#include <QHash>
#include <QLinkedList>
#include <QList>
#include <QPair>

template <typename Key> inline bool oMHashEqualToKey(const Key &key1, const Key &key2)
{
    // Key type must provide '==' operator
    return key1 == key2;
}

template <typename Ptr> inline bool oMHashEqualToKey(Ptr *key1, Ptr *key2)
{
    Q_ASSERT(sizeof(quintptr) == sizeof(Ptr *));
    return quintptr(key1) == quintptr(key2);
}

template <typename Ptr> inline bool oMHashEqualToKey(const Ptr *key1, const Ptr *key2)
{
    Q_ASSERT(sizeof(quintptr) == sizeof(const Ptr *));
    return quintptr(key1) == quintptr(key2);
}

template <typename Key, typename Value>
class OrderedMap
{

    typedef typename QLinkedList<Key>::iterator qllIterator;
    typedef QPair<Value, qllIterator> OMHashValue;

public:
    explicit OrderedMap();

    void clear();

    bool contains(const Key &key) const;

    int count() const;

    bool empty() const;

    void insert(const Key &key, const Value &value);

    bool isEmpty() const;

    std::list<Key> keys() const;

    int remove(const Key &key);

    int size() const;

#if QT_VERSION >= 0x040800
    void swap(OrderedMap<Key, Value> &other);
#endif

    Value take(const Key &key);

    Value value(const Key &key) const;

    Value value(const Key &key, const Value &defaultValue) const;

    QList<Value> values() const;

    OrderedMap<Key, Value> & operator=(OrderedMap<Key, Value> other);

    bool operator==(const OrderedMap<Key, Value> &other) const;

    bool operator!=(const OrderedMap<Key, Value> &other) const;

    Value& operator[](const Key &key);

    const Value operator[](const Key &key) const;

private:

    class OMHash : public QHash<Key, OMHashValue >
    {
    public:
        bool operator == (const OMHash &other) const
        {
            if (size() != other.size()) {
                return false;
            }

            if (QHash<Key, OMHashValue >::operator ==(other)) {
                return true;
            }

            typename QHash<Key, OMHashValue >::const_iterator it1 = this->constBegin();
            typename QHash<Key, OMHashValue >::const_iterator it2 = other.constBegin();

            while(it1 != this->end()) {
                OMHashValue v1 = it1.value();
                OMHashValue v2 = it2.value();

                if ((v1.first != v2.first) || !oMHashEqualToKey<Key>(it1.key(), it2.key())) {
                    return false;
                }
                ++it1;
                ++it2;
            }
            return true;
        }
    };

private:

    OMHash data;
    QLinkedList<Key> insertOrder;
};

template <typename Key, typename Value>
OrderedMap<Key, Value>::OrderedMap() {}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::clear()
{
    data.clear();
    insertOrder.clear();
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::contains(const Key &key) const
{
    return data.contains(key);
}

template <typename Key, typename Value>
int OrderedMap<Key, Value>::count() const
{
    return data.count();
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::empty() const
{
    return data.empty();
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::insert(const Key &key, const Value &value)
{
    typename OMHash::iterator it = data.find(key);
    if (it == data.end()) {
        // New key
        qllIterator ioIter = insertOrder.insert(insertOrder.end(), key);
        OMHashValue pair(value, ioIter);
        data.insert(key, pair);
        return;
    }

    OMHashValue pair = it.value();
    // remove old reference
    insertOrder.erase(pair.second);
    // Add new reference
    qllIterator ioIter = insertOrder.insert(insertOrder.end(), key);
    pair.first = value;
    pair.second = ioIter;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::isEmpty() const
{
    return data.isEmpty();
}

template<typename Key, typename Value>
std::list<Key> OrderedMap<Key, Value>::keys() const
{
    return insertOrder.toStdList();
}

template<typename Key, typename Value>
int OrderedMap<Key, Value>::remove(const Key &key)
{
    typename OMHash::iterator it = data.find(key);
    if (it == data.end()) {
        return 0;
    }
    OMHashValue pair = it.value();
    insertOrder.erase(pair.second);
    data.erase(it);
    return 1;
}

template<typename Key, typename Value>
int OrderedMap<Key, Value>::size() const
{
    return data.size();
}

#if QT_VERSION >= 0x040800
template<typename Key, typename Value>
void OrderedMap<Key, Value>::swap(OrderedMap<Key, Value> &other)
{
    // Swap individual components
    data.swap(other.data);
    insertOrder.swap(other.insertOrder);
}
#endif

template<typename Key, typename Value>
Value OrderedMap<Key, Value>::take(const Key &key)
{
    typename OMHash::iterator it = data.find(key);
    if (it == data.end()) {
        return Value();
    }
    OMHashValue pair = it.value();
    insertOrder.erase(pair.second);
    data.erase(it);
    return pair.first;
}

template <typename Key, typename Value>
Value OrderedMap<Key, Value>::value(const Key &key) const
{
    return data.value(key).first;
}

template <typename Key, typename Value>
Value OrderedMap<Key, Value>::value(const Key &key, const Value &defaultValue) const
{
    typename OMHash::const_iterator it = data.constFind(key);
    if (it == data.end()) {
        return defaultValue;
    }
    OMHashValue pair = it.value();
    return pair.first;
}

template <typename Key, typename Value>
QList<Value> OrderedMap<Key, Value>::values() const
{
    QList<Value> values;
    foreach (const Key &key, insertOrder.toStdList()) {
        OMHashValue v = data.value(key);
        values.append(v.first);
    }
    return values;
}

template <typename Key, typename Value>
OrderedMap<Key, Value> & OrderedMap<Key, Value>::operator=(OrderedMap<Key, Value> other)
{
#if QT_VERSION >= 0x040800
    swap(other);
#else
    data = other.data;
    insertOrder = other.insertOrder;
#endif
    return *this;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::operator==(const OrderedMap<Key, Value> &other) const
{
    // 2 Ordered maps are equal if they have the same contents in the same order
    return ((data == other.data) && (insertOrder == other.insertOrder));
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::operator!=(const OrderedMap<Key, Value> &other) const
{
    return ((data != other.data) || (insertOrder != other.insertOrder));
}

template <typename Key, typename Value>
Value& OrderedMap<Key, Value>::operator[](const Key &key)
{
    typename OMHash::iterator it = data.find(key);
    if (it == data.end()) {
        insert(key, Value());
        it = data.find(key);
    }
    OMHashValue &pair = it.value();
    return pair.first;
}

template <typename Key, typename Value>
const Value OrderedMap<Key, Value>::operator[](const Key &key) const
{
    return value(key);
}

#endif // ORDEREDMAP_H
