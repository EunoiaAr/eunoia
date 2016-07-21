#pragma once

namespace Eunoia
{
    namespace Net
    {
        public enum class TcpConnState : int
        {
            Closed      = 1,
            Listen      = 2,
            SynSent     = 3,
            SynRcvd     = 4,
            Estab       = 5,
            FinWait1    = 6,
            FinWait2    = 7,
            CloseWait   = 8,
            Closing     = 9,
            LastAck     = 10,
            TimeWait    = 11,
            DeleteTcb   = 12,
        };
    }
}