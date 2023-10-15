#include <glm/ext/matrix_transform.hpp>
#include <point_transformer.hpp>
#include <properties.hpp>

namespace PointTransformer
{

    static glm::mat3 move_matrix()
    {
        if (properties.figure.move.enable)
        {
            glm::mat3 result(1.0f);
            result[2][0] = properties.figure.move.offset.x;
            result[2][1] = properties.figure.move.offset.y;
            return result;
        }

        return glm::mat3(1.0f);
    }

    static glm::mat3 rotation_matrix()
    {
        if (properties.figure.rotate.enable)
        {
            float x     = properties.figure.rotate.point.x;
            float y     = properties.figure.rotate.point.y;
            float angle = glm::radians(properties.figure.rotate.angle);
            float csa   = glm::cos(angle);
            float sna   = glm::sin(angle);

            return {{csa, sna, 0.f}, //
                    {-sna, csa, 0.0},//
                    {-x * (csa - 1.0f) + y * sna, -y * (csa - 1.0f) - x * sna, 1.0f}};
        }

        return glm::mat3(1.0f);
    }

    static glm::mat3 scale_matrix()
    {
        if (properties.figure.scale.enable)
        {
            glm::mat3 result(1.0f);
            result[0][0] = properties.figure.scale.scale.x;
            result[1][1] = properties.figure.scale.scale.y;
            return result;
        }
        return glm::mat3(1.0f);
    }

    static glm::mat3 xz_symmentry_matrix()
    {
        if (properties.figure.symmetry.XZ)
        {
            static glm::mat3 result(1.0f);
            result[1][1] = -1.f;
            return result;
        }
        return glm::mat3(1.0f);
    }

    static glm::mat3 yz_symmentry_matrix()
    {
        if (properties.figure.symmetry.YZ)
        {
            static glm::mat3 result(1.0f);
            result[0][0] = -1.f;
            return result;
        }
        return glm::mat3(1.0f);
    }

    static glm::mat3 affine_matrix()
    {
        if (properties.affine.enable)
        {
            auto& params = properties.affine;

            return {{params.Rx.x, params.Ry.x, 0.f},
                    {params.Rx.y, params.Ry.y, 0.0f},
                    {params.R0.x, params.R0.y, 1.0f}};
        }
        return glm::mat3(1.0f);
    }

    static glm::mat3 projective_matrix()
    {
        if (properties.projective.enable)
        {
            auto& params = properties.projective;

            return {{params.Rx.x * params.W.x, params.Ry.x * params.W.x, params.W.x},
                    {params.Rx.y * params.W.y, params.Ry.y * params.W.y, params.W.y},
                    {params.R0.x * params.W0, params.R0.y * params.W0, params.W0}};
        }
        return glm::mat3(1.0f);
    }

    glm::vec2 transform(glm::vec2 point)
    {
        glm::vec3 output = projective_matrix()//
                           * affine_matrix()  //
                           * glm::vec3(point, 1.0f);

        return glm::vec2(output / output.z);
    }


    glm::vec2 figure_transform(glm::vec2 point)
    {
        glm::vec3 output = projective_matrix()    //
                           * affine_matrix()      //
                           * yz_symmentry_matrix()//
                           * xz_symmentry_matrix()//
                           * scale_matrix()       //
                           * rotation_matrix()    //
                           * move_matrix()        //
                           * glm::vec3(point, 1.0f);
        return glm::vec2(output / output.z);
    }
}// namespace PointTransformer
