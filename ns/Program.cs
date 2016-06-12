using System;
using System.Linq;
using Eunoia.Net.Collections;

namespace ns
{
    class Program
    {
        static void Main(string[] args)
        {
            var at = new TcpTable2().ToArray();
            Console.WriteLine($"Number of Entries: {at.Length}");
            const String format = "{0}\t{1,-12}\t{2,-15}\t{3,10}\t{4,-15}\t{5,11}\t{6,10}\t{7}";
            Console.WriteLine(String.Format(format, 
                    "TCP #", "State", "Local Addr", "Local Port", "Remote Addr", "Remote Port", "Owning PID", "Offload State"));
            var cnt = 0;
            foreach (var row in at) {
                Console.WriteLine(String.Format(format, 
                    cnt++, row.GetStateString(), row.LocalAddress, row.LocalPort, row.RemoteAddress, row.RemotePort, row.OwningPid, row.GetOffloadStateString()));
            }
        }
    }
}
