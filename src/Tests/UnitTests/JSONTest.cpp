/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <JSON/JSON.hpp>
#include <Tests/UnitTests/CheckUtility.hpp>

SCENARIO("Constructing JSON objects", "[JSON]")
{
    WHEN("Constructing a String")
    {
        auto initialString = std::string("a string");
        auto string = j::string(initialString);

        THEN("its content is the given value")
        {
            CHECK(string.toString() == initialString);
        }

        AND_WHEN("updating the value")
        {
            string = j::string("another string");

            THEN("its content is updated")
            {
                CHECK(string.toString() == "another string");
            }
        }

        THEN("introspection says it's a string and nothing else")
        {
            CHECK(string.isString());
            CHECK_FALSE(string.isNumber());
            CHECK_FALSE(string.isBoolean());
            CHECK_FALSE(string.isObject());
            CHECK_FALSE(string.isArray());
        }

        THEN("converting it to a string gives the same value")
        {
            CHECK(string.toString() == initialString);
        }

        THEN("converting it to something that is not a string throws")
        {
            CHECK_THROWS_AS(string.toInt(), j::BadConversion);
            CHECK_THROWS_AS(string.toBool(), j::BadConversion);
            CHECK_THROWS_AS(string["id"], j::BadConversion);
            CHECK_THROWS_AS(string[0], j::BadConversion);
        }
    }

    WHEN("Constructing a number")
    {
        auto initialValue = 5;
        auto number = j::number(initialValue);

        THEN("its content is the given value")
        {
            CHECK(number.toInt() == initialValue);
        }

        AND_WHEN("updating its value with another integer")
        {
            number = j::number(7);

            THEN("its content is updated")
            {
                CHECK(number.toInt() == 7);
            }
        }

        AND_WHEN("updating its value with a real")
        {
            number = j::number(3.14);

            THEN("its content is updated")
            {
                CHECK_APPROX_EQUAL(number.toDouble(), 3.14);
            }

            THEN("its integer content is also updated")
            {
                CHECK(number.toInt() == std::floor(3.14));
            }
        }

        THEN("introspection says it's a number and nothing else")
        {
            CHECK_FALSE(number.isString());
            CHECK(number.isNumber());
            CHECK_FALSE(number.isBoolean());
            CHECK_FALSE(number.isObject());
            CHECK_FALSE(number.isArray());
        }

        THEN("converting it to a number gives the same value")
        {
            CHECK(number.toInt() == initialValue);
        }

        THEN("converting it to something that is not a number throws")
        {
            CHECK_THROWS_AS(number.toString(), j::BadConversion);
            CHECK_THROWS_AS(number.toBool(), j::BadConversion);
            CHECK_THROWS_AS(number["id"], j::BadConversion);
            CHECK_THROWS_AS(number[0], j::BadConversion);
        }
    }

    WHEN("Constructing a boolean")
    {
        auto boolean = j::boolean(true);

        THEN("its content is the given value")
        {
            CHECK(boolean.toBool());
        }

        AND_WHEN("updating its value")
        {
            boolean = j::boolean(false);

            THEN("its content is updated")
            {
                CHECK_FALSE(boolean.toBool());
            }
        }

        THEN("introspection says it's a boolean and nothing else")
        {
            CHECK_FALSE(boolean.isString());
            CHECK_FALSE(boolean.isNumber());
            CHECK(boolean.isBoolean());
            CHECK_FALSE(boolean.isObject());
            CHECK_FALSE(boolean.isArray());
        }

        THEN("converting it to something that is not a boolean throws")
        {
            CHECK_THROWS_AS(boolean.toString(), j::BadConversion);
            CHECK_THROWS_AS(boolean.toInt(), j::BadConversion);
            CHECK_THROWS_AS(boolean["id"], j::BadConversion);
            CHECK_THROWS_AS(boolean[0], j::BadConversion);
        }
    }

    WHEN("Constructing an object")
    {
        auto object = j::object();

        auto id1 = "an id";
        auto id2 = "another id";
        auto id3 = "a third id";
        auto id4 = "4th id";
        auto id5 = "yet another id";

        auto v1 = j::string("a string");
        auto v2 = j::boolean(true);
        auto v3 = j::number(4.2);
        auto v4 = j::object();
        v4.set(id1, v1);
        auto v5 = j::array();
        v5.add(v1);

        THEN("initially it has no element")
        {
            CHECK_FALSE(object.hasValue(id1));
            CHECK_FALSE(object.hasValue(id2));
            CHECK_FALSE(object.hasValue(id3));
            CHECK_FALSE(object.hasValue(id4));
            CHECK_FALSE(object.hasValue(id5));
        }

        THEN("asking for an element that doesn't exist throws")
        {
            CHECK_THROWS_AS(object[id1], j::NoSuchElement);
        }

        AND_WHEN("adding some new elements")
        {
            auto s1 = object.set(id1, v1);
            auto s2 = object.set(id2, v2);
            auto s3 = object.set(id3, v3);
            auto s4 = object.set(id4, v4);
            auto s5 = object.set(id5, v5);

            THEN("all elements are new")
            {
                CHECK(s1);
                CHECK(s2);
                CHECK(s3);
                CHECK(s4);
                CHECK(s5);
            }

            THEN("the key set is valid")
            {
                auto const& keys = object.keys();

                CHECK(keys.size() == 5);
                CHECK(std::find(keys.begin(), keys.end(), id1) != keys.end());
                CHECK(std::find(keys.begin(), keys.end(), id2) != keys.end());
                CHECK(std::find(keys.begin(), keys.end(), id3) != keys.end());
                CHECK(std::find(keys.begin(), keys.end(), id4) != keys.end());
                CHECK(std::find(keys.begin(), keys.end(), id5) != keys.end());
            }

            THEN("the value corresponding to each element is correct")
            {
                CHECK(object[id1] == v1);
                CHECK(object[id2] == v2);
                CHECK(object[id3] == v3);
                CHECK(object[id4] == v4);
                CHECK(object[id4][id1] == v1);
                CHECK(object[id5] == v5);
                CHECK(object[id5][0] == v1);
            }

            AND_WHEN("updating an element")
            {
                auto newValueForId1 = j::string("a new string");
                auto s6 = object.set(id1, newValueForId1);

                THEN("the value is updated and it has erased a value")
                {
                    CHECK_FALSE(s6);
                    CHECK(object[id1] == newValueForId1);
                }
            }

            AND_WHEN("removing an existing element")
            {
                object.remove(id1);

                THEN("the element no longer exists in the object")
                {
                    CHECK_FALSE(object.hasValue(id1));
                }
            }

            AND_WHEN("removing an element that doesn't exist")
            {
                auto anotherId = "inexisting id";

                THEN("it fires a NoSuchElement")
                {
                    CHECK_THROWS_AS(object.remove(anotherId), j::NoSuchElement);
                }
            }
        }

        THEN("introspection says it's an object and nothing else")
        {
            CHECK_FALSE(object.isString());
            CHECK_FALSE(object.isNumber());
            CHECK_FALSE(object.isBoolean());
            CHECK(object.isObject());
            CHECK_FALSE(object.isArray());
        }

        THEN("converting it to something that is not an object throws")
        {
            CHECK_THROWS_AS(object.toString(), j::BadConversion);
            CHECK_THROWS_AS(object.toInt(), j::BadConversion);
            CHECK_THROWS_AS(object.toBool(), j::BadConversion);
            CHECK_THROWS_AS(object[0], j::BadConversion);
        }
    }

    WHEN("Constructing an array")
    {
        auto array = j::array();

        auto v1 = j::string("a string");
        auto v2 = j::boolean(true);
        auto v3 = j::number(4.2);
        auto v4 = j::object();
        auto id1 = "an id";
        v4.set(id1, v1);
        auto v5 = j::array();
        v5.add(v1);

        THEN("initially it has no element")
        {
            CHECK(array.size() == 0);
        }

        THEN("asking for an element that doesn't exist throws")
        {
            CHECK_THROWS_AS(array[0], j::NoSuchElement);
        }

        AND_WHEN("adding some new elements")
        {
            array.add(v1);
            array.add(v2);
            array.add(v3);
            array.add(v4);
            array.add(v5);

            THEN("it has the proper size")
            {
                CHECK(array.size() == 5);
            }

            THEN("the value corresponding to each element is correct")
            {
                CHECK(array[0] == v1);
                CHECK(array[1] == v2);
                CHECK(array[2] == v3);
                CHECK(array[3] == v4);
                CHECK(array[4] == v5);
            }

            AND_WHEN("updating an element")
            {
                auto newValueForIndex0 = j::string("a new string");
                array[0] = newValueForIndex0;

                THEN("the value is updated and it has erased a value")
                {
                    CHECK(array.size() == 5);
                    CHECK(array[0] == newValueForIndex0);
                }
            }

            AND_WHEN("removing an element")
            {
                array.remove(0);

                THEN("the element is really removed")
                {
                    CHECK(array.size() == 4);
                    CHECK_THROWS_AS(array[4], j::NoSuchElement);

                    CHECK(array[0] == v2);
                }
            }

            AND_WHEN("removing an element that doesn't exist")
            {
                THEN("a NoSuchElement is fired")
                {
                    CHECK_THROWS_AS(array.remove(10), j::NoSuchElement);
                }
            }
        }

        AND_WHEN("removing an element of an empty array")
        {
            THEN("a NoSuchElement is fired")
            {
                CHECK_THROWS_AS(array.remove(0), j::NoSuchElement);
            }
        }

        THEN("introspection says it's an array and nothing else")
        {
            CHECK_FALSE(array.isString());
            CHECK_FALSE(array.isNumber());
            CHECK_FALSE(array.isBoolean());
            CHECK_FALSE(array.isObject());
            CHECK(array.isArray());
        }

        THEN("converting it to something that is not an array throws")
        {
            CHECK_THROWS_AS(array.toString(), j::BadConversion);
            CHECK_THROWS_AS(array.toInt(), j::BadConversion);
            CHECK_THROWS_AS(array.toBool(), j::BadConversion);
            CHECK_THROWS_AS(array["id"], j::BadConversion);
        }
    }
}

SCENARIO("Comparing JSON values", "[JSON]")
{
    GIVEN("some JSON values")
    {
        auto id1 = "an id";
        auto id2 = "another id";
        auto id3 = "a third id";
        auto id4 = "4th id";
        auto id5 = "yet another id";

        auto str = std::string("a string");

        auto string = j::string(str);
        auto boolean = j::boolean(true);
        auto number = j::number(4.2);
        auto object = j::object();
        object.set(id1, string);
        auto array = j::array();
        auto anotherArray = j::array();
        array.add(string);
        array.add(boolean);
        array.add(number);
        array.add(object);
        array.add(anotherArray);

        THEN("the comparison operators gives the correct answer")
        {
            CHECK(string == string);
            CHECK("a string" == string);
            CHECK(string == "a string");
            CHECK(string == str);
            CHECK(str == string);
            CHECK("" != string);
            CHECK(string != "");
            CHECK(string != boolean);
            CHECK_FALSE(string == false);
            CHECK_FALSE(true == string);
            CHECK(string != true);
            CHECK(true != string);
            CHECK(boolean == boolean);
            CHECK(boolean != false);
            CHECK(boolean != number);
            CHECK(number == number);
            CHECK(number != 666);
            CHECK(777 != number);
            CHECK(4.4 != number);
            CHECK(object == object);
            CHECK(object[id1] == string);
            CHECK(array != object);
            CHECK(array == array);
            CHECK(array[4] == anotherArray);
            CHECK(array != anotherArray);
        }
    }
}
