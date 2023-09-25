#include <point_transformer.hpp>
#include <properties.hpp>

namespace PointTransformer
{

    static glm::vec2 rotate_point(glm::vec2 point, glm::vec2 relative, float angle)
    {
        glm::vec2 offset = point - relative;
        float x          = relative.x + offset.x * glm::cos(angle) - offset.y * glm::sin(angle);
        float y          = relative.y + offset.x * glm::sin(angle) + offset.y * glm::cos(angle);
        return glm::vec2(x, y);
    }

    static glm::vec2 affine(glm::vec2 point)
    {
        auto& a = properties.affine;
        float x = a.R0.x + a.Rx.x * point.x + a.Ry.x * point.y;
        float y = a.R0.y + a.Rx.y * point.x + a.Ry.y * point.y;
        return glm::vec2(x, y);
    }

    glm::vec2 transform(glm::vec2 point)
    {
        return affine(point);
    }


    glm::vec2 figure_transform(glm::vec2 point)
    {
        point += properties.figure.offset;
        point = rotate_point(point, properties.figure.rotate.point, glm::radians(properties.figure.rotate.angle));
        return transform(point);
    }
}// namespace PointTransformer
