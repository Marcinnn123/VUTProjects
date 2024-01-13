import subprocess
import re

def run_test():
    test_cases = [
        ('Neplatná kombinace možností -6 a -x', ['-r', '-s', '8.8.8.8', 'google.com', '-6', '-x'], "Možnosti -6 a -x vzájemně nejsou podporovane. \n"),
        ('Neplatná adresa serveru', ['-r', '-s', 'xxx', 'google.com', '-6'], "Chyba při rozpoznávání serveru: xxx\n"),
        ('Chybějící port', ['-r', '-s', '8.8.8.8', 'google.com', '-p'], "Musíte zadat port.\n"),
        ('Více než jedna možnost \'-s\'', ['-r', '-s', '8.8.8.8', '-s', 'google.com'], "Nepodporuje se více než jednu možnost '-s'\n"),
        ('Více než jedna možnost \'-r\'', ['-r', '-s', '8.8.8.8', '-r', 'google.com'], "Nepodporuje se více než jednu možnost '-r'\n"),
        ('Chybějící server', ['-6', '-r', 'google.com'], "Chybějící server nebo dotazovaná adresa.\n"),
        ('Chybějící dotazovaná adresa', ['-r', '-s', '8.8.8.8'], "Chybějící server nebo dotazovaná adresa.\n"),
        ('Neplatná IP adresa', ['-r', '-s', '8.8.8.8', '-x', '8.9.8'], "Neplatná IP adresa: 8.9.8\n"),
        ('Chyba při rozpoznávání IP adresy serveru', ['-r', '-s', '8.8.8.', 'google.com'], "Chyba při rozpoznávání serveru: 8.8.8.\n"),
        ('Nekorektní číslo portu', ['-r', '-s', '8.8.8.8', 'google.com', '-p', '66423'], "Port musí být mezi 1 a 65535.\n")
    ]

    pass_count = 0
    test_count = 1

    for test_name, args, expected_output in test_cases:
        print(f"\nTest {test_count}: {test_name}")
        result = subprocess.run(['./dns'] + args, stderr=subprocess.PIPE, universal_newlines=True)
        print(f"--Vstup: ./dns {' '.join(args)}")
        print(f"--Výstup aplikace:  {result.stderr}", end="")
        print(f"--Očekávaný výstup: {expected_output}", end="")
        
        if result.stderr == expected_output:
            print(f"Test {test_count} úspěšně prošel!\n")
            pass_count += 10
        else:
            print(f"Test {test_count} selhal!\n")
        test_count += 1

    return pass_count
    

def dns_answer(ip_address, type_rec):
    try:
        if type_rec == 1:
            result = subprocess.run(['./dns', '-r', '-s', '8.8.8.8', '-x', ip_address], stdout=subprocess.PIPE, universal_newlines=True, check=True)
        else:
            result = subprocess.run(['./dns', '-r', '-s', '8.8.8.8', '-6', ip_address], stdout=subprocess.PIPE, universal_newlines=True, check=True)
        match_app = re.search(r"Answer section \(\d+\)\n\s+([^\n]+)\n", result.stdout)
        return match_app.group(1) if match_app else None
    except subprocess.CalledProcessError as e:
        print(f"Error running ./dns: {e.stderr}")
        return None

def dig_answer(ip_address, type_rec):
    try:
        if type_rec == 1:
            result = subprocess.run(['dig', '@8.8.8.8', '-x', ip_address], stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True, check=True)
        else:
            result = subprocess.run(['dig', '@8.8.8.8', 'AAAA', ip_address], stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True, check=True)
        answer_section_match = re.search(r';; ANSWER SECTION:[^\n]*\n([^\n]*)', result.stdout)
        return answer_section_match.group(1).strip() if answer_section_match else None
    except subprocess.CalledProcessError as e:
        print(f"Error running dig: {e.stderr}")
        return None

def compare_outputs(ip_address, count, type_rec):
    answer_app = dns_answer(ip_address, type_rec)
    answer_dig = dig_answer(ip_address, type_rec)


    if answer_app is not None and answer_dig is not None:
        array1 = [x.strip() for x in answer_app.split(',')]
        array2 = answer_dig.split()
        app = "".join(array1[0]+" "+array1[1]+" "+array1[2]+" "+array1[4])
        dig = "".join(array2[0]+" "+array2[3]+" "+array2[2]+" "+array2[4])        
        print(f"Test {count}: {ip_address}")
        print(f"--Výstup aplikace: {app}")
        print(f"--Výstup dig:      {dig}")
        if app == dig:
            print(f"Test {count} úspěšně prošel!\n")
            return 10
        else:
            print(f"Test {count} selhal!\n")
            return 0


if __name__ == "__main__":
    print("TESTY NA KONTROLU CHYBNÝCH VSTUPŮ")
    print("----------------------------------------------")
    pass_count = run_test()
    print("TESTY NA KONTROLU AAAA A PTR ZÁZNAMŮ")
    print("----------------------------------------------")
    pass_count2 = 0
    pass_count2 += compare_outputs('77.75.77.222', 1, 1)
    pass_count2 += compare_outputs('147.229.2.90', 2, 1)
    pass_count2 += compare_outputs('140.82.121.3', 3, 1)
    pass_count2 += compare_outputs('69.164.199.26', 4, 1)
    pass_count2 += compare_outputs('147.229.9.21', 5, 1)
    pass_count2 += compare_outputs('instagram.com', 6, 2)
    pass_count2 += compare_outputs('facebook.com', 7, 2)
    pass_count2 += compare_outputs('rhino.cis.vutbr.cz', 8, 2)
    pass_count2 += compare_outputs('gmail.com', 9, 2)
    pass_count2 += compare_outputs('kazi.fit.vutbr.cz', 10, 2)
    print("----------------------------------------------")
    print(f"Prošlo {pass_count}% testů na kontrolu chybného vstupu")
    print(f"Prošlo {pass_count2}% testů na kontrolu AAAA a PTR záznamů")
    