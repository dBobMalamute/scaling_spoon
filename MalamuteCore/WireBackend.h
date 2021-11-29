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

// This class was modified slightly so the undo/redo framework would
// handle it better.

#ifndef WIREBACKEND_H
#define WIREBACKEND_H

#include <memory>

#include <QObject>
#include <QUuid>
#include <QVariant>

#include "Plug.h"
#include "InheritedClasses/DataType.h"
#include "WireState.h"
#include "CasingBackend.h"
#include "InheritedClasses/Idea.h"
#include "Wire.h"

class WireBackend: public QObject
{
    Q_OBJECT

public:
    WireBackend(PlugType plugType, CasingBackend* casingBackend, PlugNumber plugNumber);
    WireBackend(QUuid id, CasingBackend *inCasingBackend, PlugNumber inPlugNumber,
                CasingBackend *outCasingBackend, PlugNumber outPlugNumber);

    WireBackend(const WireBackend&) = delete;
    WireBackend operator=(const WireBackend&) = delete;

    ~WireBackend() override;

public:
    QJsonObject serialize() const;

public:

    QUuid id() const;

    void setRequiredPlugType(PlugType plugType);
    PlugType requiredPlugType() const;

    void setWire(Wire* wire);

    /// Assigns a casing to the required plug.
    /// It is assumed that there is a required plug, no extra checks
    void setCasingToPlug(CasingBackend *casingBackend, PlugType plugType, PlugNumber plugNumber);

    void removeFromCasings() const;

public:

    Wire *getWire() const;

    WireState const & state() const;
    WireState& state();

    CasingBackend* getCasing(PlugType plugType) const;
    CasingBackend*& getCasing(PlugType plugType);

    PlugNumber getPlugIndex(PlugType plugType) const;

    void clearCasingBackend(PlugType plugType);

    int dataType(PlugType plugType) const;
    int dataType() const;

    bool complete() const;

    void propagateData(std::shared_ptr<DataType> dataType) const;
    void propagateEmptyData() const;

signals:
    void wireBackendCompleted(WireBackend const&) const;
    void wireBackendMadeIncomplete(WireBackend const&) const;
    void updated(WireBackend& conn) const;

private:
    QUuid m_uid;

private:
    CasingBackend* m_outCasingBackend = nullptr;
    CasingBackend* m_inCasingBackend  = nullptr;

    PlugNumber m_outPlugNumber;
    PlugNumber m_inPlugNumber;

private:
    WireState m_state;
    Wire* m_wire;
};

#endif //WIREBACKEND_H
