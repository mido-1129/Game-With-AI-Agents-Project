/*
* unit.cpp
*/

#include <GL/glut.h>
#include <stdio.h>
#include <cmath>

#include "unit.hpp"

#define PI 3.14159

Unit::Unit() : _segmentCount(3),
               _isStoppingX(true),
               _isStoppingY(true),
               _isDashing(false),
               _doesDealDamage(false),
               _accAbility(15.0),
               _dashFactor(300.0),
               _energyPt(60.0),
               _bodyColor(0.8, 0.8, 0.8),
               _arrowColor(0.8, 0.8, 0.8),
               _health(H_MAX),
               _energy(E_HIGH),
               _team(NEUTRAL)
{
    reset();
}

Unit::~Unit()
{
}


void Unit::draw()
{
    float angleOffset = atan2(_front[1], _front[0]);

    glPushMatrix();
      glTranslatef(_pos[0],
                   _pos[1],
                   _pos[2]);
      drawBody(angleOffset);
      drawArrow(angleOffset);
      drawBars();
    glPopMatrix();
}

void Unit::drawBody(float angleOffset)
{
    glPushMatrix();
      glColor3f(_bodyColor[0],
                _bodyColor[1],
                _bodyColor[2]);
      glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0, 0.0, 0.0);
        for (int i = 0; i <= _segmentCount; ++i)
        {
            float angle = 2.0f * PI * static_cast<float>(i) / static_cast<float>(_segmentCount) + angleOffset;
            float x = _r * cos(angle);
            float y = _r * sin(angle);
            glVertex3f(x, y, 0.0);
        }
      glEnd();
    glPopMatrix();
}

void Unit::drawArrow(float angleOffset)
{
    glPushMatrix();
      glTranslatef(1.25 * _r * _front[0],
                   1.25 * _r * _front[1],
                   1.25 * _r * _front[2] + 0.01);
      glColor3f(_arrowColor[0],
                _arrowColor[1],
                _arrowColor[2]);
      glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0, 0.0, 0.0);
        for (int i = 0; i <= 3; ++i)
        {
            float angle = 2.0f * PI * static_cast<float>(i) / static_cast<float>(3) + angleOffset;
            float x = cos(angle) * 0.15;
            float y = sin(angle) * 0.15;
            glVertex3f(x, y, 0.0);
        }
      glEnd();
    glPopMatrix();
}

void Unit::drawBars()
{
    double length;
    switch (_health)
    {
    case H_LOW:
        glColor3f(0.8, 0.2, 0.3);
        length = 0.3;
        break;
    case H_MEDIUM:
        glColor3f(0.8, 0.8, 0.2);
        length = 0.6;
        break;
    case H_HIGH:
        glColor3f(0.2, 0.8, 0.3);
        length = 0.9;
        break;
    default:
        return;
    }
    glPushMatrix();
      glTranslatef(0.0, 1.75 * _r, 0.2);
      glBegin(GL_POLYGON);
        glVertex3f(-length, -0.05, 0.0);
        glVertex3f( length, -0.05, 0.0);
        glVertex3f( length,  0.05, 0.0);
        glVertex3f(-length,  0.05, 0.0);
      glEnd();
      glColor3f(0.5, 0.5, 0.5);
      glBegin(GL_POLYGON);
        glVertex3f(-0.9, -0.05, 0.0);
        glVertex3f( 0.9, -0.05, 0.0);
        glVertex3f( 0.9,  0.05, 0.0);
        glVertex3f(-0.9,  0.05, 0.0);
      glEnd();
    glPopMatrix();

    switch (_energy)
    {
    case H_LOW:
        length = 0.3;
        break;
    case H_MEDIUM:
        length = 0.6;
        break;
    case H_HIGH:
        length = 0.9;
        break;
    default:
        return;
    }
    glPushMatrix();
      glTranslatef(0.0, 1.75 * _r - 0.15, 0.2);
      glColor3f(0.8, 0.8, 0.8);
      glBegin(GL_POLYGON);
        glVertex3f(-length * _energyPt / 100.0, -0.05, 0.0);
        glVertex3f( length * _energyPt / 100.0, -0.05, 0.0);
        glVertex3f( length * _energyPt / 100.0,  0.05, 0.0);
        glVertex3f(-length * _energyPt / 100.0,  0.05, 0.0);
      glEnd();
    glPopMatrix();
}

void Unit::turn(double deltaTime)
{
    if (_isStoppingX && _isStoppingY)
        return;

    GameObject::turn(deltaTime);
}

void Unit::applyForce(Vector3 force, GameObject& source)
{
    Unit* sourceUnitPtr = dynamic_cast<Unit*>(&source);
    if (!_isColliding &&
        sourceUnitPtr != NULL &&
        sourceUnitPtr->getIsDashing() &&
        sourceUnitPtr->getDoesDealDamage() &&
        sourceUnitPtr->getTeam() != _team)
    {
        switch (_health)
        {
        case H_HIGH:
            _health = H_MEDIUM;
            break;
        case H_MEDIUM:
            _health = H_LOW;
            break;
        case H_LOW:
            _health = H_EMPTY;
            break;
        default:
            break;
        }
    }
    GameObject::applyForce(force, source);
}

void Unit::update(double deltaTime)
{
    GameObject::update(deltaTime);

    if (_isDashing)
    {
        _energyPt -= 2.0;
        _energyPt = _energyPt < 0.0 ? 0.0 : _energyPt;
    }
    else
    {
        _energyPt += 0.2;
        _energyPt = _energyPt > 100.0 ? 100.0 : _energyPt;
    }
}

void Unit::move(Direction dir)
{
    bool isStopping = _health == H_EMPTY;
    double accAbility = _accAbility * (isStopping ? 0.0 : 1.0);

    switch (dir)
    {
    case FORWARD:
        _isStoppingY = isStopping;
        _acc[1] = accAbility;
        break;
    case BACKWARD:
        _isStoppingY = isStopping;
        _acc[1] = -accAbility;
        break;
    case LEFTWARD:
        _isStoppingX = isStopping;
        _acc[0] = -accAbility;
        break;
    case RIGHTWARD:
        _isStoppingX = isStopping;
        _acc[0] = accAbility;
        break;
    default:
        break;
    }
}

void Unit::stop(Direction dir)
{
    if (dir == FORWARD)
    {
        _isStoppingY = true;
        if (_vel[1] < -0.1)
        {
            _acc[1] = _accAbility * 0.3;
        }
        else if (_vel[1] > 0.1)
        {
            _acc[1] = -_accAbility * 0.3;
        }
        else
        {
            _vel[1] = 0.0;
            _acc[1] = 0.0;
        }
    }

    if (dir == RIGHTWARD)
    {
        _isStoppingX = true;
        if (_vel[0] < -0.1)
        {
            _acc[0] = _accAbility * 0.3;
        }
        else if (_vel[0] > 0.1)
        {
            _acc[0] = -_accAbility * 0.3;
        }
        else
        {
            _vel[0] = 0.0;
            _acc[0] = 0.0;
        }
    }
}

void Unit::reset()
{
    GameObject::_pos = { 0.0, 0.0, 0.1 };
}

bool Unit::getIsDashing()
{
    return _isDashing;
}

void Unit::setIsDashing(bool isDashing)
{
    _isDashing = isDashing;

    if (_health == H_EMPTY || _energyPt <= 10.0)
        return;

    if (_isDashing)
        applyForce(200.0 * _front, *this);
}

bool Unit::getDoesDealDamage()
{
    return _doesDealDamage;
}

Unit::Team Unit::getTeam()
{
    return _team;
}

void Unit::setTeam(Team team)
{
    _team = team;
}

