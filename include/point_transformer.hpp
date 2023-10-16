#pragma once
#include <glm/glm.hpp>

namespace PointTransformer
{
    glm::vec2 transform(glm::vec2 point);
    glm::vec2 transform_with_euclidean(glm::vec2 point);
}// namespace PointTransformer
