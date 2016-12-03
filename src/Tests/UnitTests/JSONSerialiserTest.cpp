/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <JSON/JSONSerialiser.hpp>
#include <Tests/UnitTests/CheckUtility.hpp>

#include <string>
#include <vector>

SCENARIO("Reading valid JSON payload", "[JSON]")
{
    GIVEN("A JSON with only a string")
    {
        auto payload = R"("a string")";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                CHECK(value == j::string("a string"));
            }
        }
    }

    GIVEN("A JSON with only an integer")
    {
        auto payload = "58";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                CHECK(value == j::number(58));
            }
        }
    }

    GIVEN("A JSON with only a real")
    {
        auto payload = "5.99";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                CHECK(value == j::number(5.99));
            }
        }
    }

    GIVEN("A JSON with only true")
    {
        auto payload = "true";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                CHECK(value == j::boolean(true));
            }
        }
    }

    GIVEN("A JSON with only false")
    {
        auto payload = "false";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                CHECK(value == j::boolean(false));
            }
        }
    }

    GIVEN("A JSON with an empty object")
    {
        auto payload = "{ }";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                CHECK(value == j::object());
            }
        }
    }

    GIVEN("A JSON with an empty array")
    {
        auto payload = "[ ]";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                CHECK(value == j::array());
            }
        }
    }

    GIVEN("A JSON with a non-empty array")
    {
        auto payload = R"([ 1, "str", false, [] ])";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                auto expected = j::array();
                expected.add(j::number(1));
                expected.add(j::string("str"));
                expected.add(j::boolean(false));
                expected.add(j::array());

                CHECK(value == expected);
            }
        }
    }

    GIVEN("A JSON with a non-emtpy object")
    {
        auto payload = R"({
            "id": "a string",
            "another id": true,
            "3rd": 4.2,
            "toto": { "titi": [ 1, 2, 3 ] },
            "titi": [ 1, 2, 3 ]
        })";

        WHEN("reading the payload")
        {
            auto value = j::readFromString(payload);

            THEN("the value is properly created")
            {
                auto expected = j::object();
                expected.set("id", j::string("a string"));
                expected.set("another id", j::boolean(true));
                expected.set("3rd", j::number(4.2));
                auto toto = j::object();
                auto titi = j::array();
                titi.add(j::number(1));
                titi.add(j::number(2));
                titi.add(j::number(3));
                toto.set("titi", titi);
                expected.set("toto", toto);
                expected.set("titi", titi);

                CHECK(value == expected);
            }
        }
    }
}

SCENARIO("Reading invalid JSON payload", "[JSON]")
{
    std::vector<std::string> invalids = {
        "{", "", ",", ":", "noquote", "[", "[[]", "{ [] }",
        R"( "id" : "I am not in an object" )"
    };

    for (auto const& test : invalids) {
        GIVEN("an invalid payload")
        {
            THEN("a BadPayload is fired when reading it")
            {
                CHECK_THROWS_AS(j::readFromString(test), j::BadPayload);
            }
        }
    }
}

SCENARIO("Writing JSON value", "[JSON]")
{
    GIVEN("A JSON object")
    {
        auto value = j::object();
        value.set("id", j::string("a string"));
        value.set("another id", j::boolean(true));
        value.set("3rd", j::number(4.2));
        auto toto = j::object();
        auto titi = j::array();
        titi.add(j::number(1));
        titi.add(j::number(2));
        titi.add(j::number(3));
        toto.set("titi", titi);
        value.set("toto", toto);
        value.set("titi", titi);

        WHEN("writing the value")
        {
            auto payload = j::writeToString(value);

            THEN("the value can be loaded again")
            {
                auto copy = j::readFromString(payload);

                CHECK(copy == value);
            }
        }
    }
}
