#include <smbios/abstract_smbios_entry.h>
#include <smbios/smbios.h>

#include <cassert>
#include <string>
#include <sstream>

using namespace smbios;

AbstractSMBiosEntry::AbstractSMBiosEntry(const DMIHeader& header)
    : header_(std::make_unique<smbios::DMIHeader>(header)), dmi_strings_({"Not Specified"})
{
    parse_dmi_strings();
}

void AbstractSMBiosEntry::parse_dmi_strings()
{
    const uint8_t* string_section = header_->data + header_->length;    
    do {
        
        if (nullptr == string_section) {
            // most probably we called parsing from the entry which do not contain strings
            // it's ok, just leave only "Not Specified" entry
            return;
        }
        dmi_strings_.push_back(std::string(reinterpret_cast<const char*>(string_section)));
        string_section += dmi_strings_.back().size() + 1;
        
    } while (string_section[0] != 0 && string_section[1] != 0);
}

std::string AbstractSMBiosEntry::dmi_string(size_t string_index) const
{
    if (string_index >= dmi_strings_.size()) {
        return std::string("Bad index");
    }
    return dmi_strings_[string_index];
}

size_t smbios::AbstractSMBiosEntry::get_entry_size() const
{
    return static_cast<size_t>(header_->length);
}

std::string smbios::AbstractSMBiosEntry::address_string(uint16_t string_address) const
{
    std::stringstream address_stream;
    address_stream << std::hex << std::showbase
        << string_address
        << std::dec << std::noshowbase;
    return std::move(address_stream.str());
}

