## Rozdzielacz 1.0, autor: Jan Mleczko

Przedstawiam mój program uzytkowy pn. Rozdzielacz napisany w C. Stanowi on pewnego rodzaju osobistą malutką bazę danych działającą na zasadzie przydzielania różnych nazwanych elementów między kilka zbiorów zwanych "szufladami". Działa w systemie MS Windows, ale dzięki przenośności języka programowania C może zostać szybko przeniesiony na inne systemy operacyjne lub nawet rekompilowany na nie bez zmian w kodzie C.

![zrzut ekranu](https://github.com/Jan-Mleczko/Rozdzielacz-app/blob/main/zrz1.png?raw=true)

Program może być pomocny zastępując ręczne notatki podczas zadań organizacyjnych sprowadzających się do rozdzielania:

* przedmiotów na miejsca ich przechowywania albo drogi przewozu,
* osób na zespoły,
* zadań do wykonania na osoby,
* zadań na kilka dni
* itp.

Zaletą programu jest ładna semigraficzna prezentacja rozważanego podziału oraz możliwosć sprawnego przenoszenia elementów miedzy szufladami. Na papierze w tym celu trzeba by było albo wykreślać elementy z nieodpowiedniej szuflady albo wyciąć karteczki z zapisanym każdym elementem, które mogłyby się zsunąć z kartki.

W aktualnej wersji górna granica ilości szuflad wynosi 12, a elementów da się wprowadzić 99. Nazwa szuflady lub elementu może mieć do 15 znaków.

Ponadto Rozdzielacz uwzględnia samoczynne rozdzielanie elementów pomiędzy szuflady (możliwie) po równo albo pseudolosowo na Twoje żądanie. Później możesz dokonać ręcznych poprawek.

Jest oczywiście dostępny zapis stanu danych na dysku w wewnętrznym formacie programu jak również wytworzenie pliku tekstowego zawierającego opracowane informacje, który to będzie mógł oglądać każdy w dowolnym edytorze tekstu.

Jeszcze lepszą (w niektórych przypadkach) możliwością zewnętrznej prezentacji danych jest zapis jako dokument HTML (do przeglądania w przeglądarce internetowej) w jednym z 7 dostępnych styli:

* _Klasyczny_, także w wariantach dostowanych do standardów HTML5 albo XHTML
* _Tabela_
* _Upiększony klasyczny_
* _Nowoczesny_
* _Zielony ogród_
* _Energiczny szaro-czerwony_
* _Panele_

Polskie znaki - jeśli występują w jakichkolwiek wprowadzonych nazwach - są przekodowywane z CP852 na kodowanie Unicode zapewniające ich poprawne wyświetlanie przez prawie każdą przeglądarkę.

Tak wyglądają przykładowe dane zapisane w stylu _Zielony ogród_:

![zrzut ekranu](https://github.com/Jan-Mleczko/Rozdzielacz-app/blob/main/zrz2.png?raw=true)
