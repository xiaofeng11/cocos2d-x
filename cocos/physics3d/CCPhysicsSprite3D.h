﻿/***************************************************************************
 Copyright (c) 2015 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __PHYSICS_SPRITE_3D_H__
#define __PHYSICS_SPRITE_3D_H__

#include "base/ccConfig.h"
#include "3d/CCSprite3D.h"
#include "CCPhysics3DObject.h"
#include "CCPhysics3DComponent.h"

#if CC_USE_3D_PHYSICS

#if (CC_ENABLE_BULLET_INTEGRATION)

NS_CC_BEGIN
/**
 * @addtogroup physics
 * @{
 * @addtogroup physics_3d
 * @{
*/

/**
* @brief @~english Convenient class to create a rigid body with Sprite3D
  @~chinese 辅助类用来便利的创建一个拥有rigit的Sprite3D
*/
class CC_DLL PhysicsSprite3D : public cocos2d::Sprite3D
{
public:

    /** @~english creates a PhysicsSprite3D
    @~chinese 创建一个PhysicsSprite3D
    */
    static PhysicsSprite3D* create(const std::string& modelPath, Physics3DRigidBodyDes* rigidDes, const cocos2d::Vec3& translateInPhysics = cocos2d::Vec3::ZERO, const cocos2d::Quaternion& rotInPhsyics = cocos2d::Quaternion::ZERO);

    /** @~english creates a PhysicsSprite3D as a Collider
    @~chinese 创建一个Collider类型的PhysicsSprite3D
    */
    static PhysicsSprite3D* createWithCollider(const std::string &modelPath, Physics3DColliderDes* colliderDes, const cocos2d::Vec3& translateInPhysics = cocos2d::Vec3::ZERO, const cocos2d::Quaternion& rotInPhsyics = cocos2d::Quaternion::ZERO);
    
    /** @~english Get the Physics3DObject.
    @~chinese 得到Physics3DObject。
    */
    Physics3DObject* getPhysicsObj() const;

    /** @~english Set synchronization flag, see Physics3DComponent.
    @~chinese 设置同步标志，参考physics3dcomponent。
    */
    void setSyncFlag(Physics3DComponent::PhysicsSyncFlag syncFlag);

    /** @~english Physics synchronize rendering.
    @~chinese 物理同步渲染。
    */
    void syncNodeToPhysics();

    /** @~english Rendering synchronize physics.
    @~chinese 同步物理渲染。
    */
    void syncPhysicsToNode();

CC_CONSTRUCTOR_ACCESS:
    PhysicsSprite3D();
    virtual ~PhysicsSprite3D();

protected:
    Physics3DComponent* _physicsComponent;
};

/**
 end of physics_3d group
 @}
 end of physics group
 @}
*/
NS_CC_END

#endif // CC_ENABLE_BULLET_INTEGRATION

#endif // CC_USE_3D_PHYSICS

#endif // __PHYSICS_SPRITE_3D_H__
