/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Tests/UnitTests/CheckUtility.hpp>
#include <Utility/Vec2d.hpp>

SCENARIO("Constructing Vec2d", "[Vec2d]")
{
    WHEN("Default constructor is used")
    {
        Vec2d v;

        THEN("x and y are 0")
        {
            CHECK(v.x == 0);
            CHECK(v.y == 0);
        }

        AND_WHEN("a second vector is default constructed")
        {
            Vec2d u;

            THEN("they are equal")
            {
                CHECK(v == u);
            }
        }
    }

    WHEN("Explicit constructor is used")
    {
        Vec2d v(PI, TAU);

        THEN("x and y are set accordingly")
        {
            CHECK(v.x == PI);
            CHECK(v.y == TAU);
        }
    }

    GIVEN("A sf::Vector2i")
    {
        sf::Vector2i sfvi(1, 2);

        WHEN("conversion constructor is used")
        {
            Vec2d vi(sfvi);

            THEN("x and y are set accordingly")
            {
                CHECK(vi.x == 1);
                CHECK(vi.y == 2);
            }
        }
    }

    GIVEN("A sf::Vector2f")
    {
        auto const E = 2.7182f;
        sf::Vector2f sfvf(E, E);

        WHEN("conversion constructor is used")
        {
            Vec2d vf(sfvf);

            THEN("x and y are set accordingly")
            {
                CHECK(vf.x == E);
                CHECK(vf.y == E);
            }
        }
    }

    GIVEN("A vector")
    {
        Vec2d v(PI, TAU);

        WHEN("copy constructor is used")
        {
            Vec2d u(v);

            THEN("they are equal")
            {
                CHECK(v == u);
            }
        }
    }
}

SCENARIO("Assigning Vec2d", "[Vec2d]")
{
    GIVEN("A vector")
    {
        Vec2d v(PI, TAU);

        WHEN("copy assignment is used")
        {
            Vec2d u;
            u = v;

            THEN("they are equal")
            {
                CHECK(v == u);
            }
        }
    }
}

SCENARIO("Computing vector length", "[Vec2d]")
{
    GIVEN("Unit vectors")
    {
        Vec2d v1(0, 1);
        Vec2d v2(1, 0);
        Vec2d v3(-1, 0);
        Vec2d v4(0, -1);

        THEN("computing their length gives one")
        {
            CHECK_APPROX_EQUAL(v1.length(), 1);
            CHECK_APPROX_EQUAL(v2.length(), 1);
            CHECK_APPROX_EQUAL(v3.length(), 1);
            CHECK_APPROX_EQUAL(v4.length(), 1);
        }

        THEN("their squared length are also one")
        {
            CHECK_APPROX_EQUAL(v1.lengthSquared(), 1);
            CHECK_APPROX_EQUAL(v2.lengthSquared(), 1);
            CHECK_APPROX_EQUAL(v3.lengthSquared(), 1);
            CHECK_APPROX_EQUAL(v4.lengthSquared(), 1);
        }
    }

    GIVEN("Length 2 vectors")
    {
        Vec2d v1(1, 1);
        Vec2d v2(-1, -1);

        THEN("Their length is Sqrt 2")
        {
            CHECK_APPROX_EQUAL(v1.length(), sqrt(2));
            CHECK_APPROX_EQUAL(v2.length(), sqrt(2));
        }

        THEN("Their squared length is 2")
        {
            CHECK_APPROX_EQUAL(v1.lengthSquared(), 2);
            CHECK_APPROX_EQUAL(v2.lengthSquared(), 2);
        }
    }

    GIVEN("A vector")
    {
        Vec2d v(PI, PI);

        THEN("its length is relatively precisely computed")
        {
            CHECK_APPROX_EQUAL(v.lengthSquared(), 19.739208802);
        }
    }
}

SCENARIO("Computing normalised vector", "[Vec2d]")
{
    GIVEN("A non null vector")
    {
        Vec2d v(PI, PI);

        THEN("the length of the normalised vector is one")
        {
            CHECK_APPROX_EQUAL(v.normalised().length(), 1);
        }
    }
}

SCENARIO("Computing normal vector", "[Vec2d]")
{
    GIVEN("A non null vector")
    {
        Vec2d v(PI, PI);

        WHEN("computing its normal vector")
        {
            Vec2d w = v.normal();

            THEN("the two vectors are orthogonal")
            {
                double dot = v.x * w.x + v.y * w.y;
                CHECK_APPROX_EQUAL(dot, 0.0);
            }
        }
    }
}

SCENARIO("Computing angle of vector", "[Vec2d]")
{
    GIVEN("A vector in the first quadrant")
    {
        Vec2d v(PI, PI);

        THEN("using its length and angle we can reconstruct the vector")
        {
            double angle = v.angle();
            double len = v.length();
            Vec2d w(std::cos(angle) * len, std::sin(angle) * len);

            CHECK(v == w);
        }
    }

    GIVEN("A vector in the second quadrant")
    {
        Vec2d v(-PI, PI);

        THEN("using its length and angle we can reconstruct the vector")
        {
            double angle = v.angle();
            double len = v.length();
            Vec2d w(std::cos(angle) * len, std::sin(angle) * len);

            CHECK(v == w);
        }
    }

    GIVEN("A vector in the third quadrant")
    {
        Vec2d v(-PI, -PI);

        THEN("using its length and angle we can reconstruct the vector")
        {
            double angle = v.angle();
            double len = v.length();
            Vec2d w(std::cos(angle) * len, std::sin(angle) * len);

            CHECK(v == w);
        }
    }

    GIVEN("A vector in the fourth quadrant")
    {
        Vec2d v(PI, -PI);

        THEN("using its length and angle we can reconstruct the vector")
        {
            double angle = v.angle();
            double len = v.length();
            Vec2d w(std::cos(angle) * len, std::sin(angle) * len);

            CHECK(v == w);
        }
    }
}

SCENARIO("Computing the dot product of vectors", "[Vec2d]")
{
    GIVEN("Two orthogonal vectors")
    {
        Vec2d v(0, 1);
        Vec2d w(1, 0);

        THEN("the dot product is null")
        {
            CHECK_APPROX_EQUAL(v.dot(w), 0);
            CHECK_APPROX_EQUAL(w.dot(v), 0);
        }

        GIVEN("an additional non orthogonal vector")
        {
            Vec2d u(1.618, 2.414);

            THEN("the dot product is symmetric")
            {
                CHECK_APPROX_EQUAL(u.dot(v), v.dot(u));
                CHECK_APPROX_EQUAL(u.dot(w), w.dot(u));
            }

            THEN("the dot product is properly computed")
            {
                CHECK_APPROX_EQUAL(u.dot(v + w), 4.032);
            }
        }
    }
}

SCENARIO("Computing the sign of vectors", "[Vec2d]")
{
    GIVEN("Some vectors")
    {
        Vec2d v(1, 1);
        Vec2d w(1, -1);
        Vec2d x(0, 0);

        THEN("the sign is 0 with themselves")
        {
            CHECK_APPROX_EQUAL(v.sign(v), 0);
            CHECK_APPROX_EQUAL(w.sign(w), 0);
            CHECK_APPROX_EQUAL(x.sign(x), 0);
        }

        THEN("the sign is 0 with a null vector")
        {
            CHECK_APPROX_EQUAL(v.sign(x), 0);
            CHECK_APPROX_EQUAL(w.sign(x), 0);
        }

        THEN("the sign is properly computed")
        {
            CHECK_APPROX_EQUAL(v.sign(w), -1);
            CHECK_APPROX_EQUAL(w.sign(v), 1);
        }
    }
}

SCENARIO("Using vector algebra", "[Vec2d]")
{
    GIVEN("Two opposite vectors")
    {
        Vec2d v(PI, PI);
        Vec2d w(-PI, -PI);

        THEN("They are not equal")
        {
            CHECK(v != w);
            CHECK_FALSE(v == w);
        }

        THEN("Their opposite are equal")
        {
            CHECK(-v == w);
        }

        WHEN("one is negated")
        {
            w = -w;

            THEN("they are equal")
            {
                CHECK(v == w);
            }
        }

        THEN("their addition give a null vector")
        {
            Vec2d z;

            CHECK((v + w) == z);
            CHECK((w + v) == z);
        }

        THEN("subtracting a vector with itself gives a null vector")
        {
            Vec2d z;

            CHECK((v - v) == z);
            CHECK((w - w) == z);
        }
    }

    GIVEN("Vector (1, 15)")
    {
        Vec2d a(1, 15);

        THEN("accessing its data with [] reports the correct values")
        {
            CHECK_APPROX_EQUAL(a[0], 1);
            CHECK_APPROX_EQUAL(a[1], 15);
        }

        WHEN("multiplying it by 3")
        {
            Vec2d b = a * 3;
            Vec2d c = 3 * a;

            THEN("we get (3, 45)")
            {
                CHECK(b == Vec2d(3, 45));
                CHECK(c == Vec2d(3, 45));
            }

            AND_THEN("it was not modified")
            {
                CHECK(a == Vec2d(1, 15));
            }

            AND_THEN("dividing the result give the original")
            {
                CHECK(a == b / 3);
            }
        }
    }

    GIVEN("Vector (3, 6)")
    {
        Vec2d a(3, 6);

        WHEN("using operator *=")
        {
            a *= 0.5;

            THEN("the vector is modified appropriately")
            {
                CHECK(a == Vec2d(1.5, 3));
            }
        }

        WHEN("using operator /=")
        {
            a /= 2;

            THEN("the vector is modified appropriately")
            {
                CHECK(a == Vec2d(1.5, 3));
            }
        }

        WHEN("using operator +=")
        {
            a -= Vec2d(1.5, 3);

            THEN("the vector is modified appropriately")
            {
                CHECK(a == Vec2d(1.5, 3));
            }
        }

        WHEN("using operator +=")
        {
            a += Vec2d(-1.5, -3);

            THEN("the vector is modified appropriately")
            {
                CHECK(a == Vec2d(1.5, 3));
            }
        }
    }
}
