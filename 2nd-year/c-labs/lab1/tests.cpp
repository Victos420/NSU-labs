#include <exception>
#include <iostream>
#include <sstream>
#include "tritset.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"


TEST(ReferenceTest, Reference_Constructor) {
    size_t size = 64;
    Tritset set(size);
    ASSERT_EQ(set[0], Unknown);
    ASSERT_EQ(set.Capacity(), size);
    ASSERT_EQ(set[size], Unknown);
    ASSERT_EQ(set.Capacity(), size);
}

TEST(ReferenceTest, Reference_EQ) {
    size_t size = 50;
    Tritset set(size);
    set[2] = set[3] = False;
    set[4] = set[5] = True;
    ASSERT_EQ(set[0], set[1]);
    ASSERT_EQ(set[2], set[3]);
    ASSERT_EQ(set[4], set[5]);
    ASSERT_EQ(set[0] == Unknown, true);
    ASSERT_EQ(set[2] == False, true);
    ASSERT_EQ(set[4] == True, true);
    ASSERT_NE(set[0], set[2]);
    ASSERT_NE(set[0], set[4]);
    ASSERT_NE(set[2], set[4]);
}

TEST(ReferenceTest, Reference_AND) {
    size_t size = 50;
    Tritset set(size);
    set[0] = False;
    set[2] = True;
    ASSERT_EQ(set[0] & False  , False);
    ASSERT_EQ(set[0] & Unknown, False);
    ASSERT_EQ(set[0] & True   , False);
    ASSERT_EQ(set[1] & False  , False);
    ASSERT_EQ(set[1] & Unknown, Unknown);
    ASSERT_EQ(set[1] & True   , Unknown);
    ASSERT_EQ(set[2] & False  , False);
    ASSERT_EQ(set[2] & Unknown, Unknown);
    ASSERT_EQ(set[2] & True   , True);
    ASSERT_EQ(set[0] & set[0], False);
    ASSERT_EQ(set[0] & set[1], False);
    ASSERT_EQ(set[0] & set[2], False);
    ASSERT_EQ(set[1] & set[0], False);
    ASSERT_EQ(set[1] & set[1], Unknown);
    ASSERT_EQ(set[1] & set[2], Unknown);
    ASSERT_EQ(set[2] & set[0], False);
    ASSERT_EQ(set[2] & set[1], Unknown);
    ASSERT_EQ(set[2] & set[2], True);
    set[0] = set[1] = set[2] = False;
    set[3] = set[4] = set[5] = Unknown;
    set[6] = set[7] = set[8] = True;
    set[0] &= False;
    set[3] &= False;
    set[6] &= False;
    set[1] &= Unknown;
    set[4] &= Unknown;
    set[7] &= Unknown;
    set[2] &= True;
    set[5] &= True;
    set[8] &= True;
    ASSERT_EQ(set[0], False);
    ASSERT_EQ(set[1], False);
    ASSERT_EQ(set[2], False);
    ASSERT_EQ(set[3], False);
    ASSERT_EQ(set[4], Unknown);
    ASSERT_EQ(set[5], Unknown);
    ASSERT_EQ(set[6], False);
    ASSERT_EQ(set[7], Unknown);
    ASSERT_EQ(set[8], True);
}

TEST(ReferenceTest, Reference_OR) {
    size_t size = 50;
    Tritset set(size);
    set[0] = False;
    set[2] = True;
    ASSERT_EQ(set[0] | False  , False);
    ASSERT_EQ(set[0] | Unknown, Unknown);
    ASSERT_EQ(set[0] | True   , True);
    ASSERT_EQ(set[1] | False  , Unknown);
    ASSERT_EQ(set[1] | Unknown, Unknown);
    ASSERT_EQ(set[1] | True   , True);
    ASSERT_EQ(set[2] | False  , True);
    ASSERT_EQ(set[2] | Unknown, True);
    ASSERT_EQ(set[2] | True   , True);
    ASSERT_EQ(set[0] | set[0], False);
    ASSERT_EQ(set[0] | set[1], Unknown);
    ASSERT_EQ(set[0] | set[2], True);
    ASSERT_EQ(set[1] | set[0], Unknown);
    ASSERT_EQ(set[1] | set[1], Unknown);
    ASSERT_EQ(set[1] | set[2], True);
    ASSERT_EQ(set[2] | set[0], True);
    ASSERT_EQ(set[2] | set[1], True);
    ASSERT_EQ(set[2] | set[2], True);
    set[0] = set[1] = set[2] = False;
    set[3] = set[4] = set[5] = Unknown;
    set[6] = set[7] = set[8] = True;
    set[0] |= False;
    set[3] |= False;
    set[6] |= False;
    set[1] |= Unknown;
    set[4] |= Unknown;
    set[7] |= Unknown;
    set[2] |= True;
    set[5] |= True;
    set[8] |= True;
    ASSERT_EQ(set[0], False);
    ASSERT_EQ(set[1], Unknown);
    ASSERT_EQ(set[2], True);
    ASSERT_EQ(set[3], Unknown);
    ASSERT_EQ(set[4], Unknown);
    ASSERT_EQ(set[5], True);
    ASSERT_EQ(set[6], True);
    ASSERT_EQ(set[7], True);
    ASSERT_EQ(set[8], True);
}

TEST(ReferenceTest, Reference_NOT) {
    size_t size = 50;
    Tritset set(size);
    ASSERT_EQ(~set[0], Unknown);
    set[0] = False;
    ASSERT_EQ(~set[0], True);
    set[0] = True;
    ASSERT_EQ(~set[0], False);
}


TEST(TritsetTest, Tritset_Constructor) {
    size_t size = 50;
    Tritset set(size);
    ASSERT_EQ(set.Length(), 0);
    ASSERT_EQ(set.Capacity(), ((size-1)/4/sizeof(uint)+1)*sizeof(uint)*4);
}

TEST(TritsetTest, Tritset_Resize) {
    size_t size = 50;
    Tritset set(size);
    size *= 2;
    set.Resize(size);
    ASSERT_EQ(set.Length(), 0);
    ASSERT_EQ(set.Capacity(), ((size-1)/4/sizeof(uint)+1)*sizeof(uint)*4);
}

TEST(TritsetTest, Tritset_Shrink) {
    size_t size = 50;
    Tritset set(size);
    set[size/2-1] = True;
    ASSERT_EQ(set.Capacity(), ((size-1)/4/sizeof(uint)+1)*sizeof(uint)*4);
    set.Shrink();
    ASSERT_EQ(set.Capacity(), ((size/2-1)/4/sizeof(uint)+1)*sizeof(uint)*4);
}

TEST(TritsetTest, Tritset_Cardinality) {
    size_t size = 50;
    Tritset set(size);
    ASSERT_EQ(set.Cardinality(False), 0);
    ASSERT_EQ(set.Cardinality(Unknown), 0);
    ASSERT_EQ(set.Cardinality(True), 0);
}

TEST(TritsetTest, Tritset_AND) {
    size_t size = 9;
    Tritset set1(size), set2(size+2), set3(3);
    set1[0] = set1[1] = set1[2] = False;
    set1[3] = set1[4] = set1[5] = Unknown;
    set1[6] = set1[7] = set1[8] = True;
    set2[0] = set2[3] = set2[6] = False;
    set2[1] = set2[4] = set2[7] = Unknown;
    set2[2] = set2[5] = set2[8] = True;
    set3 = set1 & set2;
    ASSERT_EQ(set3.Size(), size+2);
    ASSERT_EQ(set3[0], False);
    ASSERT_EQ(set3[1], False);
    ASSERT_EQ(set3[2], False);
    ASSERT_EQ(set3[3], False);
    ASSERT_EQ(set3[4], Unknown);
    ASSERT_EQ(set3[5], Unknown);
    ASSERT_EQ(set3[6], False);
    ASSERT_EQ(set3[7], Unknown);
    ASSERT_EQ(set3[8], True);
    set1 &= set2;
    ASSERT_EQ(set1[0], False);
    ASSERT_EQ(set1[1], False);
    ASSERT_EQ(set1[2], False);
    ASSERT_EQ(set1[3], False);
    ASSERT_EQ(set1[4], Unknown);
    ASSERT_EQ(set1[5], Unknown);
    ASSERT_EQ(set1[6], False);
    ASSERT_EQ(set1[7], Unknown);
    ASSERT_EQ(set1[8], True);
}

TEST(TritsetTest, Tritset_OR) {
    size_t size = 10;
    Tritset set1(size), set2(size+2), set3(3);
    set1[0] = set1[1] = set1[2] = False;
    set1[3] = set1[4] = set1[5] = Unknown;
    set1[6] = set1[7] = set1[8] = True;
    set2[0] = set2[3] = set2[6] = False;
    set2[1] = set2[4] = set2[7] = Unknown;
    set2[2] = set2[5] = set2[8] = True;
    set3 = set1 | set2;
    ASSERT_EQ(set3.Size(), size+2);
    ASSERT_EQ(set3[0], False);
    ASSERT_EQ(set3[1], Unknown);
    ASSERT_EQ(set3[2], True);
    ASSERT_EQ(set3[3], Unknown);
    ASSERT_EQ(set3[4], Unknown);
    ASSERT_EQ(set3[5], True);
    ASSERT_EQ(set3[6], True);
    ASSERT_EQ(set3[7], True);
    ASSERT_EQ(set3[8], True);
    set1 |= set2;
    ASSERT_EQ(set1[0], False);
    ASSERT_EQ(set1[1], Unknown);
    ASSERT_EQ(set1[2], True);
    ASSERT_EQ(set1[3], Unknown);
    ASSERT_EQ(set1[4], Unknown);
    ASSERT_EQ(set1[5], True);
    ASSERT_EQ(set1[6], True);
    ASSERT_EQ(set1[7], True);
    ASSERT_EQ(set1[8], True);
}

TEST(TritsetTest, Tritset_NOT) {
    size_t size = 50;
    Tritset set(size);
    set[0] = False;
    set[2] = True;
    ~set;
    ASSERT_EQ(set[0], True);
    ASSERT_EQ(set[1], Unknown);
    ASSERT_EQ(set[2], False);
}

TEST(TritsetTest, Tritset_Resize1) {
    Tritset set(0);
    ASSERT_EQ(set.Length(), 0);
    set[999] = True;
    ASSERT_EQ(set.Length(), 1000);
    set[999] = Unknown;
    ASSERT_EQ(set.Length(), 0);
}


TEST(TritsetTest, Tritset_test) {
//резерв памяти для хранения 1000 тритов
    Tritset set(1000);
// length of internal array
    size_t allocLength = set.Capacity();
    assert(allocLength >= 1000 * 2 / 8 / sizeof(uint));
// 1000*2 - min bits count
// 1000*2 / 8 - min bytes count
// 1000*2 / 8 / sizeof(uint) - min uint[] size


//не выделяет никакой памяти
    set[1000000000] = Unknown;
    assert(allocLength == set.Capacity());

// false, but no exception or memory allocation
    if (set[2000000000]==True){}
    assert(allocLength == set.Capacity());

//выделение памяти
    set[1000000000] = True;
    assert(allocLength < set.Capacity());

//no memory operations
    allocLength = set.Capacity();
    set[1000000000] = Unknown;
    set[1000000] = False;
    assert(allocLength == set.Capacity());

//освобождение памяти до начального значения или
//до значения необходимого для хранения последнего установленного трита
//в данном случае для трита 1000’000
    set.Shrink();
    assert(allocLength > set.Capacity());


    Tritset setA(1000);
    Tritset setB(2000);
    Tritset setC = setA & setB;
    assert(setC.Capacity() == setB.Capacity());
}
