/***************************************************************************
 *   Copyright 2015 Michael Eischer, Philipp Nordhus                       *
 *   Robotics Erlangen e.V.                                                *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef SIMFIELD_H
#define SIMFIELD_H

#include "protobuf/world.pb.h"
#include <QList>
#include <btBulletDynamicsCommon.h>
#include <optional>

namespace camun {
    namespace simulator {
        class SimField;
    }
}

class camun::simulator::SimField
{
public:
    SimField(btDiscreteDynamicsWorld *world, const world::Geometry &geometry);
    ~SimField();
    SimField(const SimField&) = delete;
    SimField& operator=(const SimField&) = delete;

private:
    void addObject(btCollisionShape *shape, const btTransform &transform, float restitution, float friction);

private:
    btDiscreteDynamicsWorld *m_world;
    btCollisionShape *m_plane;
    btCollisionShape *m_goalSide;
    btCollisionShape *m_goalBack;
    std::optional<btBoxShape> m_cornerBlockShape;
    std::optional<btBoxShape> m_goalLineBoundaryShape;
    QList<btCollisionObject*> m_objects;
};

#endif // SIMFIELD_H
