using System;

namespace DIEngine
{
    // przykład: użycie LocalFactory do wypisywania
    // na ekran napisów w różnym kolorze

    abstract class Tone { }
    class BlueTone : Tone { }
    class YellowTone : Tone { }

    interface IPrinter
    {
        void Print(string s);
    }

    class Printer : IPrinter
    {
        private Tone t;
        public Printer(Tone t)
        {
            this.t = t;
        }

        public void Print(string s)
        {
            var color = Console.ForegroundColor;
            if (t is BlueTone) Console.ForegroundColor = ConsoleColor.Cyan;
            else if (t is YellowTone) Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine(s);
            Console.ForegroundColor = color;
        }
    }

    class PrinterFactory
    {
        private static Func<IPrinter> _provider;
        public static void SetProvider(Func<IPrinter> provider)
        {
            _provider = provider;
        }

        public IPrinter Create()
        {
            return _provider();
        }

    }


}
