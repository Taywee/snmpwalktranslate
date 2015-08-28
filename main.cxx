#include <iostream>
#include <vector>
#include <list>
#include <sstream>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

// split a string into an output iterator of some sort.
template <typename OutputIterator>
void split(const std::string &source, const std::string &delimiter, OutputIterator result)
{
    size_t prevposition = 0;
    size_t position = 0;
    while ((position < source.size()) && ((position = source.find(delimiter, position)) != source.npos))
    {
        *result = source.substr(prevposition, position - prevposition);
        result++;
        position += delimiter.size();
        prevposition = position;
    }
    *result = source.substr(prevposition);
    result++;
}

class OID
{
    public:
        std::string stringOid;
        std::vector<oid> id;
        std::string data;

        friend std::istream &operator>>(std::istream &stream, OID &oid);
};

int main()
{
    init_snmp("snmpapp");
    while (true)
    {
        OID oid;
        std::cin >> oid;
        if (std::cin.eof())
        {
            break;
        }
        std::vector<char> oidstring;
        oidstring.resize(2024, '\0');
        snprint_objid(oidstring.data(), oidstring.size(), oid.id.data(), oid.id.size());
        std::cout << oidstring.data() << " = " << oid.data << '\n';
    }

    return (0);
}


std::istream &operator>>(std::istream &stream, OID &oid)
{
    std::string dummy;
    stream >> oid.stringOid;
    stream >> dummy;
    stream.get();
    std::getline(stream, oid.data);

    oid.id.clear();
    std::list<std::string> splitOid;
    split(oid.stringOid, ".", std::back_inserter(splitOid));

    // Remove the first blank field, caused by the leading dot
    splitOid.pop_front();

    for (const std::string &field: splitOid)
    {
        ::oid temp;
        std::stringstream(field) >> temp;
        oid.id.push_back(temp);
    }

    return (stream);
}

