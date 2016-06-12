#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <map>

#include "TcpTableRow.h"
#include "TcpTable.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

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
                            IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwLocalAddr;
                            _rows[i].LocalAddress = gcnew String(inet_ntoa(IpAddr));

                            IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwRemoteAddr;
                            _rows[i].RemoteAddress = gcnew String(inet_ntoa(IpAddr));
                            _rows[i].State = pTcpTable->table[i].dwState;
                            _rows[i].LocalPort = ntohs((u_short)pTcpTable->table[i].dwLocalPort);
                            _rows[i].RemotePort = ntohs((u_short)pTcpTable->table[i].dwRemotePort);
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