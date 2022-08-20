#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QSharedPointer>
#include <QTextCodec>
#include <QSet>
#include <QDir>
#include <QDirIterator>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QStringList args;
	for(int i = 0; i < argc; ++i)
	{
		args.push_back(QString::fromUtf8(argv[i]));
	}

	QTextStream qStdout(stdout);
	if(argc < 2 || args[1] == "--help")
	{
		qStdout << "Usage:\n" << args[0] << " <input directory path> <output directory path>\nOutput directory path is optional.\n";
		return 0;
	}
	QString inputDirPath = args[1];
	QString outputDirPath;
	if(argc >= 3) {
		outputDirPath = args[2];
	} else outputDirPath = a.applicationDirPath();
	QTextCodec *codec1 = QTextCodec::codecForName("ISO 8859-15");
	QDir outDir(outputDirPath);
	QDirIterator inDir(inputDirPath);
	QString buffer;
	while(inDir.hasNext()) {
		QFileInfo inInfo(inDir.next());
		QFile inFile(inInfo.absoluteFilePath());
		QFile outFile(outDir.absoluteFilePath(QStringLiteral("%1.yml").arg(inInfo.baseName())));
		if(!(inFile.open(QFile::ReadOnly | QFile::Text) && outFile.open(QFile::WriteOnly))) continue;
		QTextStream inStream(&inFile);
		QTextStream outStream(&outFile);
		if(codec1) inStream.setCodec(codec1);
		outStream.setGenerateByteOrderMark(true);
		outStream << "l_english:\n";
		while(inStream.readLineInto(&buffer)) {
			QStringList splitString = buffer.split(';');
			if(splitString.size() >= 2) {
				splitString[1] = splitString[1].replace('\"',QStringLiteral("\\\""));
				outStream << ' ' << splitString[0] << ":0 \"" << splitString[1] << "\"\n";
			}
		}
		inFile.close();
		outStream.flush();
		outFile.flush();
		outFile.close();
	}


	return 0;
}
