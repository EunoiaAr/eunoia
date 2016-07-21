#include "stdafx.h"
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "TcpTableRow.h"
#include "TcpTable2.h"

namespace Eunoia
{
    namespace Net
    {
        namespace Collections
        {
            Generic::IEnumerator<TcpTableRow>^ TcpTable2::GetEnumerator()
            { 
                return gcnew TcpTableIterator();
            }

            TcpTable2::TcpTableIterator::TcpTableIterator()
            {
                PMIB_TCPTABLE2   pTcpTable = (MIB_TCPTABLE2 *)MALLOC(sizeof(MIB_TCPTABLE2));

                if (pTcpTable == NULL) {
                    throw gcnew ApplicationException("Error allocating memory for tcpTable");
                }

                DWORD   dwSize      = sizeof(MIB_TCPTABLE2);
                DWORD   dwRetVal    = 0;

                // Make an initial call to GetTcpTable to
                // get the necessary size into the dwSize variable
                if ((dwRetVal = GetTcpTable2(pTcpTable, &dwSize, TRUE)) == ERROR_INSUFFICIENT_BUFFER) {
                    FREE(pTcpTable);
                    pTcpTable = (MIB_TCPTABLE2 *)MALLOC(dwSize);
                    if (pTcpTable == NULL) {
                        throw gcnew ApplicationException("insufficient memory for tcpTable");
                    }
                }
                try {
                    // Make a second call to GetTcpTable to get
                    // the actual data we require
                    if ((dwRetVal = GetTcpTable2(pTcpTable, &dwSize, TRUE)) == NO_ERROR) {
                        _rows = gcnew cli::array<TcpTableRow>(pTcpTable->dwNumEntries);

                        in_addr  IpAddr;
                        for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
                            MIB_TCPROW2& row = pTcpTable->table[i];

                            IpAddr.S_un.S_addr      = (u_long) row.dwLocalAddr;
                            _rows[i].LocalAddress   = gcnew String(inet_ntoa(IpAddr));

                            IpAddr.S_un.S_addr      = (u_long) row.dwRemoteAddr;
                            _rows[i].RemoteAddress  = gcnew String(inet_ntoa(IpAddr));
                            _rows[i].State          = (TcpConnState)Enum::Parse(TcpConnState::typeid, row.dwState.ToString());
                            _rows[i].LocalPort      = ntohs((u_short)row.dwLocalPort);
                            _rows[i].RemotePort     = ntohs((u_short)row.dwRemotePort);
                            _rows[i].OwningPid      = row.dwOwningPid;
                            _rows[i].OffloadState   = row.dwOffloadState;
                        }
                    }
                    else {
                        throw gcnew ApplicationException(String::Format("GetTcpTable2 failed with {0}", dwRetVal));
                    }
                } catch (Exception^) {
                    throw;
                } finally {
                    FREE(pTcpTable);
                    _current = 0;
                }
            }

            TcpTable2::TcpTableIterator::~TcpTableIterator()
            {
                this->!TcpTableIterator();
            }

            bool TcpTable2::TcpTableIterator::MoveNext() 
            {
                return _rows != nullptr && ++_current < _rows->Length;
            }

            void TcpTable2::TcpTableIterator::Reset()
            {
                _current = 0;
            }

            TcpTable2::TcpTableIterator::!TcpTableIterator()
            {
                delete _rows;
            }
        }
    }
}