#pragma once

#include "ofMain.h"

namespace ofxLn {
    class BaseShape {
        public:
            virtual ~BaseShape() {}
            virtual bool intersect(glm::vec3 _origin, glm::vec3 _direction, float& _distance) = 0;
            virtual vector<ofPolyline> getLines(glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up, bool _isOrthographic) = 0;
    };
}
