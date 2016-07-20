using System.ComponentModel;
using System.Linq;
using System.Threading;
using System.Windows.Threading;
using Eunoia.Net.Collections;

namespace wns.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        public MainViewModel()
        {
            _tcpTable   = new TcpTable2();
            _udpTable   = new UdpTable();
            _thread     = new Thread(RefreshThread);
            _thread.Start();
        }

        public MainViewModelTcpRow[] TcpRows { get; set; }
        public MainViewModelUdpRow[] UdpRows { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        void RefreshThread()
        {
            while (true) {
                var tcpRows = _tcpTable.Select(r => new MainViewModelTcpRow {
                                                    LocalAddress    = r.LocalAddress,
                                                    LocalPort       = r.LocalPort,
                                                    RemoteAddress   = r.RemoteAddress,
                                                    RemotePort      = r.RemotePort,
                                                    State           = r.GetStateString(),
                                                    OwningPid       = r.OwningPid,
                                                    OffLoadState    = r.GetOffloadStateString()
                                                })
                                       .ToArray();
                var udpRows = _udpTable.Select(r => new MainViewModelUdpRow {
                    LocalAddress    = r.LocalAddress,
                    LocalPort       = r.LocalPort
                }).ToArray();
                if (TcpRows == null || !tcpRows.SequenceEqual(TcpRows)) {
                    Dispatcher.CurrentDispatcher.Invoke(() => {
                        TcpRows = tcpRows;
                        if (PropertyChanged != null)
                            PropertyChanged(this, new PropertyChangedEventArgs("TcpRows"));
                    });
                }
                if (UdpRows == null || !udpRows.SequenceEqual(UdpRows)) {
                    Dispatcher.CurrentDispatcher.Invoke(() => {
                        UdpRows = udpRows;
                        if (PropertyChanged != null)
                            PropertyChanged(this, new PropertyChangedEventArgs("UdpRows"));
                    });
                }
                Thread.Sleep(1500);
                if (PropertyChanged == null)
                    break;
            }
        }

        TcpTable2   _tcpTable;
        Thread      _thread;
        UdpTable    _udpTable;
    }
}
