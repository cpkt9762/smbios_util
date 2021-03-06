#pragma once
#include <cstdint>
#include <map>
#include <smbios/abstract_smbios_entry.h>

// Memory device entry
// See http://www.dmtf.org/standards/smbios
// Standard according to current SMBIOS version
// 'Memory Device' chapter
// Entry format changes version to version

namespace smbios {

struct DMIHeader;
struct SMBiosVersion;

// should be aligned to be mapped to physical memory
#pragma pack(push, 1)

/// @brief SMBIOS MemoryDevice entry Ver 2.1+
struct MemoryDeviceV21 {
    uint32_t header;
    uint16_t array_handle;
    uint16_t array_error_handle;
    uint16_t total_width;
    uint16_t data_width;
    uint16_t device_size;
    uint8_t device_form_factor;
    uint8_t device_set;
    uint8_t device_locator;
    uint8_t bank_locator;
    uint8_t device_type;
    uint16_t type_detail;
};

/// @brief SMBIOS MemoryDevice entry Ver 2.3+
struct MemoryDeviceV23 : public MemoryDeviceV21 {
    uint16_t device_speed;
    uint8_t manufacturer;
    uint8_t serial_number;
    uint8_t asset_tag;
    uint8_t part_number;
};

/// @brief SMBIOS MemoryDevice entry Ver 2.6+
struct MemoryDeviceV26 : public MemoryDeviceV23 {
    uint8_t device_rank;
};

/// @brief SMBIOS MemoryDevice entry Ver 2.7+
struct MemoryDeviceV27 : public MemoryDeviceV26 {
    
    uint32_t extended_size;
    uint16_t memory_clock_speed;
};

/// @brief SMBIOS MemoryDevice entry Ver 2.8+
struct MemoryDeviceV28 : public MemoryDeviceV27 {
    uint16_t minimum_voltage;
    uint16_t maximum_voltage;
    uint16_t configured_voltage;
};

#pragma pack(pop)

/// @brief Class-wrapper under raw memory structures
class MemoryDeviceEntry : public AbstractSMBiosEntry {
public:

    // @brief special values for ErrorHandle: uint16 - offset 0x06
    enum ErrorHandleValue : uint16_t {
        ErrorHandleNotProvided = 0xFFFE,
        ErrorHandleNoError = 0xFFFF
    };

    // @brief special values for TotalWidth & DataWidth uint16 - offset 0x08, 0x0A
    enum DataWidthValue : uint16_t {
        DataWidthUnknown1 = 0x0,
        DataWidthUnknown2 = 0xFFFF
    };

    // @brief special values for DeviceSize: uint16 - offset 0x0C
    enum DeviceSizeValue : uint16_t
    {
        DeviceSizeNoModuleInstalled = 0x0,
        DeviceSizeUnknown = 0xFFFF
    };

    // @brief special values for FormFactor: uint8 - offset 0x0E
    enum FormFactorValue : uint8_t {
        FormFactorOutOfSpec = 0x00,
        FormFactorOther = 0x01,
        FormFactorUnknown = 0x02,
        SIMM = 0x03, 
        SIP = 0x04, 
        Chip = 0x05, 
        DIP = 0x06, 
        ZIP = 0x07, 
        ProprietaryCard = 0x08, 
        DIMM = 0x09, 
        TSOP = 0x0A, 
        Rowofchips = 0x0B, 
        RIMM = 0x0C, 
        SODIMM = 0x0D, 
        SRIMM = 0x0E, 
        FBDIMM = 0x0F,
    };

    // @brief special values for DeviceSet: uint8 - offset 0x0F
    enum DeviceSetValue : uint8_t
    {
        DeviceSetNone = 0x0,
        DeviceSetUnknown = 0xFF
    };

    // @brief special values for DeviceType: uint8 - offset 0x12
    enum DeviceTypeValue : uint8_t
    {
        DeviceTypeOutOfSpec = 0x00,
        DeviceTypeOther = 0x01,
        DeviceTypeUnknown = 0x02,
        DRAM = 0x03,
        EDRAM = 0x04,
        VRAM = 0x05,
        SRAM = 0x06,
        RAM = 0x07,
        ROM = 0x08,
        FLAS = 0x09,
        EEPROM = 0x0A,
        FEPROM = 0x0B,
        EPROM = 0x0C,
        CDRAM = 0x0D,
        D3DRAM = 0x0E,
        SDRAM = 0x0F,
        SGRAM = 0x10,
        RDRAM = 0x11,
        DDR = 0x12,
        DDR2 = 0x13,
        DDR2FB = 0x14,
        Reserved1 = 0x15,
        Reserved2 = 0x16,
        Reserved3 = 0x17,
        DDR3 = 0x18,
        FBD2 = 0x19,
        DDR4 = 0x1A,
        LPDDR = 0x1B,
        LPDDR2 = 0x1C,
        LPDDR3 = 0x1D,
        LPDDR4 = 0x1E,
    };

    // @brief Bit-mask values for DeviceProperties: uint16 - offset 0x13
    enum DeviceProperties : uint16_t
    {
        DevicePropertiesOutOfSpec = 0x0,
        DevicePropertiesReserved = 0x1u << 0,
        DevicePropertiesOther = 0x1u << 1,
        DevicePropertiesUnknown = 0x1u << 2,
        FastPaged = 0x1u << 3,
        StaticColumn = 0x1u << 4,
        PseudoStatic = 0x1u << 5,
        RAMBUS = 0x1u << 6,
        Synchronous = 0x1u << 7,
        CMOS = 0x1u << 8,
        EDO = 0x1u << 9,
        WindowDRAM = 0x1u << 10,
        CacheDRAM = 0x1u << 11,
        NonVolatile = 0x1u << 12,
        Registered = 0x1u << 13,
        Unregistered = 0x1u << 14,
        LRDIMM = 0x1u << 15
    };

    enum DeviceSpeed : uint16_t {
        DeviceSpeedUnknown = 0x0,
        DeviceSpeedReserved = 0xFFFF
    };

    /// @brief Parse the header, recognize SMBIOS version and how much information 
    /// do we have in MemoryDevice SMBIOS entry
    MemoryDeviceEntry(const DMIHeader& header, const SMBiosVersion& version);

    // @brief Parent is abstract
    virtual ~MemoryDeviceEntry() = default;

    /// @brief String representation
    virtual std::string get_type() const override;

    /// @brief Render all entry information into single string
    virtual std::string render_to_description() const override;

    //////////////////////////////////////////////////////////////////////////
    // Bitwise values

    /// @brief 0x05 offset
    /// Handle, or instance number, associated with the structure
    uint16_t get_array_handle() const;

    /// @brief 0x06 offset
    /// Handle, or instance number, associated with any
    /// error that was previously detected for the device
    /// See ErrorHandleValue enum for special values
    uint16_t get_error_handle() const;

    /// @brief 0x08 offset
    /// Total width, in bits, of this memory device, including
    /// any check or error-correction bits
    /// See DataWidthValue enum for special values
    uint16_t get_total_width() const;

    /// @brief 0x0A offset
    /// Data width, in bits, of this memory device
    /// See DataWidthValue enum for special values
    uint16_t get_data_width() const;

    /// @brief 0x0C offset
    /// Size of the memory device. If the size is 32 GB-1 MB or greater, the
    /// field value is 7FFFh and the actual size is stored in
    /// the Extended Size field.
    /// See DeviceSizeValue enum for special values
    uint16_t get_device_size() const;

    /// @brief 0x0E offset
    /// Implementation form factor for this memory device
    /// See FormFactorValue enum for special values
    uint8_t get_form_factor() const;

    /// @brief 0x0F offset
    /// Identifies when the Memory Device is one of a set
    /// of Memory Devices that must be populated
    /// See DeviceSetValue enum for special values
    uint8_t get_device_set() const;

    /// @brief 0x10 offset
    /// Index of Device Locator string
    /// String number of the string that identifies the
    /// physically-labeled socket or board position where
    /// the memory device is located. EXAMPLE : 'DIMM 3'
    uint8_t get_device_locator_index() const;

    /// @brief 0x11 offset
    /// Index of Bank Locator string
    /// String number of the string that identifies the
    /// physically labeled bank where the memory device is located
    /// EXAMPLE: 'Bank 0' or 'A'
    uint8_t get_bank_locator_index() const;

    /// @brief 0x12 offset
    /// See DeviceTypeValue enum for special values
    uint8_t get_device_type() const;

    /// @brief 0x13 offset
    /// See DeviceProperties enum for special values
    uint16_t get_device_detail() const;

    /// @brief 0x15 offset
    /// Identifies the maximum capable speed of the device
    uint16_t get_device_speed() const;

    /// @brief 0x17 offset
    /// String number for the manufacturer of this memory device
    uint8_t get_manufacturer_index() const;

    /// @brief 0x18 offset
    /// String number for the serial number of this memory device
    uint8_t get_serial_number_index() const;

    /// @brief 0x19 offset
    /// String number for the asset tag of this memory device
    uint8_t get_asset_tag_index() const;

    /// @brief 0x1A offset
    /// String number for the part number of this memory device
    uint8_t get_part_number_index() const;

    /// @brief 0x1A offset
    /// Bits 7-4: reserved; Bits 3 - 0: rank; Value = 0 for unknown rank
    uint8_t get_device_rank() const;

    //////////////////////////////////////////////////////////////////////////
    // String values

    /// Handle string representation
    std::string get_array_handle_string() const;

    /// ErrorHandleValue string representation
    std::string get_error_handle_string() const;

    /// DataWidthValue string representation
    std::string get_total_width_string() const;

    /// DataWidthValue string representation
    std::string get_data_width_string() const;

    /// DeviceSizeValue string representation
    std::string get_device_size_string() const;

    /// FormFactorValue string representation
    std::string get_form_factor_string() const;

    /// DeviceSetValue string representation
    std::string get_device_set_string() const;

    /// String number of the string that identifies the
    /// physically-labeled socket or board position where
    /// the memory device is located. EXAMPLE : 'DIMM 3'
    std::string get_device_locator_string() const;

    /// String number of the string that identifies the
    /// physically labeled bank where the memory device is located
    /// EXAMPLE: 'Bank 0' or 'A'
    std::string get_bank_locator_string() const;

    /// DeviceType string representation
    std::string get_device_type_string() const;

    /// DeviceProperties string representation, separated by 
    std::string get_device_detail_string() const;

    /// Maximum capable speed string representation
    std::string get_device_speed_string() const;

    /// String number for the manufacturer of this memory device
    std::string get_manufacturer_string() const;

    /// String number for the serial number of this memory device
    std::string get_serial_number_string() const;

    /// @brief String number for the asset tag of this memory device
    std::string get_asset_tag_string() const;

    /// @brief String number for the part number of this memory device
    std::string get_part_number_string() const;

    /// @brief Device rank 0x1-0xFFFF
    std::string get_device_rank_string() const;

private:

    /// Map flags data to string values
    void init_string_values();

private:

    /// Init pointers depend on SMBIOS version
    const MemoryDeviceV21* memory_device_v21_ = nullptr;
    const MemoryDeviceV23* memory_device_v23_ = nullptr;
    const MemoryDeviceV26* memory_device_v26_ = nullptr;
    const MemoryDeviceV27* memory_device_v27_ = nullptr;
    const MemoryDeviceV28* memory_device_v28_ = nullptr;

    /// Bitwise to string representation
    std::map<uint16_t, std::string> error_handle_map_;
    std::map<uint16_t, std::string> data_width_map_;
    std::map<uint16_t, std::string> device_size_map_;
    std::map<uint8_t, std::string> form_factor_map_;
    std::map<uint8_t, std::string> device_set_map_;
    std::map<uint8_t, std::string> device_type_map_;
    std::map<uint16_t, std::string> device_properties_map_;
    std::map<uint16_t, std::string> device_speed_map_;
};

} // namespace smbios
