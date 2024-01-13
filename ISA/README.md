# DNS resolver

## Autor
Marcin Sochacki <br>
xsocha03

## Datum Vytvoření
19.10.2023

## Popis Programu
Program je jednoduchý DNS resolver v jazyce C++. Program umožňuje provádět DNS dotazy včetně možnosti reverzního dotazu a specifikace dalších parametrů prostřednictvím příkazové řádky.

## Rozšíření
* Aplikace podporuje nevyžadovaný ze zadání záznam NS v autoritativní sekci a je schopna zpracovávat a vypisovat informace obsažené v tomto záznamu.
* Aplikace podporuje nevyžadovaný ze zadání záznam SOA v autoritativní sekci a je schopna zpracovávat a vypisovat všechny informace obsažené v tomto záznamu.

## Nedostatky 
* Aplikace nepodporuje IPv6 adresu jako adresu serveru
* Aplikace nepodporuje IPv6 adresu jako dotazovací adresu 

## Příklad Spuštění
```bash
./dns -s kazi.fit.vutbr.cz www.fit.vut.cz

./dns -s 8.8.8.8 -r google.com

./dns -s kazi.fit.vutbr.cz www.github.com

./dns -s kazi.fit.vutbr.cz facebook.com -r -6

./dns -s kazi.fit.vutbr.cz -r 142.251.36.78 -x
```

## Odevzdané soubory
* dns.cpp
* dns.hpp
* Makefile
* test_script.py
* README.md
* manual.pdf

## Hodnocení 17/20b

