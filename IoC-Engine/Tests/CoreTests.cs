using System;
using DIEngine;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{

    [TestClass]
    public class CoreTests
    {
        [TestMethod]
        public void NormalRegisterType()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<Foo>(false);
            var o1 = c.Resolve<Foo>();
            var o2 = c.Resolve<Foo>();

            Assert.IsTrue(o1 is Foo);
            Assert.IsTrue(o2 is Foo);
            Assert.AreNotEqual(o1, o2);
        }

        [TestMethod]
        public void DerivedRegisterType()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<Foo, Foo2>(false);
            var o1 = c.Resolve<Foo>();
            var o2 = c.Resolve<Foo>();

            Assert.IsTrue(o1 is Foo2);
            Assert.IsTrue(o1.GetType().IsSubclassOf(typeof(Foo)));
            Assert.IsTrue(o2 is Foo2);
            Assert.IsTrue(o2.GetType().IsSubclassOf(typeof(Foo)));
            Assert.AreNotEqual(o1, o2);
        }

        [TestMethod]
        public void SingletonNormalRegisterType()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<Foo>(true);
            var o1 = c.Resolve<Foo>();
            var o2 = c.Resolve<Foo>();

            Assert.IsTrue(o1 is Foo);
            Assert.IsTrue(o2 is Foo);
            Assert.AreEqual(o1, o2);
        }

        [TestMethod]
        public void SingletonDerivedRegisterType()
        {
            // musimy zarejestrować typ Foo2, bo potem tworzymy
            // singleton używając Foo2, więc kontener musi wiedzieć,
            // jak utworzyć obiekt Foo2 (inna opcja -> RegisterInstance)

            SimpleContainer c = new SimpleContainer();
            c.RegisterType<Foo2>(false);
            c.RegisterType<Foo, Foo2>(true);
            var o1 = c.Resolve<Foo>();
            var o2 = c.Resolve<Foo>();

            Assert.IsTrue(o1 is Foo2);
            Assert.IsTrue(o1.GetType().IsSubclassOf(typeof(Foo)));
            Assert.IsTrue(o2 is Foo2);
            Assert.IsTrue(o2.GetType().IsSubclassOf(typeof(Foo)));
            Assert.AreEqual(o1, o2);
        }

        [TestMethod]
        public void OverridingNormalTypes()
        {
            SimpleContainer c = new SimpleContainer();
            
            c.RegisterType<Foo,Foo2>(false);
            var o1 = c.Resolve<Foo>();
            Assert.IsTrue(o1 is Foo2);

            c.RegisterType<Foo, Foo3>(false);
            var o2 = c.Resolve<Foo>();
            Assert.IsTrue(o2 is Foo3);

            Assert.AreNotEqual(o1, o2);
        }

        [TestMethod]
        public void OverridingSingletons()
        {
            SimpleContainer c = new SimpleContainer();

            c.RegisterType<Foo2>(false);
            c.RegisterType<Foo, Foo2>(true);
            var o1 = c.Resolve<Foo>();
            var o2 = c.Resolve<Foo>();
            Assert.AreEqual(o1, o2);
            Assert.IsTrue(o1 is Foo2);

            c.RegisterType<Foo3>(false);   
            c.RegisterType<Foo, Foo3>(true);
            o1 = c.Resolve<Foo>();
            o2 = c.Resolve<Foo>();
            Assert.AreEqual(o1, o2);
            Assert.IsTrue(o1 is Foo3);
        }


        [TestMethod]
        public void ResolveNotRegisterred()
        {
            SimpleContainer c = new SimpleContainer();
            Assert.ThrowsException<Exception>(() => { c.Resolve<Bar>(); });

            c.RegisterType<Foo, Foo2>(false);
            Assert.ThrowsException<Exception>(() => { c.Resolve<Foo2>(); });
        }

        [TestMethod]
        public void NormalFactoryAction()
        {
            SimpleContainer c = new SimpleContainer();
            c.RegisterType<Foo, Foo2>(false);

            var o1 = c.Resolve<Foo>();
            Assert.IsNotNull(o1);
        } 



    }



}
