# Smarthome Lichtsteuerung
Dieses Programm wurde mit C/C++ programmiert und beinhaltet einen Telegram Chatbot, welcher mit Python programmiert wurde. 

## Was kann das Programm? 

## Wie benutze ich das Programm?
Das Programm nutzt die PlatformIO Erweiterung in Visual Studio Code. Insgesamt, um das Programm zu starten, müssen folgende Schritte befolgt werden: 
* GitHub Repo clonen
* Repo in IDE öffnen (z.B. [VS Code](https://code.visualstudio.com))
* PlatformIO installieren
* M5Stack Core2 anschliessen

<p align="center">
  <img src="./docs/addstack.jpg"/>
</p>


* DLight Sensor Unit (Port A) und LED Streifen (Port C) an den M5Stack Core2 anschliessen
* Option 1: Command Palette öffnen
  * Mac: Shift + Command + P
  * Windows/Linux: Ctrl + Shift + P
  * &rarr; "PlatformIO: Build"
  * &rarr; "PlatformIO: Upload and Monitor"

<p align="center">
  <img src="./docs/piosymbols.jpg"/>
</p>

* Option 2: Button
  * Häkchen &#x2713; für "Build"
  * Pfeil &rarr; für "Upload and Monitor"


## Links
- [Lux Werte und Bedeutung](https://learn.microsoft.com/en-us/windows/win32/sensorsapi/understanding-and-interpreting-lux-values)
- [Telegram Chatbot](https://t.me/lightningmcbot)
- [Verwendeter Beispielcode FastLED](https://github.com/FastLED/FastLED/tree/master/examples)
- [Verwendete Grundstruktur des Projekts](https://github.com/alptbz/core2guidemo)