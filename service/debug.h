#ifndef DEBUG_H
#define DEBUG_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>

namespace DebugLevel {
    enum Level{
        None,
        Error,
        Category,
        Function,
        Comment,        
        Database,
        Debug,
        Construktor,
        Destruktor,
        All
    };
}

class Debug
{
public:
    //
    // Debug-Modus steuern
    static void enableDebugging (const bool &flag);
    // Den Pfad für das Debuggen setzen
    static void setDebugPath (const QString &path);
    // Neue Kategorie hinzufügen
    static void addCategory (const QString &cat);
    // Welches Loglevel
    static void setDebugLevel (DebugLevel::Level level);
    // Zeit mit ausgeben
    static void enableDateTime (const bool &flag);
    // Uhrezit auch in Millisekunden angeben
    static void enableMSecs (const bool &flag);
    // Eine Kategorie vom Stack holen
    static void endCategory ();
    // Den Stack leeren
    static void clearCategories ();

    // Die eigentliche Logmeldung
    static void log (const QString &message, DebugLevel::Level level=DebugLevel::Comment, const QString &filename=QString(""));
    // Wrapper für Fehlermeldungen
    static void error (const QString &message, const QString &filename=QString(""));
    // Wrapper für Funktionen
    static void function (const QString &message, const QString &filename=QString(""));    

private:
    Debug ();
    //
    // Ist das Debuging aktiviert
    static bool debugEnabled;    
    // Der Path zum Verzeichnis in dem die Debug-Dateien erstellt werden sollen
    static QString debugPath;
    // Das aktuelle Loglevel
    static int debugLevel;    
    // Soll die Zeit angegeben werden
    static bool dateTimeEnabled;
    // Millisekunden
    static bool mSecsEnabled;

    // Kategorien Stapel
    static QStack<QString> debugCategories;
};

#endif // DEBUG_H
