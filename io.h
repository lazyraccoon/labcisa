#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

std::string readString(std::string text);
bool readBoolean(std::string text);
bool readBooleanFile(std::ifstream &file);
int readNumber(std::string text);

#endif // IO_H_INCLUDED
