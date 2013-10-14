#include <QString>
#include <QDebug>

#include "lrucache.h"

int main(int argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    LruCache<int, QString> lru = LruCache<int, QString>(5);

    lru.insert(1, "one");
    lru.insert(2, "two");
    lru.insert(3, "three");
    lru.insert(4, "four");
    lru.insert(5, "five");

    // Cache should have 5 elements
    qDebug() << "Size" << lru.size();
    qDebug() << "Initial key order...";
    {
        QDebug deb = qDebug();
        foreach (int key, lru.keys()) {
            deb << key;
        }
    }

    // Lookup an element and refresh its access
    qDebug() << "Lookup Test. Value for 4 is" << lru.value(4);

    // Add an element exceeding capacity. 1 should get evicted
    qDebug() << "Adding 6 to cache. 1 should get evicted...";
    lru.insert(6, "six");
    qDebug() << "Size after eviction" << lru.size();
    qDebug() << "LRU cache contains 1?" << lru.contains(1);

    // Add another element, 2 should get evicted
    qDebug() << "Adding 7 to cache. 2 should get evicted...";
    lru.insert(7, "seven");
    qDebug() << "Size after eviction" << lru.size();
    qDebug() << "LRU cache contains 2?" << lru.contains(2);

    qDebug() << "Printing all keys...";
    {
        QDebug deb = qDebug();
        foreach (int key, lru.keys()) {
            deb << key;
        }
    }

    // Refresh an element
    lru.insert(3, "three");
    qDebug() << "Printing after refreshing 3...";
    {
        QDebug deb = qDebug();
        foreach (int key, lru.keys()) {
            deb << key;
        }
    }

    // Refresh an element
    lru.value(6);
    qDebug() << "Printing after refreshing 6...";
    {
        QDebug deb = qDebug();
        foreach (int key, lru.keys()) {
            deb << key;
        }
    }

    // Reduce capacity
    lru.setCapacity(3);

    qDebug() << "Printing after reducing capacity to 3...";
    {
        QDebug deb = qDebug();
        foreach (int key, lru.keys()) {
            deb << key;
        }
    }
}
