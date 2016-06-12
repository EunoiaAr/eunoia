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
            _tcpTable = new TcpTable2();

            _thread = new Thread(RefreshThread);
            _thread.Start();
        }

        void RefreshThread()
        {
            while (true) {
                if (PropertyChanged == null)
                    break;
                var rows = _tcpTable.Select(r => new MainViewModelTcpRow {
                    LocalAddress    = r.LocalAddress,
                    LocalPort       = r.LocalPort,
                    RemoteAddress   = r.RemoteAddress,
                    RemotePort      = r.RemotePort,
                    State           = r.GetStateString(),
                    OwningPid       = r.OwningPid,
                    OffLoadState    = r.GetOffloadStateString()
                }).ToArray();
                if (TcpRows == null || !rows.SequenceEqual(TcpRows)) {
                    Dispatcher.CurrentDispatcher.Invoke(() => {
                        TcpRows = rows;
                        if (PropertyChanged != null)
                            PropertyChanged(this, new PropertyChangedEventArgs("TcpRows"));
                    });
                }
                Thread.Sleep(1000);
            }
        }

        public MainViewModelTcpRow[] TcpRows { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        private TcpTable2   _tcpTable;
        private Thread      _thread;
    }
}
