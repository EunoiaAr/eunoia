#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <map>

#include "TcpTableRow.h"
#include "UdpTable.h"

namespace Eunoia
{
    namespace Net
    {
        namespace Collections
        {
            Generic::IEnumerator<TcpTableRow>^ UdpTable::GetEnumerator()
            { 
                return gcnew UdpTableIterator();
            }

            UdpTable::UdpTableIterator::UdpTableIterator()
            {
                PMIB_UDPTABLE   pUdpTable = (MIB_UDPTABLE *)MALLOC(sizeof(MIB_UDPTABLE));

                if (pUdpTable == NULL) {
                    throw gcnew ApplicationException("Error allocating memory for UdpTable");
                }

                DWORD           dwSize      = sizeof(MIB_UDPTABLE);
                DWORD           dwRetVal    = 0;
                struct in_addr  IpAddr;

                // Make an initial call to GetUdpTable to
                // get the necessary size into the dwSize variable
                if ((dwRetVal = GetUdpTable(pUdpTable, &dwSize, TRUE)) == ERROR_INSUFFICIENT_BUFFER) {
                    FREE(pUdpTable);
                    pUdpTable = (MIB_UDPTABLE *)MALLOC(dwSize);
                    if (pUdpTable == NULL) {
                        throw gcnew ApplicationException("insufficient memory for UdpTable");
                    }
                }
                try {
                    // Make a second call to GetUdpTable to get
                    // the actual data we require
                    if ((dwRetVal = GetUdpTable(pUdpTable, &dwSize, TRUE)) == NO_ERROR) {
                        _rows = gcnew cli::array<TcpTableRow>(pUdpTable->dwNumEntries);

                        for (int i = 0; i < (int)pUdpTable->dwNumEntries; i++) {
                            IpAddr.S_un.S_addr = (u_long)pUdpTable->table[i].dwLocalAddr;
                            _rows[i].LocalAddress = gcnew String(inet_ntoa(IpAddr));

                            _rows[i].LocalPort = ntohs((u_short)pUdpTable->table[i].dwLocalPort);
                        }
                    }
                    else {
                        throw gcnew ApplicationException(String::Format("GetUdpTable failed with {0}", dwRetVal));
                    }
                } catch (Exception^) {
                    throw;
                } finally {
                    FREE(pUdpTable);
                    _current = 0;
                }
            }

            UdpTable::UdpTableIterator::~UdpTableIterator()
            {
                this->!UdpTableIterator();
            }

            bool UdpTable::UdpTableIterator::MoveNext() 
            {
                return _rows != nullptr && ++_current < _rows->Length;
            }

            void UdpTable::UdpTableIterator::Reset()
            {
                _current = 0;
            }

            UdpTable::UdpTableIterator::!UdpTableIterator()
            {
                delete _rows;
            }
        }
    }
}