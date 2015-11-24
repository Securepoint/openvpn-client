#ifndef TRANSACTION_H
#define TRANSACTION_H

class Transaction
{
public:
    virtual ~Transaction() {}

    virtual void apply() = 0;
};

#endif // TRANSACTION_H
