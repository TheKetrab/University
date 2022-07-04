using System;
using System.Runtime.InteropServices;

namespace DIEngine
{

    class Program
    {
        static void Main(string[] args)
        {
            // Composition Root
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<Tone, YellowTone>();
            // c.RegisterType<Tone, BlueTone>();
            c.RegisterType<IPrinter,Printer>();
            PrinterFactory.SetProvider(() => c.Resolve<IPrinter>());

            // ... program ...
            var printer = new PrinterFactory().Create();
            printer.Print("Hello World!"); // na żółto

            Console.Read();
        }
    }



}
