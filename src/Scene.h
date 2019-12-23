#pragma once

#define EPS 0.001

#include "ofMain.h"

#include "BaseShape.h"

namespace ofxLn {
    
    class Scene {
        
        public:
        
            /* ~~~~~ variables ~~~~~ */
            glm::vec3 eye, center, up;
            float width, height, fovy, near, far;
            vector<shared_ptr<BaseShape>> shapes;
        
            /* ~~~~~ constructors ~~~~~ */
            Scene();
        
            /* ~~~~~ methods ~~~~~ */
            glm::mat4x4 projectionMatrix();
            glm::mat4x4 orthographicMatrix(float _width);
            glm::vec4 viewport();
            vector<ofPolyline> renderPerspective(float _step);
            vector<ofPolyline> renderOrthographic(float _step, float _width);
            vector<ofPolyline> renderWithMatrix(float _step, glm::mat4x4 _projectionMatrix, glm::vec4 _viewportMatrix, bool _isOrthographic);
            bool intersect(glm::vec3 _origin, glm::vec3 _direction, float& _distance);
            bool visible(glm::vec3 _vector, bool _isOrthographic);
            void addShape(shared_ptr<BaseShape> _shape);
            vector<ofPolyline> lines(bool _isOrthographic);
        
    };
    
}
