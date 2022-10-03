# Tema
Să se scrie în limbajul C un program care folosește apeluri sistem și funcții de bibliotecă UNIX, care respectă specificațiile descrise în continuare.<br/>  
Programul este format din două procese. Unul dintre procese folosește o comandă externă pentru a genera lista numelor fișierelor dintr-un director dat ca argument în linia de comandă a programului. Aceste nume trebuie să ajungă printr-un pipe la celălalt proces.<br/>  
Pe măsură ce datele sunt disponibile, lista va fi prelucrată de celălalt proces, care va afișa, pe câte o linie, numele, dimensiunea și identificatorul proprietarului fiecăruia dintre fișierele cu numele primite prin pipe.<br/>  
Indicație: pentru a lansa o comandă externă din interiorul unui proces, se poate folosi una dintre funcțiile de tip exec... .
## Resurse bibliografice
https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter4-IntroductiontoUNIXSystemsProgramming.pdf