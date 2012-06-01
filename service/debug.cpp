#include "debug.h"

/*
 * Log-Klasse zum Debuggen
 *
 * Autor: Oliver Dehnbostel <oliver.dehnbostel @ securepoint.de>
 * Erstellt am: 05.12.2011
 *
 *
 *
 * Benutzung:
 *
 * Init:
 *
 *      // Pfad für die Logdatei
 *      Debug::setDebugPath(QCoreApplication::applicationDirPath());
 *      // Das DebugLevel setzten -> siehe Enum Level im namespace
 *      Debug::setDebugLevel(DebugLevel::Category);
 *      // Millisekunden anzeigen aktivieren, diese Option
 *      // aktiviert auch gleich enableDateTime (true);
 *      Debug::enableMSecs(true);
 *      // Das Log aktivieren
 *      Debug::enableDebugging(true);
 *
 *
 * Benutzung:
 *
 *      Hinzufügen von Kategorien:
 *          Debug::addCategory(QLatin1String("Name"));
 *      Entfernen einer Kategorie:
 *          Debug::endCategory();
 *      Entfernen aller Kategorien:
 *          Debug::clearCategories();
 *      Ausgeben einer Meldung:
 *          Debug::log(QString::number(3));
 *      oder mit Levelangabe
 *          Debug::log(QLatin1String("Message"), DebugLevel::Construktor);
 *      oder mit Levelangabe und Datei sonst ist Standard debug.txt
 *          Debug::log(QLatin1String("Message"), DebugLevel::Construktor, QLatin1String("datei.suffix");
 *
 *      Kurzversionen:
 *          Debug::error(QLatin1String("Fehler"));
 *      und
 *          Debug::function(QLatin1String("Funktion"));
 *
 *
 *
 * Ausgabe:
 *      05.12.2011 15:08:22.414 Category -> =====================================================================
 *      05.12.2011 15:08:22.414 Category -> Category: Name now started.
 *      05.12.2011 15:08:22.414 Category -> =====================================================================
 *      05.12.2011 15:08:22.414 Comment -> Ein Kommentar
 *          05.12.2011 15:08:22.414 Category -> =====================================================================
 *          05.12.2011 15:08:22.414 Category -> Category: Name 2 now started.
 *          05.12.2011 15:08:22.414 Category -> =====================================================================
 *          05.12.2011 15:08:22.414 Comment -> In der neuen Kategorie
 *          05.12.2011 15:08:22.414 Category -> =====================================================================
 *          05.12.2011 15:08:22.430 Category -> Category: Name 2 now ended.
 *          05.12.2011 15:08:22.430 Category -> =====================================================================
 *      05.12.2011 15:08:22.430 Error -> ---------------------------------------------------------------------
 *      05.12.2011 15:08:22.430 Error -> Fehlermedlung
 *      05.12.2011 15:08:22.430 Error -> ---------------------------------------------------------------------
 *      05.12.2011 15:08:22.430 Function -> Funktion
 *      05.12.2011 15:08:22.430 Category -> =====================================================================
 *      05.12.2011 15:08:22.430 Category -> Category: Push Name now ended.
 *      05.12.2011 15:08:22.430 Category -> =====================================================================
 *
 *
 * Known Bugs: Keine
 *
 * Änderungen: Keine
 *
 *
 */

// Init der statischen Member
//
// Debugging ist deaktiviert
bool Debug::debugEnabled = false;

// Einen leeren Pfad erzeigen
QString Debug::debugPath = QString ("");
// DebugLevel is None
int Debug::debugLevel = 0;

// Standardmässig wird keine Zeit erfasst
bool Debug::dateTimeEnabled = false;
bool Debug::mSecsEnabled = false;

// Eine leere List erzeugen
QStack<QString> Debug::debugCategories;


Debug::Debug()
{
}

void Debug::enableDebugging(const bool &flag)
{
    // Schaltet das Debugging an oder aus
    debugEnabled = flag;
}

void Debug::setDebugPath(const QString &path)
{
    // Setzt den Pfad für das Debugging alle
    // Dateien werden in diesem Verzeichnis abgelegt.
    // Der Pfad muss für eine ordnungsgemäße Funktion gesetzt sein.

    // Wenn der Pfad leer ist abbrechen
    if (path.isEmpty()) {
        return;
    }

    // Den Pfad aufbereiten
    QString tmp(path);
    // Backslash durch Slash ersetzen
    tmp = tmp.replace("\\", "/");
    // Ist das letzt Zeichen ein "/"
    // Wenn nicht anfügen
    if (tmp.right(1) != QLatin1String("/")) {
        tmp += QLatin1String("/");
    }
    // Alles ok
    debugPath = tmp;
}

void Debug::addCategory(const QString &cat)
{
    // Fügt dem Stack eine Kategorie hinzu

    // Wenn keine Kategorie übergeben wurde abbrechen
    if (cat.isEmpty()) {
        return;
    }

    // Kategorie auf den Stack legen
    debugCategories.append(cat);
    log (QLatin1String("====================================================================="), DebugLevel::Category);
    log (QLatin1String("Category: ") + debugCategories.top() + QLatin1String(" now started."), DebugLevel::Category);
    log (QLatin1String("====================================================================="), DebugLevel::Category);
}

void Debug::setDebugLevel(DebugLevel::Level level) {
    // Setzt das aktuelle DebugLevel
    debugLevel = level;
}

void Debug::enableDateTime(const bool &flag)
{
    // Wenn die Option aktiviert ist,
    // wird zu jeder Ausgabe das Datum und die Uhrzeit geschrieben
    dateTimeEnabled = flag;
}

void Debug::enableMSecs(const bool &flag)
{
    // Bei gesezter Option werden zusätzlich noch die Millisekunden erfasst
    mSecsEnabled = flag;
    // Wenn die Option aktiviert ist, muss auch die DateTime an sein.
    if (flag) {
        dateTimeEnabled = true;
    }
}


void Debug::endCategory()
{
    // Beendet eine Kategorie und nimmt diese vom Stack
    if (debugCategories.size() > 0) {
        // Ausgabe des Kategorienwechsel
        log (QLatin1String("====================================================================="), DebugLevel::Category);
        log (QLatin1String("Category: ") + debugCategories.top() + QLatin1String(" now ended."), DebugLevel::Category);
        log (QLatin1String("====================================================================="), DebugLevel::Category);
        debugCategories.pop();
    }
}

void Debug::clearCategories()
{
    // Löscht die gesamte Kategorienliste
    log (QLatin1String("====================================================================="), DebugLevel::Category);
    log (QLatin1String("All categories now ended."), DebugLevel::Category);
    log (QLatin1String("====================================================================="), DebugLevel::Category);
    debugCategories.clear();
}

void Debug::log(const QString &message, DebugLevel::Level level, const QString &filename)
{
    // Schreibt die Logmeldung, wenn ein bestimmtes Level gesetzt ist
    if (debugEnabled) {
        // Level überprüfen
        if (level <= debugLevel) {
            // Soll das Datum mit erfasst werden
            QString timeDate ("");
            if (dateTimeEnabled) {
                QString dateFormat (QLatin1String("dd.MM.yyyy hh:mm:ss"));
                // Millisekunden ausgeben
                if (mSecsEnabled) {
                    dateFormat = QLatin1String("dd.MM.yyyy hh:mm:ss.zzz");
                }
                timeDate = QDateTime::currentDateTime().toString(dateFormat);
            }

            // Ist der Pfad gefüllt
            if (debugPath.isEmpty()) {
                return;
            }
            // Ist der Pfad vorhanden
            QDir debugDir (debugPath);
            if(!debugDir.exists()) {
                // Pfad erstellen
                if (!debugDir.mkpath(debugPath)) {
                    // Fehler
                    return;
                }
            }

            // Soll eine extra Datei erstellt werden?
            QString fileName ("debug.txt");
            if (!filename.isEmpty()) {
                fileName = filename;
            }

            // Nun die Einrückrung
            QString indent;
            // Die Stackgröße spiegelt die Einrücktiefe wieder
            indent = indent.fill(QChar('\t'), debugCategories.size());

            // Das Level herausfinden
            QString levelName ("");
            switch (level) {
                case DebugLevel::Error:
                    levelName = QLatin1String("Error");
                    if (debugLevel == DebugLevel::Error) {
                        // Die Einrücktiefe ist 0
                        indent.clear();
                    }
                    break;
                case DebugLevel::Category:
                    levelName = QLatin1String("Category");
                    break;
                case DebugLevel::Function:
                    levelName = QLatin1String("Function");
                    break;
                case DebugLevel::Comment:
                    levelName = QLatin1String("Comment");
                    break;                
                case DebugLevel::Database:
                    levelName = QLatin1String("Database");
                    break;
                case DebugLevel::Debug:
                    levelName = QLatin1String("Debug");
                    break;
                case DebugLevel::Construktor:
                    levelName = QLatin1String("Construktor");
                    break;
                case DebugLevel::Destruktor:
                    levelName = QLatin1String("Destruktor");
                    break;
                default:
                    // Diesen Fall sollte es nicht geben
                    // Da es nur ein Flag für die Ausgabesteuerung ist
                    levelName = QLatin1String("ALL");
            }

            // LineFeed bestimmen
            QLatin1String lineFeed ("\r\n");
#ifndef Q_OS_WIN32
            lineFeed = QLatin1String ("\n");
#endif

            // Alles bearbeitet, nun den String ausgeben
            // Datei öffnen
            QFile debugFile (debugPath + fileName);
            if (debugFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
                // Datei ist offen nun schreiben
                QString data (indent + timeDate + QLatin1String (" ") + levelName  + QLatin1String (" -> ") + message + lineFeed);
                debugFile.write(data.toAscii());
                // Kurz warten
                debugFile.waitForBytesWritten(1000);
                // Datei wieder schließen
                debugFile.close();
            }
        }
    }
}

void Debug::error(const QString &message, const QString &filename)
{
    // Wrapper für Fehlermeldungen
    log (QLatin1String("---------------------------------------------------------------------"), DebugLevel::Error, filename);
    log (message, DebugLevel::Error, filename);
    log (QLatin1String("---------------------------------------------------------------------"), DebugLevel::Error, filename);
}

void Debug::function(const QString &message, const QString &filename)
{
    // Wrapper für Funktionen
    log (message, DebugLevel::Function, filename);
}
