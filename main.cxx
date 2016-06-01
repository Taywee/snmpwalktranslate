#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <sstream>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

struct OID
{
    std::string stringOid;
    std::vector<oid> id;
    std::string data;
};

static std::list<std::string> split(const std::string &source, const std::string &delimiter);
static std::istream &operator>>(std::istream &stream, OID &oid);

int main()
{
    init_snmp("snmpwalktranslate");
    OID oid;
    std::array<char, 4096> buffer;
    while (std::cin >> oid)
    {
        const std::size_t length = snprint_objid(buffer.data(), buffer.size(), oid.id.data(), oid.id.size());
        const std::string oidstring(buffer.data(), length);
        std::cout << oidstring << " = " << oid.data << '\n';
    }

    return (0);
}

std::list<std::string> split(const std::string &source, const std::string &delimiter)
{
    std::list<std::string> output;
    size_t prevposition = 0;
    size_t position = 0;
    while ((position < source.size()) && ((position = source.find(delimiter, position)) != source.npos))
    {
        output.emplace_back(source, prevposition, position - prevposition);
        position += delimiter.size();
        prevposition = position;
    }
    output.emplace_back(source, prevposition, position - prevposition);

    return output;
}

std::istream &operator>>(std::istream &stream, OID &oid)
{
    std::string dummy;
    stream >> oid.stringOid;
    stream >> dummy;
    stream.get();
    std::getline(stream, oid.data);

    oid.id.clear();
    std::list<std::string> splitOid(split(oid.stringOid, "."));

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

