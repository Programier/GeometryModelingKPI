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

    static glm::vec2 projective(glm::vec2 point)
    {
        auto& proj        = properties.projective;
        float denominator = proj.W0 + proj.W.x * point.x + proj.W.y * point.y;
        float x = (proj.R0.x * proj.W0 + proj.Rx.x * proj.W.x * point.x + proj.Ry.x * proj.W.y * point.y) / denominator;
        float y = (proj.R0.y * proj.W0 + proj.Rx.y * proj.W.x * point.x + proj.Ry.y * proj.W.y * point.y) / denominator;
        return glm::vec2(x, y);
    }

    glm::vec2 transform(glm::vec2 point)
    {
        if (properties.affine.enable)
        {
            point = affine(point);
        }

        if (properties.projective.enable)
        {
            point = projective(point);
        }
        return point;
    }


    glm::vec2 figure_transform(glm::vec2 point)
    {
        point += properties.figure.offset;
        point = rotate_point(point, properties.figure.rotate.point, glm::radians(properties.figure.rotate.angle));
        return transform(point);
    }
}// namespace PointTransformer
