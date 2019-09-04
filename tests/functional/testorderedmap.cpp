#include <QtTest/QtTest>
#include <QString>
#include <QDebug>

#include "orderedmap.h"

class TestOrderedMap: public QObject
{
    Q_OBJECT

private slots:

#ifdef Q_COMPILER_INITIALIZER_LISTS
    void initializerListCtorTest();
#endif
    void containsTest();
    void clearSizeCountTest();
    void emptyAndIsEmptyTest();
    void removeTest();
    void orderTest();
    void takeTest();
    void valueTest();
    void valuesTest();
    void singleKeyMultipleValuesTest();
    void copyConstructorTest();
    void opAssignTest();
#if (QT_VERSION >= 0x050200)
    void moveConstructorTest();
    void opMoveAssignTest();
#endif
    void opEqualityTest();
    void opInequalityTest();
    void opSqrBracesTest();

    // Iterator tests
    void insertTest();
    void beginTest();
    void endTest();
    void findTest();
    void eraseTest();
    void iterationOrderTest();
    void foreachTest();
    void iteratorOperatorsTest();
};

#ifdef Q_COMPILER_INITIALIZER_LISTS
void TestOrderedMap::initializerListCtorTest()
{
    OrderedMap<int, QString> om = {std::make_pair<int, QString>(0, QString("0")),
                                   std::make_pair<int, QString>(1, QString("1"))};
    QVERIFY(om.contains(0));
    QVERIFY(om.contains(1));
    QVERIFY(om.value(0) == QString("0"));
    QVERIFY(om.value(1) == QString("1"));

}
#endif

void TestOrderedMap::containsTest()
{
    OrderedMap<int, QString> om;
    om.insert(0,QString("0"));
    om.insert(1,QString("1"));
    om.insert(2,QString("2"));

    QVERIFY(om.contains(0));
    QVERIFY(!om.contains(4));
}

void TestOrderedMap::clearSizeCountTest()
{
    OrderedMap<int, int> om;
    om.insert(0,0);
    om.insert(1,1);
    om.insert(2,2);

    QVERIFY(om.size() == 3);
    QVERIFY(om.count() == 3);

    om.remove(2);
    om.remove(0);

    QVERIFY(om.size() == 1);
    QVERIFY(om.count() == 1);

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

    QVERIFY(result == 1);
    QVERIFY(!om.contains(QString("2")));

    result = om.remove(QString("4"));

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
    om.insert(0,0);
    om.insert(1,1);
    om.insert(2,2);

    QVERIFY(om.value(1) == 1);
    QVERIFY(om.value(0) == 0);
    QVERIFY(om.value(2) == 2);

    // overwrite with different values
    om.insert(0, 10);
    om.insert(2, 20);

    QVERIFY(om.value(1) == 1);
    QVERIFY(om.value(0) == 10);
    QVERIFY(om.value(2) == 20);
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

void TestOrderedMap::singleKeyMultipleValuesTest()
{
    OrderedMap<int, int> om;
    for (int i=0; i<10; i++) {
        om.insert(1, i);
    }
    // only last value is retained
    QVERIFY(om.value(1) == 9);
}

void TestOrderedMap::copyConstructorTest()
{
    OrderedMap<int, int> om1;
    om1.insert(3,3);
    om1.insert(2,2);
    om1.insert(1,1);

    OrderedMap<int, int> om2(om1);

    QVERIFY(om1.size() == 3);
    QVERIFY(om1.keys().size() == 3);
    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(2) == 2);
    QVERIFY(om1.value(1) == 1);

    QVERIFY(om2.size() == 3);
    QVERIFY(om2.keys().size() == 3);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 2);
    QVERIFY(om2.value(1) == 1);

    om2.remove(2);
    QVERIFY(om2.size() == 2);
    QVERIFY(om2.keys().size() == 2);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 0); // default constructed value
    QVERIFY(om2.value(1) == 1);
    QVERIFY(om1.size() == 3);
    QVERIFY(om1.keys().size() == 3);
    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(2) == 2);
    QVERIFY(om1.value(1) == 1);

    om2.remove(1);
    QVERIFY(om2.size() == 1);
    QVERIFY(om2.keys().size() == 1);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 0); // default constructed value
    QVERIFY(om2.value(1) == 0); // default constructed value
    QVERIFY(om1.size() == 3);
    QVERIFY(om1.keys().size() == 3);
    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(2) == 2);
    QVERIFY(om1.value(1) == 1);

    om1.remove(3);
    QVERIFY(om1.size() == 2);
    QVERIFY(om1.keys().size() == 2);
    QVERIFY(om1.value(3) == 0);
    QVERIFY(om1.value(2) == 2); // default constructed value
    QVERIFY(om1.value(1) == 1); // default constructed value
    QVERIFY(om2.size() == 1);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.keys().size() == 1);

    om1.insert(4,4);
    om2.insert(5,5);

    int om1_ans[] = { 2, 1, 4};
    int om2_ans[] = { 3, 5};

    int i = 0;
    foreach (int v, om1.values()) {
        QVERIFY(v == om1_ans[i++]);
    }
    i = 0;
    foreach (int v, om2.values()) {
        QVERIFY(v == om2_ans[i++]);
    }
}

#if (QT_VERSION >= 0x050200)
OrderedMap<int, int> genRValue()
{
    OrderedMap<int, int> om1;
    om1.insert(3,3);
    om1.insert(2,2);
    om1.insert(1,1);
    return om1;
}

void TestOrderedMap::moveConstructorTest()
{
    OrderedMap<int, int> om2(genRValue());

    QVERIFY(om2.size() == 3);
    QVERIFY(om2.keys().size() == 3);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 2);
    QVERIFY(om2.value(1) == 1);

    om2.remove(2);
    QVERIFY(om2.size() == 2);
    QVERIFY(om2.keys().size() == 2);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 0); // default constructed value
    QVERIFY(om2.value(1) == 1);
}

void TestOrderedMap::opMoveAssignTest()
{
    OrderedMap<int, int> om2;
    om2 = genRValue();

    QVERIFY(om2.size() == 3);
    QVERIFY(om2.keys().size() == 3);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 2);
    QVERIFY(om2.value(1) == 1);

    om2.remove(2);
    QVERIFY(om2.size() == 2);
    QVERIFY(om2.keys().size() == 2);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 0); // default constructed value
    QVERIFY(om2.value(1) == 1);
}
#endif

void TestOrderedMap::opAssignTest()
{
    OrderedMap<int, int> om1, om2;
    om1.insert(3,3);
    om1.insert(2,2);
    om1.insert(1,1);

    om2 = om1;

    QVERIFY(om1.size() == 3);
    QVERIFY(om1.keys().size() == 3);
    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(2) == 2);
    QVERIFY(om1.value(1) == 1);

    QVERIFY(om2.size() == 3);
    QVERIFY(om2.keys().size() == 3);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 2);
    QVERIFY(om2.value(1) == 1);

    om2.remove(2);
    QVERIFY(om2.size() == 2);
    QVERIFY(om2.keys().size() == 2);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 0); // default constructed value
    QVERIFY(om2.value(1) == 1);
    QVERIFY(om1.size() == 3);
    QVERIFY(om1.keys().size() == 3);
    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(2) == 2);
    QVERIFY(om1.value(1) == 1);

    om2.remove(1);
    QVERIFY(om2.size() == 1);
    QVERIFY(om2.keys().size() == 1);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.value(2) == 0); // default constructed value
    QVERIFY(om2.value(1) == 0); // default constructed value
    QVERIFY(om1.size() == 3);
    QVERIFY(om1.keys().size() == 3);
    QVERIFY(om1.value(3) == 3);
    QVERIFY(om1.value(2) == 2);
    QVERIFY(om1.value(1) == 1);

    om1.remove(3);
    QVERIFY(om1.size() == 2);
    QVERIFY(om1.keys().size() == 2);
    QVERIFY(om1.value(3) == 0);
    QVERIFY(om1.value(2) == 2); // default constructed value
    QVERIFY(om1.value(1) == 1); // default constructed value
    QVERIFY(om2.size() == 1);
    QVERIFY(om2.value(3) == 3);
    QVERIFY(om2.keys().size() == 1);

    om1.insert(4,4);
    om2.insert(5,5);

    int om1_ans[] = { 2, 1, 4};
    int om2_ans[] = { 3, 5};

    int i = 0;
    foreach (int v, om1.values()) {
        QVERIFY(v == om1_ans[i++]);
    }
    i = 0;
    foreach (int v, om2.values()) {
        QVERIFY(v == om2_ans[i++]);
    }
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

void TestOrderedMap::insertTest()
{
    OrderedMap<int, QString> om;
    OrderedMap<int, QString>::Iterator it = om.insert(1, QString::number(1));

    QVERIFY(it.key() == 1);
    QVERIFY(it.value() == QString::number(1));

    OrderedMap<int, QString>::Iterator it2 = om.insert(2, QString::number(2));

    QVERIFY(it2.key() == 2);
    QVERIFY(it2.value() == QString::number(2));
}

void TestOrderedMap::beginTest()
{
    OrderedMap<QString, QString> om;
    om.insert(QString("1"),QString("1"));
    om.insert(QString("2"),QString("2"));
    om.insert(QString("3"),QString("3"));

    OrderedMap<QString, QString>::Iterator it  = om.begin();

    QVERIFY(it.key() == QString("1"));
    QVERIFY(it.value() == QString("1"));
}

void TestOrderedMap::endTest()
{
    OrderedMap<QString, QString> om;
    om.insert(QString("1"),QString("1"));
    om.insert(QString("2"),QString("2"));
    om.insert(QString("3"),QString("3"));

    OrderedMap<QString, QString>::Iterator it = om.end();
    --it;

    QVERIFY(it.key() == QString("3"));
    QVERIFY(it.value() == QString("3"));
}

void TestOrderedMap::findTest()
{
    OrderedMap<QString, QString> om;
    om.insert(QString("1"),QString("one"));
    om.insert(QString("2"),QString("two"));
    om.insert(QString("3"),QString("three"));

    OrderedMap<QString, QString>::Iterator it = om.find(QString("2"));

    QVERIFY(it.key() == QString("2"));
    QVERIFY(it.value() == QString("two"));

    it = om.find(QString("4"));

    QVERIFY(it == om.end());
}

void TestOrderedMap::eraseTest()
{
    OrderedMap<int, int> om;
    om.insert(3,3);
    om.insert(2,2);
    om.insert(1,1);

    OrderedMap<int, int>::Iterator it = om.erase(om.find(2));

    QVERIFY(!om.contains(2));
    QVERIFY(it.key() == 1);
    QVERIFY(it.value() == 1);

    it = om.erase(om.find(1));

    QVERIFY(!om.contains(1));
    QVERIFY(it == om.end());
}

void TestOrderedMap::iterationOrderTest()
{
    OrderedMap<int, int> om;
    om.insert(3,3);
    om.insert(2,2);
    om.insert(1,1);

    int ans[] = {3, 2, 1};
    OrderedMap<int, int>::Iterator it = om.begin();

    int counter = 0;
    while(it != om.end()) {
        QVERIFY(it.key() == ans[counter]);
        QVERIFY(it.value() == ans[counter]);
        QVERIFY(*it == ans[counter]);

        counter++;
        it++;
    }

    while(it != om.begin()) {
        counter--;
        it--;
        QVERIFY(it.key() == ans[counter]);
        QVERIFY(it.value() == ans[counter]);
        QVERIFY(*it == ans[counter]);
    }
}

void TestOrderedMap::foreachTest()
{
    OrderedMap<int, int> om;
    om.insert(3,3);
    om.insert(2,2);
    om.insert(1,1);

    int ans[] = {3, 2, 1};

    int counter = 0;
    foreach (int value, om) {
        QVERIFY(value == ans[counter]);
        counter++;
    }
}

void TestOrderedMap::iteratorOperatorsTest()
{
    OrderedMap<int, QString> om;
    om.insert(1,QString("1"));
    om.insert(2,QString("2"));
    om.insert(3,QString("3"));
    om.insert(4,QString("4"));
    om.insert(5,QString("5"));
    om.insert(6,QString("6"));

    OrderedMap<int, QString>::Iterator it = om.begin();

    // Iterator tests
    QVERIFY(it.key() == 1);
    QVERIFY(it.value() == QString("1"));
    QVERIFY(*it == QString("1"));

    OrderedMap<int, QString>::Iterator it2 = it + 2;

    QVERIFY(it2.key() == 3);
    QVERIFY(it2.value() == QString("3"));

    OrderedMap<int, QString>::Iterator it3 = it2 - 1;

    QVERIFY(it3.key() == 2);
    QVERIFY(it3.value() == QString("2"));

    it += 2;

    QVERIFY(it.key() == 3);
    QVERIFY(it.value() == QString("3"));

    it -= 1;

    QVERIFY(it.key() == 2);
    QVERIFY(it.value() == QString("2"));

    OrderedMap<int, QString>::Iterator it4 = it++;

    QVERIFY(it4.key() == 2);
    QVERIFY(it4.value() == QString("2"));
    QVERIFY(it.key() == 3);
    QVERIFY(it.value() == QString("3"));

    OrderedMap<int, QString>::Iterator it5 = ++it;

    QVERIFY(it5.key() == 4);
    QVERIFY(it5.value() == QString("4"));
    QVERIFY(it.key() == 4);
    QVERIFY(it.value() == QString("4"));

    OrderedMap<int, QString>::Iterator it6 = it--;

    QVERIFY(it6.key() == 4);
    QVERIFY(it6.value() == QString("4"));
    QVERIFY(it.key() == 3);
    QVERIFY(it.value() == QString("3"));

    OrderedMap<int, QString>::Iterator it7 = --it;

    QVERIFY(it7.key() == 2);
    QVERIFY(it7.value() == QString("2"));
    QVERIFY(it.key() == 2);
    QVERIFY(it.value() == QString("2"));

    QVERIFY(it != it2);
    QVERIFY(it == it3);

    QString &ref = it.value();
    ref.append("two");
    QVERIFY(om.value(2) == QString("2two"));
    // Reset value back to original
    ref = "2";

    // Const iterator
    OrderedMap<int, QString>::ConstIterator cit = om.begin();
    QVERIFY(cit.key() == 1);
    QVERIFY(cit.value() == QString("1"));
    QVERIFY(*cit == QString("1"));

    OrderedMap<int, QString>::ConstIterator cit2 = cit + 2;

    QVERIFY(cit2.key() == 3);
    QVERIFY(cit2.value() == QString("3"));

    OrderedMap<int, QString>::ConstIterator cit3 = cit2 - 1;


    QVERIFY(cit3.key() == 2);
    QVERIFY(cit3.value() == QString("2"));

    cit += 2;

    QVERIFY(cit.key() == 3);
    QVERIFY(cit.value() == QString("3"));

    cit -= 1;

    QVERIFY(cit.key() == 2);
    QVERIFY(cit.value() == QString("2"));

    OrderedMap<int, QString>::ConstIterator cit4 = cit++;

    QVERIFY(cit4.key() == 2);
    QVERIFY(cit4.value() == QString("2"));
    QVERIFY(cit.key() == 3);
    QVERIFY(cit.value() == QString("3"));

    OrderedMap<int, QString>::ConstIterator cit5 = ++cit;

    QVERIFY(cit5.key() == 4);
    QVERIFY(cit5.value() == QString("4"));
    QVERIFY(cit.key() == 4);
    QVERIFY(cit.value() == QString("4"));

    OrderedMap<int, QString>::ConstIterator cit6 = cit--;

    QVERIFY(cit6.key() == 4);
    QVERIFY(cit6.value() == QString("4"));
    QVERIFY(cit.key() == 3);
    QVERIFY(cit.value() == QString("3"));

    OrderedMap<int, QString>::ConstIterator cit7 = --cit;

    QVERIFY(cit7.key() == 2);
    QVERIFY(cit7.value() == QString("2"));
    QVERIFY(cit.key() == 2);
    QVERIFY(cit.value() == QString("2"));

    QVERIFY(cit != cit2);
    QVERIFY(cit == cit3);

    OrderedMap<int, QString>::ConstIterator cit8(it7);
    QVERIFY(cit8.key() == it7.key());
    QVERIFY(cit8.value() == it7.value());
}

QTEST_MAIN(TestOrderedMap)

#include "testorderedmap.moc"
