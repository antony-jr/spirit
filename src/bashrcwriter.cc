#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include "bashrcwriter.hpp"

static bool writeContents(const QString &bashrc, const QStringList &contents) {
	QFile file(bashrc);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
		return false; 
	}
	file.seek(0);
	QTextStream out(&file);

	for(auto line : contents) {
		out << line;
	}
	file.close();
	return true;
}

BashRCWriter::BashRCWriter() { }
BashRCWriter::~BashRCWriter() { }

void BashRCWriter::setProgram(const QString &value) {
	m_Program = value;
}

bool BashRCWriter::write() {
	if(m_Program.isEmpty()) {
		return false;
	}
	QString comment = QString::fromUtf8("### spirit trap commands\n");
	QString commentWithNewLine = QString::fromUtf8("\n### spirit trap commands\n");
	QString trap_line = QString::fromUtf8("trap \"%1 %2\" %3\n");
	const QString bashrc = QDir::homePath() + QDir::separator() + ".bashrc";
	
	QFile file(bashrc);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	file.seek(0);

	QStringList contents;
	bool appended = false;
	while(!file.atEnd()) {
		auto line = file.readLine();
		if(line.contains("### spirit trap commands")) {
			auto prev = contents.at(contents.size() - 1);
			if(prev == "\n") {
			   contents << comment;
			}else {
			   contents << commentWithNewLine;
			}
			contents << trap_line.arg(m_Program).arg("error").arg("ERR");
			contents << trap_line.arg(m_Program).arg("nonerror").arg("DEBUG");

			/// Go past two lines
			line = file.readLine();
			line = file.readLine();

			appended = true;
		}else {
			contents << line;
		}
	}

	if(!appended) {
		auto prev = contents.at(contents.size() - 1);
		if(prev == "\n") {
			contents << comment;
		}else {
			contents << commentWithNewLine;
		}
		contents << trap_line.arg(m_Program).arg("error").arg("ERR");
		contents << trap_line.arg(m_Program).arg("nonerror").arg("DEBUG");
	}
	file.close();

	return writeContents(bashrc, contents);
}

bool BashRCWriter::unwrite() {
	if(m_Program.isEmpty()) {
		return false;
	}
	
	const QString bashrc = QDir::homePath() + QDir::separator() + ".bashrc";
	QFile file(bashrc);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	file.seek(0);

	QStringList contents;
	while(!file.atEnd()) {
		auto line = file.readLine();
		if(line.contains("### spirit trap commands")) {
			/// Go past two lines
			line = file.readLine();
			line = file.readLine();
		}else {
			contents << line;
		}
	}
	file.close();
	
	return writeContents(bashrc, contents);
}
