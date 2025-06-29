#pragma once
#include "pch.h"
#include "WorldRenderedObject.h"

//Constructors for the object utilizing base class constructors
WorldRenderedObject::WorldRenderedObject() : WorldObject(), GraphicsObject() {};

WorldRenderedObject::WorldRenderedObject(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale) :
	WorldObject(position, rotation, scale),
	GraphicsObject() 
{};