#include "sslconnections.h"

SslConnections::SslConnections()
{
}

SslServerConnection *SslConnections::element(const int &index) const
{
    //
    // Return the element from the given index
    //
    return (index < this->connectionList.size() ? this->connectionList.at(index) : 0);
}

int SslConnections::count() const
{
    //
    // Return the list siize
    //
    return this->connectionList.size();
}

void SslConnections::remove(const quint64 &id)
{
    //
    // Remove the item with the given index
    //
    QMutableListIterator<SslServerConnection *> i(this->connectionList);
    while (i.hasNext()) {
        SslServerConnection *connection = i.next();
        // Vaild obejct
        if (connection) {
            // Is id is equal
            if (connection->threadId() == id) {
                // Remove the item
                i.remove();
                break;
            }
        }
    }
}

void SslConnections::add(SslServerConnection *element)
{
    //
    // Add a new element to the list
    //
    this->connectionList.append(element);
}

void SslConnections::clear()
{
    //
    // Removes all elements from the list
    //    
    QMutableListIterator<SslServerConnection *> i(this->connectionList);
    while (i.hasNext()) {
        i.next();        
        i.remove();
    }
}

int SslConnections::indexOf(SslServerConnection *element) const
{
    //
    // Return the index of the given element in the list
    //
    return this->connectionList.indexOf(element);
}
