#pragma once

using namespace System;

namespace Eunoia
{
    namespace Net
    {
        namespace Collections
        {
            public value struct TcpTableRow
            {
                String^ LocalAddress;
                UInt16  LocalPort;
                String^ RemoteAddress;
                UInt16  RemotePort;
                int     State;
                UInt32  OwningPid;
                UInt32  OffloadState;

                String^ GetStateString()
                {
                    switch (State) {
                    case MIB_TCP_STATE_CLOSED:
                        return "CLOSED";
                    case MIB_TCP_STATE_LISTEN:
                        return "LISTEN";
                    case MIB_TCP_STATE_SYN_SENT:
                        return "SYN-SENT";
                    case MIB_TCP_STATE_SYN_RCVD:
                        return "SYN-RECEIVED";
                    case MIB_TCP_STATE_ESTAB:
                        return "ESTABLISHED";
                    case MIB_TCP_STATE_FIN_WAIT1:
                        return "FIN-WAIT-1";
                    case MIB_TCP_STATE_FIN_WAIT2:
                        return "FIN-WAIT-2 ";
                    case MIB_TCP_STATE_CLOSE_WAIT:
                        return "CLOSE-WAIT";
                    case MIB_TCP_STATE_CLOSING:
                        return "CLOSING";
                    case MIB_TCP_STATE_LAST_ACK:
                        return "LAST-ACK";
                    case MIB_TCP_STATE_TIME_WAIT:
                        return "TIME-WAIT";
                    case MIB_TCP_STATE_DELETE_TCB:
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