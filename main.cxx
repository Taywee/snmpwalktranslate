#include <iostream>
#include <vector>
#include <sstream>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>


// Split string into vector of strings based on single-character delimiter
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

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
        oidstring.resize(256, '\0');
        snprint_objid(oidstring.data(), oidstring.size(), oid.id.data(), oid.id.size());
        std::cout << oidstring.data() << " = " << oid.data << '\n';
    }
    std::cout << std::flush;

    return (0);
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::istream &operator>>(std::istream &stream, OID &oid)
{
    std::string dummy;
    stream >> oid.stringOid;
    stream >> dummy;
    stream.get();
    std::getline(stream, oid.data);

    oid.id.clear();
    for (std::string field: split(oid.stringOid, '.'))
    {
        if (field.length())
        {
            ::oid temp;
            std::stringstream(field) >> temp;
            oid.id.push_back(temp);
        }
    }

    return (stream);
}

