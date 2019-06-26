// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "TTauri/geometry.hpp"
#include "TTauri/required.hpp"
#include "TTauri/utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace TTauri::Draw {

struct BezierPoint {
    enum class Type { Anchor, QuadraticControl, CubicControl1, CubicControl2 };

    Type type;
    glm::vec2 p;

    BezierPoint(glm::vec2 p, Type type) : p(p), type(type) {}
    BezierPoint(float x, float y, Type type) : BezierPoint({x, y}, type) {}

    /*! Normalize points in a list.
     * The following normalizations are executed:
     *  - Missing anchor points between two quadratic-control-points are added.
     *  - Missing first-cubic-control-points are added by reflecting the previous second-control point around the previous anchor.
     *  - The list of points will start with an anchor.
     *  - The list will close with the first anchor.
     */
    static std::vector<BezierPoint> normalizePoints(std::vector<BezierPoint> originalPoints) {
        std::vector<BezierPoint> r;

        let numberOfPoints = static_cast<ptrdiff_t>(originalPoints.size());
        for (ptrdiff_t i = 0; i < numberOfPoints; i++) {
            let point = originalPoints[i];
            let previousPoint = originalPoints[safe_modulo(i - 1, numberOfPoints)];
            let previousPreviousPoint = originalPoints[safe_modulo(i - 2, numberOfPoints)];

            switch (point.type) {
            case BezierPoint::Type::Anchor:
                required_assert(previousPoint.type != BezierPoint::Type::CubicControl1);
                r.push_back(point);
                break;

            case BezierPoint::Type::QuadraticControl:
                if (previousPoint.type == BezierPoint::Type::QuadraticControl) {
                    r.emplace_back(midpoint(previousPoint.p, point.p), BezierPoint::Type::Anchor);

                } else {
                    required_assert(previousPoint.type == BezierPoint::Type::Anchor);
                }
                r.push_back(point);
                break;

            case BezierPoint::Type::CubicControl1:
                r.push_back(point);
                break;

            case BezierPoint::Type::CubicControl2:
                if (previousPoint.type == BezierPoint::Type::Anchor) {
                    required_assert(previousPreviousPoint.type == BezierPoint::Type::CubicControl2);

                    r.emplace_back(glm::reflect(previousPreviousPoint.p, previousPoint.p), BezierPoint::Type::CubicControl1);
                } else {
                    required_assert(previousPoint.type == BezierPoint::Type::CubicControl1);
                }
                r.push_back(point);
                break;

            default:
                no_default;
            }
        }

        for (size_t i = 0; i < r.size(); i++) {
            if (r[i].type == BezierPoint::Type::Anchor) {
                std::rotate(r.begin(), r.begin() + i, r.end());
                r.push_back(r.front());
                return r;
            }
        }

        // The result did not contain an anchor.
        required_assert(false);
    }
};

inline bool operator==(BezierPoint const &lhs, BezierPoint const &rhs) {
    return (lhs.p == rhs.p) && (lhs.type == rhs.type);
}

inline BezierPoint operator*(glm::mat2x2 const &lhs, BezierPoint const &rhs) {
    return { lhs * rhs.p, rhs.type };
}

inline BezierPoint operator*(glm::mat3x3 const &lhs, BezierPoint const &rhs) {
    return { (lhs * glm::vec3{rhs.p, 1.0f}).xy, rhs.type };
}

inline BezierPoint &operator*=(BezierPoint &lhs, glm::mat3x3 const &rhs) {
    lhs.p = (rhs * glm::vec3{lhs.p, 1.0f}).xy;
    return lhs;
}

inline BezierPoint operator+(BezierPoint const &lhs, glm::vec2 rhs) {
    return { lhs.p + rhs, lhs.type };
}

}