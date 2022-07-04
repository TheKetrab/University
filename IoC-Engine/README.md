# Inversion of Control / Dependency Injection

Własna implementacja kontenera - dwa słowniki:
* registeredTypes: Kontrakt typu -> konkretny typ, którym go implementujemy
* singletons: Typ -> obiekt, który obecnie jest wydawany przy prośbie o obiekt tego typu

Przy rozwiązywaniu problemu zależności, kontener wybiera konstruktor z największą liczbą argumentów lub oznaczony atrybutem **[DependencyConstructor]**. Kontener rozwiązuje również zależności dla właściwości i metod oznaczonych odpowiednio [DependencyProperty] i [DependencyMethod].

Uwaga: W przypadku wykrycia cyklu przy rozwiązywaniu zależności wyrzucany jest wyjątek. Np.: T1 -> T2 -> T3 -> T1 ERROR !!!

## Metody
* Registered(Type) - czy typ jest zarejestrowany
* RegisterType<T>(singleton?) - rejestruje typ T, opcjonalnie ustawia jako singleton
* RegisterType<Base,Concrete>(singleton?) - rejestruje typ Base typem Concrete
* BuildUp(instance) - wstrzykuje do obiektu instance zależności, które są pobierane z kontenera (do składowych oznaczonych DependencyProperty, DependencyMethod)
* RegisterInstance - rejestruje konkretną instancję jako singleton dla klasy, aby jej uzywać przy rozwiązywaniu zależności
* Resolve<T> - zwraca obiekt danego typu rozwiązując zależności zapisane w kontenerze

## Użycie

SimpleContainer

```csharp
    SimpleContainer c = new SimpleContainer();
    c.RegisterInstance<int>(1);
    c.RegisterType<IFoo,Foo>();

    IFoo obj = c.Resolve<IFoo>();
    // obj.GetType() == typeof(Foo)
```

Service Locator
```csharp
    // main
    SimpleContainer c = new SimpleContainer();
    c.RegisterInstance<int>(1);
    ServiceLocator.SetContainerProvider(() => c);

    // another context
    int res = ServiceLocator.Current.GetInstance<int>();
```

Local Factory
```csharp
    // Composition Root
    SimpleContainer c = new SimpleContainer();
    c.RegisterType<Tone, YellowTone>();
    c.RegisterType<IPrinter,Printer>();
    PrinterFactory.SetProvider(() => c.Resolve<IPrinter>());

    // ... program ...
    var printer = new PrinterFactory().Create();
    printer.Print("Hello World!"); // na żółto
```