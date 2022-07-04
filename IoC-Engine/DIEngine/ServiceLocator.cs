using System;

namespace DIEngine
{
    // deklaracja delegata
    public delegate SimpleContainer ContainerProviderDelegate();

    public class ServiceLocator
    {
        private ContainerProviderDelegate _provider;
        private static ServiceLocator _instance;
        private static object _lock = new object();

        public static void SetContainerProvider(
            ContainerProviderDelegate ContainerProvider)
        {
            lock(_lock)
            {
                if (_instance == null) _instance = new ServiceLocator();
                _instance._provider = ContainerProvider;
            }
        }

        public static ServiceLocator Current
        {
            get
            {
                if (_instance == null)
                {
                    lock(_lock)
                    {
                        if (_instance == null)
                            _instance = new ServiceLocator();
                    }
                }
                return _instance;
            }
        }

        public T GetInstance<T>()
        {

            if (_provider == null)
                throw new Exception("Container provider is not declared!");

            if (typeof(T) == typeof(SimpleContainer))
                return (T)(object)_provider.Invoke();

            var c = _provider.Invoke();
            return c.Resolve<T>();
        }
    }
}
