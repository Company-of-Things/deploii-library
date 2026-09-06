#ifndef DEPLOII_UTILS_h
#define DEPLOII_UTILS_h

/*
 * Fowler–Noll–Vo hash function
 */
uint16_t DEPLOII_fnv1a16(const char *str)
{
  const uint16_t FNV_OFFSET_BASIS = 0x811C;
  const uint16_t FNV_PRIME = 0x0101;

  uint16_t hash = FNV_OFFSET_BASIS;

  while (*str)
  {
    hash ^= (uint8_t)(*str++);
    hash *= FNV_PRIME;
  }

  return hash;
}

#endif DEPLOII_UTILS_h
