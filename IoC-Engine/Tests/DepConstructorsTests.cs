using DIEngine;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace Tests
{
    [TestClass]
    public class DepConstructorsTests
    {
        [TestMethod]
        public void RegisterInstance()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterInstance<string>("Hello world!");
            c.RegisterType<Qux>(false);
            var o1 = c.Resolve<Qux>();

            Assert.AreEqual("Hello world!", o1.GetStr());
        }

        [TestMethod]
        public void RegisterCustomClassInstance()
        {
            SimpleContainer c = new SimpleContainer();
            IBar bar1 = new ImplementedBar();
            c.RegisterInstance<IBar>(bar1);
            IBar bar2 = c.Resolve<IBar>();

            Assert.AreEqual(bar1, bar2);
        }

        [TestMethod]
        public void ResolvingConstructorsDependency()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<Baz1>(false);
            Assert.ThrowsException<Exception>(() => { c.Resolve<Baz1>(); });

            c.RegisterInstance<int>(1);
            c.RegisterInstance<string>("str");
            c.RegisterType<Baz2>(false);
            c.RegisterType<Baz3>(false);

            var b = c.Resolve<Baz1>();
            Assert.IsNotNull(b);
            Assert.IsTrue(b is Baz1);
        }

        [TestMethod]
        public void FindingCycleWhileResolvingDependency()
        {
            // T1 -> T2 -> T3 -> T4 -> T5 -> T6 -> T1 ERROR cycle

            SimpleContainer c = new SimpleContainer();
            c.RegisterType<T1>(false);
            c.RegisterType<T2>(false);
            c.RegisterType<T3>(false);
            c.RegisterType<T4>(false);
            c.RegisterType<T5>(false);
            c.RegisterType<T6>(false);


            var ex = Assert.ThrowsException<Exception>(() => { c.Resolve<T1>(); });
            Assert.IsTrue(ex.Message.Contains("T1 -> T2 -> T3 -> T4 -> T5 -> T6 -> T1"));
        }

        [TestMethod]
        public void TakingProperConstructor1()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<ProperCon>(false);
            c.RegisterInstance<int>(1);
            c.RegisterInstance<string>("A");

            // powinno wybrać konstruktor z najdłuższą listą argumentów
            var o = c.Resolve<ProperCon>();
            Assert.AreEqual("4", o.GetMsg());
        }

        [TestMethod]
        public void TakingProperConstructor2()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<ProperCon,ProperCon1>(false);

            // powinno wybrać konstruktor oznaczony jako DependencyConstructor
            var o = c.Resolve<ProperCon>();
            Assert.AreEqual("1", o.GetMsg());
        }

        [TestMethod]
        public void TakingProperConstructor3()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<ProperCon,ProperCon2>(false);
            c.RegisterInstance<int>(1);
            c.RegisterInstance<string>("A");

            // są dwa konstruktory o trzech argumentach, nie wiadomo, który wybrać
            var ex = Assert.ThrowsException<Exception>(() => { c.Resolve<ProperCon>(); });
            Assert.IsTrue(ex.Message.Contains("Ambiguity"));
        }

        [TestMethod]
        public void TakingProperConstructor4()
        {
            // preferowanie DependencyConstructor,
            // nawet jeśli ma tyle argumentów co inne

            SimpleContainer c = new SimpleContainer();
            c.RegisterType<ProperCon, ProperCon3>(false);
            c.RegisterInstance<int>(1);
            c.RegisterInstance<string>("A");

            var o = c.Resolve<ProperCon>();
            Assert.IsTrue(o.GetMsg().Equals("DependencyCons"));
        }


    }


}
