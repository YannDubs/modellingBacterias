/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Lab/CircularBody.hpp>

#include <catch.hpp>

#include <iostream>

// Create a dummy CircularBody subclass
class Body : public CircularBody
{
public:
    Body(Vec2d const& position, double radius)
    : CircularBody(position, radius)
    {
    }

    using CircularBody::move;
    using CircularBody::setRadius;
    using CircularBody::setPosition;
};

SCENARIO("Collision/IsInside with CircularBody", "[Body]")
{
    GIVEN("Two identical bodies")
    {
        auto b1 = Body({ 1, 1 }, 2);
        auto b2 = b1;

        THEN("they collide")
        {
            std::cout << b1 << std::endl;

            CHECK(b1.isColliding(b2));
            CHECK(b2.isColliding(b1));
            CHECK((b1 | b2));
            CHECK((b2 | b1));
        }

        THEN("they are inside of each other")
        {
            CHECK(b1.isInside(b2));
            CHECK(b2.isInside(b1));
            CHECK(b1 > b2);
            CHECK(b2 > b1);
        }
    }

    GIVEN("Two bodies that don't overlap")
    {
        auto b1 = Body({ 1, 1 }, 0.5);
        auto b2 = Body({ -1, -1 }, 0.5);

        THEN("they don't collide")
        {
            CHECK_FALSE(b1.isColliding(b2));
            CHECK_FALSE(b2.isColliding(b1));
            CHECK_FALSE((b1 | b2));
            CHECK_FALSE((b2 | b1));
        }

        AND_THEN("they are not inside of each other")
        {
            CHECK_FALSE(b1.isInside(b2));
            CHECK_FALSE(b2.isInside(b1));
            CHECK_FALSE(b1 > b2);
            CHECK_FALSE(b2 > b1);
        }
    }

    GIVEN("Two bodies that overlap but are not inside of each other")
    {
        auto b1 = Body({ 0, 0 }, 2);
        auto b2 = Body({ 3, 0 }, 2);

        THEN("they collide")
        {
            CHECK(b1.isColliding(b2));
            CHECK(b2.isColliding(b1));
            CHECK((b1 | b2));
            CHECK((b2 | b1));
        }

        THEN("they are not inside of each other")
        {
            CHECK_FALSE(b1.isInside(b2));
            CHECK_FALSE(b2.isInside(b1));
            CHECK_FALSE(b1 > b2);
            CHECK_FALSE(b2 > b1);
        }
    }

    GIVEN("A body inside another one")
    {
        auto b1 = Body({ 0, 0 }, 5);
        auto b2 = Body({ 0, 0 }, 1);

        THEN("they collide")
        {
            CHECK(b1.isColliding(b2));
            CHECK(b2.isColliding(b1));
            CHECK((b1 | b2));
            CHECK((b2 | b1));
        }

        THEN("the smaller one is inside the bigger one")
        {
            CHECK(b1.isInside(b2));
            CHECK(b1 > b2);
        }

        THEN("the bigger one is not inside the smaller one")
        {
            CHECK_FALSE(b2.isInside(b1));
            CHECK_FALSE(b2 > b1);
        }
    }

    GIVEN("A body and two points, one inside and one outside the body")
    {
        auto b = Body({ 0, 0 }, 5);
        auto p1 = Vec2d(0, 0);
        auto p2 = Vec2d(6, 0);

        THEN("only one point is inside")
        {
            CHECK(b.isPointInside(p1));
            CHECK_FALSE(b.isPointInside(p2));
            CHECK(b > p1);
            CHECK_FALSE(b > p2);
        }
    }

    GIVEN("A body")
    {
        auto b = Body({ 1, 2 }, 2);

        THEN("it moves correctly")
        {
            b.move({ 1, 0 });
            CHECK(Vec2d(2, 2) == b.getPosition());
            b.move({ -2, -2 });
            CHECK(Vec2d(0, 0) == b.getPosition());
        }

        THEN("it can be copied")
        {
            auto copy = b;
            CHECK(b.getPosition() == copy.getPosition());
            CHECK(b.getRadius() == copy.getRadius());

            auto docy = Body({ 3, 4 }, 5);
            docy = b;
            CHECK(b.getPosition() == docy.getPosition());
            CHECK(b.getRadius() == docy.getRadius());
        }
    }
}
