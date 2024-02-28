
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

void processFile(const QString& fileName, const QSet<QString>& flags)
{
    // Define your regular expressions just once, statically if they are unchanged
    static QRegularExpression capitalWordRegex("\\b[A-Z][A-Za-z]{3,}\\b");
    static QRegularExpression hyphenatedWordRegex("\\b\\w+\\-\\w+\\b");
    static QRegularExpression sameCharWordRegex("\\b(\\w)\\w*\\1\\b");
    static QRegularExpression nonVowelWordRegex("\\b[^AEIOUaeiou]\\w*\\b");

    QString resourcePath = ":/resources/" + fileName;


    QFile file(resourcePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file: " << fileName;
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    //Perform counts based on flags
    if(flags.contains("-a")|| flags.isEmpty())
    {
        int count = content.count(capitalWordRegex);
        qDebug() << "Words longer than 4 characters with a capital letter." << count;
    }
    if(flags.contains("-b")|| flags.isEmpty())
    {
        int count = content.count(hyphenatedWordRegex);
        qDebug() << "Hyphenated words: " << count;
    }
    if(flags.contains("-c")|| flags.isEmpty())
    {
        int count = content.count(sameCharWordRegex);
        qDebug() << "Words that start and end on the same character: " << count;
    }
    if(flags.contains("-d")|| flags.isEmpty())
    {
        int count = content.count(nonVowelWordRegex);
        qDebug() << "Words that do not start with a vowel: " << count;
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream inputStream(stdin);
    QTextStream outputStream(stdout);
    QString input;

    while(true)
    {
        outputStream << "Enter the command(For help 'count -help' to quit 'exit'): ";
        outputStream.flush();
        input = inputStream.readLine().trimmed();

        if(input.toLower()=="exit")
        {
            break;
        }

        QStringList arguments = input.split(" ", Qt::SkipEmptyParts);

        if(arguments.isEmpty())
        {
            qDebug() << "Please provide the command.";
        }

        QString command = arguments.first();
        if(command != "count")
        {
            qDebug() << "Invalid command. Please start with 'count'.";
        }

        arguments.removeFirst();

        QSet<QString> flags;
        QStringList fileNames;

        for(const QString& arg : arguments)
        {
            if(arg.startsWith("-"))
            {
                flags.insert(arg);
            }
            else
            {
                fileNames.append(arg);
            }
        }

        if(flags.contains("-help"))
        {
            qDebug() << "Available commands:";
            qDebug() << "example: count -a TextFileOne.txt";
            qDebug() << "Text Files Available: TextFileOne.txt and TextFileTwo.txt";
            qDebug() << "count -a - Count words longer than 4 characters that start with a capital letter.";
            qDebug() << "count -b - Count hyphenated words.";
            qDebug() << "count -c - Count words that start and end on the same character.";
            qDebug() << "count -d - Count words that do not start with a vowel.";
            qDebug() << "count -help - Display this help information.";
        }

        foreach (const QString& fileName, fileNames)
        {
            qDebug() << "Occurrences in the file" << fileName << ":";
            processFile(fileName, flags);
        }
    }




    return 0;
}
