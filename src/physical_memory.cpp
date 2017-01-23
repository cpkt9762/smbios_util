#include <boost/iostreams/device/mapped_file.hpp>

#include <smbios_utility/physical_memory.h>
#if defined(_WIN32) || defined(_WIN64)
#include <smbios_utility/win_physical_memory.h>
#elif defined(__linux__) || defined (__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun)
#include <smbios_utility/posix_physical_memory.h>
#endif


namespace boost_io = boost::iostreams;

PhysicalMemory::PhysicalMemory() : native_physical_memory_(std::make_unique<NativePhysicalMemory>())
{

}

PhysicalMemory::PhysicalMemory(size_t base, size_t length)
    : native_physical_memory_(std::make_unique<NativePhysicalMemory>(base, length))
{

}

PhysicalMemory::~PhysicalMemory()
{

}

void PhysicalMemory::map_physical_memory(size_t base, size_t length)
{
    native_physical_memory_->map_physical_memory(base, length);
}

bool PhysicalMemory::is_mapped() const
{
    return native_physical_memory_->is_mapped();
}

std::vector<uint8_t> PhysicalMemory::get_memory_dump(size_t offset, size_t length) const
{
    return native_physical_memory_->get_memory_dump(offset, length);
}

const uint8_t* PhysicalMemory::get_memory_offset(size_t offset) const
{
    return native_physical_memory_->get_memory_offset(offset);
}
