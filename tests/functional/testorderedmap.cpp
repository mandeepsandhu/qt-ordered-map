#include <QtTest/QtTest>
#include <QString>

#include "orderedmap.h"

class TestOrderedMap: public QObject
{
    Q_OBJECT

private slots:

    void containsTest();
    void clearSizeCountTest();
    void emptyAndIsEmptyTest();
    void removeTest();
    void orderTest();
#if QT_VERSION >= 0x040800
    void swapTest();
#endif
    void takeTest();
    void valueTest();
    void valuesTest();
    void opAssignTest();
    void opEqualityTest();
    void opInequalityTest();
    void opSqrBracesTest();
};

void TestOrderedMap::containsTest()
{
    OrderedMap<int, QString> om;
    // Add 3 elements
    om.insert(0,QString("0"));
    om.insert(1,QString("1"));
    om.insert(2,QString("2"));

    QVERIFY(om.contains(0));
    QVERIFY(!om.contains(4));
}

void TestOrderedMap::clearSizeCountTest()
{
    OrderedMap<int, int> om;
    // Add 3 elements
    om.insert(0,0);
    om.insert(1,1);
    om.insert(2,2);
    QVERIFY(om.size() == 3);
    QVERIFY(om.count() == 3);

    // Remove 2 elements
    om.remove(2);
    om.remove(0);
    QVERIFY(om.size() == 1);
    QVERIFY(om.count() == 1);

    // Remove All elements
    om.clear();
    QVERIFY(om.size() == 0);
    QVERIFY(om.count() == 0);
}

void TestOrderedMap::emptyAndIsEmptyTest()
{
    OrderedMap<int, int> om;
    QVERIFY(om.empty());
    QVERIFY(om.isEmpty());

    om.insert(0,0);
    om.insert(1,1);
    om.insert(2,2);

    QVERIFY(!om.empty());
    QVERIFY(!om.isEmpty());

    om.clear();
    QVERIFY(om.empty());
    QVERIFY(om.isEmpty());
}

void TestOrderedMap::removeTest()
{
    OrderedMap<QString, int> om;
    om.insert(QString("1"),1);
    om.insert(QString("2"),2);
    om.insert(QString("3"),3);

    int result = om.remove(QString("2"));

    // Only 1 item should be removed
    QVERIFY(result == 1);
    QVERIFY(!om.contains(QString("2")));

    result = om.remove(QString("4"));
    // No item should be removed
    QVERIFY(result == 0);
}

void TestOrderedMap::orderTest()
{
    OrderedMap<int, int> om, om2;
    om.insert(2,2);
    om.insert(1,1);
    om.insert(0,0);
    om.remove(1);
    om.insert(3,3);
    om.insert(1,1);
    om.remove(3);
    om.remove(2);
    om.insert(5,5);
    om.remove(1);
    om.insert(6,6);

    int ans[] = {0, 5, 6};
    int i = 0;
    foreach (int k, om.keys()) {
        QVERIFY(k == ans[i++]);
    }

    om2.insert(1,1);
    om2.insert(2,2);
    om2.insert(1,1);

    int ans2[] = {2, 1};
    i = 0;
    foreach (int k, om2.keys()) {
        QVERIFY(k == ans2[i++]);
    }
}

#if QT_VERSION >= 0x040800
void TestOrderedMap::swapTest()
{
    OrderedMap<int, int> om1, om2;
    om1.insert(1,1);
    om1.insert(2,2);

    om2.insert(3,3);
    om2.insert(4,4);
    om2.insert(5,5);

    om1.swap(om2);

    QVERIFY(om1.size() == 3);
    QVERIFY(om2.size() == 2);

    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(4) == 4);
    QVERIFY(om1.value(5) == 5);
    QVERIFY(om2.value(1) == 1);
    QVERIFY(om2.value(2) == 2);
}
#endif

void TestOrderedMap::takeTest()
{
    OrderedMap<int, int> om;
    om.insert(1,1);
    om.insert(2,2);
    om.insert(3,3);

    int result = om.take(1);
    QVERIFY(result == 1);
    QVERIFY(om.size() == 2);
}

void TestOrderedMap::valueTest()
{
    OrderedMap<int, int> om;
    // Add 3 elements
    om.insert(0,0);
    om.insert(1,1);
    om.insert(2,2);

    QVERIFY(om.value(1) == 1);
    QVERIFY(om.value(0) == 0);
    QVERIFY(om.value(2) == 2);
}

void TestOrderedMap::valuesTest()
{
    OrderedMap<int, int> om;
    om.insert(6,6);
    om.insert(1,1);
    om.insert(0,0);
    om.remove(1);
    om.insert(3,3);
    om.insert(1,1);
    om.remove(0);
    om.insert(5,5);
    om.remove(6);
    om.insert(6,6);

    int ans[] = {3, 1, 5, 6};
    int i = 0;
    foreach (int v, om.values()) {
        QVERIFY(v == ans[i++]);
    }
}

void TestOrderedMap::opAssignTest()
{
    OrderedMap<int, int> om1, om2;
    om1.insert(3,3);
    om1.insert(2,2);
    om1.insert(1,1);

    om2 = om1;
    QVERIFY(om2.size() == 3);
    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(2) == 2);
    QVERIFY(om1.value(1) == 1);
}

void TestOrderedMap::opEqualityTest()
{
    OrderedMap<int, int> om1, om2, om3;
    om1.insert(3,3);
    om1.insert(2,2);
    om1.insert(1,1);

    om2.insert(3,3);
    om2.insert(2,2);
    om2.insert(1,1);

    QVERIFY(om1 == om2);

    om3.insert(3,3);
    om3.insert(1,1);
    om3.insert(2,2);
    om3.remove(1);
    om3.insert(1,1);

    QVERIFY(om1 == om3);
}

void TestOrderedMap::opInequalityTest()
{
    OrderedMap<int, int> om1, om2;
    om1.insert(3,3);
    om1.insert(2,2);
    om1.insert(1,1);

    om2.insert(1,1);
    om2.insert(2,2);
    om2.insert(3,3);

    QVERIFY(om1 != om2);
}

void TestOrderedMap::opSqrBracesTest()
{
    OrderedMap<QString, QString> om;

    om.insert(QString("1"),QString("1"));
    om.insert(QString("2"),QString("2"));
    om.insert(QString("3"),QString("3"));

    const QString &result = om[QString("2")];

    QVERIFY(result == QString("2"));

    QString &modResult = om[QString("3")];
    modResult.append("3");

    QVERIFY(om.value(QString("3")) == QString("33"));

    const QString &invalidResult = om[QString("4")];

    QVERIFY(invalidResult.isEmpty());
    QVERIFY(om.size() == 4);
}

QTEST_MAIN(TestOrderedMap)

#include "testorderedmap.moc"
