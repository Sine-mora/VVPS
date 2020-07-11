#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include <FPGrowth/FPGrowth.hpp>
#include <DataComponent/DataComponent.h>

void test_1() {
    const FP::Item a{ "A" };
    const FP::Item b{ "B" };
    const FP::Item c{ "C" };
    const FP::Item d{ "D" };
    const FP::Item e{ "E" };
    
    const FP::DataBase transactions{
        { a, b },
        { b, c, d },
        { a, c, d, e },
        { a, d, e },
        { a, b, c },
        { a, b, c, d },
        { a },
        { a, b, c },
        { a, b, d },
        { b, c, e }
    };
    
    const uint64_t unMinRequiredTreshold = 2;
    
    const FP::Tree fpTree{ transactions, unMinRequiredTreshold };
    const auto& patterns = FP::TreeGrowth::ConstructPatternSetFromTree(fpTree);
    
    assert(patterns.size() == 19);
    assert(patterns.count({ { a }, 8 }));
    assert(patterns.count({ { b, a }, 5 }));
    assert(patterns.count({ { b }, 7 }));
    assert(patterns.count({ { c, b }, 5 }));
    assert(patterns.count({ { c, a, b }, 3 }));
    assert(patterns.count({ { c, a }, 4 }));
    assert(patterns.count({ { c }, 6 }));
    assert(patterns.count({ { d, a }, 4 }));
    assert(patterns.count({ { d, c, a }, 2 }));
    assert(patterns.count({ { d, c }, 3 }));
    assert(patterns.count({ { d, b, a }, 2 }));
    assert(patterns.count({ { d, b, c }, 2 }));
    assert(patterns.count({ { d, b }, 3 }));
    assert(patterns.count({ { d }, 5 }));
    assert(patterns.count({ { e, d }, 2 }));
    assert(patterns.count({ { e, c }, 2 }));
    assert(patterns.count({ { e, a, d }, 2 }));
    assert(patterns.count({ { e, a }, 2 }));
    assert(patterns.count({ { e }, 3 }));
}

void test_2() {
    const FP::Item a{ "A" };
    const FP::Item b{ "B" };
    const FP::Item c{ "C" };
    const FP::Item d{ "D" };
    const FP::Item e{ "E" };
    
    const FP::DataBase transactions{
        { a, b, d, e },
        { b, c, e },
        { a, b, d, e },
        { a, b, c, e },
        { a, b, c, d, e },
        { b, c, d },
    };
    
    const uint64_t unMinRequiredTreshold = 3;
    
    const FP::Tree fpTree{ transactions, unMinRequiredTreshold };
    const auto& patterns = FP::TreeGrowth::ConstructPatternSetFromTree(fpTree);
    
    assert(patterns.size() == 19);
    assert(patterns.count({ { e, b }, 5 }));
    assert(patterns.count({ { e }, 5 }));
    assert(patterns.count({ { a, b, e }, 4 }));
    assert(patterns.count({ { a, b }, 4 }));
    assert(patterns.count({ { a, e }, 4 }));
    assert(patterns.count({ { a }, 4 }));
    assert(patterns.count({ { d, a, b }, 3 }));
    assert(patterns.count({ { d, a }, 3 }));
    assert(patterns.count({ { d, e, b, a }, 3 }));
    assert(patterns.count({ { d, e, b }, 3 }));
    assert(patterns.count({ { d, e, a }, 3 }));
    assert(patterns.count({ { d, e }, 3 }));
    assert(patterns.count({ { d, b }, 4 }));
    assert(patterns.count({ { d }, 4 }));
    assert(patterns.count({ { c, e, b }, 3 }));
    assert(patterns.count({ { c, e }, 3 }));
    assert(patterns.count({ { c, b }, 4 }));
    assert(patterns.count({ { c }, 4 }));
    assert(patterns.count({ { b }, 6 }));
}

void test_3() {
    const FP::Item a{ "A" };
    const FP::Item b{ "B" };
    const FP::Item c{ "C" };
    const FP::Item d{ "D" };
    const FP::Item e{ "E" };
    const FP::Item f{ "F" };
    const FP::Item g{ "G" };
    const FP::Item h{ "H" };
    const FP::Item i{ "I" };
    const FP::Item j{ "J" };
    const FP::Item k{ "K" };
    const FP::Item l{ "L" };
    const FP::Item m{ "M" };
    const FP::Item n{ "N" };
    const FP::Item o{ "O" };
    const FP::Item p{ "P" };
    const FP::Item s{ "S" };
    
    const FP::DataBase transactions{
        { f, a, c, d, g, i, m, p },
        { a, b, c, f, l, m, o },
        { b, f, h, j, o },
        { b, c, k, s, p },
        { a, f, c, e, l, p, m, n }
    };
    
    const uint64_t unMinRequiredTreshold = 3;

    const FP::Tree fpTree{ transactions, unMinRequiredTreshold };
    const auto& patterns = FP::TreeGrowth::ConstructPatternSetFromTree(fpTree);
    
    assert(patterns.size() == 18);
    assert(patterns.count({ { f }, 4 }));
    assert(patterns.count({ { c, f }, 3 }));
    assert(patterns.count({ { c }, 4 }));
    assert(patterns.count({ { b }, 3 }));
    assert(patterns.count({ { p, c }, 3 }));
    assert(patterns.count({ { p }, 3 }));
    assert(patterns.count({ { m, f, c }, 3 }));
    assert(patterns.count({ { m, f }, 3 }));
    assert(patterns.count({ { m, c }, 3 }));
    assert(patterns.count({ { m }, 3 }));
    assert(patterns.count({ { a, f, c, m }, 3 }));
    assert(patterns.count({ { a, f, c }, 3 }));
    assert(patterns.count({ { a, f, m }, 3 }));
    assert(patterns.count({ { a, f }, 3 }));
    assert(patterns.count({ { a, c, m }, 3 }));
    assert(patterns.count({ { a, c }, 3 }));
    assert(patterns.count({ { a, m }, 3 }));
    assert(patterns.count({ { a }, 3 }));
}

int main(int argc, const char *argv[]) {

    DataReader::GetInstance().ParseCSV("../../../../logs_BCS37_20181103.csv");

    for(const auto& dataComp: DataReader::GetInstance().GetComponents())
    {
        dataComp.Print();
    }

    test_1();
    test_2();
    test_3();
    std::cout << "All tests passed!" << std::endl;
    
    return EXIT_SUCCESS;
}
