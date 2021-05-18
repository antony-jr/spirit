#ifndef BASHRC_WRITER_HPP_INCLUDED
#define BASHRC_WRITER_HPP_INCLUDED
#include <QString>

class BashRCWriter {
public:
	BashRCWriter();
	~BashRCWriter();
	
	void setProgram(const QString&);
	bool write();
	bool unwrite();
private:
	QString m_Program;
};

#endif // BASHRC_WRITER_HPP_INCLUDED
