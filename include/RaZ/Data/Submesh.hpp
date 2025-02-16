#pragma once

#ifndef RAZ_SUBMESH_HPP
#define RAZ_SUBMESH_HPP

#include "RaZ/Render/GraphicObjects.hpp"
#include "RaZ/Utils/Shape.hpp"

#include <functional>
#include <memory>

namespace Raz {

class Submesh {
public:
  Submesh() noexcept = default;
  Submesh(const Submesh&) = delete;
  Submesh(Submesh&&) noexcept = default;

  const std::vector<Vertex>& getVertices() const { return m_vertices; }
  std::vector<Vertex>& getVertices() { return m_vertices; }
  std::size_t getVertexCount() const { return m_vertices.size(); }
  const std::vector<unsigned int>& getLineIndices() const { return m_lineIndices; }
  std::vector<unsigned int>& getLineIndices() { return m_lineIndices; }
  std::size_t getLineIndexCount() const { return m_lineIndices.size(); }
  const std::vector<unsigned int>& getTriangleIndices() const { return m_triangleIndices; }
  std::vector<unsigned int>& getTriangleIndices() { return m_triangleIndices; }
  std::size_t getTriangleIndexCount() const { return m_triangleIndices.size(); }
  const AABB& getBoundingBox() const { return m_boundingBox; }

  /// Computes & updates the submesh's bounding box.
  /// \return Submesh's bounding box.
  const AABB& computeBoundingBox();

  Submesh& operator=(const Submesh&) = delete;
  Submesh& operator=(Submesh&&) noexcept = default;

private:
  std::vector<Vertex> m_vertices {};
  std::vector<unsigned int> m_lineIndices {};
  std::vector<unsigned int> m_triangleIndices {};

  AABB m_boundingBox = AABB(Vec3f(), Vec3f());
};

} // namespace Raz

#endif // RAZ_SUBMESH_HPP
