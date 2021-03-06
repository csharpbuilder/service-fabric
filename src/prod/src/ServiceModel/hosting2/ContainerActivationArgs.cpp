// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace ServiceModel;
using namespace Hosting2;

ContainerActivationArgs::ContainerActivationArgs()
    : IsUserLocalSystem(false)
    , AppHostId()
    , NodeId()
    , ContainerDescriptionObj()
    , ProcessDescriptionObj()
    , FabricBinPath()
    , GatewayIpAddress()
{
}

ContainerActivationArgs::ContainerActivationArgs(
    bool isUserLocalSystem,
    wstring const & appHostId,
    wstring const & nodeId,
    ContainerDescription const & containerDesc,
    ProcessDescription const & processDesc,
    wstring const & fabricBinPath,
    wstring const & gatewayIpAddress)
    : IsUserLocalSystem(isUserLocalSystem)
    , AppHostId(appHostId)
    , NodeId(nodeId)
    , ContainerDescriptionObj(containerDesc)
    , ProcessDescriptionObj(processDesc)
    , FabricBinPath(fabricBinPath)
    , GatewayIpAddress(gatewayIpAddress)
{
}

ErrorCode ContainerActivationArgs::ToPublicApi(
    __in ScopedHeap & heap,
    __out FABRIC_CONTAINER_ACTIVATION_ARGS & fabricActivationArgs) const
{
    fabricActivationArgs.IsUserLocalSystem = this->IsUserLocalSystem;
    fabricActivationArgs.AppHostId = heap.AddString(this->AppHostId);
    fabricActivationArgs.NodeId = heap.AddString(this->NodeId);

    auto containerDesc = heap.AddItem<FABRIC_CONTAINER_DESCRIPTION>();
    auto error = this->ContainerDescriptionObj.ToPublicApi(heap, *containerDesc);
    if (!error.IsSuccess())
    {
        return error;
    }

    fabricActivationArgs.ContainerDescription = containerDesc.GetRawPointer();

    auto processDesc = heap.AddItem<FABRIC_PROCESS_DESCRIPTION>();
    error = this->ProcessDescriptionObj.ToPublicApi(heap, *processDesc);
    if (!error.IsSuccess())
    {
        return error;
    }

    fabricActivationArgs.ProcessDescription = processDesc.GetRawPointer();

    fabricActivationArgs.FabricBinPath = heap.AddString(this->FabricBinPath);
    fabricActivationArgs.GatewayIpAddress = heap.AddString(this->GatewayIpAddress);

    fabricActivationArgs.Reserved = nullptr;

    return ErrorCode::Success();
}

void ContainerActivationArgs::WriteTo(TextWriter & w, FormatOptions const &) const
{
    w.Write("ContainerActivationArgs { ");
    w.Write("IsUserLocalSystem = {0}", IsUserLocalSystem);
    w.Write("AppHostId = {0}", AppHostId);
    w.Write("NodeId = {0}", NodeId);
    w.Write("ContainerDescription = {0}, ", ContainerDescriptionObj);
    w.Write("ProcessDescriptionObj = {0}", ProcessDescriptionObj);
    w.Write("FabricBinPath = {0}", FabricBinPath);
    w.Write("GatewayIpAddress = {0}", GatewayIpAddress);
    w.Write("}");
}

