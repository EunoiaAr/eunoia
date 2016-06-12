#pragma once

using namespace System;
using namespace System::Collections;

namespace Eunoia
{
    namespace Net
    {
        namespace Collections
        {
            // see: http://stackoverflow.com/a/956252/41236
            public ref class TcpTable2 : public Generic::IEnumerable<TcpTableRow>
            {
            public:
                virtual Generic::IEnumerator<TcpTableRow>^ GetEnumerator();

            private:
                virtual IEnumerator^ IEnumerableGetEnumerator() sealed = System::Collections::IEnumerable::GetEnumerator
                {
                    return GetEnumerator();
                }

                ref class TcpTableIterator : Generic::IEnumerator<TcpTableRow> {
                public:
                    TcpTableIterator();
                    !TcpTableIterator();
                    virtual property TcpTableRow Current { TcpTableRow get() { return _rows[_current]; } }
                    virtual bool MoveNext();
                    virtual void Reset();
                protected:
                    ~TcpTableIterator();
                private:
                    virtual property Object^ EnumeratorCurrent { 
                        Object^ get() sealed = System::Collections::IEnumerator::Current::get { return _rows[_current]; }
                    }
                    cli::array<TcpTableRow>^ _rows;
                    int _current;
                };
            };
        }
    }
}