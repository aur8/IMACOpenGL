#pragma once

class TrackballCamera {
private:
  float m_distance;
  float m_AngleX;
  float m_AngleY;

public:
  TrackballCamera() : m_distance(5), m_AngleX(0), m_AngleY(0) {}

  // methods

  void moveFront(float delta) {}
  void rotateLeft(float degrees) {}
  void rotateUp(float degrees)
};