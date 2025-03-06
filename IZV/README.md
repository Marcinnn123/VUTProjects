# Analýza dopravních nehod v ČR (IZV)

Tento projekt analyzuje dopravní nehody v České republice. Obsahuje dvě části:

- `izv-part01` – Práce s matematickými operacemi, vizualizací dat a webovým scrappingem.
- `izv-part02` – Pokročilejší analýza dat a vizualizace statistik nehod.

## Struktura projektu

```
IZV/
│── izv-part01/
│   ├── part01.py
│   ├── test_part01.py
│   ├── tmp_fn.png
│   ├── tmp_sin.png
│   ├── zadani.pdf
│── izv-part02/
│   ├── analysis.py
│   ├── alcohol_plot.png
│   ├── state_plot.png
│   ├── type_plot.png
│   ├── data_23_24.zip
│   ├── zadani.pdf
│── README.md
```

## Instalace a spuštění

### Naklonování repozitáře
```
git clone https://github.com/Marcinnn123/VUTProjects.git
cd VUTProjects/IZV
```

### Vytvoření virtuálního prostředí a instalace závislostí
```
python -m venv venv
source venv/bin/activate   # Linux/macOS
venv\Scripts\activate      # Windows

pip install numpy matplotlib pandas seaborn requests beautifulsoup4
```

## Spuštění první části (`izv-part01`)

Skript `part01.py` obsahuje:
- Výpočet vzdáleností mezi body pomocí numpy.
- Generování sinusových křivek s vizualizací v matplotlib.
- Web scraping dat o meteorologických stanicích pomocí BeautifulSoup.

Spuštění:
```
cd izv-part01
python part01.py
```
Tímto se vygenerují a uloží obrázky funkcí (`tmp_fn.png`, `tmp_sin.png`).

## Spuštění druhé části (`izv-part02`)

Skript `analysis.py` provádí analýzu dopravních nehod. Obsahuje:
- Čištění a zpracování dat.
- Vizualizaci nehod podle krajů a povrchu vozovky.
- Analýzu vlivu alkoholu na dopravní nehody.
- Časový vývoj typů nehod.

Spuštění:
```
cd izv-part02
python analysis.py
```
Vygenerují se vizualizace a uloží jako:
- `alcohol_plot.png` – Počet nehod pod vlivem alkoholu podle krajů.
- `state_plot.png` – Stav povrchu vozovky a jeho vliv na nehody.
- `type_plot.png` – Druhy srážek v průběhu času.

## Požadavky
- Python 3.8+
- Numpy, Pandas – pro výpočty a zpracování dat
- Matplotlib, Seaborn – pro vizualizace
- Requests, BeautifulSoup – pro web scraping
