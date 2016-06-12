using System;
using System.Linq;
using Eunoia.Net.Collections;

namespace ns
{
    class Program
    {
        static void Main(string[] args)
        {
            var at = new TcpTable().ToArray();
            Console.WriteLine($"Number of Entries: {at.Length}");
            const String format = "{0:5} {1:-22} {2:-16} {3:10} {4:-16} {5:10}";
            Console.WriteLine(String.Format(format, "TCP #", "State", "Local Addr", "Local Port", "Remote Addr", "Remote Port"));
            var cnt = 0;
            foreach (var row in at) {
                Console.WriteLine(String.Format(format, cnt, row.GetStateString(), row.LocalAddress, row.LocalPort, row.RemoteAddress, row.RemotePort));
            }
        }
    }
}
