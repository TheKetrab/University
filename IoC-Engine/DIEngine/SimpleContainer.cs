using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace DIEngine
{

    public class SimpleContainer
    {

        // Typ A ma być implementowany za pomocą typu B
        private Dictionary<Type,Type> registeredTypes;

        // Typ A odwołuje się do stworzonego obiektu object
        private Dictionary<Type, object> singletons;


        public SimpleContainer()
        {
            registeredTypes = new Dictionary<Type,Type>();
            singletons = new Dictionary<Type, object>();
        }

        public void BuildUp<T>(T instance)
        {
            // zasil wszystkie zależności tego obiektu, tym czym umiesz
            var onResolving = new List<Type>();
            InjectProperties(instance, onResolving);
            InjectMethods(instance, onResolving);
        }


        public bool Registered(Type type)
        {
            return registeredTypes.ContainsKey(type);
        }

        public void RegisterType<T>(bool singleton=false)
            where T : class
        {
            if (Registered(typeof(T)))
            {
                // jeśli to wsześniej był singleton,
                // to trzeba go usunąć albo zaktualizować
                if (singletons.ContainsKey(typeof(T)))
                {
                    if (!singleton) singletons.Remove(typeof(T));
                    else singletons[typeof(T)] = Resolve<T>(); // set singleton
                }
            }

            else
            {
                registeredTypes.Add(typeof(T), typeof(T));
                if (singleton) singletons.Add(typeof(T), Resolve<T>());
            }
        }

        public void RegisterType<From, To>(bool singleton=false)
            where To : From
        {
            if (Registered(typeof(From)))
            {
                // przerejestrowanie typu na inny: jeśli był
                // implementowany innym typem, to trzeba go zmienić
                if (!(registeredTypes[typeof(From)] is To))
                {
                    registeredTypes[typeof(From)] = typeof(To);
                }

                // jeśli to wsześniej był singleton,
                // to trzeba go usunąć albo zaktualizować
                if (singletons.ContainsKey(typeof(From)))
                {
                    if (!singleton) singletons.Remove(typeof(From));
                    else singletons[typeof(From)] = Resolve<To>();
                }
            }
                
            else
            {
                registeredTypes.Add(typeof(From), typeof(To));
                if (singleton) singletons.Add(typeof(From), Resolve<To>());

            }
        }

        public T Resolve<T>()
        {
            var onResolving = new List<Type>();
            onResolving.Add(typeof(T));
            T result = (T)Resolve(typeof(T), onResolving);
            onResolving.Remove(typeof(T));
            return result;
        }

        /// <summary>
        /// Metoda do rekurencyjnego rozwiązywania zależności.
        /// </summary>
        /// <param name="type">Typ obiektu, który chcemy rozwiązać.</typeparam>
        /// <param name="onResolving">
        /// Lista typów, które są teraz rozwiązywane. Inaczej mówiąc ścieżka
        /// od korzenia wywołań rekurencyjnych. (Zapobieganie cyklom)
        /// </param>
        /// <returns></returns>
        private object Resolve(Type type, List<Type> onResolving)
        {
            if (!Registered(type))
                throw new Exception(string.Format(
                    "Unregistered type {0}.", type
                    ));

            // singleton?
            if (singletons.ContainsKey(type))
                return singletons[type];

            // ----- ----- ----- ----- -----
            // else -> new obj: wybierz konstruktor oznaczony atrybutem
            // [DependencyConstructor] lub o najdłuższej liczbie arg i go uzyj

            Type t = registeredTypes[type];
            var constructor = ChooseConstructor(t);
            object[] parameters = ResolveDependiencesForMethod(constructor, onResolving);

            // stwórz obiekt używając konstruktora
            var obj = constructor.Invoke(parameters);

            // do rozwiązywania zależności metod i właściwości 
            // już umiemy używać aktualnego typu 'type', bo udało się
            // znaleźć konstruktor i utworzyć dany typ
            var onResolvingCpy = new List<Type>(onResolving);
            onResolvingCpy.Remove(type);
            InjectProperties(obj, onResolvingCpy);
            InjectMethods(obj, onResolvingCpy);


            return obj;
        }

        private void InjectMethods(object obj, List<Type> onResolving)
        {
            Type t = obj.GetType();
            var methods = t.GetMethods();
            foreach (var m in methods)
            {
                // tylko te, które zwracają void i mają niepustą listę argumentów
                // i jest oznaczona jako [DependencyMethod]
                if (m.ReturnType.Equals(typeof(void))
                    && m.GetParameters().Length >= 1
                    && IsDependencyMethod(m))
                {
                    object[] parameters =
                        ResolveDependiencesForMethod(m, onResolving);
                    m.Invoke(obj, parameters);
                }
            }
        }

        private void InjectProperties(object obj, List<Type> onResolving)
        {
            Type t = obj.GetType();
            var properties = t.GetProperties();
            foreach (var prop in properties)
            {
                // tylko te, które mają publiczne {set} i są [DependencyProperty]
                if (prop.SetMethod.IsPublic && IsDependencyProperty(prop))
                {
                    object[] parameters = 
                        ResolveDependiencesForMethod(prop.SetMethod, onResolving);
                    prop.SetMethod.Invoke(obj, parameters);
                }
            }
        }

        /// <summary>
        /// Rozwiązuje zależności dla argumentów metody i zwraca 
        /// tablicę obiektów, którymi należy zasilić tę funkcję.
        /// </summary>
        private object[] ResolveDependiencesForMethod(
            MethodBase function, List<Type> onResolving)
        {
            // rozwiąż zależności dla każdego parametru metody
            var types = function.GetParameters()
                .Select((pi) => { return pi.ParameterType; }).ToList();
            int n = types.Count();
            object[] parameters = new object[n];
            for (int i = 0; i < n; i++)
            {
                // cykl -> próbuje rozwiązać typ, który właśnie rozwiązuje
                if (onResolving.Contains(types[i]))
                    throw new Exception(ResolvingCycleErrorMessage(onResolving, types[i]));

                onResolving.Add(types[i]);
                parameters[i] = Resolve(types[i], onResolving);
                onResolving.Remove(types[i]);
            }

            return parameters;
        }

        public void RegisterInstance<T>(T instance)
        {
            // rejestrujemy instancję, czyli domyślnie to MUSI być singleton

            if (!Registered(typeof(T)))
                registeredTypes.Add(typeof(T), typeof(T));

            if (singletons.ContainsKey(typeof(T)))
                singletons[typeof(T)] = instance;
            else singletons.Add(typeof(T), instance);
        }

        private bool IsDependencyConstructor(ConstructorInfo ci)
        {
            var attr = ci.GetCustomAttribute<DependencyConstructor>();
            return attr != null;
        }

        private bool IsDependencyProperty(PropertyInfo pi)
        {
            var attr = pi.GetCustomAttribute<DependencyProperty>();
            return attr != null;
        }

        private bool IsDependencyMethod(MethodInfo mi)
        {
            var attr = mi.GetCustomAttribute<DependencyMethod>();
            return attr != null;
        }


        /// <summary>
        /// Zwraca konstruktory swoje i wszystkich klas
        /// bazowych aż do Object (bez Object).
        /// </summary>
        private ConstructorInfo[] GetAllConstructors(Type t)
        {
            var constructors = new ConstructorInfo[] { };
            if (!t.Name.Equals("Object"))
                constructors = GetAllConstructors(t.BaseType);

            constructors = constructors.Concat(t.GetConstructors()).ToArray();
            return constructors;
        }

        private ConstructorInfo ChooseConstructor(Type t)
        {
            int maxConArgsSize = 0;
            bool foundDepAttr = false;
            var candidates = new List<ConstructorInfo>();

            var constructors = t.GetConstructors();
            foreach (var con in constructors)
            {
                // to ma być publiczny konstruktor
                if (!con.IsPublic) continue;

                if (foundDepAttr)
                {
                    // jeśli już był jakiś z atrybutem,
                    // dodawaj tylko te z atrybutem
                    if (IsDependencyConstructor(con))
                        candidates.Add(con);
                }
                else
                {
                    // pierwszy oznaczony atrybutem
                    if (IsDependencyConstructor(con))
                    {
                        foundDepAttr = true;
                        candidates.Clear();
                        candidates.Add(con);
                    }
                    else
                    {
                        // wybieraj te z najdłuższą listą argumentów
                        var len = con.GetParameters().Length;
                        if (len == maxConArgsSize) candidates.Add(con);
                        else if (len > maxConArgsSize)
                        {
                            maxConArgsSize = len;
                            candidates.Clear();
                            candidates.Add(con);
                        }

                    }
                }

            }

            if (candidates.Count == 0)
                throw new Exception(string.Format(
                    "Not found any constructor in class {0}", t
                    ));

            if (candidates.Count > 1)
                throw new Exception(string.Format(
                    "Ambiguity! To cannot choose one constructor for class {0}. Candidates: {0}", t, candidates.Count
                    ));

            // --> success
            return candidates[0];
        }

        private string ResolvingCycleErrorMessage(List<Type> onResolving, Type type)
        {
            StringBuilder sb = new StringBuilder("Found cycle while resolving dependency: ");
            foreach(var item in onResolving)
                sb.Append(string.Format("{0} -> ", item.Name));

            sb.Append(type.Name);
            return sb.ToString();
        }


    }
}
