#include "Interface.hpp"
#include "test.hpp"

//#define DA_TEST
//#define LL_TEST
//#define AS_TEST
//#define LS_TEST
#define ALL_TEST

int main() {

#ifdef DA_TEST
    DynamicArrayTest();
#endif

#ifdef LL_TEST
    LinkedListTest();
#endif

#ifdef AS_TEST
    ArraySequenceTest();
#endif

#ifdef LS_TEST
    ListSequenceTest();
#endif

#ifdef ALL_TEST
    AllTests();
#endif

    std::vector<ISequenceWrapper*> sequences;
    interface(sequences);
}