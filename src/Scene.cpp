#include "Scene.h"

namespace ofxLn {
    /* ~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* ~~~~~ constructors ~~~~~ */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~ */
    Scene::Scene() {
        eye = glm::vec3(0, 0, 1);
        center = glm::vec3(0, 0, 0);
        up = glm::vec3(0, -1, 0);
        width = ofGetWidth();
        height = ofGetHeight();
        fovy = 40 * DEG_TO_RAD;
        near = 0.1;
        far = 10;
    }
    /* ~~~~~~~~~~~~~~~~~~~ */
    /* ~~~~~ methods ~~~~~ */
    /* ~~~~~~~~~~~~~~~~~~~ */
    glm::mat4x4 Scene::projectionMatrix() {
        return glm::perspective(fovy, width / height, near, far) * glm::lookAt(eye, center, up);
    }
    glm::mat4x4 Scene::orthographicMatrix(float _width) {
        float halfWidth = 0.5f * _width;
        float aspectRatio = height / width;
        return glm::ortho(halfWidth, -halfWidth, halfWidth * aspectRatio, -halfWidth * aspectRatio, near, far) * glm::lookAt(eye, center, -up);
    }
    glm::vec4 Scene::viewport() {
        return glm::vec4(0, 0, width, height);
    }
    vector<ofPolyline> Scene::renderPerspective(float _step) {
        return renderWithMatrix(_step, projectionMatrix(), viewport(), false);
    }
    vector<ofPolyline> Scene::renderOrthographic(float _step, float _width) {
        return renderWithMatrix(_step, orthographicMatrix(_width), viewport(), true);
    };
    vector<ofPolyline> Scene::renderWithMatrix(float _step, glm::mat4x4 _projectionMatrix, glm::vec4 _viewportMatrix, bool _isOrthographic) {
        vector<ofPolyline> result;
        vector<ofPolyline> allLines = lines(_isOrthographic);
        int count = 0;
        for (ofPolyline line : allLines) {
            ofPolyline resampledLine = line.getResampledBySpacing(_step);
            ofPolyline tmpLine;
            for (glm::vec3 vector : resampledLine.getVertices()) {
                glm::vec3 projectedVector = glm::project(vector, glm::mat4x4(1.0), _projectionMatrix, _viewportMatrix);
                bool xCheckProjected = projectedVector.x > 0 && projectedVector.x < width;
                bool yCheckProjected = projectedVector.y > 0 && projectedVector.y < height;
                bool zCheckProjected = abs(projectedVector.z) <= 1;
                if (xCheckProjected && yCheckProjected && zCheckProjected) {
                    if (visible(vector, _isOrthographic)) {
                        tmpLine.addVertex(glm::vec3(projectedVector.x, projectedVector.y, 0));
                    } else {
                        if (tmpLine.size() > 1) result.push_back(tmpLine);
                        tmpLine.clear();
                    }
                } else {
                    if (tmpLine.size() > 1) result.push_back(tmpLine);
                    tmpLine.clear();
                }
            }
            if (tmpLine.size() > 1) result.push_back(tmpLine);
        }
        return result;
    }
    bool Scene::intersect(glm::vec3 _origin, glm::vec3 _direction, float& _distance) {
        _distance = INFINITY;
        for (shared_ptr<BaseShape> shape : shapes) {
            float thisDistance;
            if (shape->intersect(_origin, _direction, thisDistance)) {
                if (thisDistance < _distance) _distance = thisDistance;
            }
        }
        return _distance < INFINITY;
    }
    bool Scene::visible(glm::vec3 _vector, bool _isOrthographic) {
        float distance;
        if (_isOrthographic) {
            if (intersect(_vector, glm::normalize(eye - center), distance)) {
                if (distance < EPS) return true;
                else return false;
            }
            else return true;
        } else {
            if (intersect(eye, glm::normalize(_vector - eye), distance)) return distance + EPS > glm::length(_vector - eye);
            else return true;
        }
    }
    void Scene::addShape(shared_ptr<BaseShape> _shape) {
        shapes.push_back(_shape);
    }
    vector<ofPolyline> Scene::lines(bool _isOrthographic) {
        vector<ofPolyline> result;
        for (shared_ptr<BaseShape> shape : shapes) {
            vector<ofPolyline> thisLines = shape->getLines(eye, center, up, _isOrthographic);
            result.insert(result.end(), thisLines.begin(), thisLines.end());
        }
        return result;
    }
}
