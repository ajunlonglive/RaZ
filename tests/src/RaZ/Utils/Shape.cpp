#include "Catch.hpp"

#include "RaZ/Utils/Shape.hpp"

namespace {

//       Line 1         |      Line 2       |        Line 3        |       Line 4
//                      |                   |                      |
//                      |     [ 0; 1 ]      |  [ 1.5; 5 ]          |              [ 6; 6 ]
//                      |         |         |       \              |                /
//   ----------------   |         |         |         \            |              /
//   ^              ^   |         |         |           \          |            /
//  [ 0; 0 ]  [ 1; 0 ]  |         |         |             \        |          /
//                      |         |         |               \      |        /
//                      |     [ 0; 0 ]      |         [ 5.5; 2.5]  |  [ -10; -10 ]

const Raz::Line line1(Raz::Vec3f(0.f, 0.f, 0.f), Raz::Vec3f(1.f, 0.f, 0.f));
const Raz::Line line2(Raz::Vec3f(0.f, 0.f, 0.f), Raz::Vec3f(0.f, 1.f, 0.f));
const Raz::Line line3(Raz::Vec3f(1.5f, 5.f, 0.f), Raz::Vec3f(5.5f, 2.5f, 0.f));
const Raz::Line line4(Raz::Vec3f(-10.f, -10.f, 0.f), Raz::Vec3f(6.f, 6.f, 0.f));

//      Plane 1      |       Plane 2      |      Plane 3
//                   |                    |
//       normal      |   \      normal    |    normal      /
//         ^         |     \      ^       |       ^      /
//         |         |       \   /        |        \   /
//   ______|______   |         \/         |         \/
//                   |           \        |        /
//                   |             \      |      /
//     [ 0; 0 ]      |    [ 0; 0 ]   \    |    /   [ 0; 0 ]

const Raz::Plane plane1(1.f, Raz::Axis::Y);
const Raz::Plane plane2(0.5f, Raz::Vec3f(1.f, 1.f, 0.f).normalize());
const Raz::Plane plane3(0.5f, Raz::Vec3f(-1.f, 1.f, 0.f).normalize());

//      Sphere 1      |      Sphere 2     |      Sphere 3
//                    |                   |
//      .------.      |         .-"""-.   |                ^
//    .'        `.    |        /       \  |                |
//   /     ^      \   |        |[5; 10]|  |                +--->
//  |      |       |  |        \       /  |   .-"""-.
//  |      +--->   |  |         '-...-'   |  /       \
//   \   [0; 0]   /   |  ^                |  |   x   |
//    `._      _.'    |  |                |  \       /
//      `"----"'      |  +--->            |   '-...-'
//                    |                   |  [-10; -10]

const Raz::Sphere sphere1(Raz::Vec3f(0.f), 1.f);
const Raz::Sphere sphere2(Raz::Vec3f(5.f, 10.f, 0.f), 5.f);
const Raz::Sphere sphere3(Raz::Vec3f(-10.f, -10.f, 0.f), 1.f);

// These triangles are defined so that:
//  - triangle1 is laying flat slightly above 0
//  - triangle2 is standing, parallel to the Y/Z plane (facing the X direction)
//  - triangle3 is crooked, its head pointing to [ -X; +Y ], slightly below 0

const Raz::Triangle triangle1(Raz::Vec3f(-3.f, 0.5f, 3.f), Raz::Vec3f(3.f, 0.5f, 3.f), Raz::Vec3f(0.f, 0.5f, -6.f));
const Raz::Triangle triangle2(Raz::Vec3f(0.5f, -0.5f, 3.f), Raz::Vec3f(0.5f, -0.5f, -3.f), Raz::Vec3f(0.5f, 3.f, 0.f));
const Raz::Triangle triangle3(Raz::Vec3f(0.f, -1.f, 1.f), Raz::Vec3f(-1.5f, -1.5f, 0.f), Raz::Vec3f(0.f, -1.75f, -1.f));

//         _______________________
//        /|                    /|
//       / |                   / | / 1 -> [ 0.5; 0.5; 0.5 ]
//      |---------------------| < {  2 -> [   5;   5;   5 ]
//      |  |                  |  | \ 3 -> [  -6;  -5;   5 ]
//      |  |                  |  |
//      |  |                  |  |
//      |  |                  |  |
//      | /-------------------|-/
//      |/ ^                  |/
//      ---|-------------------
//         |
//  1 -> [ -0.5; -0.5; -0.5 ]
//  2 -> [    2;    3;   -5 ]
//  3 -> [  -10;  -10;   -5 ]

const Raz::AABB aabb1(Raz::Vec3f(-0.5f), Raz::Vec3f(0.5f));
const Raz::AABB aabb2(Raz::Vec3f(2.f, 3.f, -5.f), Raz::Vec3f(5.f));
const Raz::AABB aabb3(Raz::Vec3f(-10.f, -10.f, -5.f), Raz::Vec3f(-6.f, -5.f, 5.f));

} // namespace

TEST_CASE("Line basic") {
  // See: https://www.geogebra.org/m/fbq8scce

  CHECK(line1.computeCentroid() == Raz::Vec3f(0.5f, 0.f, 0.f));
  CHECK(line2.computeCentroid() == Raz::Vec3f(0.f, 0.5f, 0.f));
  CHECK(line3.computeCentroid() == Raz::Vec3f(3.5f, 3.75f, 0.f));
  CHECK(line4.computeCentroid() == Raz::Vec3f(-2.f, -2.f, 0.f));

  CHECK_THAT(line1.computeLength(), IsNearlyEqualTo(1.f));
  CHECK_THAT(line1.computeSquaredLength(), IsNearlyEqualTo(1.f));

  CHECK_THAT(line2.computeLength(), IsNearlyEqualTo(1.f));
  CHECK_THAT(line2.computeSquaredLength(), IsNearlyEqualTo(1.f));

  CHECK_THAT(line3.computeLength(), IsNearlyEqualTo(4.7169905f));
  CHECK_THAT(line3.computeSquaredLength(), IsNearlyEqualTo(22.25f));

  CHECK_THAT(line4.computeLength(), IsNearlyEqualTo(22.6274166f));
  CHECK_THAT(line4.computeSquaredLength(), IsNearlyEqualTo(512.f));
}

TEST_CASE("Line point containment") {
  CHECK(line1.contains(line1.getBeginPos()));
  CHECK(line1.contains(line1.getEndPos()));
  CHECK(line1.contains(line1.computeCentroid()));
  const Raz::Vec3f furtherEnd(line1.getEndPos().x() + std::numeric_limits<float>::epsilon() * 2.f, line1.getEndPos().y(), line1.getEndPos().z());
  CHECK_FALSE(line1.contains(furtherEnd));

  CHECK_FALSE(line2.contains(line2.computeCentroid() + Raz::Vec3f(0.f, 0.f, std::numeric_limits<float>::epsilon() * 2.f)));

  CHECK(line3.contains(line3.computeCentroid()));

  CHECK(line4.contains(line4.computeCentroid()));
}

TEST_CASE("Line-plane intersection") {
  CHECK_FALSE(line1.intersects(plane1));
  CHECK(line1.intersects(plane2));
  CHECK_FALSE(line1.intersects(plane3));

  CHECK(line2.intersects(plane1));
  CHECK(line2.intersects(plane2));
  CHECK(line2.intersects(plane3));

  CHECK_FALSE(line3.intersects(plane1));
  CHECK_FALSE(line3.intersects(plane2));
  CHECK(line3.intersects(plane3));

  CHECK(line4.intersects(plane1));
  CHECK(line4.intersects(plane2));
  CHECK_FALSE(line4.intersects(plane3));

  // Testing intersection in both ways, with a very close line
  const Raz::Plane testPlane(-5.f);
  const Raz::Line testLine(Raz::Vec3f(3.f, -4.9999f, 1.f), Raz::Vec3f(3.f, -5.0001f, 1.f));
  CHECK(testLine.intersects(testPlane));

  const Raz::Line reverseTestLine(testLine.getEndPos(), testLine.getBeginPos());
  CHECK(reverseTestLine.intersects(testPlane));
}

TEST_CASE("Line-AABB intersection") {
  // See: https://www.geogebra.org/m/fru9r3r6

  CHECK(line1.intersects(aabb1));
  CHECK_FALSE(line1.intersects(aabb2));
  CHECK_FALSE(line1.intersects(aabb3));

  CHECK(line2.intersects(aabb1));
  CHECK_FALSE(line2.intersects(aabb2));
  CHECK_FALSE(line2.intersects(aabb3));

  CHECK_FALSE(line3.intersects(aabb1));
  CHECK(line3.intersects(aabb2));
  CHECK_FALSE(line3.intersects(aabb3));

  CHECK(line4.intersects(aabb1));
  CHECK(line4.intersects(aabb2));
  CHECK(line4.intersects(aabb3));
}

TEST_CASE("Line translation") {
  Raz::Line line1Copy = line1;

  line1Copy.translate(Raz::Vec3f(1.f));
  CHECK(line1Copy.getBeginPos() == line1.getBeginPos() + Raz::Vec3f(1.f));
  CHECK(line1Copy.getEndPos() == line1.getEndPos() + Raz::Vec3f(1.f));

  line1Copy.translate(Raz::Vec3f(-1.f));
  CHECK(line1Copy.getBeginPos() == line1.getBeginPos());
  CHECK(line1Copy.getEndPos() == line1.getEndPos());
}

TEST_CASE("Line point projection") {
  CHECK(line1.computeProjection(line1.getBeginPos()) == line1.getBeginPos());
  CHECK(line1.computeProjection(line1.getEndPos()) == line1.getEndPos());
  const Raz::Vec3f centroid1 = line1.computeCentroid();
  CHECK(line1.computeProjection(centroid1) == centroid1);

  const Raz::Vec3f centroid2 = line2.computeCentroid();
  CHECK(line2.computeProjection(centroid2) == centroid2);
  const Raz::Vec3f furtherEnd(line2.getEndPos().x(), line2.getEndPos().y() * 2.f, line2.getEndPos().z());
  CHECK(line2.computeProjection(furtherEnd) == line2.getEndPos()); // The result is clamped to the line's bounds
  CHECK(line2.computeProjection(Raz::Vec3f(-100.f, 0.5f, 0.f)) == centroid2);

  const Raz::Vec3f centroid3 = line3.computeCentroid();
  CHECK(line3.computeProjection(centroid3) == centroid3);
  CHECK(line3.computeProjection(centroid3 + Raz::Axis::Z) == centroid3);
  CHECK(line3.computeProjection(Raz::Vec3f(3.543f, 3.478f, 0.0239f)) == Raz::Vec3f(3.653168678f, 3.654269695f, 0.f));

  const Raz::Vec3f centroid4 = line4.computeCentroid();
  CHECK(line4.computeProjection(centroid4) == centroid4);
  //   x      /
  //    \   /
  //      P
  //    /
  //  /
  CHECK(line4.computeProjection(centroid4 - Raz::Axis::X + Raz::Axis::Y) == centroid4);
  const Raz::Vec3f furtherBegin(line4.getBeginPos().x(), line4.getBeginPos().y() + 1.f, line4.getBeginPos().z());
  CHECK(line4.computeProjection(furtherBegin) == Raz::Vec3f(-9.5f, -9.5f, 0.f));
}

TEST_CASE("Line bounding box") {
  CHECK(line1.computeBoundingBox() == Raz::AABB(Raz::Vec3f(0.f), Raz::Vec3f(1.f, 0.f, 0.f)));
  CHECK(line2.computeBoundingBox() == Raz::AABB(Raz::Vec3f(0.f), Raz::Vec3f(0.f, 1.f, 0.f)));
  CHECK(line3.computeBoundingBox() == Raz::AABB(Raz::Vec3f(1.5f, 2.5f, 0.f), Raz::Vec3f(5.5f, 5.f, 0.f)));
  CHECK(line4.computeBoundingBox() == Raz::AABB(Raz::Vec3f(-10.f, -10.f, 0.f), Raz::Vec3f(6.f, 6.f, 0.f)));
}

TEST_CASE("Line equality") {
  CHECK(line1 == line1);
  CHECK(line2 == line2);

  CHECK(line1 != line2);
  CHECK(line1 != line3);

  Raz::Line line1Copy = line1;
  line1Copy.translate(Raz::Vec3f(std::numeric_limits<float>::epsilon()));
  CHECK(line1Copy == line1);
}

TEST_CASE("Plane basic") {
  const Raz::Plane testPlane1(1.f, Raz::Axis::Y);
  CHECK(testPlane1.getDistance() == 1.f);

  constexpr Raz::Vec3f planePos = Raz::Vec3f(0.f, 1.f, 0.f);
  const Raz::Plane testPlane2(planePos, Raz::Axis::Y);
  CHECK(testPlane2.computeCentroid() == planePos);

  const Raz::Plane testPlane3(Raz::Vec3f(1.f, 1.f, 0.f), Raz::Vec3f(-1.f, 1.f, -1.f), Raz::Vec3f(0.f, 1.f, 1.f));

  // Checking that the 3 planes are strictly equal to each other
  CHECK(testPlane1.computeCentroid() == testPlane2.computeCentroid());
  CHECK(testPlane1.getNormal() == testPlane2.getNormal());

  CHECK(testPlane2.computeCentroid() == testPlane3.computeCentroid());
  CHECK(testPlane2.getNormal() == testPlane3.getNormal());
}

TEST_CASE("Plane-plane intersection") {
  const Raz::Plane testPlane(2.f, -Raz::Axis::Y);

  CHECK_FALSE(plane1.intersects(testPlane));
  CHECK(plane2.intersects(testPlane));
  CHECK(plane3.intersects(testPlane));

  CHECK(plane1.intersects(plane2));
  CHECK(plane1.intersects(plane3));
  CHECK(plane2.intersects(plane3));

  // A plane should not intersect itself
  CHECK_FALSE(plane1.intersects(plane1));
  CHECK_FALSE(plane2.intersects(plane2));
  CHECK_FALSE(plane3.intersects(plane3));
}

TEST_CASE("Plane-sphere intersection") {
  // See: https://www.geogebra.org/m/r3brvcsn

  CHECK(plane1.intersects(sphere1));
  CHECK_FALSE(plane1.intersects(sphere2));
  CHECK_FALSE(plane1.intersects(sphere3));

  CHECK(plane2.intersects(sphere1));
  CHECK_FALSE(plane2.intersects(sphere2));
  CHECK_FALSE(plane2.intersects(sphere3));

  CHECK(plane3.intersects(sphere1));
  CHECK(plane3.intersects(sphere2));
  CHECK(plane3.intersects(sphere3));
}

TEST_CASE("Plane translation") {
  {
    Raz::Plane plane1Copy = plane1;

    plane1Copy.translate(Raz::Vec3f(1.f));
    CHECK(plane1Copy.getDistance() == plane1.getDistance() + 1.f);
    CHECK(plane1Copy.getNormal().strictlyEquals(plane1.getNormal())); // The normal doesn't change

    plane1Copy.translate(Raz::Vec3f(-1.f));
    CHECK(plane1Copy.getDistance() == plane1.getDistance());
    CHECK(plane1Copy.getNormal().strictlyEquals(plane1.getNormal()));
  }

  {
    Raz::Plane plane2Copy = plane2;

    plane2Copy.translate(Raz::Vec3f(1.f));
    CHECK(plane2Copy.getDistance() == plane2.getDistance() + std::sqrt(2.f) /* 1.4142135f */);
    CHECK(plane2Copy.getNormal().strictlyEquals(plane2.getNormal()));

    plane2Copy.translate(Raz::Vec3f(-1.f));
    CHECK(Raz::FloatUtils::areNearlyEqual(plane2Copy.getDistance(), plane2.getDistance(), 15e-8f)); // A small error has been introduced due to the operations
    CHECK(plane2Copy.getNormal().strictlyEquals(plane2.getNormal()));
  }

  {
    Raz::Plane plane3Copy = plane3;

    plane3Copy.translate(Raz::Vec3f(1.f));
    CHECK(plane3Copy.getDistance() == plane3.getDistance()); // The point lies on the plane; nothing is done
    CHECK(plane3Copy.getNormal().strictlyEquals(plane3.getNormal()));

    plane3Copy.translate(Raz::Vec3f(-1.f));
    CHECK(plane3Copy.getDistance() == plane3.getDistance());
    CHECK(plane3Copy.getNormal().strictlyEquals(plane3.getNormal()));
  }
}

TEST_CASE("Plane equality") {
  CHECK(plane1 == plane1);
  CHECK(plane2 == plane2);

  CHECK(plane1 != plane2);
  CHECK(plane1 != plane3);

  Raz::Plane plane1Copy = plane1;
  plane1Copy.translate(Raz::Vec3f(std::numeric_limits<float>::epsilon()));
  CHECK(plane1Copy == plane1);
}

TEST_CASE("Sphere point containment") {
  CHECK(sphere1.contains(sphere1.getCenter()));
  CHECK(sphere1.contains(Raz::Vec3f(0.f, 1.f, 0.f))); // Right on the sphere's border
  CHECK_FALSE(sphere1.contains(Raz::Vec3f(0.f, 1.f + std::numeric_limits<float>::epsilon(), 0.f)));

  CHECK(sphere2.contains(sphere2.getCenter()));
  CHECK_FALSE(sphere2.contains(Raz::Vec3f(0.f)));
  CHECK_FALSE(sphere2.contains(sphere2.getCenter() / 2.f));

  CHECK(sphere3.contains(sphere3.getCenter()));
  CHECK_FALSE(sphere3.contains(-sphere3.getCenter()));
  CHECK_FALSE(sphere3.contains(Raz::Vec3f(sphere3.getCenter().x(), sphere3.getCenter().y(), sphere3.getRadius() + std::numeric_limits<float>::epsilon())));
}

TEST_CASE("Sphere-sphere intersection") {
  // See: https://www.geogebra.org/m/tsfueyp5

  const Raz::Sphere testSphere(Raz::Vec3f(0.f), 13.15f); // This sphere intersects all the test ones

  CHECK(sphere1.intersects(sphere1));
  CHECK_FALSE(sphere1.intersects(sphere2));
  CHECK_FALSE(sphere1.intersects(sphere3));
  CHECK(sphere1.intersects(testSphere)); // Containment implies intersection

  CHECK(sphere2.intersects(sphere2));
  CHECK_FALSE(sphere2.intersects(sphere3));
  CHECK(sphere2.intersects(testSphere));

  CHECK(sphere3.intersects(sphere3));
  CHECK(sphere3.intersects(Raz::Sphere(sphere3.getCenter() + std::numeric_limits<float>::epsilon(), 0.f))); // Intersects with a 0-radius sphere
  CHECK(sphere3.intersects(testSphere));
  CHECK(testSphere.intersects(sphere3));
}

TEST_CASE("Sphere translation") {
  Raz::Sphere sphere1Copy = sphere1;

  sphere1Copy.translate(Raz::Vec3f(1.f));
  CHECK(sphere1Copy.getCenter() == sphere1.getCenter() + Raz::Vec3f(1.f));
  CHECK(sphere1Copy.getRadius() == sphere1.getRadius()); // The radius doesn't change

  sphere1Copy.translate(Raz::Vec3f(-1.f));
  CHECK(sphere1Copy.getCenter() == sphere1.getCenter());
  CHECK(sphere1Copy.getRadius() == sphere1.getRadius());
}

TEST_CASE("Sphere bounding box") {
  CHECK(sphere1.computeBoundingBox() == Raz::AABB(Raz::Vec3f(-1.f), Raz::Vec3f(1.f)));
  CHECK(sphere2.computeBoundingBox() == Raz::AABB(Raz::Vec3f(0.f, 5.f, -5.f), Raz::Vec3f(10.f, 15.f, 5.f)));
  CHECK(sphere3.computeBoundingBox() == Raz::AABB(Raz::Vec3f(-11.f, -11.f, -1.f), Raz::Vec3f(-9.f, -9.f, 1.f)));
}

TEST_CASE("Sphere equality") {
  CHECK(sphere1 == sphere1);
  CHECK(sphere2 == sphere2);

  CHECK(sphere1 != sphere2);
  CHECK(sphere1 != sphere3);

  Raz::Sphere sphere1Copy = sphere1;
  sphere1Copy.translate(Raz::Vec3f(std::numeric_limits<float>::epsilon()));
  CHECK(sphere1Copy == sphere1);
}

TEST_CASE("Triangle basic") {
  // See: https://www.geogebra.org/m/gszsn33d

  CHECK(triangle1.computeCentroid() == Raz::Vec3f(0.f, 0.5f, 0.f));
  CHECK(triangle1.computeNormal() == Raz::Axis::Y);

  CHECK(triangle2.computeCentroid() == Raz::Vec3f(0.5f, 0.666666666f, 0.f));
  CHECK(triangle2.computeNormal() == Raz::Axis::X);

  CHECK(triangle3.computeCentroid() == Raz::Vec3f(-0.5f, -1.416666666f, 0.f));
  CHECK_THAT(triangle3.computeNormal(), IsNearlyEqualToVector(Raz::Vec3f(0.077791f, -0.93349177f, 0.35005942f)));
}

TEST_CASE("Triangle translation") {
  Raz::Triangle triangle1Copy = triangle1;

  triangle1Copy.translate(Raz::Vec3f(1.f));
  CHECK(triangle1Copy.getFirstPos() == triangle1.getFirstPos() + Raz::Vec3f(1.f));
  CHECK(triangle1Copy.getSecondPos() == triangle1.getSecondPos() + Raz::Vec3f(1.f));
  CHECK(triangle1Copy.getThirdPos() == triangle1.getThirdPos() + Raz::Vec3f(1.f));

  triangle1Copy.translate(Raz::Vec3f(-1.f));
  CHECK(triangle1Copy.getFirstPos() == triangle1.getFirstPos());
  CHECK(triangle1Copy.getSecondPos() == triangle1.getSecondPos());
  CHECK(triangle1Copy.getThirdPos() == triangle1.getThirdPos());
}

TEST_CASE("Triangle bounding box") {
  CHECK(triangle1.computeBoundingBox() == Raz::AABB(Raz::Vec3f(-3.f, 0.5f, -6.f), Raz::Vec3f(3.f, 0.5f, 3.f)));
  CHECK(triangle2.computeBoundingBox() == Raz::AABB(Raz::Vec3f(0.5f, -0.5f, -3.f), Raz::Vec3f(0.5f, 3.f, 3.f)));
  CHECK(triangle3.computeBoundingBox() == Raz::AABB(Raz::Vec3f(-1.5f, -1.75f, -1.f), Raz::Vec3f(0.f, -1.f, 1.f)));
}

TEST_CASE("Triangle clockwiseness") {
  CHECK(triangle1.isCounterClockwise(Raz::Axis::Y));
  CHECK(triangle2.isCounterClockwise(Raz::Axis::X));
  CHECK(triangle3.isCounterClockwise(-Raz::Axis::Y)); // Pointing roughly towards -Y

  // Creating two triangles with the same points but in a different ordering
  Raz::Triangle testTriangle1(Raz::Vec3f(-1.f, 0.f, 0.f), Raz::Vec3f(0.f, 1.f, 0.f), Raz::Vec3f(1.f, 0.f, 0.f));
  Raz::Triangle testTriangle2(Raz::Vec3f(1.f, 0.f, 0.f), Raz::Vec3f(0.f, 1.f, 0.f), Raz::Vec3f(-1.f, 0.f, 0.f));

  CHECK_FALSE(testTriangle1.isCounterClockwise(Raz::Axis::Z));
  testTriangle1.makeCounterClockwise(Raz::Axis::Z);
  CHECK(testTriangle1.isCounterClockwise(Raz::Axis::Z));

  // Trying to make it counter-clockwise while it already is has no effect
  CHECK(testTriangle2.isCounterClockwise(Raz::Axis::Z));
  testTriangle2.makeCounterClockwise(Raz::Axis::Z);
  CHECK(testTriangle2.isCounterClockwise(Raz::Axis::Z));
}

TEST_CASE("Triangle equality") {
  CHECK(triangle1 == triangle1);
  CHECK(triangle2 == triangle2);

  CHECK(triangle1 != triangle2);
  CHECK(triangle1 != triangle3);

  Raz::Triangle triangle1Copy = triangle1;
  triangle1Copy.translate(Raz::Vec3f(std::numeric_limits<float>::epsilon()));
  CHECK(triangle1Copy == triangle1);
}

TEST_CASE("AABB basic") {
  CHECK(aabb1.computeCentroid() == Raz::Vec3f(0.f));
  CHECK(aabb2.computeCentroid() == Raz::Vec3f(3.5f, 4.f, 0.f));
  CHECK(aabb3.computeCentroid() == Raz::Vec3f(-8.f, -7.5f, 0.f));

  CHECK(aabb1.computeHalfExtents() == Raz::Vec3f(0.5f));
  CHECK(aabb2.computeHalfExtents() == Raz::Vec3f(1.5f, 1.f, 5.f));
  CHECK(aabb3.computeHalfExtents() == Raz::Vec3f(2.f, 2.5f, 5.f));
}

TEST_CASE("AABB point containment") {
  // See: https://www.geogebra.org/m/kwkkt9ry

  CHECK(aabb1.contains(aabb1.computeCentroid()));
  CHECK(aabb1.contains(aabb1.getMinPosition()));
  CHECK(aabb1.contains(aabb1.getMaxPosition()));

  const Raz::Vec3f point1(-0.25f, -0.5f, -0.5f); // Should be contained by aabb1
  const Raz::Vec3f point2(   4.f,   3.f,   0.f); // Should be contained by aabb2 (lying on a face)
  const Raz::Vec3f point3(  -7.f,  -7.f,  -3.f); // Should be contained by aabb3
  const Raz::Vec3f point4(-4.95f,  -6.f,   0.f); // Should be contained by none (really close to aabb3)
  const Raz::Vec3f point5(  1.5f,   2.f,   0.f); // Should be contained by none (between aabb1 & aabb2)

  CHECK(aabb1.contains(point1));
  CHECK_FALSE(aabb2.contains(point1));
  CHECK_FALSE(aabb3.contains(point1));

  CHECK_FALSE(aabb1.contains(point2));
  CHECK(aabb2.contains(point2));
  CHECK_FALSE(aabb3.contains(point2));

  CHECK_FALSE(aabb1.contains(point3));
  CHECK_FALSE(aabb2.contains(point3));
  CHECK(aabb3.contains(point3));

  CHECK_FALSE(aabb1.contains(point4));
  CHECK_FALSE(aabb2.contains(point4));
  CHECK_FALSE(aabb3.contains(point4));

  CHECK_FALSE(aabb1.contains(point5));
  CHECK_FALSE(aabb2.contains(point5));
  CHECK_FALSE(aabb3.contains(point5));
}

TEST_CASE("AABB-AABB intersection") {
  CHECK(aabb1.intersects(aabb1)); // A box always intersects itself
  CHECK(aabb2.intersects(aabb2));
  CHECK(aabb3.intersects(aabb3));

  {
    const Raz::AABB largerBox1(aabb1.getMinPosition() - Raz::Vec3f(0.0000001f), aabb1.getMaxPosition() + Raz::Vec3f(0.0000001f));
    REQUIRE_FALSE(aabb1.getMinPosition().strictlyEquals(largerBox1.getMinPosition()));
    REQUIRE_FALSE(aabb1.getMaxPosition().strictlyEquals(largerBox1.getMaxPosition()));

    CHECK(aabb1.intersects(largerBox1));
    CHECK(largerBox1.intersects(aabb1)); // The intersection is commutative
  }

  {
    const Raz::AABB smallerBox2(aabb2.getMinPosition() + Raz::Vec3f(0.000001f), aabb2.getMaxPosition() - Raz::Vec3f(0.000001f));
    REQUIRE_FALSE(aabb2.getMinPosition().strictlyEquals(smallerBox2.getMinPosition()));
    REQUIRE_FALSE(aabb2.getMaxPosition().strictlyEquals(smallerBox2.getMaxPosition()));

    CHECK(aabb2.intersects(smallerBox2));
    CHECK(smallerBox2.intersects(aabb2));
  }

  {
    const Raz::AABB hugeBox(Raz::Vec3f(-10.f, -10.f, -5.f), Raz::Vec3f(5.f));

    CHECK(aabb1.intersects(hugeBox));
    CHECK(hugeBox.intersects(aabb1));

    CHECK(aabb2.intersects(hugeBox));
    CHECK(hugeBox.intersects(aabb2));

    CHECK(aabb3.intersects(hugeBox));
    CHECK(hugeBox.intersects(aabb3));
  }
}

TEST_CASE("AABB translation") {
  Raz::AABB aabb1Copy = aabb1;

  aabb1Copy.translate(Raz::Vec3f(1.f));
  CHECK(aabb1Copy.getMinPosition() == aabb1.getMinPosition() + Raz::Vec3f(1.f));
  CHECK(aabb1Copy.getMaxPosition() == aabb1.getMaxPosition() + Raz::Vec3f(1.f));

  aabb1Copy.translate(Raz::Vec3f(-1.f));
  CHECK(aabb1Copy.getMinPosition() == aabb1.getMinPosition());
  CHECK(aabb1Copy.getMaxPosition() == aabb1.getMaxPosition());
}

TEST_CASE("AABB bounding box") {
  CHECK(aabb1.computeBoundingBox() == aabb1);
  CHECK(aabb2.computeBoundingBox() == aabb2);
  CHECK(aabb3.computeBoundingBox() == aabb3);
}

TEST_CASE("AABB equality") {
  CHECK(aabb1 == aabb1);
  CHECK(aabb2 == aabb2);

  CHECK(aabb1 != aabb2);
  CHECK(aabb1 != aabb3);

  Raz::AABB aabb1Copy = aabb1;
  aabb1Copy.translate(Raz::Vec3f(std::numeric_limits<float>::epsilon()));
  CHECK(aabb1Copy == aabb1);
}
