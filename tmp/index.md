For English scroll below.

/tmp - tiny/tani mp3 player
===========================

Cel projektu
------------

Zbudowanie jak najmniejszego, jak najprostszego i jak najtańszego,
przenośnego odtwarzacza MP3. Po co kupować coś, co można zrobić samemu?
Po co składać [cudzy projekt](http://www.yampp.com/),
skoro można mieć własny od zera? Przy okazji można nauczyć się wielu
przydatnych rzeczy. I mieć sensowne zajęcie na kilka wieczorów.
Skąd taka głupia nazwa? Nie mam pojęcia.

![Zdjęcie /tmp](foto1.jpg)

Co nowego?
----------

* 2005-03-03: Informacja o zakłóceniach i sposobie obejścia problemu.
* 2005-02-07: Dodanie informacji o szczegółach implementacji i zdjęć projektu.
* 2005-01-23: Umieszczenie na stronie uaktualnionego oprogramowania.
* 2005-01-15: Projekt można uznać za zakończony. Urządzenie zostało złożone, działa, mam w końcu odtwarzacz MP3.
* 2004-03-28: Z powodu braku czasu na dopieszczenie płytki, postanowiłem opublikować źródła. Komentarze, sugestie, uwagi i poprawki mile widziane. Dla tych, których nie lubią dużo czytać, droga na [skróty](https://github.com/wojtekka/tmp/releases/)
* 2004-02-13: Znalazłem już odpowiednią obudowę (radyjko kupione w sklepie _Wszystko za 5zł_). Niestety ze względu na mikroskopijne rozmiary nie dam rady upakować wszyskiego na płytce jednostronnej. Nie zmieszczę też żadnej przetwornicy. Za to mam łądną obudowę z miejscem na 3 przyciski (regulacja głośności będzie cyklicznie w górę) i dwie baterie AAA. Zostało tylko upchnąć
wszystko na płytce wymiarów 5x2,5 cm.
* 2003-12-23: Pierwsza wersja strony.

Wykorzystany sprzęt
-------------------

* Procesor [AT90S2313](http://www.atmel.com/dyn/products/product_card.asp?part_id=1993). Kosztuje poniżej 5zł, jest mały, bez problemu działa z zasilaniem 3V (nawet w wersji bez _L_), łatwo go oprogramować dzięki istnieniu portu _gcc_. Jego 15 pinów w zupełności wystarczy do obsługi karty pamięci, dekodera MP3 i kilku przycisków, a przy zegarze 6 MHz bez problemu obsłuży strumień 128 kbps.
* Dekoder [VS1001K](http://www.vlsi.fi/vs1001/vs1001.htm). Tani nie jest (około $20, najdroższa część projektu), ale wygląda na to, że jest najpopularniejszy i najłatwiej dostępny. Poza tym nie wymaga zewnętrznego przetwornika C/A, ani wzmacniacza słuchawkowego.
* Karta pamięci [MMC](http://www.mmca.org/). Do obsługi wystarczą tylko 4 piny procesora.

Wykorzystane oprogramowanie i materiały
---------------------------------------
* _avr-gcc_
* Dokumentacja kart MMC firm [SanDisk](http://www.sandisk.com/) i [Hitachi](http://www.hitachi.com/)
* Dokumentacja dekodera VS1001K
* W sytuacjach kryzysowych zaglądałem do źródeł [YAMPP-7](http://www.yampp.com/)

Wady
----

* Brak wyświetlacza LCD. Przez większość czasu odtwarzacz i tak będzie leżeć w kieszeni albo plecaku, więc z wyświetlacza nie skorzystam. Walkmany nie wyświetlają aktualnego czasu i nikt z tego powodu nie płacze. Tagi ID3 nie są mi potrzebne, bo dobrze wiem, co załadowałem na kartę.

* Brak możliwości wrzucenia plików na kartę. Pamięć użytego procesora jest zbyt mała, żeby zmieścić obsługę USB i zapisu na kartę. Poza tym, kolejny układ i gniazdo USB znacznie zwiększyłyby rozmiar odtwarzacza. W tej chwili najprostsze czytniki kart MMC kosztują mniej niż 50zł. Poza tym, jeśli ktoś ma cyfrowy aparat fotograficzny na karty tego typu, może z niego korzystać do wrzucania plików (ja tak robię).

* Brak możliwości przewijania utworów. Tak jak w komputerowych odtwarzaczach CD, są dwa przyciski: ![Stop](stop.png) zatrzymujący odtwarzanie i wracający na początek listy oraz ![Play/Next](playnext.png) rozpoczynający odtwarzanie lub przechodzący do kolejnego utworu.

Zalety
------

* Bardzo prosta budowa. ~~Projektu płytki jeszcze nie ma, ale będzie to płytka jednostronna, którą będzie można wykonać nawet ręcznie.~~ Większość części można dostać w każdym sensownie zaopatrzonym sklepie elektronicznym. Dekoder MP3 kupiłem w [Jelu Handelsbolag](http://www.jelu.se/) w Szwecji jeszcze przed wstąpieniem Polski do Unii Europejskiej (teraz jest droższy ze względu na podatek). Złącze karty MMC w [Cyfronice](http://www.cyfronika.com.pl/). Jeśli w Twoim sklepie nie ma AT90S2313, spróbuj w [Seguro](http://www.seguro.pl/).

* Obsługa systemów plików FAT12 i FAT16, a nie własnego, jak w innych odtwarzaczach MP3 do samodzielnego złożenia. Dlatego pliki można wrzucać na kartę za pomocą dowolnego czytnika, pod dowolnym systemem operacyjnym.

* Oprogramowanie dostępne na licencji GPL.

Szczegóły implementacji
-----------------------

* Procesor jest taktowany zegarem 6MHz, ponieważ jest to najniższa częstotliwość przy której poprawnie były odtwarzane pliki 128kbps. Jeśli chcesz odtwarzać pliki większej jakości, kosztem większego zużycia prądu, powinieneś użyć szybszego zegara.

* Po uruchomieniu procesor wchodzi w stan uśpienia, dzięki czemu pobiera minimalną ilość energii. Przycisk rozpoczynający odtwarzanie jest podłączony do wejścia przerwania, więc wciśnięcie budzi procesor ze snu i rozpoczynane jest otwarzanie. Przejście do kolejnego utworu jest obsługiwane programowo.  Przycisk zatrzymujący odtwarzanie podłączono do wejścia zerującego procesora, dzięki czemu nie ma konieczności programowej obsługi.

* Ze względu na trudności ze zdobyciem kwarcu 12,288MHz, wykorzystano zwykły kwarc 12MHz. Różnica prędkości odtwarzania około 2% jest praktycznie niezauważalna.

* <span style="color: red;">Ścieżki masy i zasilania w pierwszej wersji płytki są dość cienkie, co powoduje słyszalne zakłócenia. Pomaga połączenie kawałkiem grubego przewodu masy procesora z padem baterii tak jak to widać na zdjęciu prototypu.</span>

Schemat
-------

![Schemat](tmp.sch.png)

Płytka
------

![Płytka](tmp.pcb.png)

Zdjęcia prototypu
-----------------

![Zdjęcie 1](foto2.jpg) ![Zdjęcie 2](foto3.jpg)

Firmware
--------

Dostępne na [stronie projektu](https://github.com/wojtekka/tmp/releases/)

Co potem?
---------

* Zrobić sobie czytnik kart MMC, żeby nie musieć przekładać kart w aparacie co chwilę. Ale to już inna bajka.

* Jeśli [Atmel](http://www.atmel.com/) wypuści układ zgodny od strony pinów z 2313, ale z większą pamięcią Flash, pomyśleć co jeszcze można dorzucić.


<h3>Project goals</h3>
<p>To build small, simple and as cheap as possible portable MP3 player.
Why buy something when you can do it yourself? Why assemble
<a href="http://www.yampp.com/">someone else's</a> project when you can have
your own? By the way, you can learn many useful things. The word ,,tani" in
the name means ,,cheap'' in Polish.</p>
<p class="screenshot"><img src="foto1.jpg" alt="[Picture of /tmp]" width="379" height="268" /></p>

<h3>What's new?</h3>
<ul class="space">
<li>2005-03-03: Information about interferences and the workaround.</li>
<li>2005-01-23: Added implementation details and pictures of the project.</li>
<li>2005-01-23: Updated firmware has been put on the site.</li>
<li>2005-01-15: The project has been finished. The device has been assembled,
it works and at last I have my own MP3 player.</li>
<li>2004-08-06: First English version.</li>
</ul>

<h3>Hardware</h3>
<ul class="space">
<li>MCU: <a href="http://www.atmel.com/dyn/products/product_card.asp?part_id=1993">AT90S2313</a>. It costs about $3 in Poland, it's small, works fine at 3V (even the non-,,L'' version), easy to program with gcc port. Its 15 I/O pins are enough for memory card, MP3 decoder and some buttons. It will handle 128kbps at 6MHz crystal.</li>
<li>MP3 decoder: <a href="http://www.vlsi.fi/vs1001/vs1001.htm">VS1001K</a>. The most expensive part of the project, but it seems that this is the most popular and the most available decoder chip. Besides, it doesn't require external D/A nor headphone amplifier.</li>
<li>Flash card: <a href="http://www.mmca.org/">MMC</a>. It can be handles with only 4 I/O pins.</li>
</ul>

<h3>Software and docs</h3>
<ul class="space">
<li>avr-gcc.</li>
<li>MMC flash card docs from <a href="http://www.sandisk.com/">SanDisk</a> and <a href="http://www.hitachi.com/">Hitachi</a>.</li>
<li>VS1001K docs.</li>
<li>In crisis situations I've look at <a href="http://www.yampp.com/">YAMPP-7</a> source code.</li>
</ul>

<h3>Cons</h3>
<ul class="space">
<li>No LCD. Most of the time, the player will be in my pocket or backpack, so I won't use it anyway. Walkmans don't show current time and no one complains. I don't need ID3 because I know what I have uploaded to the card.</li>

<li>No PC connectivity. MCU's flash memory is too small to fit USB and MMC write support and another chip (USB inteface) and a socket would drastically increase player size. Currently the cheapest MMC card readers cost about $10, so it isn't an issue. Besides, if someone has a digital camera with MMC/SD cards, he already has a MMC reader (like I do.)</li>

<li>No fast-forward or rewind. Like in some CD-ROM drives, there are only two buttons: <img src="stop.png" alt="Stop" width="15" height="16" /> stopping the playback and going back to the beginning of the playlist and <img src="playnext.png" alt="Play/Next" width="32" height="16" /> starting playback or skipping to the next track.</li>

</ul>

<h3>Pros</h3>
<ul class="space">
<li>It's very simple. The PCB is about 2"x1". Most of the parts can be bought in any electronics store. I bought a MP3 decoder in <a href="http://www.jelu.se/">Jelu Handelsbolag</a> in Sweden.</li>

<li>It supports FAT12 and FAT16 filesystems not some own like some DIY MP3 players. That's why you can upload files using any MMC reader under any operating system.</li>

<li>Its software is available under the terms of GPL.</li>
</ul>

<h3>Implementation details</h3>
<ul class="space">
<li>MCU is run at 6MHz because it's the lowest frequency at which 128kbps
files are played correctly. If you want to play higher quality files at the
cost of higher power consumption, you should use faster crystal.</li>

<li>After power-up the MCU goes to idle mode so it consumes minimal amount
of energy. Play button is connected to external interrupt pin and pressing
it wakes up the MCU and starts the playback. When playing, the forward
button is polled by software. Stop button is connected to reset input so
it doesn't have to be checked by software.</li>

<li>Because of troubles with getting 12.288MHz crystal, 12MHz has been used.
The difference in playback speed is about 2% to it's barely practically
unnoticable.</li>

<li style="color: red;">Ground and power wires on the first revision of the
board are are too thin and it causes audible interferences. The workaround
it to connect MCU ground with battery pad with a thick wire as seen on the
prototype image.</li>

</ul>

<h3>Schematic</h3>
<p class="center">
<img src="tmpschem2.png" width="550" height="282" alt="Schematic" />
</p>
<p class="center">
<a href="tmp.sch">Eagle</a> | <a href="tmpschem.png">PNG</a>
</p>

<h3>Board</h3>
<p class="center">
<img src="tmppcb2.png" width="549" height="353" alt="Board" />
</p>
<p class="center">
<a href="tmp.brd">Eagle</a> | <a href="tmppcb.png">PNG</a>
</p>

<h3>Pictures of prototype</h3>
<p class="center">
<img src="foto2.jpg" width="241" height="182" alt="Picture" />
<img src="foto3.jpg" width="241" height="182" alt="Picture" />
</p>

<h3 id="pliki">Firmware</h3>

<ul class="space">
<li><a href="tmp-20040328.zip"><b>tmp-20040328.zip</b></a> (14kB)</li>
<li><a href="tmp-20050123.zip"><b>tmp-20050123.zip</b></a> (15kB)</li>
</ul>

