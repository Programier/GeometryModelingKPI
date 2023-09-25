#include <figure.hpp>
#include <point_transformer.hpp>
#include <properties.hpp>


Figure::Figure() = default;

Figure& Figure::instance()
{
    static Figure figure;
    return figure;
}

void Figure::add_offset(glm::vec2 offset)
{
    Line& line = _M_lines.back();
    line.push_back(line.back() + offset);
}


void Figure::build_arc(glm::vec2 center, float radius, float start_angle, float end_angle, int num_segments)
{
    float angle_step = (end_angle - start_angle) / static_cast<float>(num_segments);

    for (int i = 1; i <= num_segments; ++i)
    {
        float angle = start_angle + i * angle_step;
        glm::vec2 p1(center.x + radius * cos(angle), center.y + radius * sin(angle));
        _M_lines.back().push_back(p1);
    }
}


bool Figure::build()
{
    clear();

    const float A = properties.figure_prop(PROP_INDEX_A);
    const float B = properties.figure_prop(PROP_INDEX_B);
    const float C = properties.figure_prop(PROP_INDEX_C);
    const float D = properties.figure_prop(PROP_INDEX_D);
    const float E = properties.figure_prop(PROP_INDEX_E);
    const float F = properties.figure_prop(PROP_INDEX_F);
    const float G = properties.figure_prop(PROP_INDEX_G);
    const float H = properties.figure_prop(PROP_INDEX_H);

    _M_lines.emplace_back();

    // Line 1
    {
        Line& line = _M_lines.back();
        line.push_back({0, 0});


        float tmp;
        add_offset({0, F});
        add_offset({A, 0});
        add_offset({0, -F});
        tmp = (A - B) / 2.0f;
        add_offset({-tmp, 0});
        add_offset({0, D});
        add_offset({-B, 0});
        add_offset({0, -D});
        add_offset({-tmp, 0});

        // Check
        {
            glm::vec2 accuracy = glm::abs(line.front() - line.back());
            if (accuracy.x > ACCURACY || accuracy.y > ACCURACY)
            {
                clear();
                return false;
            }
        }
    }

    _M_lines.emplace_back();
    // Line 2
    {
        Line& line = _M_lines.back();

        glm::vec2 circle_center1;
        circle_center1.x = (A - C) / 2.0f;
        circle_center1.y = E;

        glm::vec2 circle_center2 = circle_center1 + glm::vec2(C, 0.0f);

        line.push_back({circle_center1.x, circle_center1.y - G / 2.f});
        add_offset(glm::vec2(C, 0.0f));


        float angle = glm::asin((H - G / 2) / (G / 2));
        build_arc(circle_center2, G / 2, glm::radians(-90.f), glm::radians(180.f) - angle, 100);
        add_offset({C - (line.back().x - circle_center1.x) * 2.0f, 0.0});
        build_arc(circle_center1, G / 2, angle, glm::radians(270.f), 100);

        // Check
        {
            glm::vec2 accuracy = glm::abs(line.front() - line.back());
            if (accuracy.x > ACCURACY || accuracy.y > ACCURACY)
            {
                clear();
                return false;
            }
        }
    }

    return true;
}

void Figure::clear()
{
    _M_lines.clear();
}

void Figure::render(DrawFunc drawer)
{
    for (const Line& line : _M_lines)
    {
        for (std::size_t index = 1, end = line.size(); index < end; ++index)
        {
            glm::vec2 p1 = PointTransformer::figure_transform(line[index - 1]);
            glm::vec2 p2 = PointTransformer::figure_transform(line[index]);
            drawer(p1, p2, properties.figure.color, properties.figure.color, false);
        }
    }
}
