pir-medl
========

Dette skal bli en kjapp og grei løsning på en medlemsdatabase som til å 
begynne med skal brukes internt i Piratpartiet. Ny funksjonalitet vil 
legges til etterhvert. Siden dataene blir lagret i en SQLite-database, 
er det fort gjort å lage små scripts som lager rapporter eller 
oppdaterer databasen på en enkel og fleksibel måte.

Kommentarer og pullrequester er selvfølgelig veldig gode ting, spesielt 
hvis noen ser at det går galt avgårde. For å holde kommunikasjonen 
samlet, er det sikkert best å åpne en [issue på 
Gitlab](https://gitlab.com/piratpartiet/pir-medl/issues) hvis det er noe 
som kommer opp. [Eller 
Github](https://github.com/piratpartiet/pir-medl/issues), hvis du liker 
å henge rundt der istedenfor. Begge repoene på Gitlab og Github blir 
oppdatert samtidig.

Status
------

Har begynt på databasestrukturen og de første scriptene. `medl-init` 
funker, den setter opp databasen, men flere kolonner må sikkert legges 
til.

Det ser ut til at saken kanskje ender opp i C, og istedenfor mange små 
scripts skjer alt i en fil, `medl`.

Ellers er prosjektet litt i planleggingsfasen, men det skal gå greit å 
få noe med enkel funksjonalitet på plass i løpet av et par dager. Det 
trengs en maskin der databasen kan kjøres, men systemet inkluderer også 
en distribuert løsning der alle med behov for det får en lokal kopi av 
databasen som de kan jobbe med, og synce den mot de andre brukerne av 
medlemsdatabasen.

Hvis den distribuerte løsningen skal brukes, kreves det (som vanlig) 
Git, ellers må folk som vil hente ut eller legge inn info i databasen ha 
tilgang til en server programmet kan kjøres på.

make-kommandoer
---------------

Disse make-kommandoene er tilgjengelige:

- **make**<br />
  Lag `README.html`, utenom det gjør den ikke mye.
- **make clean**<br />
  Slett alle genererte filer og sett alt tilbake til en jomfruelig 
  tilstand.
- **make test**<br />
  Kjør diverse tester for å se at systemet fungerer greit nok. Sjekker 
  også at alle forutsetninger er til stede, som f.eks. installert 
  programvare osv.
- **make valgrind**<br />
  Kjør `make test` og bruk Valgrind. Tar mye lengre tid enn `make test`, 
  men den tar det meste av tvilsomme ting.
- **make edit**<br />
  Rediger alle filene i repoen. Bruker favoritteditoren din, det vil si 
  den som er definert i `EDITOR`.
- **make README.html**<br />
  Generer `README.html` fra `README.md`. Trenger `cmark`(1) fra 
  <http://commonmark.org>. Gjøres automatisk av `make`.
- **make view**<br />
  Kjør `make README.html` og vis den med `lynx`(1) for å se at alt ser 
  greit ut.

Lisens
======

This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2 of the License, or (at your 
option) any later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along 
with this program.
If not, see <http://www.gnu.org/licenses/>.

---

    File ID: e1eeeb4e-38b5-11e7-a4b8-f74d993421b0
    vim: set et fenc=utf8 fo=clnqtw sts=2 sw=2 ts=2 tw=72 :
