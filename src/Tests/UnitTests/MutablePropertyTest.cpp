/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Tests/UnitTests/CheckUtility.hpp>
#include <Utility/MutableNumber.hpp>

SCENARIO("Testing mutable parameters", "[MutableParameter]")
{
    GIVEN("a positive mutable number")
    {
        auto number = MutableNumber::positive(1.0, 1.0, 1.0);

        WHEN("mutating several times")
        {
            THEN("the number is still positive")
            {
                for (auto i = 0; i < 1000; ++i) {
                    number.mutate();
                    CHECK(0.0 <= number.get());
                }
            }
        }
    }

    GIVEN("a negative mutable number")
    {
        auto number = MutableNumber(-1.0, 1.0, 1.0, false, 0.0, true, 0.0);

        WHEN("mutating several times")
        {
            THEN("the number is still negative")
            {
                for (auto i = 0; i < 1000; ++i) {
                    number.mutate();
                    CHECK(0.0 >= number.get());
                }
            }
        }
    }

    GIVEN("a mutable probability")
    {
        auto probability = MutableNumber::probability(0.5, 1.0, 1.0);

        WHEN("mutating several times")
        {
            THEN("the probability is still in [0, 1]")
            {
                for (auto i = 0; i < 1000; ++i) {
                    probability.mutate();
                    CHECK(0.0 <= probability.get());
                    CHECK(probability.get() <= 1.0);
                }
            }
        }
    }

    GIVEN("a mutable number with mutation rate of 100%")
    {
        auto number = MutableNumber(0.0, 1.0, 1.0);

        WHEN("mutating")
        {
            THEN("it always mutates")
            {
                for (auto i = 0; i < 1000; ++i) {
                    auto last = number.get();
                    number.mutate();
                    CHECK(last != number.get());
                }
            }
        }
    }

    GIVEN("a mutable number with mutation rate of 0%")
    {
        auto number = MutableNumber(0.0, 0.0, 1.0);

        WHEN("mutating")
        {
            THEN("it never mutates")
            {
                for (auto i = 0; i < 1000; ++i) {
                    auto last = number.get();
                    number.mutate();
                    CHECK(last == number.get());
                }
            }
        }
    }
}
