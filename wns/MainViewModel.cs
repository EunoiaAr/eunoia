using System.Linq;
using System.ComponentModel;
using System.Threading;
using Eunoia.Net.Collections;
using System.Windows.Threading;

namespace wns
{
    public class MainViewModel : INotifyPropertyChanged
    {
        public MainViewModel()
        {
            _tcpTable = new TcpTable2();

            _thread = new Thread(RefreshThread);
            _thread.Start();
        }

        void RefreshThread()
        {
            while (true) {
                if (PropertyChanged == null)
                    break;
                var rows = _tcpTable.Select(r => new MainViewModelRow {
                    LocalAddress    = r.LocalAddress,
                    LocalPort       = r.LocalPort,
                    RemoteAddress   = r.RemoteAddress,
                    RemotePort      = r.RemotePort,
                    State           = r.GetStateString(),
                    OwningPid       = r.OwningPid,
                    OffLoadState    = r.GetOffloadStateString()
                }).ToArray();
                if (Rows == null || !rows.SequenceEqual(Rows)) {
                    Dispatcher.CurrentDispatcher.Invoke(() => {
                        Rows = rows;
                        if (PropertyChanged != null)
                            PropertyChanged(this, new PropertyChangedEventArgs("Rows"));
                    });
                }
                Thread.Sleep(1000);
            }
        }

        public MainViewModelRow[] Rows { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        private TcpTable2   _tcpTable;
        private Thread      _thread;
    }
}
