#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <map>

#include "TcpTableRow.h"
#include "TcpTable.h"

namespace Eunoia
{
    namespace Net
    {
        namespace Collections
        {
            Generic::IEnumerator<TcpTableRow>^ TcpTable::GetEnumerator()
            { 
                return gcnew TcpTableIterator();
            }

            TcpTable::TcpTableIterator::TcpTableIterator()
            {
                PMIB_TCPTABLE   pTcpTable = (MIB_TCPTABLE *)MALLOC(sizeof(MIB_TCPTABLE));

                if (pTcpTable == NULL) {
                    throw gcnew ApplicationException("Error allocating memory for tcpTable");
                }

                DWORD           dwSize = sizeof(MIB_TCPTABLE);
                DWORD           dwRetVal = 0;
                struct in_addr  IpAddr;

                // Make an initial call to GetTcpTable to
                // get the necessary size into the dwSize variable
                if ((dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) == ERROR_INSUFFICIENT_BUFFER) {
                    FREE(pTcpTable);
                    pTcpTable = (MIB_TCPTABLE *)MALLOC(dwSize);
                    if (pTcpTable == NULL) {
                        throw gcnew ApplicationException("insufficient memory for tcpTable");
                    }
                }
                try {
                    // Make a second call to GetTcpTable to get
                    // the actual data we require
                    if ((dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) == NO_ERROR) {
                        _rows = gcnew cli::array<TcpTableRow>(pTcpTable->dwNumEntries);

                        for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
                            MIB_TCPROW& row         = pTcpTable->table[i];

                            IpAddr.S_un.S_addr      = (u_long)row.dwLocalAddr;
                            _rows[i].LocalAddress   = gcnew String(inet_ntoa(IpAddr));

                            IpAddr.S_un.S_addr      = (u_long)row.dwRemoteAddr;
                            _rows[i].RemoteAddress  = gcnew String(inet_ntoa(IpAddr));
                            _rows[i].State          = (TcpConnState) Enum::Parse(TcpConnState::typeid, row.dwState.ToString());
                            _rows[i].LocalPort      = ntohs((u_short)row.dwLocalPort);
                            _rows[i].RemotePort     = ntohs((u_short)row.dwRemotePort);
                        }
                    }
                    else {
                        throw gcnew ApplicationException(String::Format("GetTcpTable failed with {0}", dwRetVal));
                    }
                } catch (Exception^) {
                    throw;
                } finally {
                    FREE(pTcpTable);
                    _current = 0;
                }
            }

            TcpTable::TcpTableIterator::~TcpTableIterator()
            {
                this->!TcpTableIterator();
            }

            bool TcpTable::TcpTableIterator::MoveNext() 
            {
                return _rows != nullptr && ++_current < _rows->Length;
            }

            void TcpTable::TcpTableIterator::Reset()
            {
                _current = 0;
            }

            TcpTable::TcpTableIterator::!TcpTableIterator()
            {
                delete _rows;
            }
        }
    }
}