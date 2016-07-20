using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using Microsoft.Win32.SafeHandles;

namespace SpawnAndWait
{
    class Program
    {
        const string programName = "notepad.exe";

        static void Main(string[] args)
        {
            var processes = new Dictionary<int, Process>();

            for (int i = 0; i < 4; i++) {
                var processStartInfo = new ProcessStartInfo {
                    FileName = programName
                };

                var process = Process.Start(processStartInfo);
                processes.Add(i, process);
            }

            while (processes.Count > 0) {
                var handles = processes.Values.Select(p => new EventWaitHandle(false, EventResetMode.ManualReset) {
                    SafeWaitHandle = new SafeWaitHandle(p.Handle, false)
                }).ToArray();

                var i = WaitHandle.WaitAny(handles);

                var terminatedProcess = processes.Values.Where(p => p.Handle == handles[i].SafeWaitHandle.DangerousGetHandle()).FirstOrDefault();
                Console.WriteLine("Process {0} was terminated", terminatedProcess.Id);

                processes.Remove(processes.Where(p => p.Value.Id == terminatedProcess.Id).FirstOrDefault().Key);
            }
        }
    }
}
