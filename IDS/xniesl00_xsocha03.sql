-- ZADÁNÍ Č. 33 - REALITNÍ KANCELÁŘ

DROP TABLE NEMOVITOST CASCADE CONSTRAINTS;
DROP TABLE KLIENT CASCADE CONSTRAINTS;
DROP TABLE MAKLER CASCADE CONSTRAINTS;
DROP TABLE ZAKAZNIK CASCADE CONSTRAINTS;
DROP TABLE OSOBA CASCADE CONSTRAINTS;
DROP TABLE SMLOUVA CASCADE CONSTRAINTS;


CREATE TABLE NEMOVITOST (
    ID_NEMOVITOSTI NUMBER(8) GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    NEMOVITOST_CENA NUMBER(8) NOT NULL,
    NEMOVITOST_ADRESA VARCHAR2(255) NOT NULL,
    NEMOVITOST_TYP VARCHAR2(255) NOT NULL,
    NEMOVITOST_STAV VARCHAR2(255) NOT NULL
);
-- ENTITA "OSOBA" SE SPECIALIZUJE NA ENTITY "MAKLER" a "ZAKAZNIK" <--- změna oproti ER diagramu.
CREATE TABLE OSOBA (
    ID NUMBER(8) GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    JMENO VARCHAR(255) NOT NULL,
    PRIJMENI VARCHAR(255) NOT NULL,
    TELEFON NUMBER(9) NOT NULL
);

CREATE TABLE KLIENT (
    ID NUMBER(8) GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    EMAIL VARCHAR(255) CHECK (EMAIL LIKE '%@%.%') NOT NULL,
    KL_LOGIN VARCHAR(255) NOT NULL
);

CREATE TABLE ZAKAZNIK (
    ID NUMBER(8) GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    ID_OSOBY NUMBER(8),
    FOREIGN KEY(ID_OSOBY) REFERENCES OSOBA(ID),
    DATUM_NAROZENI DATE NOT NULL,
    BYDLISTE VARCHAR(255) NOT NULL
);

CREATE TABLE MAKLER (
    ID NUMBER(8) GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    ID_OSOBY NUMBER(8),
    FOREIGN KEY(ID_OSOBY) REFERENCES OSOBA(ID)
);

-- BYLY OPRAVENY CHYBĚJÍCÍ CIZÍ KLÍČE 
CREATE TABLE SMLOUVA (
    ID NUMBER(8) GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    DATUM_UZAVRENI_SMLOUVY DATE NOT NULL,
    STAV_SMLOUVY VARCHAR(255) NOT NULL,
    ID_ZAKAZNIKA NUMBER(8),
    ID_MAKLERE NUMBER(8),
    ID_NEMOVITOSTI NUMBER(8),
    FOREIGN KEY(ID_MAKLERE) REFERENCES MAKLER(ID),
    FOREIGN KEY(ID_ZAKAZNIKA) REFERENCES ZAKAZNIK(ID),
    FOREIGN KEY(ID_NEMOVITOSTI) REFERENCES NEMOVITOST(ID_NEMOVITOSTI)
);

-------TRIGGERS----------------------------------------------------
CREATE OR REPLACE TRIGGER prevent_duplicate_email
BEFORE INSERT ON KLIENT
FOR EACH ROW
DECLARE
email_count INTEGER;
BEGIN
SELECT COUNT(*) INTO email_count
FROM KLIENT
WHERE EMAIL = :NEW.EMAIL;
IF email_count > 0 THEN
RAISE_APPLICATION_ERROR(-20001, 'Klient s tímto emailem již existuje.');
END IF;
END;
/

CREATE OR REPLACE TRIGGER check_osoba
BEFORE INSERT OR UPDATE ON OSOBA
FOR EACH ROW
DECLARE
    v_jmeno VARCHAR(255);
    v_prijmeni VARCHAR(255);
BEGIN
    v_jmeno := :NEW.JMENO;
    v_prijmeni := :NEW.PRIJMENI;
    
    IF NOT REGEXP_LIKE(v_jmeno, '^[[:alpha:]]+$') THEN
        RAISE_APPLICATION_ERROR(-20001, 'Jméno musí obsahovat pouze písmena.');
    END IF;
    
    IF NOT REGEXP_LIKE(v_prijmeni, '^[[:alpha:]]+$') THEN
        RAISE_APPLICATION_ERROR(-20001, 'Příjmení musí obsahovat pouze písmena.');
    END IF;
END;
/

-------PROCEDURES---------------------------------------------------
CREATE OR REPLACE PROCEDURE popis_nemovitosti IS  
    nemovitost_r NEMOVITOST%ROWTYPE;
    CURSOR popis IS SELECT * FROM NEMOVITOST;
BEGIN
OPEN popis;
FETCH popis INTO nemovitost_r;
WHILE popis%FOUND
    LOOP
        DBMS_OUTPUT.PUT_LINE(nemovitost_r.NEMOVITOST_ADRESA || ': ' || CAST(nemovitost_r.NEMOVITOST_CENA AS VARCHAR));
        FETCH popis INTO nemovitost_r;
    END LOOP;
CLOSE popis;
END;
/

CREATE OR REPLACE PROCEDURE cena_nemovitosti IS  
    nemovitost_c INTEGER;
    nemovitost_t VARCHAR(255);
    CURSOR cena IS SELECT NEMOVITOST_CENA, NEMOVITOST_TYP FROM NEMOVITOST;
BEGIN
OPEN cena;
FETCH cena INTO nemovitost_c, nemovitost_t;
WHILE cena%FOUND
    LOOP
        DBMS_OUTPUT.PUT_LINE(nemovitost_t || ': ' || nemovitost_c);
        FETCH cena INTO nemovitost_c, nemovitost_t;
    END LOOP;
    EXCEPTION
            WHEN VALUE_ERROR THEN
                DBMS_OUTPUT.PUT_LINE('Špatný datový typ');
CLOSE cena;
END;
/

------ INSERTS -----------------------------------------------------
-------NEMOVITOST ------
INSERT INTO NEMOVITOST(NEMOVITOST_ADRESA, NEMOVITOST_TYP, NEMOVITOST_CENA, NEMOVITOST_STAV)
VALUES ('Mikulov 1255', 'byt', '2399000', 'NA PRODEJ');

INSERT INTO NEMOVITOST(NEMOVITOST_ADRESA, NEMOVITOST_TYP, NEMOVITOST_CENA, NEMOVITOST_STAV)
VALUES ('Praha, Polská 325', 'byt', '5500000', 'NA PRODEJ');

INSERT INTO NEMOVITOST(NEMOVITOST_ADRESA, NEMOVITOST_TYP, NEMOVITOST_CENA, NEMOVITOST_STAV)
VALUES ('Brno, Veveří 121', 'byt', '17000', 'PRONÁJEM');

------- OSOBA ----------
INSERT INTO OSOBA(JMENO, PRIJMENI,TELEFON)
VALUES('Pavel', 'Petr', '733598244');

INSERT INTO OSOBA(JMENO, PRIJMENI,TELEFON)
VALUES('Libor', 'Král', '606771005');

INSERT INTO OSOBA(JMENO, PRIJMENI,TELEFON)
VALUES('Jan', 'Kachnička', '720846988');

INSERT INTO OSOBA(JMENO, PRIJMENI,TELEFON)
VALUES('Ondřej', 'Růžička', '724842999');

INSERT INTO OSOBA(JMENO, PRIJMENI,TELEFON)
VALUES('Jaroslav', 'Švancara', '729846228');

------ KLIENT ----------
INSERT INTO KLIENT(EMAIL, KL_LOGIN)
VALUES('novak87@email.cz', 'novak.jiri');

INSERT INTO KLIENT(EMAIL, KL_LOGIN)
VALUES('pokemon32@centrum.cz', 'pokemon32');
------ ZAKAZNIK --------
INSERT INTO ZAKAZNIK(BYDLISTE,DATUM_NAROZENI,ID_OSOBY)
VALUES('Poděbrady, Dlouhá 23', TO_DATE('23.05.1978', 'dd/mm/yyyy'), '3');

INSERT INTO ZAKAZNIK(BYDLISTE,DATUM_NAROZENI,ID_OSOBY)
VALUES('Bukovec 305', TO_DATE('14.11.1966', 'dd/mm/yyyy'),'4');

INSERT INTO ZAKAZNIK(BYDLISTE,DATUM_NAROZENI,ID_OSOBY)
VALUES('Jablonec nad Nisou, Veselá 603', TO_DATE('01.08.1995', 'dd/mm/yyyy'), '5');
------ MAKLER ----------
INSERT INTO MAKLER(ID_OSOBY)
VALUES('1');

INSERT INTO MAKLER(ID_OSOBY)
VALUES('2');

------ SMLOUVA ---------
INSERT INTO SMLOUVA(STAV_SMLOUVY,DATUM_UZAVRENI_SMLOUVY,ID_ZAKAZNIKA,ID_MAKLERE,ID_NEMOVITOSTI)
VALUES('UZAVŘENÁ', TO_DATE('12.08.2022', 'dd/mm/yyyy'),'3','1','1');

INSERT INTO SMLOUVA(STAV_SMLOUVY,DATUM_UZAVRENI_SMLOUVY,ID_ZAKAZNIKA,ID_MAKLERE,ID_NEMOVITOSTI)
VALUES('UZAVŘENÁ', TO_DATE('23.02.2022', 'dd/mm/yyyy'),'1','2','2');

INSERT INTO SMLOUVA(STAV_SMLOUVY,DATUM_UZAVRENI_SMLOUVY,ID_ZAKAZNIKA,ID_MAKLERE,ID_NEMOVITOSTI)
VALUES('UZAVŘENÁ', TO_DATE('14.01.2023', 'dd/mm/yyyy'),'2','2','2');

-----------EXEC-------------------------------------------------------
EXEC popis_nemovitosti;
EXEC cena_nemovitosti;

--------------
GRANT ALL ON NEMOVITOST TO xsocha03;
GRANT ALL ON OSOBA TO xsocha03;
GRANT ALL ON ZAKAZNIK TO xsocha03;
GRANT ALL ON KLIENT TO xsocha03;
GRANT ALL ON MAKLER TO xsocha03;
GRANT ALL ON SMLOUVA TO xsocha03;

GRANT EXECUTE ON popis_nemovitosti TO xsocha03;
GRANT EXECUTE ON cena_nemovitosti TO xsocha03;

------------------DOTAZY----------------------------------------------
-- dotaz pro vyhledání ID nemovitostí, u kterých již je uzavřená smlouva
SELECT ID_NEMOVITOSTI FROM NEMOVITOST NATURAL JOIN SMLOUVA
WHERE STAV_SMLOUVY = 'UZAVŘENÁ';

-- dotaz pro vyhledání ID makléře, který uzavřel smlouvu 12.8.2022
SELECT ID FROM MAKLER NATURAL JOIN SMLOUVA
WHERE DATUM_UZAVRENI_SMLOUVY = TO_DATE('12.08.2022','dd/mm/yyyy');

-- dotaz pro vyhledání bydliště zákazníků, kteří si zakoupili nemovitost za 2 000 000 a více.
SELECT DISTINCT BYDLISTE FROM ZAKAZNIK NATURAL JOIN SMLOUVA NATURAL JOIN NEMOVITOST
WHERE NEMOVITOST_CENA >= '2000000' AND NEMOVITOST_STAV = 'NA PRODEJ';

-- dotaz pro vyhledání počtu smluv, které uzavřel jednotlivý makléř
SELECT MAKLER.ID, COUNT(SMLOUVA.ID) as pocet_uzavrenych_smluv
FROM MAKLER
JOIN SMLOUVA ON MAKLER.ID = SMLOUVA.ID_MAKLERE
WHERE SMLOUVA.STAV_SMLOUVY = 'UZAVŘENÁ'
GROUP BY MAKLER.ID;

-- dotaz pro vyhledání nejdražších nemovitostí, u jejíchž smluv figurovali jednotliví makléři.
SELECT SMLOUVA.ID_MAKLERE, MAX(NEMOVITOST.NEMOVITOST_CENA) as nejdrazsi_nemovitost
FROM SMLOUVA
JOIN NEMOVITOST ON SMLOUVA.ID_NEMOVITOSTI = NEMOVITOST.ID_NEMOVITOSTI
GROUP BY SMLOUVA.ID_MAKLERE;

-- dotaz pro vyhledání adres nemovitostí, které jsou dražší než 2 000 000,-
SELECT NEMOVITOST_ADRESA FROM NEMOVITOST
WHERE EXISTS (
    SELECT * FROM SMLOUVA
    WHERE SMLOUVA.ID_NEMOVITOSTI = NEMOVITOST.ID_NEMOVITOSTI AND NEMOVITOST.NEMOVITOST_CENA > 2000000
);

-- dotaz pro vyhledání dat o nejlevnější nemovitosti, z nemovitostí nabízených na prodej.
SELECT *
FROM NEMOVITOST
WHERE NEMOVITOST_CENA IN (
    SELECT MIN(NEMOVITOST_CENA)
    FROM NEMOVITOST
    WHERE NEMOVITOST_STAV = 'NA PRODEJ'
);

-------- EXPLAIN PLANS -----------------------------------

EXPLAIN PLAN
    SET STATEMENT_ID = 'stm1'
    FOR SELECT MAKLER.ID, COUNT(SMLOUVA.ID) as pocet_uzavrenych_smluv
    FROM MAKLER
    JOIN SMLOUVA ON MAKLER.ID = SMLOUVA.ID_MAKLERE
    WHERE SMLOUVA.STAV_SMLOUVY = 'UZAVŘENÁ'
    GROUP BY MAKLER.ID;
    SELECT PLAN_TABLE_OUTPUT 
  FROM TABLE(DBMS_XPLAN.DISPLAY);

CREATE INDEX planIndex ON SMLOUVA(ID_MAKLERE);

  EXPLAIN PLAN
    SET STATEMENT_ID = 'stm2'
    FOR SELECT MAKLER.ID, COUNT(SMLOUVA.ID) as pocet_uzavrenych_smluv
    FROM MAKLER
    JOIN SMLOUVA ON MAKLER.ID = SMLOUVA.ID_MAKLERE
    WHERE SMLOUVA.STAV_SMLOUVY = 'UZAVŘENÁ'
    GROUP BY MAKLER.ID;
    SELECT PLAN_TABLE_OUTPUT 
  FROM TABLE(DBMS_XPLAN.DISPLAY);

  ------- MATERIALIZED VIEWS -------------------------------
DROP MATERIALIZED VIEW nemovitost_info;
CREATE MATERIALIZED VIEW nemovitost_info BUILD IMMEDIATE
    REFRESH COMPLETE
    ON DEMAND AS
SELECT * FROM NEMOVITOST
WHERE EXISTS (
    SELECT * FROM SMLOUVA
    WHERE SMLOUVA.ID_NEMOVITOSTI = NEMOVITOST.ID_NEMOVITOSTI AND NEMOVITOST.NEMOVITOST_CENA > 2000000
);

SELECT * FROM nemovitost_info;
INSERT INTO NEMOVITOST(NEMOVITOST_ADRESA, NEMOVITOST_TYP, NEMOVITOST_CENA, NEMOVITOST_STAV)
VALUES ('Praha, Husova 472', 'rodinný dům', '3800000', 'NA PRODEJ');
COMMIT;
SELECT * FROM nemovitost_info;
SELECT * FROM NEMOVITOST;

GRANT ALL ON nemovitost_info TO xsocha03;

-------- WITH/CASE  SELECTS --------------------------------

WITH
pocet_smluv AS (
SELECT ID_NEMOVITOSTI, COUNT(ID_NEMOVITOSTI) AS POCET FROM SMLOUVA GROUP BY ID_NEMOVITOSTI
)

SELECT DISTINCT
NEMOVITOST.NEMOVITOST_ADRESA,
NEMOVITOST.NEMOVITOST_TYP,
CASE WHEN pocet_smluv.POCET IS NULL THEN 'NA PRODEJ' ELSE STAV_SMLOUVY END AS STAV_SMLOUVY,
pocet_smluv.POCET AS POCET_SMLUV
FROM
    NEMOVITOST
    LEFT JOIN pocet_smluv ON NEMOVITOST.ID_NEMOVITOSTI = pocet_smluv.ID_NEMOVITOSTI
    LEFT JOIN SMLOUVA ON pocet_smluv.ID_NEMOVITOSTI = SMLOUVA.ID_NEMOVITOSTI
ORDER BY
    pocet_smluv.POCET DESC;