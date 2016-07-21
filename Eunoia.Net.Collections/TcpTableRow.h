#pragma once

#include "TcpConnState.h"

using namespace System;

namespace Eunoia
{
    namespace Net
    {
        namespace Collections
        {
            public value struct TcpTableRow
            {
                String^         LocalAddress;
                UInt16          LocalPort;
                String^         RemoteAddress;
                UInt16          RemotePort;
                TcpConnState    State;
                UInt32          OwningPid;
                UInt32          OffloadState;

                String^ GetStateString()
                {
                    switch (State) {
                    case TcpConnState::Closed:
                        return "CLOSED";
                    case TcpConnState::Listen:
                        return "LISTEN";
                    case TcpConnState::SynSent:
                        return "SYN-SENT";
                    case TcpConnState::SynRcvd:
                        return "SYN-RECEIVED";
                    case TcpConnState::Estab:
                        return "ESTABLISHED";
                    case TcpConnState::FinWait1:
                        return "FIN-WAIT-1";
                    case TcpConnState::FinWait2:
                        return "FIN-WAIT-2 ";
                    case TcpConnState::CloseWait:
                        return "CLOSE-WAIT";
                    case TcpConnState::Closing:
                        return "CLOSING";
                    case TcpConnState::LastAck:
                        return "LAST-ACK";
                    case TcpConnState::TimeWait:
                        return "TIME-WAIT";
                    case TcpConnState::DeleteTcb:
                        return "DELETE-TCB";
                    default:
                        return "UNKNOWN dwState value";
                    }
                }
                String^ GetOffloadStateString() 
                {
                    switch (OffloadState) {
                    case TcpConnectionOffloadStateInHost:
                        return "Owned by the network stack and not offloaded";
                    case TcpConnectionOffloadStateOffloading:
                        return "In the process of being offloaded";
                    case TcpConnectionOffloadStateOffloaded:
                        return "Offloaded to the network interface control";
                    case TcpConnectionOffloadStateUploading:
                        return "In the process of being uploaded back to the network stack";
                    default:
                        return "UNKNOWN Offload state value";
                    }
                }
            };
        }
    }
}