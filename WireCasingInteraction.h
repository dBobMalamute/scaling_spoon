//Copyright (c) 2017, Dmitry Pinaev
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are
//met:

//    * Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
//copyright notice, this list of conditions and the following disclaimer
//in the documentation and/or other materials provided with the
//distribution.
//    * Neither the name of copyright holder, nor the names of its
//contributors may be used to endorse or promote products derived from
//this software without specific prior written permission.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This class was modified to work better with the undo/redo framework.
// Otherwise it was renamed a bit.

#ifndef WIRECASINGINTERACTION_H
#define WIRECASINGINTERACTION_H

#include "CasingBackend.h"
#include "WireBackend.h"

/// Handles the interaction between a wire and a casing.
/// Created and destroyed frequently.

class WireCasingInteraction
{
public:
    WireCasingInteraction(CasingBackend& casingBackend, WireBackend& wireBackend);

    bool canConnect(PlugNumber & plugNumber) const;
    bool tryConnect() const;
    bool disconnect(PlugType disconnectingPlug) const;

    bool tryHover();

private:
    PlugType wireRequiredPlug() const;
    QPointF wireEndScenePosition(PlugType plugType) const;
    PlugNumber casingPlugNumberUnderScenePoint(PlugType plugType, QPointF const &p) const;
    bool casingPlugIsEmpty(PlugType plugType, PlugNumber plugNumber) const;

private:
    CasingBackend* m_casingBackend;
    WireBackend* m_wireBackend;
};

#endif
