#include "RaZ/Math/Constants.hpp"

namespace Raz {

template <typename T>
constexpr Quaternion<T>::Quaternion(Radians<T> angle, const Vec3<T>& axis) noexcept {
  const T halfAngle = angle.value / 2;
  const T sinAngle  = std::sin(halfAngle);

  m_real      = std::cos(halfAngle);
  m_complexes = axis * sinAngle;
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::normalize() const noexcept {
  Quaternion<T> res = *this;
  const T sqNorm    = computeSquaredNorm();

  if (sqNorm > 0) {
    const T invSqNorm = 1 / std::sqrt(sqNorm);

    res.m_real      *= invSqNorm;
    res.m_complexes *= invSqNorm;
  }

  return res;
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::lerp(const Quaternion& quat, T coeff) const noexcept {
  assert("Error: The interpolation coefficient must be between 0 & 1." && (coeff >= 0 && coeff <= 1));

  const T currCoeff = 1 - coeff;
  return lerp(quat, currCoeff, coeff);
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::nlerp(const Quaternion& quat, T coeff) const noexcept {
  assert("Error: The interpolation coefficient must be between 0 & 1." && (coeff >= 0 && coeff <= 1));

  const T currCoeff = 1 - coeff;
  // A rotation may be represented by two opposite quaternions; should the dot product between those be negative,
  //  one quaternion must be negated. This is made by negating the coefficient
  const T otherCoeff = (dot(quat) > 0 ? coeff : -coeff);

  return lerp(quat, currCoeff, otherCoeff).normalize();
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::slerp(const Quaternion& quat, T coeff) const noexcept {
  assert("Error: The interpolation coefficient must be between 0 & 1." && (coeff >= 0 && coeff <= 1));
  assert("Error: Quaternions must be normalized for a slerp to be performed." && FloatUtils::areNearlyEqual(computeSquaredNorm(), static_cast<T>(1))
                                                                              && FloatUtils::areNearlyEqual(quat.computeSquaredNorm(), static_cast<T>(1)));

  const T cosAngle = dot(quat);

  T currCoeff {};
  T otherCoeff {};

  // Checking the angle between the quaternions; if the angle is sufficient, perform an actual spherical interpolation
  if (std::abs(cosAngle) < static_cast<T>(0.99999)) {
    const T angle       = std::acos(cosAngle);
    const T invSinAngle = 1 / std::sin(angle);

    currCoeff  = std::sin((1 - coeff) * angle) * invSinAngle;
    otherCoeff = std::sin(coeff * angle) * invSinAngle;
  } else {
    // If the angle is really small, a simple linear interpolation is performed
    currCoeff  = 1 - coeff;
    otherCoeff = coeff;
  }

  // Negating one quaternion if the angle is negative
  otherCoeff = (cosAngle > 0 ? otherCoeff : -otherCoeff);
  return lerp(quat, currCoeff, otherCoeff);
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::conjugate() const noexcept {
  Quaternion<T> res = *this;
  res.m_complexes   = -m_complexes;

  return res;
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::inverse() const noexcept {
  Quaternion<T> res = *this;
  const T sqNorm    = computeSquaredNorm();

  if (sqNorm > 0) {
    const T invSqNorm = 1 / sqNorm;

    res.m_real      *= invSqNorm;
    res.m_complexes *= -invSqNorm;
  }

  return res;
}

template <typename T>
constexpr Mat4<T> Quaternion<T>::computeMatrix() const noexcept(std::is_integral_v<T> || std::numeric_limits<T>::is_iec559) {
  const T invSqNorm = 1 / computeSquaredNorm();

  const T xx = (2 * m_complexes.x() * m_complexes.x()) * invSqNorm;
  const T yy = (2 * m_complexes.y() * m_complexes.y()) * invSqNorm;
  const T zz = (2 * m_complexes.z() * m_complexes.z()) * invSqNorm;

  const T xy = (2 * m_complexes.x() * m_complexes.y()) * invSqNorm;
  const T xz = (2 * m_complexes.x() * m_complexes.z()) * invSqNorm;
  const T yz = (2 * m_complexes.y() * m_complexes.z()) * invSqNorm;

  const T xw = (2 * m_complexes.x() * m_real) * invSqNorm;
  const T yw = (2 * m_complexes.y() * m_real) * invSqNorm;
  const T zw = (2 * m_complexes.z() * m_real) * invSqNorm;

  return Mat4<T>(1 - yy - zz,       xy - zw,           xz + yw,           static_cast<T>(0),
                 xy + zw,           1 - xx - zz,       yz - xw,           static_cast<T>(0),
                 xz - yw,           yz + xw,           1 - xx - yy,       static_cast<T>(0),
                 static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
}

template <typename T>
constexpr Vec3<T> Quaternion<T>::operator*(const Vec3<T>& vec) const noexcept {
  // Quaternion/vector multiplications are supposed to be made with unit quaternions only, hence the conjugation instead of the inversion
  // Most likely because of the error accumulation due to floating-point numbers, the norm of a supposedly unit quaternion is almost never very close to 1
  // That norm is thus not checked here; it may require changing to an actual inversion in the future to avoid further approximation errors

  Quaternion<T> vecQuat(0.f, vec.x(), vec.y(), vec.z());
  vecQuat = *this * vecQuat * conjugate();
  return vecQuat.m_complexes;
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::operator*(const Quaternion& quat) const noexcept {
  Quaternion<T> res = *this;
  res *= quat;
  return res;
}

template <typename T>
constexpr Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& quat) noexcept {
  const Quaternion<T> copy = *this;

  m_real = copy.m_real          * quat.m_real
         - copy.m_complexes.x() * quat.m_complexes.x()
         - copy.m_complexes.y() * quat.m_complexes.y()
         - copy.m_complexes.z() * quat.m_complexes.z();

  m_complexes.x() = copy.m_real          * quat.m_complexes.x()
                  + copy.m_complexes.x() * quat.m_real
                  + copy.m_complexes.y() * quat.m_complexes.z()
                  - copy.m_complexes.z() * quat.m_complexes.y();

  m_complexes.y() = copy.m_real          * quat.m_complexes.y()
                  - copy.m_complexes.x() * quat.m_complexes.z()
                  + copy.m_complexes.y() * quat.m_real
                  + copy.m_complexes.z() * quat.m_complexes.x();

  m_complexes.z() = copy.m_real          * quat.m_complexes.z()
                  + copy.m_complexes.x() * quat.m_complexes.y()
                  - copy.m_complexes.y() * quat.m_complexes.x()
                  + copy.m_complexes.z() * quat.m_real;

  return *this;
}

template <typename T>
constexpr bool Quaternion<T>::operator==(const Quaternion& quat) const noexcept {
  return FloatUtils::areNearlyEqual(*this, quat);
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Quaternion<T>& quat) {
  stream << "[ " << quat.w() << ", " << quat.x() << ", " << quat.y() << ", " << quat.z() << " ]";
  return stream;
}

template <typename T>
constexpr Quaternion<T> Quaternion<T>::lerp(const Quaternion& quat, T currCoeff, T otherCoeff) const noexcept {
  return Quaternion<T>(m_real          * currCoeff + quat.m_real          * otherCoeff,
                       m_complexes.x() * currCoeff + quat.m_complexes.x() * otherCoeff,
                       m_complexes.y() * currCoeff + quat.m_complexes.y() * otherCoeff,
                       m_complexes.z() * currCoeff + quat.m_complexes.z() * otherCoeff);
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& vec, const Quaternion<T>& quat) {
  // Quaternion/vector multiplications are supposed to be made with unit quaternions only, hence the conjugation instead of the inversion
  // Most likely because of the error accumulation due to floating-point numbers, the norm of a supposedly unit quaternion is almost never very close to 1
  // That norm is thus not checked here; it may require changing to an actual inversion in the future to avoid further approximation errors

  Quaternion<T> vecQuat(0.f, vec.x(), vec.y(), vec.z());
  vecQuat = quat.conjugate() * vecQuat * quat;
  return Vec3<T>(vecQuat.x(), vecQuat.y(), vecQuat.z());
}

} // namespace Raz
