#include <iostream>
#include "tritset.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv)
{
    Tritset set(10);
    Trit trit = set[5];
    set[4] = Unknown;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}