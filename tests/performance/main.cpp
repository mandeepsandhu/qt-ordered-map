#include <QMap>
#include <QHash>
#include <QLinkedList>
#include <QString>
#include <QTime>
#include <QDebug>

#include "orderedmap.h"

int main(int argc, char **argv)
{
    int itemCount = 0;
    bool ok = false;

    if (argc > 1) {
        itemCount = QString(argv[1]).toInt(&ok);
        if (!ok) {
            qWarning() << "\nError! Enter a valid number.";
            qDebug() << "\nUsage:\n\t" << *argv << "<number of items for testing>\n";
            return 1;
        }
    } else {
        qDebug() << "\nUsage:\n\t" << *argv << "<number of items for testing>\n";
        return 1;
    }

    QMap<int, QString> map;
    QHash<int, QString> hash;
    QLinkedList<QString> linkList;

    OrderedMap<int, QString> om;

    QTime timer;

    qDebug() << "Timing insertion of" << itemCount << "items...\n";

    timer.start();
    for (int i=0; i<itemCount; i++) {
        map.insert(i, QString::number(i));
    }
    qDebug() << "Map :" << timer.elapsed() << "msecs";

    timer.start();
    for (int i=0; i<itemCount; i++) {
        hash.insert(i, QString::number(i));
    }
    qDebug() << "Hash :" << timer.elapsed() << "msecs";

    timer.start();
    for (int i=0; i<itemCount; i++) {
        linkList.append(QString::number(i));
    }
    qDebug() << "Link list :" << timer.elapsed() << "msecs";

    timer.start();
    for (int i=0; i<itemCount; i++) {
        om.insert(i, QString::number(i));
    }
    qDebug() << "Ordered map :" << timer.elapsed() << "msecs";
    qDebug() << "\n";

    qDebug() << "Timing iteration over" << itemCount << "items...\n";

    int dummy = 0;
    timer.start();
    foreach (const QString& val, map.values()) {
        dummy += val.size();
    }
    qDebug() << "Map :" << timer.elapsed() << "msecs";

    dummy = 0;
    timer.start();
    foreach (const QString& val, hash.values()) {
        dummy += val.size();
    }
    qDebug() << "Hash :" << timer.elapsed() << "msecs";

    dummy = 0;
    timer.start();
    foreach (const QString& val, linkList) {
        dummy += val.size();
    }
    qDebug() << "Linked list :" << timer.elapsed() << "msecs";

    dummy = 0;
    timer.start();
    foreach (const QString& val, om.values()) {
        dummy += val.size();
    }
    qDebug() << "Ordered map :" << timer.elapsed() << "msecs";
    qDebug() << "\n";

    qDebug() << "Timing removal of random item from" << itemCount << "items...\n";

    int rand = qrand() % itemCount;
    timer.start();
    map.remove(rand);
    qDebug() << "Map :" << timer.elapsed() << "msecs";

    rand = qrand() % itemCount;
    timer.start();
    hash.remove(rand);
    qDebug() << "Hash :" << timer.elapsed() << "msecs";

    rand = qrand() % itemCount;
    timer.start();
    linkList.removeOne(QString::number(rand));
    qDebug() << "Link list :" << timer.elapsed() << "msecs";

    rand = qrand() % itemCount;
    timer.start();
    om.remove(rand);
    qDebug() << "Ordered map :" << timer.elapsed() << "msecs";
    qDebug() << "\n";

    return 0;
}
