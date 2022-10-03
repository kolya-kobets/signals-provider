#ifndef IDATASOURCE_H
#define IDATASOURCE_H


class IDataSource
{
public:
    virtual int read(char* dest, int size) = 0;
    virtual ~IDataSource() = default;
};

#endif // IDATASOURCE_H
