#ifndef ASSEMBLER_CODE_H_
#define ASSEMBLER_CODE_H_

#include <string>
#include <string_view>

namespace hack {

// Converts dest mnemonic into binary string.
std::string DestToBinary(std::string_view dest);

// Converts comp mnemonic into binary string.
std::string CompToBinary(std::string_view comp);

// Converts jump mnemonic into binary string.
std::string JumpToBinary(std::string_view jump);

}  // namespace hack

#endif
