namespace wns.ViewModels
{
    public class MainViewModelTcpRow
    {
        public  string  LocalAddress    { get; internal set; }
        public  ushort  LocalPort       { get; internal set; }
        public  string  OffLoadState    { get; internal set; }
        public  uint    OwningPid       { get; internal set; }
        public  string  RemoteAddress   { get; internal set; }
        public  ushort  RemotePort      { get; internal set; }
        public  string  State           { get; internal set; }
    }
}