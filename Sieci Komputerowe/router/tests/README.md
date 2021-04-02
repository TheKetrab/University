## Konfiguracja środowiska maszyn wirtualnych

1. W VirtualBoxie włączamy odpowiednią ilość kart sieciowych.
2. Każda jest podpięta do Internetu przez sieć Linuxa (NAT) i są podpięte między sobą (internal -> localXY).
3. Następnie na wystartowanych maszynach wpisujemy odpowiednie polecenia, aby skonfigurować karty sieciowe.
4. Na Linuxie kompilujemy i składamy archiwum (make zip).
5. Potem wrzucamy do sieci (np. dysk Google) i na każdej maszynie pobieramy to, co potrzeba.
6. Rozpakowujemy, składamy make'iem i odpalamy, np: ./router input2A.txt

```py
Komendy to edycji plików w terminalu:
tar -xvf yourfile.tar   # rozpakowanie archiwum
rm Downloads/*          # usuniecie wszystkich plikow
mv filename1 filename2  # zmiana nazwy (np mv inputA.txt input.txt)
tar -czvf program.tar.gz Makefile entry.c entry.h main.c router.c router.h util.h util.c
# @up - tworzy paczkę z plikami do przesłania na Virbiany

Edycja vimem:
vim file   # odpala vima na pliku
i          # wejdz to trybu edycji (INSERT)
...        # edytujemy plik
ESC        # wroc do menu
:wq        # wyjdz zapisujac
```

## Test 1

```
          2 -- C
    4    /     |
A ----- B      | 2
         \     |
          3 -- D

VirbianA:
1 enp0s3 - NAT
2 enp0s8 - localAB

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 172.16.1.13/16 dev enp0s8

VirbianB:
1 enp0s3 - NAT
2 enp0s8 - localAB
3 enp0s9 - localBC
4 enp0s10 - localBD

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 172.16.1.14/16 dev enp0s8
sudo ip link set up dev enp0s9
sudo ip addr add 192.168.2.10/24 dev enp0s9
sudo ip link set up dev enp0s10
sudo ip addr add 10.0.1.2/8 dev enp0s10

VirbianC:
1 enp0s3 - NAT
2 enp0s8 - localBC
3 enp0s9 - localCD

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 192.168.2.5/24 dev enp0s8
sudo ip link set up dev enp0s9
sudo ip addr add 192.168.5.5/24 dev enp0s9

VirbianD:
1 enp0s3 - NAT
2 enp0s8 - localBD
3 enp0s9 - localCD

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 10.0.1.1/8 dev enp0s8
sudo ip link set up dev enp0s9
sudo ip addr add 192.168.5.43/24 dev enp0s9

```

## Test 2

```
                3 (192.168.0.0/16)
            A ----- B
    1       |       |
(130.170.)  |       | 4 (252.33.128.0/17)
(30.208/28) |       |
            D ----- C
                6 (8.224.0.0/11)

VirbianA:
1 enp0s3 - NAT
2 enp0s8 - localAB
3 enp0s9 - localAD

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 192.168.2.10/16 dev enp0s8
sudo ip link set up dev enp0s9
sudo ip addr add 130.170.30.210/28 dev enp0s9

VirbianB:
1 enp0s3 - NAT
2 enp0s8 - localAB
3 enp0s9 - localBC

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 192.168.8.77/16 dev enp0s8
sudo ip link set up dev enp0s9
sudo ip addr add 252.33.147.5/17 dev enp0s9

VirbianC:
1 enp0s3 - NAT
2 enp0s8 - localBC
3 enp0s9 - localCD

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 252.33.252.13/17 dev enp0s8
sudo ip link set up dev enp0s9
sudo ip addr add 8.249.73.11/11 dev enp0s9

VirbianD:
1 enp0s3 - NAT
2 enp0s8 - localCD
3 enp0s9 - localAD

sudo ip link set up dev enp0s3
sudo dhclient -v enp0s3
sudo ip link set up dev enp0s8
sudo ip addr add 8.232.7.55/11 dev enp0s8
sudo ip link set up dev enp0s9
sudo ip addr add 130.170.30.210/28 dev enp0s9
```
