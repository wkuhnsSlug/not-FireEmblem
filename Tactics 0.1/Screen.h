#pragma once
#include "KeyboardBase.h"
#include "Mouse.h"
#include "GraphicsInterface.h"
#include "Viewport.h"

class Screen
{
public:
	Screen( KeyboardBase& Kbds, MouseInterface& m, GraphicsInterface& Gfx, Screen ** ptr );
	virtual ~Screen();
	virtual bool logic() = 0;
	virtual void render() = 0;

protected:
	KeyboardBase& kbd;
	MouseInterface& mouse;
	GraphicsInterface& gfx;
	Viewport viewport;
	Screen ** levelPointer;
};