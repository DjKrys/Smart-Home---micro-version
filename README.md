# Smart-Home---micro-version
Urządzenie do kontroli temperatury w pokoju oraz sterowanie oświetleniem.

# Założenia

# Opis
Dzięki temu urządzeniu możliwa jest regulacja jasnością oświetlenia w pokoju. Oświetlenie te pracuje głównie przy zmiennym napięciu zasilania 230V. 
Aby osiągnać efekt ściemniania oraz rozjaśniania żarówki należy zastosować detektor zera. Napięcie w sieci energetycznej jest napięciem przemiennym o częstotliwości 50Hz.
Jest to sinusoida, która należy modulować poprzez wycinanie odpowiedniej części połówek sygnału, aby uzyskać różny poziom świecenia żarówki. Po wykryciu zera uruchamiany jest pwm zbudowany na liczniku, który przez określony czas otwiera lub zamyka obwód z żarówką.

Urządzenie pozwala również kontrolę temperatury w pokoju. Po nastawieniu zadanej temperatury układ automatycznie steruje ogrzewaniem. Odbywa sie to poprzez zastosowanie siłownika sterującego otwarciem lub zamknięciem zaworu, który znajduje się przy grzejniku.
Siłownik ten posiada dużą histereze, dzięki czemu regulacja temperatury będzie dokładniejsza.
 
